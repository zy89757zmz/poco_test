

#ifndef XML_NamespaceStrategy_INCLUDED
#define XML_NamespaceStrategy_INCLUDED

#include "XMLString.h"

namespace Poco {
namespace XML {

class ContentHandler;

class XML_API NamespaceStrategy
{
public:
    virtual ~NamespaceStrategy();

    virtual void startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler) = 0;

    virtual void endElement(const XMLChar* name, ContentHandler* pContentHandler) = 0;

protected:
    static void splitName(const XMLChar* qname, XMLString& uri, XMLString& localName);
    static void splitName(const XMLChar* qname, XMLString& uri, XMLString& localName, XMLString& prefix);

    static const XMLString NOTHING;
};

class XML_API NoNamespacesStrategy: public NamespaceStrategy
{
public:
    NoNamespacesStrategy();
    ~NoNamespacesStrategy();

    void startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler);
    void endElement(const XMLChar* name, ContentHandler* pContentHandler);

private:
    XMLString _name;
    AttributesImpl _attrs;
};

class XML_API NoNamespacePrefixesStrategy: public NamespaceStrategy
{
public:
    NoNamespacePrefixesStrategy();
    ~NoNamespacePrefixesStrategy();

    void startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler);
    void endElement(const XMLChar* name, ContentHandler* pContentHandler);

private:
    XMLString _uri;
    XMLString _local;
    AttributesImpl _attrs;
};

class XML_API NamespacePrefixesStrategy: public NamespaceStrategy
{
public:
    NamespacePrefixesStrategy();
    ~NamespacePrefixesStrategy();

    void startElement(const XMLChar* name, const XMLChar** atts, int specifiedCount, ContentHandler* pContentHandler);
    void endElement(const XMLChar* name, ContentHandler* pContentHandler);

private:
    XMLString _uri;
    XMLString _local;
    XMLString _qname;
    AttributesImpl _attrs;
};


}
}

#endif