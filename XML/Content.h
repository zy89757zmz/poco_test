

#ifndef XML_Content_INCLUDED
#define XML_Content_INCLUDED

namespace Poco {
namespace XML {

struct Content
    /// XML content model. C++11 enum class emulated for C++98.
	///
	///               element   characters  whitespaces  notes
	///     Empty     no          no        ignored
	///     Simple    no          yes       preserved    content accumulated
	///     Complex   yes         no        ignored
	///     Mixed     yes         yes       preserved
{
    enum value
    {
        Empty,
        Simple,
        Complex,
        Mixed
    };

    Content(value v)
        :_v(v)
    {
    }

    operator value() const
    {
        return _v;
    }

private:
    value _v;
};

}
}

#endif