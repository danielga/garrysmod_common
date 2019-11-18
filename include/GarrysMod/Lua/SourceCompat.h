#ifndef GARRYSMOD_LUA_SOURCECOMPAT_H
#define GARRYSMOD_LUA_SOURCECOMPAT_H

#ifdef GMOD_USE_SOURCESDK
    #include <mathlib/vector.h>

#if defined __has_include && __has_include(<color.h>)
    #include <color.h>
#else
    #include <Color.h>
#endif

#else
    struct Vector
    {
        Vector()
            : x( 0.f )
            , y( 0.f )
            , z( 0.f )
        {}

        float x, y, z;
    };

    struct QAngle
    {
        QAngle()
            : x( 0.f )
            , y( 0.f )
            , z( 0.f )
        {}

        float x, y, z;
    };

    class Color
    {
    public:
        Color( )
            : r( 0 )
            , g( 0 )
            , b( 0 )
            , a( 0 )
        {}

        unsigned char r, g, b, a;
    };
#endif

#endif
