

#include "mystring.h"
#include <memory.h>
#include <stdio.h>

#ifdef OS_WCHAR_H
size_t  OS_wcslen(const OS_wchar_t* pswString)
{
    if(NULL == pswString)
    {
        return 0;
    }
    const OS_wchar_t* p = pswString;
    size_t iLen = 0;
    while(0 != *p)
    {
        iLen++;
        p++;
    }

    return iLen;
}

bool  OS_wcsncpy(OS_wchar_t* dest,const OS_wchar_t* src, size_t n)
{
    if((NULL ==dest)||(NULL == src))
    {
        return false;
    }

    size_t nSrcLen = OS_wcslen(src);

    size_t nCopyLen =(nSrcLen > n)?n:nSrcLen;

    size_t i = 0;
    for( i=0 ;i<nCopyLen;i++)
    {
        dest[i] = src[i];
    }

    dest[i] = 0;

    return true;
}

bool  OS_wcscpy(OS_wchar_t* dest,const OS_wchar_t* src)
{
    return OS_wcsncpy(dest,src,OS_wcslen(src));
}




int  OS_wtoi(const OS_wchar_t *w_string) //this local string variable has the same name of the std::string variable
{
    return (int)OS_wtol(w_string);
}

long  OS_itow(int value,OS_wchar_t *w_string, unsigned radix)// this local string variable has the same name of the std::string variable
{
    if(radix != 10)
    {
        return 0;
    }

    char Buf[32];
    memset(Buf, 0, sizeof(Buf));
    sprintf(Buf,"%d",value);
    //as digit the UTF8str == ASCII Str
    OS_wstring str = UTF8ToOSwstring(Buf);
    OS_wcscpy(w_string,str.c_str());
    return 0;
}


long OS_wtol(const OS_wchar_t *w_string) // this local string variable has the same name of the std::string variable
{
    wstring strTmp = OS2wstring(w_string);
    wchar_t* stopStr;
    long lResult = wcstol(strTmp.c_str(),&stopStr,10);
    return lResult;
}

OS_wstring wstring2OS(wstring wstr)
{
        if(0 == wstr.length())
        {
                return OS_wstring();
        }
        OS_wstring wstrRet;
        if (sizeof(wchar_t) == sizeof(OS_wchar_t))
        {
                //on windows the wstring is same as the OS_wstring
                wstrRet = (OS_wchar_t*)wstr.c_str();
        }
        else
        {
                // only support wchar_t as 4 Bytes on Unix(Linux&Mac)

                size_t size = wstr.length();
                ushort *uc = new ushort[2*size+1];
                if (NULL == uc)
                {
                        return OS_wstring();
                }
                memset(uc,0,(2*size+1)*sizeof(ushort));
                uint *wc = (uint*)wstr.c_str();
                ushort *p = uc;
                uint c = 0;
                for(size_t i = 0; i< size ;i++)
                {
                    c = (*wc);
                    if (c > 0xffff)
                    {
                        // decompose into a surrogate pair
                        *p++ = (c>>10) + 0xd7c0;
                        c = c%0x400 + 0xdc00;
                    }
                    *p++ = c;
                    wc++;
                }
                //uc[size] = 0;
                wstrRet = uc;
                delete[] uc;
        }
        return wstrRet;
}

void OS2wstring(OS_wstring wstrSrc, wstring& wstrRet)
{
    if (sizeof(wchar_t) == sizeof(OS_wchar_t))
    {
            //on windows the wstring is same as the OS_wstring
            wstrRet = (wchar_t*)wstrSrc.c_str();
    }
    else
    {
            const ushort *uc = (ushort*)wstrSrc.c_str();
            size_t size = wstrSrc.length();
            wchar_t* pswz = new wchar_t[size+1];
            memset(pswz,0,(size+1)*sizeof(wchar_t));
            wchar_t* p = pswz;

            for (size_t i = 0; i < size; ++i)
            {
                    uint u = uc[i];
                    if (u >= 0xd800 && u < 0xdc00 && i < size-1) {
                            ushort low = uc[i+1];
                            if (low >= 0xdc00 && low < 0xe000) {
                                    ++i;
                                    u = (u - 0xd800)*0x400 + (low - 0xdc00) + 0x10000;
                            }
                    }
                    *p++ = (wchar_t)u;
            }
            pswz[size] = 0;
            wstrRet = pswz;
            delete[] pswz;
            pswz = NULL;
    }
}

