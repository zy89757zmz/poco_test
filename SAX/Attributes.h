

#ifndef SAX_Attributes_INCLUDED
#define SAX_Attributes_INCLUDED

#include "XMLString.h"

namespace Poco {
namespace XML {

class XML_API Attributes
{
public:
    virtual int getIndex(const XMLString& name) const = 0;

    virtual int getIndex(const XMLString& namespaceURI, const XMLString& localName) const = 0;

    virtual int getLength() const = 0;

    virtual const XMLString& getLocalName(int i) const = 0;

    virtual const XMLString& getQName(int i) const = 0;

    virtual const XMLString& getType(int i) const = 0;

    virtual const XMLString& getType(const XMLString& qname) const = 0;

    virtual const XMLString& getType(const XMLString& namespaceURI, const XMLString& localName) const = 0;

    virtual const XMLString& getValue(int i) const = 0;

    virtual const XMLString& getValue(const XMLString& qname) const = 0;

    virtual const XMLString& getValue(const XMLString& namespaceURI, const XMLString& localName) const = 0;

    virtual const XMLString& getURI(int i) const = 0;

protected:
    virtual ~Attributes();
};


}
}

#endif