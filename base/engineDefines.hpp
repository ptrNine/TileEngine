#ifndef ENGINE_DEFINES_HPP
#define ENGINE_DEFINES_HPP

#define FILESYSTEM_FILE			"filesystem"

#define CFG_SYSTEM_DEBUG
#define SHARED_DEBUG


// OS Macro
#ifdef __linux__
    #define __SYSTEM_LINUX
#elif defined (_WIN32)
    #define __SYSTEM_WINDOWS
#elif
    #error "Unsupported OS."
#endif


// Endianess macro
#ifdef __SYSTEM_WIDOWS
    #if defined(__i386__) || defined(_M_IX86) || defined(_M_IX64) || defined(__x86__)
    #define _LITTLE_ENDIAN_
    #else
    #define _BIG_ENDIAN_
    #endif
#elif defined(__SYSTEM_LINUX)
    #if defined(__LITTLE_ENDIAN)
        #define _LITTLE_ENDIAN_
    #elif defined(__BIG_ENDIAN_)
        #define _BIG_ENDIAN_
    #endif
#endif

// SSE_Extension macro
#if defined(_M_IX86)
#define SSE_EXTENSION_ENABLE
#endif // defined(_M_IX86)



//#define DEBUG


#endif // !ENGINE_DEFINES_HPP