#ifndef LS_LOGGING_H
#define LS_LOGGING_H


#include <stdarg.h>


#ifndef LS_TYPES_INCLUDED
#define LS_TYPES_INCLUDED

	#include <stdint.h>	

	typedef uint8_t		ls_bool_t;

	#define LS_TRUE		1
	#define LS_FALSE	0

	typedef uint8_t		ls_u8_t;
	typedef int8_t		ls_i8_t;
	typedef uint16_t	ls_u16_t;
	typedef int16_t		ls_i16_t;
	typedef uint32_t	ls_u32_t;
	typedef int32_t		ls_i32_t;
	typedef uint64_t	ls_u64_t;
	typedef int64_t		ls_i64_t;

	typedef float       ls_f32_t;
	typedef double      ls_f64_t;

	typedef void 	 *  ls_void_p;
	typedef ls_u64_t *	ls_u64_p;

	typedef ls_u32_t	ls_result_t;

	#define LS_NULL		0

#endif

#ifndef LS_MACROS_INCLUDED
#define LS_MACROS_INCLUDED

	#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && (defined(__GNUC__) || defined(__clang__))
		#define LS_INLINE inline __attribute__((always_inline, unused))
	#else
		#define LS_INLINE
	#endif

	#if defined(__GNUC__) || defined(__clang__)
		#define LS_USED __attribute__((unused))
	#else
		#define LS_USED
	#endif

	#define LS_CAST(v, t) ((t) (v))

	#define LS_FLOOR_LOG2(n) (63 - __builtin_clzll(n))
	#define LS_CEIL_LOG2(n) (64 - __builtin_clzll((n) - 1))

    #define LS_ROUND_DOWN_TO(n, m) ((n) - ((n) % (m))) 	  		/* rounds n down to nearest multiple of m, integers only */
	#define LS_ROUND_UP_TO(n, m) (((n) + (m) - 1) / (m) * (m))  /* rounds n up to nearest multiple of m, integers only */

    #include <stdlib.h>
	#include <string.h>

	#define LS_EXIT 	exit
	#define LS_MEMSET 	memset
	#define LS_MEMCPY 	memcpy

    #define LS_SUCCESS  0
    #define LS_FAIL     1
    
#endif


#ifndef LS_NO_SHORT_NAMES

	#define INLINE 		LS_INLINE
	#define USED		LS_USED

    #define bool_t 		ls_bool_t

	#define TRUE		LS_TRUE
	#define FALSE		LS_FALSE

	#define u8_t	    ls_u8_t
	#define i8_t        ls_i8_t
	#define u16_t	    ls_u16_t
	#define i16_t       ls_i16_t
	#define u32_t	    ls_u32_t
	#define i32_t       ls_i32_t
	#define u64_t	    ls_u64_t
	#define i64_t       ls_i64_t

	#define f32_t       ls_f32_t
	#define f64_t       ls_f64_t

	#define void_p      ls_void_p
	#define u64_p       ls_u64_p

	#define result_t    ls_result_t

	#define INLINE      LS_INLINE
	#define USED        LS_USED

	#define CAST        LS_CAST

	#define FLOOR_LOG2  LS_FLOOR_LOG2
	#define CEIL_LOG2   LS_CEIL_LOG2

    #define ROUND_DOWN_TO   LS_ROUND_DOWN_TO
	#define ROUND_UP_TO     LS_ROUND_UP_TO

	#define EXIT        LS_EXIT
	#define MEMSET      LS_MEMSET
	#define MEMCPY      LS_MEMCPY

    #define SUCCESS     LS_SUCCESS
    #define FAIL        LS_FAIL

#endif


#ifndef LS_LOGGER_NO_SHORT_NAMES

    #define logger_s        ls_logger_s
    #define logger_init     ls_logger_init
    #define set_logger_out  ls_set_logger_out
    #define log             ls_logger_log

#endif


typedef struct
{
    FILE *out;
    char  prefix[64];
}
ls_logger_s;


#define ls_logger_init(   prefix,   out_file) { out_file, prefix }
#define ls_set_logger_out(logger,   out_file) ((logger).out = out_file);

INLINE void ls_logger_log(ls_logger_s logger, const char *format, ...)
{
    va_list arg_v;

    if (logger.out == NULL)
        return;

    va_start(arg_v, format);

    fprintf(logger.out, "%s: ", logger.prefix);
    vfprintf(logger.out, format, arg_v);
    fprintf(logger.out, "\n");

    va_end(arg_v);
}

#endif  /* #ifndef LS_LOGGING_H */