wstring OS2wstring(OS_wstring wstrSrc)
{
        if(0 == wstrSrc.length())
        {
                return wstring();
        }
        wstring wstrRet;

        if (sizeof(wchar_t) == sizeof(OS_wchar_t))
        {
                //on windows the wstring is same as the OS_wstring
                wstrRet = (wchar_t*)wstrSrc.c_str();
        }else
        {
                const ushort *uc = (ushort*)wstrSrc.c_str();
                size_t size = wstrSrc.length();
                wchar_t* pswz = new wchar_t[size+1];
                memset(pswz,0,(size+1)*sizeof(wchar_t));
                wchar_t* p = pswz;

                for (size_t i = 0; i < size; ++i)
                {
                        uint u = uc[i];
                        if (u >= 0xd800 && u < 0xdc00 && i < size-1) {
                                ushort low = uc[i+1];
                                if (low >= 0xdc00 && low < 0xe000) {
                                        ++i;
                                        u = (u - 0xd800)*0x400 + (low - 0xdc00) + 0x10000;
                                }
                        }
                        *p++ = (wchar_t)u;
                }
                pswz[size] = 0;
                wstrRet = pswz;
                delete[] pswz;
        }
        return wstrRet;
}

void OSwstringToUTF8(OS_wstring wstr,string& strRet)
{
    size_t size = wstr.length();
    if (0 == size)
    {
         return;
    }
    char* pUtfStr = new char[size*3+1];
    if (NULL == pUtfStr)
    {
            return;
    }
    memset(pUtfStr,0,(size*3+1)*sizeof(char));
    wstr_to_utfstr(wstr.c_str(),pUtfStr);
    strRet = pUtfStr;
    delete[] pUtfStr;
    pUtfStr = NULL;
}

string OSwstringToUTF8(OS_wstring wstr)
{
        string strRet = "";
        size_t size = wstr.length();
        if (0 == size)
        {
                return strRet;
        }
        char* pUtfStr = new char[size*3+1];
        if (NULL == pUtfStr)
        {
                return strRet;
        }
        memset(pUtfStr,0,(size*3+1)*sizeof(char));
        wstr_to_utfstr(wstr.c_str(),pUtfStr);
        strRet = pUtfStr;
        delete[] pUtfStr;
        return strRet;
}

OS_wstring UTF8ToOSwstring(string strUTF8)
{
        OS_wstring wstrRet;
        wstrRet.clear();

        size_t size = strUTF8.length();
        if (0 == size)
        {
                return wstrRet;
        }
        OS_wchar_t* pwstr = new OS_wchar_t[size+1];
        if (NULL == pwstr)
        {
                return wstrRet;
        }
        memset(pwstr,0,(size+1)*sizeof(OS_wchar_t));
        utfstr_to_wstr(strUTF8.c_str(),pwstr);
        wstrRet = pwstr;
        delete[] pwstr;
        return wstrRet;
}
// Change wchar to ushort
inline size_t g_f_wctou8(char * dest_str, const OS_wchar_t src_wchar)
{
        int count_bytes = 0;
        ushort byte_one = 0, byte_other = 0x3f; // 用于位与运算以提取位值 0x3f--->00111111
        unsigned char utf_one = 0, utf_other = 0x80; // 用于"位或"置标UTF-8编码 0x80--->1000000
        ushort tmp_wchar = 0; // 用于宽字符位置析取和位移(右移6位)
        unsigned char tmp_char = '0';

        if (!src_wchar)//
                return (size_t)-1;

        for (;;) // 检测字节序列长度
        {
                if (src_wchar <= 0x7f){ // <=01111111
                        count_bytes = 1; // ASCII字符: 0xxxxxxx( ~ 01111111)
                        byte_one = 0x7f; // 用于位与运算, 提取有效位值, 下同
                        utf_one = 0x0;
                        break;
                }
                if ( (src_wchar > 0x7f) && (src_wchar <= 0x7ff) ){ // <=0111,11111111
                        count_bytes = 2; // 110xxxxx 10xxxxxx[1](最多11个1位, 简写为11*1)
                        byte_one = 0x1f; // 00011111, 下类推(1位的数量递减)
                        utf_one = 0xc0; // 11000000
                        break;
                }
                if ( (src_wchar > 0x7ff)/* && (src_wchar <= 0xffff) */){ //0111,11111111<=11111111,11111111 //g122020 2010.02.10 modify for lint
                        count_bytes = 3; // 1110xxxx 10xxxxxx[2](MaxBits: 16*1)
                        byte_one = 0xf; // 00001111
                        utf_one = 0xe0; // 11100000
                        break;
                }
                //g122020 2010.02.10 modify for lint begin
#if 0
                if ( (src_wchar > 0xffff) && (src_wchar <= 0x1fffff) ){ //对UCS-4的支持..
                        count_bytes = 4; // 11110xxx 10xxxxxx[3](MaxBits: 21*1)
                        byte_one = 0x7; // 00000111
                        utf_one = 0xf0; // 11110000
                        break;
                }
                if ( (src_wchar > 0x1fffff) && (src_wchar <= 0x3ffffff) ){
                        count_bytes = 5; // 111110xx 10xxxxxx[4](MaxBits: 26*1)
                        byte_one = 0x3; // 00000011
                        utf_one = 0xf8; // 11111000
                        break;
                }
                if ( (src_wchar > 0x3ffffff) && (src_wchar <= 0x7fffffff) ){
                        count_bytes = 6; // 1111110x 10xxxxxx[5](MaxBits: 31*1)
                        byte_one = 0x1; // 00000001
                        utf_one = 0xfc; // 11111100
                        break;
                }
#endif
                return (size_t)-1; // 以上皆不满足则为非法序列
        }
        // 以下几行析取宽字节中的相应位, 并分组为UTF-8编码的各个字节
        tmp_wchar = src_wchar;
        for (int i = count_bytes; i > 1; i--)
        { // 一个宽字符的多字节降序赋值
                tmp_char = (unsigned char)(tmp_wchar & byte_other);///后6位与byte_other 00111111
                dest_str[i - 1] = (tmp_char | utf_other);/// 在前面加10----跟10000000或
                tmp_wchar >>= 6;//右移6位
        }
        //这个时候i=1
        //对UTF-8第一个字节位处理，
        //第一个字节的开头"1"的数目就是整个串中字节的数目
        tmp_char = (unsigned char)(tmp_wchar & byte_one);//根据上面附值得来，有效位个数
        dest_str[0] = (tmp_char | utf_one);//根据上面附值得来 1的个数
        // 位值析取分组__End!
        return count_bytes;
}

