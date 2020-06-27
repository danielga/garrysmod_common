#ifndef GARRYSMOD_LUA_TYPES_H
#define GARRYSMOD_LUA_TYPES_H

#ifdef GMOD_ALLOW_OLD_TYPES
#ifdef ENTITY
#undef ENTITY
#endif

#ifdef VECTOR
#undef VECTOR
#endif
#endif

namespace GarrysMod
{
    namespace Lua
    {
        namespace Type
        {
            enum
            {
#ifdef GMOD_ALLOW_DEPRECATED
                // Deprecated: Use `None` instead of `Invalid`
                Invalid = -1,
#endif

                // Default Lua Types
                None = -1,
                Nil,
                Bool,
                LightUserData,
                Number,
                String,
                Table,
                Function,
                UserData,
                Thread,

                // GMod Types
                Entity,
                Vector,
                Angle,
                PhysObj,
                Save,
                Restore,
                DamageInfo,
                EffectData,
                MoveData,
                RecipientFilter,
                UserCmd,
                ScriptedVehicle,
                Material,
                Panel,
                Particle,
                ParticleEmitter,
                Texture,
                UserMsg,
                ConVar,
                IMesh,
                Matrix,
                Sound,
                PixelVisHandle,
                DLight,
                Video,
                File,
                Locomotion,
                Path,
                NavArea,
                SoundHandle,
                NavLadder,
                ParticleSystem,
                ProjectedTexture,
                PhysCollide,
                SurfaceInfo,

                Type_Count,

#ifdef GMOD_ALLOW_OLD_TYPES
#ifdef GMOD_ALLOW_DEPRECATED
                // Deprecated: Use NONE instead of INVALID
                INVALID = Invalid,
#endif

                // Lua Types
                NONE = None,
                NIL = Nil,
                BOOL = Bool,
                LIGHTUSERDATA = LightUserData,
                NUMBER = Number,
                STRING = String,
                TABLE = Table,
                FUNCTION = Function,
                USERDATA = UserData,
                THREAD = Thread,

                // GMod Types
                ENTITY = Entity,
                VECTOR = Vector,
                ANGLE = Angle,
                PHYSOBJ = PhysObj,
                SAVE = Save,
                RESTORE = Restore,
                DAMAGEINFO = DamageInfo,
                EFFECTDATA = EffectData,
                MOVEDATA = MoveData,
                RECIPIENTFILTER = RecipientFilter,
                USERCMD = UserCmd,
                SCRIPTEDVEHICLE = ScriptedVehicle,
                MATERIAL = Material,
                PANEL = Panel,
                PARTICLE = Particle,
                PARTICLEEMITTER = ParticleEmitter,
                TEXTURE = Texture,
                USERMSG = UserMsg,
                CONVAR = ConVar,
                IMESH = IMesh,
                MATRIX = Matrix,
                SOUND = Sound,
                PIXELVISHANDLE = PixelVisHandle,
                DLIGHT = DLight,
                VIDEO = Video,
                FILE = File,
                LOCOMOTION = Locomotion,
                PATH = Path,
                NAVAREA = NavArea,
                SOUNDHANDLE = SoundHandle,
                NAVLADDER = NavLadder,
                PARTICLESYSTEM = ParticleSystem,
                PROJECTEDTEXTURE = ProjectedTexture,
                PHYSCOLLIDE = PhysCollide,

                COUNT = Type_Count
#endif
            };

#if ( defined( GMOD ) || defined( GMOD_ALLOW_DEPRECATED ) )
            // You should use ILuaBase::GetTypeName instead of directly accessing this array
            static const char* Name[] =
            {
                "nil",
                "bool",
                "lightuserdata",
                "number",
                "string",
                "table",
                "function",
                "userdata",
                "thread",
                "entity",
                "vector",
                "angle",
                "physobj",
                "save",
                "restore",
                "damageinfo",
                "effectdata",
                "movedata",
                "recipientfilter",
                "usercmd",
                "vehicle",
                "material",
                "panel",
                "particle",
                "particleemitter",
                "texture",
                "usermsg",
                "convar",
                "mesh",
                "matrix",
                "sound",
                "pixelvishandle",
                "dlight",
                "video",
                "file",
                "locomotion",
                "path",
                "navarea",
                "soundhandle",
                "navladder",
                "particlesystem",
                "projectedtexture",
                "physcollide",
                "surfaceinfo",
                nullptr
            };
#endif
        }
    }
}

#endif
