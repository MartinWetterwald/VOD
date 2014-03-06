#ifndef _VOD_PARSING_UTILS_HPP_
#define _VOD_PARSING_UTILS_HPP_

#include <fstream>
#include <sstream>

namespace Vod
{
    inline std::istream & customGetLine ( std::istream & is, std::string & str )
    {
        std::getline ( is, str );
        if ( str.size ( ) > 0 && str [ str.size ( ) - 1 ] == '\r' )
        {
            str.erase ( str.size ( ) - 1 );
        }

        return is;
    }
}

#define VOD_MIN_PORT 1
#define VOD_MAX_PORT 65535

#define Vod_parse(LINE,VAR,RETURN) if ( ! std::getline ( f, tmp, ':' ) )\
{\
    std::cerr << "Missing ':' at line " << LINE << std::endl;\
    return RETURN;\
}\
if ( tmp != VAR )\
{\
    std::cerr << "Missing '" << VAR << "' at line " << LINE << std::endl;\
    return RETURN;\
}\
if ( f.get ( ) != ' ' )\
{\
    std::cerr << "Missing space after '" << VAR << ":' at line " << LINE << std::endl;\
    return RETURN;\
}\
if ( ! customGetLine ( f, tmp ) )\
{\
    std::cerr << "Unable to read stream " << VAR << " after '" << VAR << ": ' at line " << LINE << std::endl;\
    return RETURN;\
}

#define Vod_parseNumber(LINE,VAR,RETURN,MIN,MAX) Vod_parse(LINE,VAR,RETURN)\
char * endptr;\
unsigned long tmpNumber = strtoul ( tmp.c_str ( ), &endptr, 10 );\
if ( * endptr != '\0' || tmpNumber < MIN || tmpNumber > MAX )\
{\
    std::cerr << "Invalid stream " << VAR << " after '" << VAR << ": ' at line " << LINE << std::endl;\
    return RETURN;\
}

#endif
