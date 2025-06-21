
#ifndef MB_EXPORT_H
#define MB_EXPORT_H

#ifdef MB_STATIC_DEFINE
#  define MB_EXPORT
#  define MB_NO_EXPORT
#else
#  ifndef MB_EXPORT
#    ifdef mbus_EXPORTS
        /* We are building this library */
#      define MB_EXPORT 
#    else
        /* We are using this library */
#      define MB_EXPORT 
#    endif
#  endif

#  ifndef MB_NO_EXPORT
#    define MB_NO_EXPORT 
#  endif
#endif

#ifndef MB_DEPRECATED
#  define MB_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MB_DEPRECATED_EXPORT
#  define MB_DEPRECATED_EXPORT MB_EXPORT MB_DEPRECATED
#endif

#ifndef MB_DEPRECATED_NO_EXPORT
#  define MB_DEPRECATED_NO_EXPORT MB_NO_EXPORT MB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MB_NO_DEPRECATED
#    define MB_NO_DEPRECATED
#  endif
#endif

#endif /* MB_EXPORT_H */
