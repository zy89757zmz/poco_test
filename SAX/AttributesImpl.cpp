

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



}
}