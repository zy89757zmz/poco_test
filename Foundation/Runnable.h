#ifndef Foundation_Runnable_INCLUDED
#define Foundation_Runnable_INCLUDED

#include "Foundation.h"
namespace Poco {

    class Foundation_API Runnable
    {
    public:
        Runnable() {}

        virtual ~Runnable() {}
        virtual void run() = 0;
    };

}
#endif // Foundation_Runnable_INCLUDED