#ifndef GARRYSMOD_LUA_SOURCECOMPAT_H
#define GARRYSMOD_LUA_SOURCECOMPAT_H

#ifdef GMOD_USE_SOURCESDK
    #include "mathlib/vector.h"
    #include "color.h"
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
