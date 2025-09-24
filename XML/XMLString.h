

#ifndef XML_XMLString_INCLUDED
#define XML_XMLString_INCLUDED

#include "XML.h"
#include <string>

namespace Poco {
namespace XML {

    // Characters are UTF-8 encoded
    using XMLChar = char;
    using XMLString = std::string;

    inline const std::string& fromXMLString(const XMLString& str)
    {
        return str;
    }

    inline const XMLString& toXMLString(const std::string& str)
    {
        return str;
    }

    #define XML_LIT(lit) lit
}
}

#endif