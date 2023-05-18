#ifndef STRING_COMMON_H
#define STRING_COMMON_H

#include <vector>
#include <string>
#include <sstream>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

using namespace std;

class SString
{
public:
    static string GBKToUTF8(const std::string& strGBK)
    {
#ifdef Q_OS_WIN
        string strOutUTF8 = ""; 
        WCHAR * str1;
        int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0); 
        str1 = new WCHAR[n]; 
        MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n); 
        n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
        char * str2 = new char[n]; 
        WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL); 
        strOutUTF8 = str2; 
        delete[]str1; 
        str1 = NULL; 
        delete[]str2;
        str2 = NULL; 
        return strOutUTF8;
#else
        return strGBK;
#endif
    }

    static string UTF8ToGBK(const std::string& strUTF8) 
    { 
#ifdef Q_OS_WIN
        int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
        unsigned short * wszGBK = new unsigned short[len + 1]; memset(wszGBK, 0, len * 2 + 2); 
        MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

        len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
        char *szGBK = new char[len + 1];
        memset(szGBK, 0, len + 1); 
        WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK;
        std::string strTemp(szGBK); 
        delete[]szGBK; 
        delete[]wszGBK; 
        return strTemp; 
#else
        return strUTF8;
#endif
    }

    //字符串分割函数
    static std::vector<std::string> split(std::string str, std::string pattern)
    {
        std::string::size_type pos;
        std::vector<std::string> result;
        str+=pattern;//扩展字符串以方便操作
        string::size_type size=str.size();

        for(int i=0; i<size; i++)
        {
            pos=str.find(pattern,i);
            if(pos<size)
            {
                std::string s=str.substr(i,pos-i);
                result.push_back(s);
                i=pos+pattern.size()-1;
            }
        }
        return result;
    }

	static string VectToString(std::vector<std::string>& vect, std::string pattern)
	{
		string  result;

		for (int i = 0; i < vect.size(); i++)
		{
			result += vect.at(i);
			if (i < vect.size() -1)
			{
				result += pattern;
			}	
		}
		return result;
	}

	static string ULongLongToStr( unsigned long long number, int width = 1 )
	{
		std::stringstream newstr;
		newstr.fill('0');
		newstr.width(width);
		newstr<<number;
		return newstr.str();
	}

    static string IntToStr( int number, int width = 1 )
    {
        std::stringstream newstr;
        newstr.fill('0');
        newstr.width(width);
        newstr<<number;
        return newstr.str();
    }

    static string DoubleToStr( double number )
    {
        std::stringstream newstr;
        newstr.setf(ios::fixed);
        newstr.precision(3);
        newstr<<number;
        return newstr.str();
    }

    static string IntArrayToStr( int number[], int count )
    {
        std::stringstream newstr;
        for (int i=0; i<count; i++)
        {
            newstr<<number[i];
            if (i != count-1 )
            {
                newstr<<",";
            }
        }

        return newstr.str();
    }

    static string BoolArrayToStr( bool number[], int count )
    {
        std::stringstream newstr;
        for (int i=0; i<count; i++)
        {
            newstr<<number[i];
            if (i != count-1 )
            {
                newstr<<",";
            }
        }

        return newstr.str();
    }

    static void erase(string &str, char c)
    {
        string::iterator it = str.begin();
        while (it != str.end())
        {
            if ( *it == c)
            {
                it = str.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

	// 替换子串，原始字符串会被修改
	static string& replace_all(string &str, const string &old_value, const string &new_value)     
	{     
		for(string::size_type pos(0);   pos!=string::npos; pos+=new_value.length())  
		{     
			if((pos=str.find(old_value,pos)) != string::npos   )     
			{ 
				str.replace(pos,old_value.length(),new_value);  
			}   
			else
			{ 
				break;
			}    
		}     
		return str;     
	}    

    static bool strtoBool(const string strValue,bool bDefault = false)
    {
        bool toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return bDefault;
        return toret;
    }

    static int strtoInt(const string strValue,int iDefault = 0)
    {
        int toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return iDefault;
        return toret;
    }

    static float strtoFloat(const string strValue,float fDefault = 0.0)
    {
        float toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return fDefault;
        return toret;
    }

    static double strtoDouble(const string strValue,double dDefault =0.0)
    {
        double toret;
        istringstream convert( strValue );
        if ( !( ( convert >> toret) && convert.eof() ) )
            return dDefault;
        return toret;
    }

	// 去掉字符串首尾部空格
	static string trim(string &s)
	{
		string tmp = s;
		if (!tmp.empty())
		{
			tmp.erase(0, s.find_first_not_of(" "));
			tmp.erase(s.find_last_not_of(" ") + 1);
		}
		return tmp;
	}
};

#endif // STRING_COMMON_H

