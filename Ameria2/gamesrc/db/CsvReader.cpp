#include "stdafx.h"
#include "CsvReader.h"
#include <fstream>
#include <algorithm>

#ifndef Assert
    #include <assert.h>
    #define Assert assert
    #define LogToFile (void)(0);
#endif

namespace
{
    /// ÆÄ½Ì¿ë state ¿­°Å°ª
    enum ParseState
    {
        STATE_NORMAL = 0, ///< ÀÏ¹Ý »óÅÂ
        STATE_QUOTE       ///< µû¿ÈÇ¥ µÚÀÇ »óÅÂ
    };

    /// ¹®ÀÚ¿­ ÁÂ¿ìÀÇ °ø¹éÀ» Á¦°ÅÇØ¼­ ¹ÝÈ¯ÇÑ´Ù.
    std::string Trim(std::string str)
    {
        str = str.erase(str.find_last_not_of(" \t\r\n") + 1);
        str = str.erase(0, str.find_first_not_of(" \t\r\n"));
        return str;
    }

    /// \brief ÁÖ¾îÁø ¹®Àå¿¡ ÀÖ´Â ¾ËÆÄºªÀ» ¸ðµÎ ¼Ò¹®ÀÚ·Î ¹Ù²Û´Ù.
    std::string Lower(std::string original)
    {
        std::transform(original.begin(), original.end(), original.begin(), tolower);
        return original;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ¼¿À» ¾×¼¼½ºÇÒ ¶§, ¼ýÀÚ ´ë½Å »ç¿ëÇÒ ÀÌ¸§À» µî·ÏÇÑ´Ù.
/// \param name ¼¿ ÀÌ¸§
/// \param index ¼¿ ÀÎµ¦½º
////////////////////////////////////////////////////////////////////////////////
void cCsvAlias::AddAlias(const char* name, size_t index)
{
    std::string converted(Lower(name));

    Assert(m_Name2Index.find(converted) == m_Name2Index.end());
    Assert(m_Index2Name.find(index) == m_Index2Name.end());

    m_Name2Index.insert(NAME2INDEX_MAP::value_type(converted, index));
    m_Index2Name.insert(INDEX2NAME_MAP::value_type(index, name));
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ¸ðµç µ¥ÀÌÅÍ¸¦ »èÁ¦ÇÑ´Ù.
////////////////////////////////////////////////////////////////////////////////
void cCsvAlias::Destroy()
{
    m_Name2Index.clear();
    m_Index2Name.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ¼ýÀÚ ÀÎµ¦½º¸¦ ÀÌ¸§À¸·Î º¯È¯ÇÑ´Ù.
/// \param index ¼ýÀÚ ÀÎµ¦½º
/// \return const char* ÀÌ¸§
////////////////////////////////////////////////////////////////////////////////
const char* cCsvAlias::operator [] (size_t index) const
{
    INDEX2NAME_MAP::const_iterator itr(m_Index2Name.find(index));
    if (itr == m_Index2Name.end())
    {
        //LogToFile(NULL, "cannot find suitable conversion for %d", index);
        Assert(false && "cannot find suitable conversion");
        return NULL;
    }

    return itr->second.c_str();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÀÌ¸§À» ¼ýÀÚ ÀÎµ¦½º·Î º¯È¯ÇÑ´Ù.
/// \param name ÀÌ¸§
/// \return size_t ¼ýÀÚ ÀÎµ¦½º
////////////////////////////////////////////////////////////////////////////////
size_t cCsvAlias::operator [] (const char* name) const
{
    NAME2INDEX_MAP::const_iterator itr(m_Name2Index.find(Lower(name)));
    if (itr == m_Name2Index.end())
    {
        //LogToFile(NULL, "cannot find suitable conversion for %s", name);
        Assert(false && "cannot find suitable conversion");
        return 0;
    }

    return itr->second;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÁöÁ¤µÈ ÀÌ¸§ÀÇ CSV ÆÄÀÏÀ» ·ÎµåÇÑ´Ù.
/// \param fileName CSV ÆÄÀÏ ÀÌ¸§
/// \param seperator ÇÊµå ºÐ¸®ÀÚ·Î »ç¿ëÇÒ ±ÛÀÚ. ±âº»°ªÀº ','ÀÌ´Ù.
/// \param quote µû¿ÈÇ¥·Î »ç¿ëÇÒ ±ÛÀÚ. ±âº»°ªÀº '"'ÀÌ´Ù.
/// \return bool ¹«»çÈ÷ ·ÎµåÇß´Ù¸é true, ¾Æ´Ï¶ó¸é false
////////////////////////////////////////////////////////////////////////////////
bool cCsvFile::Load(const char* fileName, const char seperator, const char quote)
{
    Assert(seperator != quote);

    std::ifstream file(fileName, std::ios::in);
    if (!file) return false;

    Destroy(); // ±âÁ¸ÀÇ µ¥ÀÌÅÍ¸¦ »èÁ¦

    cCsvRow* row = NULL;
    ParseState state = STATE_NORMAL;
    std::string token = "";
    char buf[2048+1] = {0,};

    while (file.good())
    {
        file.getline(buf, 2048);
        buf[sizeof(buf)-1] = 0;

        std::string line(Trim(buf));
        if (line.empty() || (state == STATE_NORMAL && line[0] == '#')) continue;

        std::string text  = std::string(line) + "  "; // ÆÄ½Ì lookahead ¶§¹®¿¡ ºÙ¿©ÁØ´Ù.
        size_t cur = 0;

        while (cur < text.size())
        {
            // ÇöÀç ¸ðµå°¡ QUOTE ¸ðµåÀÏ ¶§,
            if (state == STATE_QUOTE)
            {
                // '"' ¹®ÀÚÀÇ Á¾·ù´Â µÎ °¡ÁöÀÌ´Ù.
                // 1. ¼¿ ³»ºÎ¿¡ Æ¯¼ö ¹®ÀÚ°¡ ÀÖÀ» °æ¿ì ÀÌ¸¦ ¾Ë¸®´Â ¼¿ ÁÂ¿ìÀÇ °Í
                // 2. ¼¿ ³»ºÎÀÇ '"' ¹®ÀÚ°¡ '"' 2°³·Î Ä¡È¯µÈ °Í
                // ÀÌ Áß Ã¹¹øÂ° °æ¿ìÀÇ ÁÂÃø¿¡ ÀÖ´Â °ÍÀº CSV ÆÄÀÏÀÌ Á¤»óÀûÀÌ¶ó¸é,
                // ¹«Á¶°Ç STATE_NORMAL¿¡ °É¸®°Ô µÇ¾îÀÖ´Ù.
                // ±×·¯¹Ç·Î ¿©±â¼­ °É¸®´Â °ÍÀº 1¹øÀÇ ¿ìÃø °æ¿ì³ª, 2¹ø °æ¿ì »ÓÀÌ´Ù.
                // 2¹øÀÇ °æ¿ì¿¡´Â ¹«Á¶°Ç '"' ¹®ÀÚ°¡ 2°³¾¿ ³ªÅ¸³­´Ù. ÇÏÁö¸¸ 1¹øÀÇ
                // ¿ìÃø °æ¿ì¿¡´Â ¾Æ´Ï´Ù. ÀÌ¸¦ ¹ÙÅÁÀ¸·Î ÇØ¼­ ÄÚµå¸¦ Â¥¸é...
                if (text[cur] == quote)
                {
                    // ´ÙÀ½ ¹®ÀÚ°¡ '"' ¹®ÀÚ¶ó¸é, Áï ¿¬¼ÓµÈ '"' ¹®ÀÚ¶ó¸é
                    // ÀÌ´Â ¼¿ ³»ºÎÀÇ '"' ¹®ÀÚ°¡ Ä¡È¯µÈ °ÍÀÌ´Ù.
                    if (text[cur+1] == quote)
                    {
                        token += quote;
                        ++cur;
                    }
                    // ´ÙÀ½ ¹®ÀÚ°¡ '"' ¹®ÀÚ°¡ ¾Æ´Ï¶ó¸é
                    // ÇöÀçÀÇ '"'¹®ÀÚ´Â ¼¿ÀÇ ³¡À» ¾Ë¸®´Â ¹®ÀÚ¶ó°í ÇÒ ¼ö ÀÖ´Ù.
                    else
                    {
                        state = STATE_NORMAL;
                    }
                }
                else
                {
                    token += text[cur];
                }
            }
            // ÇöÀç ¸ðµå°¡ NORMAL ¸ðµåÀÏ ¶§,
            else if (state == STATE_NORMAL)
            {
                if (row == NULL)
                    row = new cCsvRow();

                // ',' ¹®ÀÚ¸¦ ¸¸³µ´Ù¸é ¼¿ÀÇ ³¡ÀÇ ÀÇ¹ÌÇÑ´Ù.
                // ÅäÅ«À¸·Î¼­ ¼¿ ¸®½ºÆ®¿¡´Ù°¡ Áý¾î³Ö°í, ÅäÅ«À» ÃÊ±âÈ­ÇÑ´Ù.
                if (text[cur] == seperator)
                {
                    row->push_back(token);
                    token.clear();
                }
                // '"' ¹®ÀÚ¸¦ ¸¸³µ´Ù¸é, QUOTE ¸ðµå·Î ÀüÈ¯ÇÑ´Ù.
                else if (text[cur] == quote)
                {
                    state = STATE_QUOTE;
                }
                // ´Ù¸¥ ÀÏ¹Ý ¹®ÀÚ¶ó¸é ÇöÀç ÅäÅ«¿¡´Ù°¡ µ¡ºÙÀÎ´Ù.
                else
                {
                    token += text[cur];
                }
            }

            ++cur;
        }

        // ¸¶Áö¸· ¼¿Àº ³¡¿¡ ',' ¹®ÀÚ°¡ ¾ø±â ¶§¹®¿¡ ¿©±â¼­ Ãß°¡ÇØÁà¾ßÇÑ´Ù.
        // ´Ü, Ã³À½¿¡ ÆÄ½Ì lookahead ¶§¹®¿¡ ºÙÀÎ ½ºÆäÀÌ½º ¹®ÀÚ µÎ °³¸¦ ¶¾´Ù.
        if (state == STATE_NORMAL)
        {
            Assert(row != NULL);
            row->push_back(token.substr(0, token.size()-2));
            m_Rows.push_back(row);
            token.clear();
            row = NULL;
        }
        else
        {
            token = token.substr(0, token.size()-2) + "\r\n";
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief °¡Áö°í ÀÖ´Â ³»¿ëÀ» CSV ÆÄÀÏ¿¡´Ù ÀúÀåÇÑ´Ù.
/// \param fileName CSV ÆÄÀÏ ÀÌ¸§
/// \param append trueÀÏ °æ¿ì, ±âÁ¸ÀÇ ÆÄÀÏ¿¡´Ù µ¡ºÙÀÎ´Ù. falseÀÎ °æ¿ì¿¡´Â
/// ±âÁ¸ÀÇ ÆÄÀÏ ³»¿ëÀ» »èÁ¦ÇÏ°í, »õ·Î ¾´´Ù.
/// \param seperator ÇÊµå ºÐ¸®ÀÚ·Î »ç¿ëÇÒ ±ÛÀÚ. ±âº»°ªÀº ','ÀÌ´Ù.
/// \param quote µû¿ÈÇ¥·Î »ç¿ëÇÒ ±ÛÀÚ. ±âº»°ªÀº '"'ÀÌ´Ù.
/// \return bool ¹«»çÈ÷ ÀúÀåÇß´Ù¸é true, ¿¡·¯°¡ »ý±ä °æ¿ì¿¡´Â false
////////////////////////////////////////////////////////////////////////////////
bool cCsvFile::Save(const char* fileName, bool append, char seperator, char quote) const
{
    Assert(seperator != quote);

    // Ãâ·Â ¸ðµå¿¡ µû¶ó ÆÄÀÏÀ» Àû´çÇÑ ÇÃ·¡±×·Î »ý¼ºÇÑ´Ù.
    std::ofstream file;
    if (append) { file.open(fileName, std::ios::out | std::ios::app); }
    else { file.open(fileName, std::ios::out | std::ios::trunc); }

    // ÆÄÀÏÀ» ¿­Áö ¸øÇß´Ù¸é, false¸¦ ¸®ÅÏÇÑ´Ù.
    if (!file) return false;

    char special_chars[5] = { seperator, quote, '\r', '\n', 0 };
    char quote_escape_string[3] = { quote, quote, 0 };

    // ¸ðµç ÇàÀ» È¾´ÜÇÏ¸é¼­...
    for (size_t i=0; i<m_Rows.size(); i++)
    {
        const cCsvRow& row = *((*this)[i]);

        std::string line;

        // Çà ¾ÈÀÇ ¸ðµç ÅäÅ«À» È¾´ÜÇÏ¸é¼­...
        for (size_t j=0; j<row.size(); j++)
        {
            const std::string& token = row[j];

            // ÀÏ¹ÝÀûÀÎ('"' ¶Ç´Â ','¸¦ Æ÷ÇÔÇÏÁö ¾ÊÀº)
            // ÅäÅ«ÀÌ¶ó¸é ±×³É ÀúÀåÇÏ¸é µÈ´Ù.
            if (token.find_first_of(special_chars) == std::string::npos)
            {
                line += token;
            }
            // Æ¯¼ö¹®ÀÚ¸¦ Æ÷ÇÔÇÑ ÅäÅ«ÀÌ¶ó¸é ¹®ÀÚ¿­ ÁÂ¿ì¿¡ '"'¸¦ ºÙ¿©ÁÖ°í,
            // ¹®ÀÚ¿­ ³»ºÎÀÇ '"'¸¦ µÎ °³·Î ¸¸µé¾îÁà¾ßÇÑ´Ù.
            else
            {
                line += quote;

                for (size_t k=0; k<token.size(); k++)
                {
                    if (token[k] == quote) line += quote_escape_string;
                    else line += token[k];
                }

                line += quote;
            }

            // ¸¶Áö¸· ¼¿ÀÌ ¾Æ´Ï¶ó¸é ','¸¦ ÅäÅ«ÀÇ µÚ¿¡´Ù ºÙ¿©Áà¾ßÇÑ´Ù.
            if (j != row.size() - 1) { line += seperator; }
        }

        // ¶óÀÎÀ» Ãâ·ÂÇÑ´Ù.
        file << line << std::endl;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ¸ðµç µ¥ÀÌÅÍ¸¦ ¸Þ¸ð¸®¿¡¼­ »èÁ¦ÇÑ´Ù.
////////////////////////////////////////////////////////////////////////////////
void cCsvFile::Destroy()
{
    for (ROWS::iterator itr(m_Rows.begin()); itr != m_Rows.end(); ++itr)
        delete *itr;

    m_Rows.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÇØ´çÇÏ´Â ÀÎµ¦½ºÀÇ ÇàÀ» ¹ÝÈ¯ÇÑ´Ù.
/// \param index ÀÎµ¦½º
/// \return cCsvRow* ÇØ´ç Çà
////////////////////////////////////////////////////////////////////////////////
cCsvRow* cCsvFile::operator [] (size_t index)
{
    Assert(index < m_Rows.size());
    return m_Rows[index];
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÇØ´çÇÏ´Â ÀÎµ¦½ºÀÇ ÇàÀ» ¹ÝÈ¯ÇÑ´Ù.
/// \param index ÀÎµ¦½º
/// \return const cCsvRow* ÇØ´ç Çà
////////////////////////////////////////////////////////////////////////////////
const cCsvRow* cCsvFile::operator [] (size_t index) const
{
    Assert(index < m_Rows.size());
    return m_Rows[index];
}

////////////////////////////////////////////////////////////////////////////////
/// \brief »ý¼ºÀÚ
////////////////////////////////////////////////////////////////////////////////
cCsvTable::cCsvTable()
: m_CurRow(-1)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ¼Ò¸êÀÚ
////////////////////////////////////////////////////////////////////////////////
cCsvTable::~cCsvTable()
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÁöÁ¤µÈ ÀÌ¸§ÀÇ CSV ÆÄÀÏÀ» ·ÎµåÇÑ´Ù.
/// \param fileName CSV ÆÄÀÏ ÀÌ¸§
/// \param seperator ÇÊµå ºÐ¸®ÀÚ·Î »ç¿ëÇÒ ±ÛÀÚ. ±âº»°ªÀº ','ÀÌ´Ù.
/// \param quote µû¿ÈÇ¥·Î »ç¿ëÇÒ ±ÛÀÚ. ±âº»°ªÀº '"'ÀÌ´Ù.
/// \return bool ¹«»çÈ÷ ·ÎµåÇß´Ù¸é true, ¾Æ´Ï¶ó¸é false
////////////////////////////////////////////////////////////////////////////////
bool cCsvTable::Load(const char* fileName, const char seperator, const char quote)
{
    Destroy();
    return m_File.Load(fileName, seperator, quote);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ´ÙÀ½ ÇàÀ¸·Î ³Ñ¾î°£´Ù.
/// \return bool ´ÙÀ½ ÇàÀ¸·Î ¹«»çÈ÷ ³Ñ¾î°£ °æ¿ì true¸¦ ¹ÝÈ¯ÇÏ°í, ´õ ÀÌ»ó
/// ³Ñ¾î°¥ ÇàÀÌ Á¸ÀçÇÏÁö ¾Ê´Â °æ¿ì¿¡´Â false¸¦ ¹ÝÈ¯ÇÑ´Ù.
////////////////////////////////////////////////////////////////////////////////
bool cCsvTable::Next()
{
    // 20¾ï¹ø Á¤µµ È£ÃâÇÏ¸é ¿À¹öÇÃ·Î°¡ ÀÏ¾î³¯ÅÙµ¥...±¦Âú°ÚÁö?
    return ++m_CurRow < (int)m_File.GetRowCount() ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÇöÀç ÇàÀÇ ¼¿ ¼ýÀÚ¸¦ ¹ÝÈ¯ÇÑ´Ù.
/// \return size_t ÇöÀç ÇàÀÇ ¼¿ ¼ýÀÚ
////////////////////////////////////////////////////////////////////////////////
size_t cCsvTable::ColCount() const
{
    return CurRow()->size();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÀÎµ¦½º¸¦ ÀÌ¿ëÇØ int ÇüÀ¸·Î ¼¿ °ªÀ» ¹ÝÈ¯ÇÑ´Ù.
/// \param index ¼¿ ÀÎµ¦½º
/// \return int ¼¿ °ª
////////////////////////////////////////////////////////////////////////////////
int cCsvTable::AsInt(size_t index) const
{
    const cCsvRow* const row = CurRow();
    Assert(row);
    Assert(index < row->size());
    return row->AsInt(index);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÀÎµ¦½º¸¦ ÀÌ¿ëÇØ double ÇüÀ¸·Î ¼¿ °ªÀ» ¹ÝÈ¯ÇÑ´Ù.
/// \param index ¼¿ ÀÎµ¦½º
/// \return double ¼¿ °ª
////////////////////////////////////////////////////////////////////////////////
double cCsvTable::AsDouble(size_t index) const
{
    const cCsvRow* const row = CurRow();
    Assert(row);
    Assert(index < row->size());
    return row->AsDouble(index);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÀÎµ¦½º¸¦ ÀÌ¿ëÇØ std::string ÇüÀ¸·Î ¼¿ °ªÀ» ¹ÝÈ¯ÇÑ´Ù.
/// \param index ¼¿ ÀÎµ¦½º
/// \return const char* ¼¿ °ª
////////////////////////////////////////////////////////////////////////////////
const char* cCsvTable::AsStringByIndex(size_t index) const
{
    const cCsvRow* const row = CurRow();
    Assert(row);
    Assert(index < row->size());
    return row->AsString(index);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief alias¸¦ Æ÷ÇÔÇØ ¸ðµç µ¥ÀÌÅÍ¸¦ »èÁ¦ÇÑ´Ù.
////////////////////////////////////////////////////////////////////////////////
void cCsvTable::Destroy()
{
    m_File.Destroy();
    m_Alias.Destroy();
    m_CurRow = -1;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief ÇöÀç ÇàÀ» ¹ÝÈ¯ÇÑ´Ù.
/// \return const cCsvRow* ¾×¼¼½º°¡ °¡´ÉÇÑ ÇöÀç ÇàÀÌ Á¸ÀçÇÏ´Â °æ¿ì¿¡´Â ±× ÇàÀÇ
/// Æ÷ÀÎÅÍ¸¦ ¹ÝÈ¯ÇÏ°í, ´õ ÀÌ»ó ¾×¼¼½º °¡´ÉÇÑ ÇàÀÌ ¾ø´Â °æ¿ì¿¡´Â NULLÀ»
/// ¹ÝÈ¯ÇÑ´Ù.
////////////////////////////////////////////////////////////////////////////////
const cCsvRow* const cCsvTable::CurRow() const
{
    if (m_CurRow < 0)
    {
        Assert(false && "call Next() first!");
        return NULL;
    }
    else if (m_CurRow >= (int)m_File.GetRowCount())
    {
        Assert(false && "no more rows!");
        return NULL;
    }

    return m_File[m_CurRow];
}

