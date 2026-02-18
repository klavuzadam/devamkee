#include <iterator>

#include <cassert>
#include <chrono>
#include <cstring>
#include <limits>
#include <climits>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdint>
#include <random>

#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "desc.h"
#include "profiler.h"
#include "log.h"
#include "priv_manager.h"
#include "db.h"
#include "vector.h"
#include "ani.h"
#include "packet.h"
#include "constants.h"
#include "char.h"

#ifdef ENABLE_NINJEYE

constexpr uint32_t gNinjEyeExpectedValue = 0xC595282D;
constexpr uint32_t gNinjEyeValidationSeconds = 60;
constexpr uint32_t gNinjEyeValidationTimeoutSeconds = 10;
constexpr uint32_t gNinjEyeValidationRetryMaxCount = 2;

namespace Utils
{

static inline uint32_t GenerateRandomUI32()
{
#ifdef ENABLE_NINJEYE_MULTITHREAD
    static thread_local std::mt19937 generator(std::random_device{}());
    static thread_local std::uniform_int_distribution<uint32_t> distribution(1, UINT32_MAX);
#else
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution<uint32_t> distribution(1, UINT32_MAX);
#endif
    return distribution(generator);
}

} // namespace Utils

struct NinjEyeTimeoutCheckEventInfo : public char_event_info
{
    uint32_t SessionID;
};

struct NinjEyeHeartbeatValidationEventInfo : public char_event_info
{
    uint32_t SessionID;
};

EVENTFUNC(ninjeye_timeout_check)
{
    NinjEyeTimeoutCheckEventInfo* eventInfo = static_cast<NinjEyeTimeoutCheckEventInfo*>(event->info);
    if (!eventInfo)
    {
        return 0;
    }

    LPCHARACTER ch = eventInfo->ch;
    if (!ch)
    {
        return 0;
    }

    ch->NinjEyeCheckTimeout(eventInfo->SessionID);

    return 0;
}

EVENTFUNC(ninjeye_heartbeat_validation)
{
    NinjEyeHeartbeatValidationEventInfo* eventInfo = static_cast<NinjEyeHeartbeatValidationEventInfo*>(event->info);
    if (!eventInfo)
    {
        return 0;
    }

    LPCHARACTER ch = eventInfo->ch;
    if (!ch)
    {
        return 0;
    }

    if (!ch->GetDesc())
    {
        return 0;
    }

    ch->NinjEyeSendHeartbeatRequest(eventInfo->SessionID);

    return 0;
}

void
CHARACTER::NinjEyeSendHeartbeatRequest(uint32_t EventSessionID)
{
    uint32_t requestMessage = 0;

    {
#ifdef ENABLE_NINJEYE_MULTITHREAD
        std::lock_guard<std::mutex> lock(NinjEyeMutex);
#endif

        {
            if (!NinjEyeSessionStatus)
            {
                return;
            }

            if (EventSessionID != NinjEyeSessionID)
            {
                return;
            }
        }

        //
        // Create a timeout check event.
        //
        {
            NinjEyeResponseStatus = false;

            NinjEyeTimeoutCheckEventInfo* eventInfo{ AllocEventInfo<NinjEyeTimeoutCheckEventInfo>() };
            eventInfo->ch = this;
            eventInfo->SessionID = EventSessionID;
            NinjEyeTimeoutEvent = event_create(ninjeye_timeout_check, eventInfo, PASSES_PER_SEC(gNinjEyeValidationTimeoutSeconds));
        }

        NinjEyeKey = Utils::GenerateRandomUI32();
        requestMessage = NinjEyeKey;
    }

    //
    // Don't hold the lock while sending the request.
    //
    TPacketGCNinjEyeRequest request = { 0 };
    request.Header = HEADER_GC_NINJEYE_REQUEST;
    request.Message = requestMessage;

    GetDesc()->Packet(&request, sizeof(request));
}

void
CHARACTER::OnNinjEyeResponse(struct TPacketCGNinjEyeResponse* NinjEyeResponse)
{
#ifdef ENABLE_NINJEYE_MULTITHREAD
    std::lock_guard<std::mutex> lock(NinjEyeMutex);
#endif

    {
        if (!NinjEyeSessionStatus)
        {
            if (test_server)
            {
                ChatPacket(CHAT_TYPE_INFO, "[NinjEye] Received response but no active session.");
            }

            return;
        }
    }

    if (test_server)
    {
        ChatPacket(CHAT_TYPE_INFO, "[NinjEye] Received response.");
    }

    NinjEyeResponseStatus = true;

    uint32_t decryptedMessage = NinjEyeResponse->Message ^ NinjEyeKey;
    bool isNinjEyeResponseValid = (decryptedMessage == gNinjEyeExpectedValue);
    if (isNinjEyeResponseValid)
    {
        OnNinjEyeChecksSuccess();
        return;
    }

    sys_err("[NinjEye] Value mismatch for [%s]: Received: [0x%lX] vs Expected [0x%lX]", GetName(), decryptedMessage, gNinjEyeExpectedValue);

    OnNinjEyeChecksFailed();
}

