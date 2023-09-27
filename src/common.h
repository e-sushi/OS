#ifndef OS_COMMON_H
#define OS_COMMON_H

typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;
typedef ptrdiff_t          spt;   //signed pointer type
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef size_t             upt;   //unsigned pointer type
typedef float              f32;
typedef double             f64;
typedef s32                b32;   //sized boolean type
typedef wchar_t            wchar;

#define Kilobytes(a) (((u64)(a)) << 10)
#define Megabytes(a) (((u64)(a)) << 20)
#define Gigabytes(a) (((u64)(a)) << 30)
#define Terabytes(a) (((u64)(a)) << 40)

#define forx(var_name,iterations) for(int var_name=0; var_name<(iterations); ++var_name)
#define forx_reverse(var_name,iterations) for(int var_name=(iterations)-1; var_name>=0; --var_name)
#define fori(iterations) for(int i=0; i<(iterations); ++i)
#define fori_reverse(iterations) for(int i=(iterations)-1; i>=0; --i)

#define FORCE_INLINE inline __attribute__((always_inline))

template<typename A, typename... T> FORCE_INLINE b32
any(A matched, T... args) { return ((matched == args) || ...); }

#endif // OS_COMMON_H