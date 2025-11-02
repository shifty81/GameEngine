/*
 * Khronos platform-specific types and definitions
 */

#ifndef __khrplatform_h_
#define __khrplatform_h_

#if defined(_WIN32) && !defined(__SCITECH_SNAP__)
#   define KHRONOS_APICALL __declspec(dllimport)
#   define KHRONOS_APIENTRY __stdcall
#   define KHRONOS_APIATTRIBUTES
#elif defined (__SYMBIAN32__)
#   define KHRONOS_APICALL IMPORT_C
#   define KHRONOS_APIENTRY
#   define KHRONOS_APIATTRIBUTES
#else
#   define KHRONOS_APICALL
#   define KHRONOS_APIENTRY
#   define KHRONOS_APIATTRIBUTES
#endif

typedef signed   char          khronos_int8_t;
typedef unsigned char          khronos_uint8_t;
typedef signed   short int     khronos_int16_t;
typedef unsigned short int     khronos_uint16_t;
typedef signed   long  int     khronos_int32_t;
typedef unsigned long  int     khronos_uint32_t;
typedef signed   long long int khronos_int64_t;
typedef unsigned long long int khronos_uint64_t;
typedef float                  khronos_float_t;

#endif /* __khrplatform_h_ */