/*-----------------------------------------------------------------------------
char *-->wchar_t
它通过简单的码位截取与合成即可完成.
本函数提供这一实现.
dest_wchar:
保存转换后的宽字节字符目标地址.
src_str:
被转换的UTF-8编码源字符的多字节序列.
返回值:
返回被转换的字符的字节数. 若遇到错误或检测到非法字节序列, 则返回-1.

注意! 传递进来的宽字符应是能被合法转换为UTF-8编码的字符.

------------------------------------------------------------------------------*/
inline size_t g_f_u8towc(OS_wchar_t &dest_wchar, const unsigned char * src_str)
{
        int count_bytes = 0;
        unsigned char byte_one = 0, byte_other = 0x3f; // 用于位与运算以提取位值 0x3f-->00111111
        ushort tmp_wchar = 0;

        if (!src_str)
                return (size_t)-1;

        for (;;) // 检测字节序列长度,根据第一个字节头的1个个数
        {
                if (src_str[0] <= 0x7f){
                        count_bytes = 1; // ASCII字符: 0xxxxxxx( ~ 01111111)
                        byte_one = 0x7f; // 用于位与运算, 提取有效位值, 下同 01111111
                        break;
                }
                if ( (src_str[0] >= 0xc0) && (src_str[0] <= 0xdf) ){
                        count_bytes = 2; // 110xxxxx(110 00000 ~ 110 111111)
                        byte_one = 0x1f; //00011111 第一字节有效位的个数
                        break;
                }
                if ( (src_str[0] >= 0xe0) && (src_str[0] <= 0xef) ){
                        count_bytes = 3; // 1110xxxx(1110 0000 ~ 1110 1111)
                        byte_one = 0xf; //00001111
                        break;
                }
                if ( (src_str[0] >= 0xf0) && (src_str[0] <= 0xf7) ){
                        count_bytes = 4; // 11110xxx(11110 000 ~ 11110 111)
                        byte_one = 0x7;
                        break;
                }
                if ( (src_str[0] >= 0xf8) && (src_str[0] <= 0xfb) ){
                        count_bytes = 5; // 111110xx(111110 00 ~ 111110 11)
                        byte_one = 0x3;
                        break;
                }
                if ( (src_str[0] >= 0xfc) && (src_str[0] <= 0xfd) ){
                        count_bytes = 6; // 1111110x(1111110 0 ~ 1111110 1)
                        byte_one = 0x1;
                        break;
                }
                return (size_t)-1;//return (size_t)-1; // 以上皆不满足则为非法序列
        }
        // 以下几行析取UTF-8编码字符各个字节的有效位值
        //先得到第一个字节的有效位数据
        tmp_wchar = src_str[0] & byte_one;
        for (int i=1; i<count_bytes;i++)
        {
                tmp_wchar <<= 6; // 左移6位后与后续字节的有效位值"位或"赋值
                tmp_wchar = tmp_wchar | (src_str[i] & byte_other);//先与后或
        }
        // 位值析取__End!
        dest_wchar = tmp_wchar;
        return count_bytes;
}

