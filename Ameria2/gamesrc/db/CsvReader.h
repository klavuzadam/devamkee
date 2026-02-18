#ifndef __CSVFILE_H__
#define __CSVFILE_H__

#include <string>
#include <vector>
#include <unordered_map>

////////////////////////////////////////////////////////////////////////////////
/// \class cCsvAlias
/// \brief CSV ÆÄÀÏÀ» ¼öÁ¤ÇßÀ» ¶§ ¹ß»ýÇÏ´Â ÀÎµ¦½º ¹®Á¦¸¦ ÁÙÀÌ±â À§ÇÑ
/// º°¸í °´Ã¼.
///
/// ¿¹¸¦ µé¾î 0¹ø ÄÃ·³ÀÌ A¿¡ °üÇÑ ³»¿ëÀ» Æ÷ÇÔÇÏ°í, 1¹ø ÄÃ·³ÀÌ B¿¡ °üÇÑ ³»¿ëÀ»
/// Æ÷ÇÔÇÏ°í ÀÖ¾ú´Âµ¥...
///
/// <pre>
/// int a = row.AsInt(0);
/// int b = row.AsInt(1);
/// </pre>
///
/// ±× »çÀÌ¿¡ C¿¡ °üÇÑ ³»¿ëÀ» Æ÷ÇÔÇÏ´Â ÄÃ·³ÀÌ ³¢¾îµç °æ¿ì, ÇÏµåÄÚµùµÇ¾î ÀÖ´Â
/// 1¹øÀ» Ã£¾Æ¼­ °íÃÄ¾ß ÇÏ´Âµ¥, »ó´çÈ÷ ¿¡·¯°¡ ¹ß»ýÇÏ±â ½¬¿î ÀÛ¾÷ÀÌ´Ù.
///
/// <pre>
/// int a = row.AsInt(0);
/// int c = row.AsInt(1);
/// int b = row.AsInt(2); <-- ÀÌ ºÎºÐÀ» ÀÏÀÏÀÌ ½Å°æ½á¾ß ÇÑ´Ù.
/// </pre>
///
/// ÀÌ ºÎºÐÀ» ¹®ÀÚ¿­·Î Ã³¸®ÇÏ¸é À¯Áöº¸¼ö¿¡ µé¾î°¡´Â ¼ö°í¸¦ ¾à°£ÀÌ³ª¸¶ ÁÙÀÏ ¼ö
/// ÀÖ´Ù.
////////////////////////////////////////////////////////////////////////////////

class cCsvAlias
{
private:
    typedef std::unordered_map<std::string, size_t> NAME2INDEX_MAP;
    typedef std::unordered_map<size_t, std::string> INDEX2NAME_MAP;

    NAME2INDEX_MAP m_Name2Index;  ///< ¼¿ ÀÎµ¦½º ´ë½ÅÀ¸·Î »ç¿ëÇÏ±â À§ÇÑ ÀÌ¸§µé
    INDEX2NAME_MAP m_Index2Name;  ///< Àß¸øµÈ alias¸¦ °Ë»çÇÏ±â À§ÇÑ Ãß°¡ÀûÀÎ ¸Ê


public:
    /// \brief »ý¼ºÀÚ
    cCsvAlias() {}

    /// \brief ¼Ò¸êÀÚ
    virtual ~cCsvAlias() {}


public:
    /// \brief ¼¿À» ¾×¼¼½ºÇÒ ¶§, ¼ýÀÚ ´ë½Å »ç¿ëÇÒ ÀÌ¸§À» µî·ÏÇÑ´Ù.
    void AddAlias(const char* name, size_t index);

    /// \brief ¸ðµç µ¥ÀÌÅÍ¸¦ »èÁ¦ÇÑ´Ù.
    void Destroy();

    /// \brief ¼ýÀÚ ÀÎµ¦½º¸¦ ÀÌ¸§À¸·Î º¯È¯ÇÑ´Ù.
    const char* operator [] (size_t index) const;

    /// \brief ÀÌ¸§À» ¼ýÀÚ ÀÎµ¦½º·Î º¯È¯ÇÑ´Ù.
    size_t operator [] (const char* name) const;


private:
    /// \brief º¹»ç »ý¼ºÀÚ ±ÝÁö
    cCsvAlias(const cCsvAlias&) {}

    /// \brief ´ëÀÔ ¿¬»êÀÚ ±ÝÁö
    const cCsvAlias& operator = (const cCsvAlias&) { return *this; }
};


