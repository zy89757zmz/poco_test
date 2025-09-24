

#ifndef Foundation_Random_INCLUDED
#define Foundation_Random_INCLUDED

#include "Foundation.h"
#include "Types.h"

namespace Poco {

class Foundation_API Random
{
public:
    enum Type
    {
        RND_STATE_0 = 0,
        RND_STATE_32 = 32,
        RND_STATE_64 = 64,
        RND_STATE_128 = 128,
        RND_STATE_256 = 256
    };

    Random(int stateSize = 256);

    ~Random();

    void seed(UInt32 seed);

    void seed();

    UInt32 next();

    UInt32 next(UInt32 n);

    char nextChar();

    bool nextBool();

    float nextFloat();

    double nextDouble();

protected:
    void initState(UInt32 seed, char* arg_state, Int32 n);
    static UInt32 goodRand(Int32 x);

private:
    enum
    {
        MAX_TYPES = 5,
        NSHUFF = 50
    };

    UInt32* _fptr;
    UInt32* _rptr;
    UInt32* _state;
    int _randType;
    int _randDeg;
    int _randSep;
    UInt32* _endPtr;
    char* _pBuffer;
};

inline UInt32 Random::next(UInt32 n)
{
    return next() % n;
}

inline char Random::nextChar()
{
    return char((next() >> 3) & 0xFF);
}

inline bool Random::nextBool()
{
    return (next() & 0x1000) != 0;
}

inline float Random::nextFloat()
{
    return static_cast<float>(nextDouble());
}

inline double Random::nextDouble()
{
    return static_cast<double>(next()) / static_cast<double>(0x7FFFFFFF);
}

}



#endif