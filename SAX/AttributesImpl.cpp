

#include "AttributesImpl.h"

namespace Poco {
namespace XML {

AttributesImpl::EmptyAttribute::EmptyAttribute()
{
    specified = false;
    type = XML_LIT("CDATA");
}

AttributesImpl::EmptyAttribute AttributesImpl::_empty;

AttributesImpl::AttributesImpl()
{
}

AttributesImpl::AttributesImpl(const Attributes& attributes)
{
    setAttributes(attributes);
}

AttributesImpl::AttributesImpl(const AttributesImpl& attributes):
    _attributes(attributes._attributes)
{
}

AttributesImpl::AttributesImpl(AttributesImpl&& attributes) noexcept:
    _attributes(std::move(attributes._attributes))
{
}

AttributesImpl::~AttributesImpl()
{
}

AttributesImpl& AttributesImpl::operator = (const AttributesImpl& attributes)
{
    if (&attributes != this)
    {
        _attributes = attributes._attributes;
    }
    return *this;
}

AttributesImpl& AttributesImpl::operator = (AttributesImpl&& attributes) noexcept
{
    _attributes = std::move(attributes._attributes);

    return *this;
}

int AttributesImpl::getIndex(const XMLString& qname) const
{
    int i = 0;
    AttributeVec::const_iterator it;
    for (it = _attributes.begin(); it != _attributes.end(); ++it)
    {
        if (it->qname == qname)
            return i;

        ++i;
    }
    return -1;
}

int AttributesImpl::getIndex(const XMLString& namespaceURI, const XMLString& localName) const
{
    int i = 0;
    AttributeVec::const_iterator it;
    for (it = _attributes.begin(); it != _attributes.end(); ++it)
    {
        if (it->namespaceURI == namespaceURI && it->localName == localName)
            return i;
        ++i;
    }
    return -1;
}

void AttributesImpl::setValue(int i, const XMLString& value)
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    _attributes[i].value = value;
    _attributes[i].specified = true;
}

void AttributesImpl::setValue(const XMLString& qname, const XMLString& value)
{
    Attribute* pAttr = find(qname);
    if (pAttr)
    {
        pAttr->value = value;
        pAttr->specified = true;
    }
}

void AttributesImpl::setValue(const XMLString& namespaceURI, const XMLString& localName, const XMLString& value)
{
    Attribute* pAttr = find(namespaceURI, localName);
    if (pAttr)
    {
        pAttr->value = value;
        pAttr->specified = true;
    }
}

void AttributesImpl::setAttributes(const Attributes& attributes)
{
    if (&attributes != this)
    {
        int count = attributes.getLength();
        _attributes.clear();
        _attributes.reserve(count);
        for (int i = 0; i < count; i++)
        {
            addAttribute(attributes.getURI(i), attributes.getLocalName(i),attributes.getQName(i), attributes.getType(i), attributes.getValue(i));
        }
    }
}

void AttributesImpl::setAttribute(int i, const XMLString& namespaceURI, const XMLString& localName, const XMLString& qname, const XMLString& type, const XMLString& value)
{
    poco_assert (0 <= i && i < static_cast<int>(_attributes.size()));
    _attributes[i].namespaceURI = namespaceURI;
    _attributes[i].localName = localName;
    _attributes[i].qname = qname;
    _attributes[i].type = type;
    _attributes[i].value = value;
    _attributes[i].specified = true;
}



}
}