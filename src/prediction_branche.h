#ifndef PREDICTION_BRANCHE_H
#define PREDICTION_BRANCHE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Macros de prédiction de branche.
 * Utilisées pour optimiser les performances sur les chemins critiques (ex: LIKELY(gs)).
 */

/* GCC / Clang */
#if defined(__GNUC__) || defined(__clang__)

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

/* Visual Studio (MSVC) */
#elif defined(_MSC_VER)

#define LIKELY(x) (x)
#define UNLIKELY(x) (x)

/* Autres compilateurs */
#else

#define LIKELY(x) (x)
#define UNLIKELY(x) (x)

#endif

#ifdef __cplusplus
}
#endif

#endif /* PREDICTION_BRANCHE_H */