#include "stdafx.h"
#include "start_position.h"


char g_nation_name[4][32] =
{
	"",
	"Shinsoo",
	"Chunjo",
	"Jinno",
};

//	"934"
//	"935"
//	"936"

long g_start_map[4] =
{
	0,	// reserved
	90,	// ½Å¼ö±¹
	21,	// ÃµÁ¶±¹
	91	// Áø³ë±¹
};

DWORD g_start_position[4][2] =
{
	{      0,      0 },	// reserved
	{ 469300, 964200 },	// ½Å¼ö±¹
	{  55700, 157900 },	// ÃµÁ¶±¹
	{ 969600, 278400 }	// Áø³ë±¹
};


DWORD arena_return_position[4][2] =
{
	{       0,  0       },
	{   347600, 882700  }, // ÀÚ¾çÇö
	{   138600, 236600  }, // º¹Á¤Çö
	{   857200, 251800  }  // ¹Ú¶óÇö
};


DWORD g_create_position[4][2] =
{
	{      0,      0 },	// reserved
	{ 469300, 964200 },	// ½Å¼ö±¹
	{  55700, 157900 },	// ÃµÁ¶±¹
	{ 969600, 278400 }	// Áø³ë±¹
};

DWORD g_create_position_canada[4][2] =
{
	{      0,      0 },	// reserved
	{ 469300, 964200 },	// ½Å¼ö±¹
	{  55700, 157900 },	// ÃµÁ¶±¹
	{ 969600, 278400 }	// Áø³ë±¹
};

