#ifndef SAX_AttributesImpl_INCLUDED
#define SAX_AttributesImpl_INCLUDED

#include "Attributes.h"
#include "Bugcheck.h"
#include <vector>

namespace Poco {
namespace XML {

class XML_API AttributesImpl: public Attributes
{
public:
    struct Attribute
    {
        XMLString localName;
        XMLString namespaceURI;
        XMLString qname;
        XMLString value;
        XMLString type;
        bool specified;
    };
    using AttributeVec = std::vector<Attribute>;
    using iterator = AttributeVec::const_iterator;

    AttributesImpl();

    AttributesImpl(const Attributes& attributes);

    AttributesImpl(const AttributesImpl& attributes);

    AttributesImpl(AttributesImpl&& attributes) noexcept;

    ~AttributesImpl();

    AttributesImpl& operator = (const AttributesImpl& attributes);

    AttributesImpl& operator = (AttributesImpl&& attributes) noexcept;

    int getIndex(const XMLString& name) const override;
    int getIndex(const XMLString& namespaceURI, const XMLString& localName) const override;
    int getLength() const override;
    const XMLString& getLocalName(int i) const override;
    const XMLString& getQName(int i) const override;
    const XMLString& getType(int i) const override;
    const XMLString& getType(const XMLString& qname) const override;
    const XMLString& getType(const XMLString& namespaceURI, const XMLString& localName) const override;
    const XMLString& getValue(int i) const override;
    const XMLString& getValue(const XMLString& qname) const override;
    const XMLString& getValue(const XMLString& namespaceURI, const XMLString& localName) const override;
    const XMLString& getURI(int i) const override;

    bool isSpecified(int i) const;
    bool isSpecified(const XMLString& qname) const;
    bool isSpecified(const XMLString& namespaceURI, const XMLString& localName) const;
    void setValue(int i, const XMLString& value);
    void setValue(const XMLString& qname, const XMLString& value);
    void setValue(const XMLString& namespaceURI, const XMLString& localName, const XMLString& value);
    void setAttributes(const Attributes& attributes);
    void setAttribute(int i, const XMLString& namespaceURI, const XMLString& localName, const XMLString& qname, const XMLString& type, const XMLString& value);
    void addAttribute(const XMLString& namespaceURI, const XMLString& localName, const XMLString& qname, const XMLString& type, const XMLString& value);
    void addAttribute(const XMLString& namespaceURI, const XMLString& localName, const XMLString& qname, const XMLString& type, const XMLString& value, bool specified);
    void addAttribute(const XMLChar* namespaceURI, const XMLChar* localName, const XMLChar* qname, const XMLChar* type, const XMLChar* value, bool specified);
    Attribute& addAttribute();
    void removeAttribute(int i);
    void removeAttribute(const XMLString& qname);
    void removeAttribute(const XMLString& namespaceURI, const XMLString& localName);
    void clear();
    void reserve(std::size_t capacity);
    void setLocalName(int i, const XMLString& localName);
    void setQName(int i, const XMLString& qname);
    void setType(int i, const XMLString& type);
    void setURI(int i, const XMLString& namespaceURI);
    iterator begin() const;
    iterator end() const;

protected:
    Attribute* find(const XMLString& qname) const;
    Attribute* find(const XMLString& namespaceURI, const XMLString& localName) const;

    struct EmptyAttribute: Attribute
    {
        EmptyAttribute();
    };

private:
    AttributeVec _attributes;
    static EmptyAttribute _empty;
};

inline AttributesImpl::iterator AttributesImpl::begin() const
{
    return _attributes.begin();
}

inline AttributesImpl::iterator AttributesImpl::end() const
{
    return _attributes.end();
}

inline AttributesImpl::Attribute& AttributesImpl::addAttribute()
{
    _attributes.push_back(_empty);
    return _attributes.back();
}

inline int AttributesImpl::getLength() const
{
    return (int) _attributes.size();
}

inline const XMLString& AttributesImpl::getLocalName(int i) const
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    return _attributes[i].localName;
}

inline const XMLString& AttributesImpl::getQName(int i) const
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    return _attributes[i].qname;
}

inline const XMLString& AttributesImpl::getType(int i) const
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    return _attributes[i].type;
}

inline const XMLString& AttributesImpl::getType(const XMLString& qname) const
{
    Attribute* pAttr = find(qname);
    if (pAttr)
        return pAttr->type;
    else
        return _empty.type;
}

inline const XMLString& AttributesImpl::getType(const XMLString& namespaceURI, const XMLString& localName) const
{
    Attribute* pAttr = find(namespaceURI, localName);
    if (pAttr)
        return pAttr->type;
    else
        return _empty.type;
}

inline const XMLString& AttributesImpl::getValue(int i) const
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    return _attributes[i].value;
}

inline const XMLString& AttributesImpl::getValue(const XMLString& qname) const
{
    Attribute* pAttr = find(qname);
    if (pAttr)
        return pAttr->value;
    else
        return _empty.value;
}

inline const XMLString& AttributesImpl::getValue(const XMLString& namespaceURI, const XMLString& localName) const
{
    Attribute* pAttr = find(namespaceURI, localName);
    if (pAttr)
        return pAttr->value;
    else
        return pAttr->value;
}

inline const XMLString& AttributesImpl::getURI(int i) const
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    return _attributes[i].namespaceURI;
}

inline bool AttributesImpl::isSpecified(int i) const
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    return _attributes[i].specified;
}

inline bool AttributesImpl::isSpecified(const XMLString& qname) const
{
    Attribute* pAttr = find(qname);
    if (pAttr)
        return pAttr->specified;
    else
        return false;
}

inline bool AttributesImpl::isSpecified(const XMLString& namespaceURI, const XMLString& localName) const
{
    Attribute* pAttr = find(namespaceURI, localName);
    if (pAttr)
        return pAttr->specified;
    else
        return false;
}


}
}

#endif