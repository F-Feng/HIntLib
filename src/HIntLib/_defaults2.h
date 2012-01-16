
#ifdef __GNUG__
  #define GNU_NORETURN __attribute((__noreturn__))
  #define GNU_CONST __attribute((__const))
#else
  #define GNU_NORETURN
  #define GNU_CONST
#endif


/**
 *  real
 *
 *  This is the type used for normal floating point operations.
 *
 *  By default, double is used.  float or long double can be used altermatively.
 */

#ifndef HINTLIB_REAL
   #define HINTLIB_REAL 2
#endif

#if HINTLIB_REAL == 1
  typedef float real;
#elif HINTLIB_REAL == 2
  typedef double real;
#elif HINTLIB_REAL == 3
  typedef long double real;
#else
  #error "Value of HINTLIB_REAL is invalid.\nIt must be either 1 (float), 2 (double), or 3 (long double)!"
#endif


/**
 *  u32, u64
 *
 *  Shorthand for an unsigned integer with at least 32 and 64 bits
 *
 *  If unsigned and u32 do not refere to the same type,
 *     HINTLIB_UNSIGNED_NOT_EQUAL_U32 ist set.
 *  If u32 and u64 do not refere to the same type,
 *     HINTLIB_U32_NOT_EQUAL_U64 ist set.
 *  If unsigned and u64 do not refere to the same type,
 *     HINTLIB_UNSIGNED_NOT_EQUAL_U32 or HINTLIB_U32_NOT_EQUAL_U64 ist set.
 */

#if SIZEOF_UNSIGNED_INT >= 4
   #undef HINTLIB_UNSIGNED_NOT_EQUAL_U32
   typedef unsigned u32;
#else
#if SIZEOF_UNSIGNED_LONG_INT >= 4
   #define HINTLIB_UNSIGNED_NOT_EQUAL_U32 1
   typedef unsigned long u32;
#else
   error "unsigned long int does not have 32 bits!"
#endif
#endif

#if SIZEOF_UNSIGNED_INT >= 8
   #undef HINTLIB_U32_NOT_EQUAL_U64
   typedef unsigned u64;
#else
#if SIZEOF_UNSIGNED_LONG_INT >= 8
   #ifdef HINTLIB_UNSIGNED_NOT_EQUAL_U32
      #undef HINTLIB_U32_NOT_EQUAL_U64
   #else
      #define HINTLIB_U32_NOT_EQUAL_U64
   #endif
   typedef unsigned long u64;
#else
#ifdef HAVE_UNSIGNED_LONG_LONG_INT
   #if SIZEOF_UNSIGNED_LONG_LONG_INT >= 8
      #define HINTLIB_U32_NOT_EQUAL_U64 1
      typedef unsigned long long u64;
   #else
   #error "Can not determine an unsigned integer type with at least 64 bits!"
   #endif
#else
   #error "Can not determine an unsigned integer type with at least 64 bits!"
#endif
#endif
#endif


/**
 *  Index
 *
 *  Type for index variables of long-running loops.
 *
 *  By default, only 32 bits are guaranteed to be available.
 *  If a larger range is required, u64 has to be used instead of u32.
 */

#ifndef HINTLIB_INDEX
   #define HINTLIB_INDEX 32
#endif

#if HINTLIB_INDEX == 32
   typedef u32 Index;
#elif HINTLIB_INDEX == 64
   typedef u64 Index;
#else
   #error "Value of HINTLIB_INDEX is invalid.  It must be either 32 or 64!"
#endif


/**
 *  PRIME_TABLE_SIZE
 */

#ifndef HINTLIB_PRIME_TABLE_SIZE
   #define HINTLIB_PRIME_TABLE_SIZE 5000
#endif

}  // namespace HIntLib

#endif