void
CHARACTER::OnNinjEyeChecksFailed()
{
    if (test_server)
    {
        ChatPacket(CHAT_TYPE_INFO, "[NinjEye] Validation failed.");
    }

    NinjEyeBlockStatus = true;
    InvalidateNinjEyeSession_Unsafe();
}

void
CHARACTER::OnNinjEyeChecksSuccess()
{
    if (test_server)
    {
        ChatPacket(CHAT_TYPE_INFO, "[NinjEye] Validation success.");
    }

    NinjEyeHeartbeatValidationEventInfo* eventInfo{ AllocEventInfo<NinjEyeHeartbeatValidationEventInfo>() };
    eventInfo->ch = this;
    eventInfo->SessionID = NinjEyeSessionID;
    NinjEyeDelayEvent = event_create(ninjeye_heartbeat_validation, eventInfo, PASSES_PER_SEC(gNinjEyeValidationSeconds));
}

void
CHARACTER::NinjEyeCheckTimeout(uint32_t EventSessionID)
{
    {
#ifdef ENABLE_NINJEYE_MULTITHREAD
        std::lock_guard<std::mutex> lock(NinjEyeMutex);
#endif

        {
            if (!NinjEyeSessionStatus)
            {
                return;
            }

            if (NinjEyeResponseStatus)
            {
                return;
            }

            if (EventSessionID != NinjEyeSessionID)
            {
                return;
            }
        }

        if (test_server)
        {
            ChatPacket(CHAT_TYPE_INFO, "[NinjEye] No response received. Validation timed out.");
        }

        sys_log(0, "[NinjEye] No response received. Validation timed out for [%s]", GetName());

        if (NinjEyeValidationRetryCount >= gNinjEyeValidationRetryMaxCount)
        {
            sys_err("[NinjEye] Max retry count reached for [%s]", GetName());
            OnNinjEyeChecksFailed();
            return;
        }

        ++NinjEyeValidationRetryCount;
    }

    NinjEyeSendHeartbeatRequest(EventSessionID);
}

void
CHARACTER::StartNinjEyeSession()
{
#ifdef ENABLE_NINJEYE_MULTITHREAD
    std::lock_guard<std::mutex> lock(NinjEyeMutex);
#endif

    //
    // Invalidate any pre-existing session.
    //
    InvalidateNinjEyeSession_Unsafe();

    if (test_server)
    {
        ChatPacket(CHAT_TYPE_INFO, "[NinjEye] Starting new session.");
    }

    sys_log(0, "[NinjEye] Starting new session for [%s]", GetName());

    try
    {
        NinjEyeSessionStatus = true;
        NinjEyeSessionID = Utils::GenerateRandomUI32();
        NinjEyeBlockStatus = false;

        //
        // Start validation event.
        //
        if (test_server)
        {
            ChatPacket(CHAT_TYPE_INFO, "[NinjEye] Validation will start soon.");
        }

        NinjEyeHeartbeatValidationEventInfo* eventInfo = AllocEventInfo<NinjEyeHeartbeatValidationEventInfo>();
        if (!eventInfo)
        {
            throw std::runtime_error("Failed to allocate event info");
        }

        eventInfo->ch = this;
        eventInfo->SessionID = NinjEyeSessionID;
        NinjEyeDelayEvent = event_create(ninjeye_heartbeat_validation, eventInfo, PASSES_PER_SEC(3));
        if (!NinjEyeDelayEvent)
        {
            throw std::runtime_error("Failed to create delay event");
        }
    }
    catch (const std::runtime_error& Exception)
    {
        InvalidateNinjEyeSession_Unsafe();
    }
}

void CHARACTER::InvalidateNinjEyeSession_Unsafe()
{
    if (NinjEyeTimeoutEvent)
    {
        event_cancel(&NinjEyeTimeoutEvent);
        NinjEyeTimeoutEvent = nullptr;
    }

    if (NinjEyeDelayEvent)
    {
        event_cancel(&NinjEyeDelayEvent);
        NinjEyeDelayEvent = nullptr;
    }

    NinjEyeSessionStatus = false;
}

void CHARACTER::InvalidateNinjEyeSession()
{
#ifdef ENABLE_NINJEYE_MULTITHREAD
    std::lock_guard<std::mutex> lock(NinjEyeMutex);
#endif
    InvalidateNinjEyeSession_Unsafe();
}

bool CHARACTER::IsNinjEyeBlocked()
{
    //
    // Do not hold the lock here since it's not that relevant if we miss one check.
    // This procedure must be highly performant.
    //
    return NinjEyeBlockStatus;
}

#endif
