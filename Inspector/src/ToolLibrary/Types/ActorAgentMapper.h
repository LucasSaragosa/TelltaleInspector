// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _AAM
#define _AAM

#include "PropertySet.h"
#include "../Meta.hpp"
#include "Set.h"

//.AAM FILES
struct ActorAgentMapper {

	PropertySet mActorAgentMap;
	Set<String> mActionActors;

};

#endif