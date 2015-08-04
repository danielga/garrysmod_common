#pragma once

#ifdef ENTITY

#undef ENTITY

#endif 

#ifdef VECTOR

#undef VECTOR

#endif 

namespace GarrysMod 
{
	namespace Lua
	{
		namespace Type
		{
			enum
			{
				INVALID = -1,
				NIL, 
				BOOL,
				LIGHTUSERDATA,
				NUMBER, 
				STRING, 
				TABLE,
				FUNCTION,
				USERDATA,
				THREAD,
				ENTITY, 
				VECTOR, 
				ANGLE,
				PHYSOBJ,
				SAVE,
				RESTORE,
				DAMAGEINFO,
				EFFECTDATA,
				MOVEDATA,
				RECIPIENTFILTER, // Server
				USERCMD,
				SCRIPTEDVEHICLE,
				MATERIAL,
				PANEL, // Client
				PARTICLE, // Client
				PARTICLEEMITTER, // Client
				TEXTURE,
				USERMSG, // Client
				CONVAR,
				IMESH, // Client
				MATRIX,
				SOUND,
				PIXELVISHANDLE, // Client
				DLIGHT, // Client
				VIDEO, // Client
				FILE,
				LUALOCOMOTION, // Server
				PATHFOLLOWER, // Server
				NAVAREA, // Server
				GMODAUDIOCHANNEL, // Client

				COUNT
			};

			static const char *Name[] = 
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
				"Entity", // aka Vehicle, NPC, Weapon, Player - Server: NextBot - Client: CSEnt
				"Vector",
				"Angle",
				"PhysObj",
				"ISave",
				"IRestore",
				"CTakeDamageInfo",
				"CEffectData",
				"CMoveData",
				"CRecipientFilter", // Server
				"CUserCmd",
				"Unknown",
				"IMaterial",
				"Panel", // Client
				"CLuaParticle", // Client
				"CLuaEmitter", // Client
				"ITexture",
				"bf_read", // Client
				"ConVar",
				"IMesh", // Client
				"VMatrix",
				"CSoundPatch",
				"pixelvis_handle_t", // Client
				"dlight_t", // Client
				"IVideoWriter", // Client
				"File",
				"CLuaLocomotion", // Server
				"PathFollower", // Server
				"CNavArea", // Server
				"IGModAudioChannel", // Client

				// No MetaIDs: Vector2, Color, Quaternion

				0
			};
		}
	}
}
