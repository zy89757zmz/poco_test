#ifndef _TYPE_H
#define _TYPE_H

#include <cstdint>

#define __S32_TYPE      int

#define __STD_TYPE       typedef
#define __PID_T_TYPE     __S32_TYPE

__STD_TYPE __PID_T_TYPE __pid_t;

typedef __pid_t pid_t;

using Int64   = std::int64_t;
using UInt64  = std::uint64_t;

#endif // _TYPE_H