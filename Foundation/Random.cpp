

#include "Random.h"
#include "Bugcheck.h"

#include <ctime>

#define TYPE_0        0
#define BREAK_0       8
#define DEG_0         0
#define SEP_0         0

#define TYPE_1        1
#define BREAK_1       32
#define DEG_1         7
#define SEP_1         3

#define TYPE_2        2
#define BREAK_2       64
#define DEG_2         15
#define SEP_2         1

#define TYPE_3        3
#define BREAK_3       128
#define DEG_3         31
#define SEP_3         3

#define TYPE_4        4
#define BREAK_4       256
#define DEG_4         63
#define SEP_4         1


namespace Poco {

Random::Random(int stateSize)
{
    poco_assert (BREAK_0 <= stateSize && stateSize <= BREAK_4);

    _pBuffer = new char[stateSize];

    initState((UInt32) std::time(NULL), _pBuffer, stateSize);
}

Random::~Random()
{
    delete [] _pBuffer;
}

inline UInt32 Random::goodRand(Int32 x)
{
    Int32 hi, lo;

    if (x == 0) x = 123459876;
    hi = x / 127773;
    lo = x % 127773;
    x = 16807 * lo - 2836 * hi;
    if (x < 0) x += 0x7FFFFFFF;

    return x;
}

void Random::seed(UInt32 x)
{
    int i, lim;

    _state[0] = x;
    if (_randType == TYPE_0)
        lim = NSHUFF;
    else
    {
        for (i = 1; i < _randDeg; i++)
            _state[i] = goodRand(_state[i - 1]);
        _fptr = &_state[_randSep];
        _rptr = &_state[0];
        lim = 10 * _randDeg;
    }
    for (i = 0; i < lim; i++)
        next();
}

void Random::seed()
{
//    std::streamsize len;
//
//    if (_randType == TYPE_0)
//        len = sizeof(_state[0]);
//    else
//        len = _randDeg * sizeof(_state[0]);
//
//    RandomInputStream rstr;
//    rstr.read((char*) _state, len);
}

void Random::initState(UInt32 s, char* argState, Int32 n)
{
    UInt32* intArgState = (UInt32*) argState;

    if (n < BREAK_0)
    {
        poco_bugcheck_msg("not enough state");
        return;
    }
    if (n < BREAK_1)
    {
        _randType = TYPE_0;
        _randDeg = DEG_0;
        _randSep = SEP_0;
    }
    else if (n < BREAK_2)
    {
        _randType = TYPE_1;
        _randDeg = DEG_1;
        _randSep = SEP_1;
    }
    else if (n < BREAK_3)
    {
        _randType = TYPE_2;
        _randDeg = DEG_2;
        _randSep = SEP_2;
    }
    else if (n < BREAK_4)
    {
        _randType = TYPE_3;
        _randDeg = DEG_3;
        _randSep = SEP_3;
    }
    else
    {
        _randType = TYPE_4;
        _randDeg = DEG_4;
        _randSep = SEP_4;
    }
    _state = intArgState + 1;
    _endPtr = &_state[_randDeg];
    seed(s);
    if (_randType == TYPE_0)
        intArgState[0] = _randType;
    else
        intArgState[0] = MAX_TYPES * (int) (_rptr - _state) + _randType;
}

UInt32 Random::next()
{
    UInt32 i;
    UInt32 *f, *r;

    if (_randType == TYPE_0)
    {
        i = _state[0];
        _state[0] = i = goodRand(i) & 0x7FFFFFFF;
    }
    else
    {
        f = _fptr;
        r = _rptr;
        *f += *r;
        i = (*f >> 1) & 0x7FFFFFFF;
        if (++f >= _endPtr)
        {
            f = _state;
            ++r;
        }
        else if (++r >= _endPtr)
        {
            r = _state;
        }

        _fptr = f;
        _rptr = r;
    }

    return i;
}

}