////////////////////////////////////////////////////////////////////////////////
/// \class cCsvRow
/// \brief CSV ÆÄÀÏÀÇ ÇÑ ÇàÀ» Ä¸½¶È­ÇÑ Å¬·¡½º
///
/// CSVÀÇ ±âº» Æ÷¸ËÀº ¿¢¼¿¿¡¼­ º¸ÀÌ´Â ÇÏ³ªÀÇ ¼¿À» ',' ¹®ÀÚ·Î ±¸ºÐÇÑ °ÍÀÌ´Ù.
/// ÇÏÁö¸¸, ¼¿ ¾È¿¡ Æ¯¼ö ¹®ÀÚ·Î ¾²ÀÌ´Â ',' ¹®ÀÚ³ª '"' ¹®ÀÚ°¡ µé¾î°¥ °æ¿ì,
/// ¸ð¾çÀÌ ¾à°£ ÀÌ»óÇÏ°Ô º¯ÇÑ´Ù. ´ÙÀ½Àº ±× º¯È­ÀÇ ¿¹ÀÌ´Ù.
///
/// <pre>
/// ¿¢¼¿¿¡¼­ º¸ÀÌ´Â ¸ð¾ç | ½ÇÁ¦ CSV ÆÄÀÏ¿¡ µé¾î°¡ÀÖ´Â ¸ð¾ç
/// ---------------------+----------------------------------------------------
/// ItemPrice            | ItemPrice
/// Item,Price           | "Item,Price"
/// Item"Price           | "Item""Price"
/// "ItemPrice"          | """ItemPrice"""
/// "Item,Price"         | """Item,Price"""
/// Item",Price          | "Item"",Price"
/// </pre>
///
/// ÀÌ ¿¹·Î¼­ ´ÙÀ½°ú °°Àº »çÇ×À» ¾Ë ¼ö ÀÖ´Ù.
/// - ¼¿ ³»ºÎ¿¡ ',' ¶Ç´Â '"' ¹®ÀÚ°¡ µé¾î°¥ °æ¿ì, ¼¿ ÁÂ¿ì¿¡ '"' ¹®ÀÚ°¡ »ý±ä´Ù.
/// - ¼¿ ³»ºÎÀÇ '"' ¹®ÀÚ´Â 2°³·Î Ä¡È¯µÈ´Ù.
///
/// \sa cCsvFile
////////////////////////////////////////////////////////////////////////////////

class cCsvRow : public std::vector<std::string>
{
public:
    /// \brief ±âº» »ý¼ºÀÚ
    cCsvRow() {}

    /// \brief ¼Ò¸êÀÚ
    ~cCsvRow() {}


public:
    /// \brief ÇØ´ç ¼¿ÀÇ µ¥ÀÌÅÍ¸¦ int ÇüÀ¸·Î ¹ÝÈ¯ÇÑ´Ù.
    int AsInt(size_t index) const { return atoi(at(index).c_str()); }

    /// \brief ÇØ´ç ¼¿ÀÇ µ¥ÀÌÅÍ¸¦ double ÇüÀ¸·Î ¹ÝÈ¯ÇÑ´Ù.
    double AsDouble(size_t index) const { return atof(at(index).c_str()); }

    /// \brief ÇØ´ç ¼¿ÀÇ µ¥ÀÌÅÍ¸¦ ¹®ÀÚ¿­·Î ¹ÝÈ¯ÇÑ´Ù.
    const char* AsString(size_t index) const { return at(index).c_str(); }

    /// \brief ÇØ´çÇÏ´Â ÀÌ¸§ÀÇ ¼¿ µ¥ÀÌÅÍ¸¦ int ÇüÀ¸·Î ¹ÝÈ¯ÇÑ´Ù.
    int AsInt(const char* name, const cCsvAlias& alias) const {
        return atoi( at(alias[name]).c_str() );
    }

    /// \brief ÇØ´çÇÏ´Â ÀÌ¸§ÀÇ ¼¿ µ¥ÀÌÅÍ¸¦ int ÇüÀ¸·Î ¹ÝÈ¯ÇÑ´Ù.
    double AsDouble(const char* name, const cCsvAlias& alias) const {
        return atof( at(alias[name]).c_str() );
    }

    /// \brief ÇØ´çÇÏ´Â ÀÌ¸§ÀÇ ¼¿ µ¥ÀÌÅÍ¸¦ ¹®ÀÚ¿­·Î ¹ÝÈ¯ÇÑ´Ù.
    const char* AsString(const char* name, const cCsvAlias& alias) const {
        return at(alias[name]).c_str();
    }


private:
    /// \brief º¹»ç »ý¼ºÀÚ ±ÝÁö
    cCsvRow(const cCsvRow&) {}

    /// \brief ´ëÀÔ ¿¬»êÀÚ ±ÝÁö
    const cCsvRow& operator = (const cCsvRow&) { return *this; }
};