inline size_t wstr_to_utfstr(const OS_wchar_t* wstr,char* utfstr)
{
        int iPos = 0;
        for (int i = 0; wstr[i] != 0; i++)
        {
                iPos += g_f_wctou8(&utfstr[iPos], wstr[i]);
        }
        utfstr[iPos] = 0;
        return (size_t)iPos;
}

inline size_t utfstr_to_wstr(const char* utfstr,OS_wchar_t* wstr)
{

        try
        {
                int iPos = 0;
                int i = 0;
                for (; utfstr[iPos] != 0; i++)
                {
                        iPos += g_f_u8towc(wstr[i],(const unsigned char*)&utfstr[iPos]);
                }
                wstr[i] = 0;
                return (size_t)i;
        }
        catch (...)
        {
                return 0;  // in this function should can not return -1, because the -1 not belongs to unsigned int type.
        }
                return 0; // this fucntion should has a return value.
}


#endif





#ifndef Q_OS_WIN
void xtoa (
        unsigned long val,
        char *buf,
        unsigned radix,
        int is_neg
        )
{
    char *p;                /* pointer to traverse string */
    char *firstdig;         /* pointer to first digit */
    char temp;              /* temp char */
    unsigned digval;        /* value of digit */

    p = buf;

    if (is_neg) {
        /* negative, so output '-' and negate */
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }

    firstdig = p;           /* save pointer to first digit */

    do {
        digval = (unsigned) (val % radix);
        val /= radix;       /* get next digit */

        /* convert to ascii and store */
        if (digval > 9)
            *p++ = (char) (digval - 10 + 'a');  /* a letter */
        else
            *p++ = (char) (digval + '0');       /* a digit */
    } while (val > 0);

    /* We now have the digit of the number in the buffer, but in reverse
    order.  Thus we reverse them now. */

    *p-- = '\0';            /* terminate string; p points to last digit */

    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;   /* swap *p and *firstdig */
        --p;
        ++firstdig;         /* advance to next two digits */
    } while (firstdig < p); /* repeat until halfway */
}

void x64toa (      /* stdcall is faster and smaller... Might as well use it for the helper. */
        uint64 val,
        char *buf,
        unsigned radix,
        int is_neg
        )
{
    char *p;                /* pointer to traverse string */
    char *firstdig;         /* pointer to first digit */
    char temp;              /* temp char */
    unsigned digval;        /* value of digit */

    p = buf;

    if ( is_neg )
    {
        *p++ = '-';         /* negative, so output '-' and negate */
        val = (unsigned __int64)(-(__int64)val);
    }

    firstdig = p;           /* save pointer to first digit */

    do {
        digval = (unsigned) (val % radix);
        val /= radix;       /* get next digit */

        /* convert to ascii and store */
        if (digval > 9)
            *p++ = (char) (digval - 10 + 'a');  /* a letter */
        else
            *p++ = (char) (digval + '0');       /* a digit */
    } while (val > 0);

    /* We now have the digit of the number in the buffer, but in reverse
    order.  Thus we reverse them now. */

    *p-- = '\0';            /* terminate string; p points to last digit */

    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;   /* swap *p and *firstdig */
        --p;
        ++firstdig;         /* advance to next two digits */
    } while (firstdig < p); /* repeat until halfway */
}

char * itoa( int iValue, char* buf, int iRoot )
{
    if (iRoot == 10 && iValue < 0)
        xtoa((unsigned long)iValue, buf, iRoot, 1);
    else
        xtoa((unsigned long)(unsigned int)iValue, buf, iRoot, 0);
    return buf;
}

char*  _ui64toa(uint64 numVal, char* szBuf, int iRoot )
{
    x64toa(numVal, szBuf, iRoot, 0);
    return szBuf;
}
#endif

#ifdef  _MSC_VER 
int strcasecmp(const char* szSrc, const char *szDes )
{
    return strcmp( szSrc, szDes );
}
#endif  //MSVC_COMPILE

#ifdef Q_OS_WIN
#else
int code_convert(const char *from_charset,const char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
        iconv_t cd;
        int rc;
        char **pin = &inbuf;
        char **pout = &outbuf;

        cd = iconv_open(to_charset,from_charset);
        if (cd==0)
                return -1;
        memset(outbuf,0,outlen);
        if (iconv(cd,pin,&inlen,pout,&outlen) == -1)
                return -1;
        iconv_close(cd);
        return 0;
}
#endif
