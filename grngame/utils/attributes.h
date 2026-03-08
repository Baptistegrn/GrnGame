#pragma once

/**
 * LIKELY: This branch is likely to happen
 * UNLIKELY: This branch is unlikely to happen
 * UNREACHABLE: The execution of the code cannot reach this place
 * HOT: This function is called often (rather useless nowadays)
 * COLD: This function is not called often (rather useless nowadays)
 * FORCE_INLINE: This function should always be inlined whenever it is called
 * NO_INLINE: This function should never be inlined whenever it is called
 * PREFETCH: Load the cacheline that starts here before it's really accesses
 * PREFETCH_WRITE: Prefetch but allows writing
 * ASSUME: Tell the compiler something is true
 * FLATTEN: All calls inside this function should be inlined
 */

#if defined(__clang__)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define UNREACHABLE() __builtin_unreachable()
#define HOT __attribute__((hot))
#define COLD __attribute__((cold))
#define FORCE_INLINE __attribute__((always_inline)) inline
#define NO_INLINE __attribute__((noinline))
#define PREFETCH(ptr) __builtin_prefetch((ptr), 0, 1)
#define PREFETCH_WRITE(ptr) __builtin_prefetch((ptr), 1, 1)
#define ASSUME(x) __builtin_assume(x)
#define FLATTEN __attribute__((flatten))

#elif defined(__GNUC__)
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define UNREACHABLE() __builtin_unreachable()
#define HOT __attribute__((hot))
#define COLD __attribute__((cold))
#define FORCE_INLINE __attribute__((always_inline)) inline
#define NO_INLINE __attribute__((noinline))
#define PREFETCH(ptr) __builtin_prefetch((ptr), 0, 1)
#define PREFETCH_WRITE(ptr) __builtin_prefetch((ptr), 1, 1)
#define ASSUME(x)                                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
            __builtin_unreachable();                                                                                   \
    } while (0)
#define FLATTEN __attribute__((flatten))

#elif defined(_MSC_VER)
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#define UNREACHABLE() __assume(0)
#define HOT
#define COLD
#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)
#define PREFETCH(ptr) ((void)(ptr))
#define PREFETCH_WRITE(ptr) ((void)(ptr))
#define ASSUME(x) __assume(x)
#define FLATTEN

#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#define UNREACHABLE() ((void)0)
#define HOT
#define COLD
#define FORCE_INLINE inline
#define NO_INLINE
#define PREFETCH(ptr) ((void)(ptr))
#define PREFETCH_WRITE(ptr) ((void)(ptr))
#define ASSUME(x) ((void)0)
#define FLATTEN

#endif

/* STACK_ALLOC : stack allocation */
#ifdef _MSC_VER
#include <malloc.h>
#define STACK_ALLOC(type, n) ((type *)_alloca(sizeof(type) * (n)))
#else
#define STACK_ALLOC(type, n) ((type[n]){})
#endif