////////////////////////////////////////////////////////////////////////////////
/// \class cCsvFile
/// \brief CSV(Comma Seperated Values) ÆÄÀÏÀ» read/writeÇÏ±â À§ÇÑ Å¬·¡½º
///
/// <b>sample</b>
/// <pre>
/// cCsvFile file;
///
/// cCsvRow row1, row2, row3;
/// row1.push_back("ItemPrice");
/// row1.push_back("Item,Price");
/// row1.push_back("Item\"Price");
///
/// row2.reserve(3);
/// row2[0] = "\"ItemPrice\"";
/// row2[1] = "\"Item,Price\"";
/// row2[2] = "Item\",Price\"";
///
/// row3 = "\"ItemPrice\"\"Item,Price\"Item\",Price\"";
///
/// file.add(row1);
/// file.add(row2);
/// file.add(row3);
/// file.save("test.csv", false);
/// </pre>
///
/// \todo ÆÄÀÏ¿¡¼­¸¸ ÀÐ¾îµéÀÏ °ÍÀÌ ¾Æ´Ï¶ó, ¸Þ¸ð¸® ¼Ò½º·ÎºÎÅÍ ÀÐ´Â ÇÔ¼öµµ
/// ÀÖ¾î¾ß ÇÒ µí ÇÏ´Ù.
////////////////////////////////////////////////////////////////////////////////

class cCsvFile
{
private:
    typedef std::vector<cCsvRow*> ROWS;

    ROWS m_Rows; ///< Çà ÄÃ·º¼Ç


public:
    /// \brief »ý¼ºÀÚ
    cCsvFile() {}

    /// \brief ¼Ò¸êÀÚ
    virtual ~cCsvFile() { Destroy(); }


public:
    /// \brief ÁöÁ¤µÈ ÀÌ¸§ÀÇ CSV ÆÄÀÏÀ» ·ÎµåÇÑ´Ù.
    bool Load(const char* fileName, const char seperator=',', const char quote='"');

    /// \brief °¡Áö°í ÀÖ´Â ³»¿ëÀ» CSV ÆÄÀÏ¿¡´Ù ÀúÀåÇÑ´Ù.
    bool Save(const char* fileName, bool append=false, char seperator=',', char quote='"') const;

    /// \brief ¸ðµç µ¥ÀÌÅÍ¸¦ ¸Þ¸ð¸®¿¡¼­ »èÁ¦ÇÑ´Ù.
    void Destroy();

    /// \brief ÇØ´çÇÏ´Â ÀÎµ¦½ºÀÇ ÇàÀ» ¹ÝÈ¯ÇÑ´Ù.
    cCsvRow* operator [] (size_t index);

    /// \brief ÇØ´çÇÏ´Â ÀÎµ¦½ºÀÇ ÇàÀ» ¹ÝÈ¯ÇÑ´Ù.
    const cCsvRow* operator [] (size_t index) const;

    /// \brief ÇàÀÇ °¹¼ö¸¦ ¹ÝÈ¯ÇÑ´Ù.
    size_t GetRowCount() const { return m_Rows.size(); }


private:
    /// \brief º¹»ç »ý¼ºÀÚ ±ÝÁö
    cCsvFile(const cCsvFile&) {}

    /// \brief ´ëÀÔ ¿¬»êÀÚ ±ÝÁö
    const cCsvFile& operator = (const cCsvFile&) { return *this; }
};


////////////////////////////////////////////////////////////////////////////////
/// \class cCsvTable
/// \brief CSV ÆÄÀÏÀ» ÀÌ¿ëÇØ Å×ÀÌºí µ¥ÀÌÅÍ¸¦ ·ÎµåÇÏ´Â °æ¿ì°¡ ¸¹Àºµ¥, ÀÌ Å¬·¡½º´Â
/// ±× ÀÛ¾÷À» Á» ´õ ½±°Ô ÇÏ±â À§ÇØ ¸¸µç À¯Æ¿¸®Æ¼ Å¬·¡½º´Ù.
///
/// CSV ÆÄÀÏÀ» ·ÎµåÇÏ´Â °æ¿ì, ¼ýÀÚ¸¦ ÀÌ¿ëÇØ ¼¿À» ¾×¼¼½ºÇØ¾ß ÇÏ´Âµ¥, CSV
/// ÆÄÀÏÀÇ Æ÷¸ËÀÌ ¹Ù²î´Â °æ¿ì, ÀÌ ¼ýÀÚµéÀ» º¯°æÇØÁà¾ßÇÑ´Ù. ÀÌ ÀÛ¾÷ÀÌ ²Ï
/// ½Å°æ ÁýÁßÀ» ¿ä±¸ÇÏ´Â µ¥´Ù°¡, ¿¡·¯°¡ ¹ß»ýÇÏ±â ½±´Ù. ±×·¯¹Ç·Î ¼ýÀÚ·Î
/// ¾×¼¼½ºÇÏ±âº¸´Ù´Â ¹®ÀÚ¿­·Î ¾×¼¼½ºÇÏ´Â °ÍÀÌ ¾à°£ ´À¸®Áö¸¸ ³´´Ù°í ÇÒ ¼ö ÀÖ´Ù.
///
/// <b>sample</b>
/// <pre>
/// cCsvTable table;
///
/// table.alias(0, "ItemClass");
/// table.alias(1, "ItemType");
///
/// if (table.load("test.csv"))
/// {
///     while (table.next())
///     {
///         std::string item_class = table.AsString("ItemClass");
///         int         item_type  = table.AsInt("ItemType");
///     }
/// }
/// </pre>
////////////////////////////////////////////////////////////////////////////////

