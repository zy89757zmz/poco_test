


#ifndef XML_NamePool_INCLUDED
#define XML_NamePool_INCLUDED

#include "Name.h"

#ifndef POCO_XML_NAMEPOOL_DEFAULT_SIZE
#define POCO_XML_NAMEPOOL_DEFAULT_SIZE 509

#endif

namespace Poco {
namespace XML {

class NamePoolItem;

class XML_API NamePool
{
public:
    NamePool(unsigned long size = POCO_XML_NAMEPOOL_DEFAULT_SIZE);

    const Name& insert(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName);

    const Name& insert(const Name& name);

    void duplicate();

    void release();

protected:
    unsigned long hash(const XMLString& qname, const XMLString& namespaceURI, const XMLString& localName);
    ~NamePool();

private:
    NamePool(const NamePool&);
    NamePool& operator = (const NamePool&);

    NamePoolItem* _pItems;
    unsigned long _size;
    unsigned long _salt;
    int _rc;
};


}
}



#endif