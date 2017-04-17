#ifndef GARRYSMOD_LUA_SOURCECOMPAT_H
#define GARRYSMOD_LUA_SOURCECOMPAT_H

#ifdef GMOD_USE_SOURCESDK
    #include "mathlib/vector.h"
    #include "Color.h"
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

    struct Color
    {
        Color( )
            : r( 0.f )
            , g( 0.f )
            , b( 0.f )
            , a( 0.f )
        { }

        unsigned char r, g, b, a;
    };
#endif

#endif