class cCsvTable
{
public :
    cCsvFile  m_File;   ///< CSV ÆÄÀÏ °´Ã¼
private:
    cCsvAlias m_Alias;  ///< ¹®ÀÚ¿­À» ¼¿ ÀÎµ¦½º·Î º¯È¯ÇÏ±â À§ÇÑ °´Ã¼
    int       m_CurRow; ///< ÇöÀç È¾´Ü ÁßÀÎ Çà ¹øÈ£


public:
    /// \brief »ý¼ºÀÚ
    cCsvTable();

    /// \brief ¼Ò¸êÀÚ
    virtual ~cCsvTable();


public:
    /// \brief ÁöÁ¤µÈ ÀÌ¸§ÀÇ CSV ÆÄÀÏÀ» ·ÎµåÇÑ´Ù.
    bool Load(const char* fileName, const char seperator=',', const char quote='"');

    /// \brief ¼¿À» ¾×¼¼½ºÇÒ ¶§, ¼ýÀÚ ´ë½Å »ç¿ëÇÒ ÀÌ¸§À» µî·ÏÇÑ´Ù.
    void AddAlias(const char* name, size_t index) { m_Alias.AddAlias(name, index); }

    /// \brief ´ÙÀ½ ÇàÀ¸·Î ³Ñ¾î°£´Ù.
    bool Next();

    /// \brief ÇöÀç ÇàÀÇ ¼¿ ¼ýÀÚ¸¦ ¹ÝÈ¯ÇÑ´Ù.
    size_t ColCount() const;

    /// \brief ÀÎµ¦½º¸¦ ÀÌ¿ëÇØ int ÇüÀ¸·Î ¼¿°ªÀ» ¹ÝÈ¯ÇÑ´Ù.
    int AsInt(size_t index) const;

    /// \brief ÀÎµ¦½º¸¦ ÀÌ¿ëÇØ double ÇüÀ¸·Î ¼¿°ªÀ» ¹ÝÈ¯ÇÑ´Ù.
    double AsDouble(size_t index) const;

    /// \brief ÀÎµ¦½º¸¦ ÀÌ¿ëÇØ std::string ÇüÀ¸·Î ¼¿°ªÀ» ¹ÝÈ¯ÇÑ´Ù.
    const char* AsStringByIndex(size_t index) const;

    /// \brief ¼¿ ÀÌ¸§À» ÀÌ¿ëÇØ int ÇüÀ¸·Î ¼¿°ªÀ» ¹ÝÈ¯ÇÑ´Ù.
    int AsInt(const char* name) const { return AsInt(m_Alias[name]); }

    /// \brief ¼¿ ÀÌ¸§À» ÀÌ¿ëÇØ double ÇüÀ¸·Î ¼¿°ªÀ» ¹ÝÈ¯ÇÑ´Ù.
    double AsDouble(const char* name) const { return AsDouble(m_Alias[name]); }

    /// \brief ¼¿ ÀÌ¸§À» ÀÌ¿ëÇØ std::string ÇüÀ¸·Î ¼¿°ªÀ» ¹ÝÈ¯ÇÑ´Ù.
    const char* AsString(const char* name) const { return AsStringByIndex(m_Alias[name]); }

    /// \brief alias¸¦ Æ÷ÇÔÇØ ¸ðµç µ¥ÀÌÅÍ¸¦ »èÁ¦ÇÑ´Ù.
    void Destroy();


private:
    /// \brief ÇöÀç ÇàÀ» ¹ÝÈ¯ÇÑ´Ù.
    const cCsvRow* const CurRow() const;

    /// \brief º¹»ç »ý¼ºÀÚ ±ÝÁö
    cCsvTable(const cCsvTable&) {}

    /// \brief ´ëÀÔ ¿¬»êÀÚ ±ÝÁö
    const cCsvTable& operator = (const cCsvTable&) { return *this; }
};

#endif //__CSVFILE_H__
