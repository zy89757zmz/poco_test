

#ifndef XML_Name_INCLUDED
#define XML_Name_INCLUDED

#include "XMLString.h"

namespace Poco {
namespace XML {

class XML_API Name
{
public:
    Name();

    Name(const XMLString& qname);

    Name(const XMLString& qname, const XMLString& namespaceURI);

    Name(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName);

    Name(const Name& name);

    Name(Name&& name) noexcept;

    ~Name();

    Name& operator = (const Name& name);

    Name& operator = (Name&& name) noexcept;

    void swap(Name& name) noexcept;

    void assign(const XMLString& qname);

    void assign(const XMLString& qname, const XMLString& namespaceURI);

    void assign(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName);

    bool equals(const Name& name) const;

    bool equals(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName) const;

    bool equalsWeakly(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName) const;

    const XMLString& qname() const;

    const XMLString& namespaceURI() const;

    const XMLString& localName() const;

    XMLString prefix() const;

    static void split(const XMLString& qname, XMLString& prefix, XMLString& localName);

    static XMLString localName(const XMLString& qname);

    static XMLString prefix(const XMLString& qname);

    static const XMLString EMPTY_NAME;

private:
    XMLString _qname;
    XMLString _namespaceURI;
    XMLString _localName;
};

inline const XMLString& Name::qname() const
{
    return _qname;
}

inline const XMLString& Name::namespaceURI() const
{
    return _namespaceURI;
}

inline const XMLString& Name::localName() const
{
    return _localName;
}

inline void swap(Name& n1, Name& n2) noexcept
{
    n1.swap(n2);
}

}
}


#endif