#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

namespace Base64
{
    std::string base64_encode(const std::string &);

    const std::string &SALT1 = "LM::TB::BB";
    const std::string &SALT2 = "_:/_77";
    const std::string &SALT3 = "line=wowC++";

    std::string EncryptB64(std::string s)
    {
        s = SALT1 + s + SALT2 + SALT3;
        s = base64_encode(s);
        s.insert(7, SALT3); // insert SALT3 in the 3rd position of s
        s += SALT1;
        s = base64_encode(s);
        s += SALT2 + SALT3 + SALT1; // he has just "=" in his one (so far)
        s = base64_encode(s);
        s.insert(1, "L");
        s.insert(7, "M");

        return s;
    }

    const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string base64_encode(const std::string &s)
    {
        std::string ret;    // the output string
        int val = 0;        // index to map input to our table
        int bits = -6;      // to represent number of bits to a subgroup
        const unsigned int b63 = 0x3F;
        // "for-each" loop is new in C++ 11, you can't find it in previous C++ versions
        // "auto" when you don't know the type of the variable and want C++ to recognize it by itself
        for(const auto &c : s)
        {
            val = (val << 8) + c; // similar to: val = val * 2^8;
            bits += 8;
            while(bits >= 0)    // in the resulting string, it basically inserts the mapped number
            {
                ret.push_back(BASE64_CODES[(val >> bits) & b63]);   // & = logical "and" operation
                bits -= 6;
            }
        }

        if(bits > -6)   // so at least 1 character was inserted
        {
            ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);
        }

        while(ret.size() % 4)   // this is evaluated as a true (it's not 0) or false (it's 0)
        {
            ret.push_back('=');
        }

        return ret;
    }

}

#endif // BASE_64
