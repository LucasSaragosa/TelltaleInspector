// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _AMAP
#define _AMAP

#include "Map.h"
#include "Set.h"
#include "../HashManager.h"
#include "../Meta.hpp"

//.AMAP FILES
struct AgentMap {

	constexpr static Symbol const mszFolder = Symbol("ActorMaps");
	constexpr static Symbol const mszPropertyDefaultValue = Symbol("_agents.amap");
	constexpr static Symbol const kszPropertyKey = Symbol("Agent Map");

	struct AgentMapEntry {
		String mzName;
		String mzActor;
		Set<String> mazModels;
		Set<String> mazGuides;
		Set<String> mazStyleIdles;
	};

	Map<String, AgentMapEntry> maAgents;

	unsigned int GetCRC() {
		unsigned int crc = 0;
		for (auto it = maAgents.begin(); it != maAgents.end(); it++) {
			crc = CRC32(crc, it->second.mzName.c_str(), it->second.mzName.size());
			crc = CRC32(crc, it->second.mzActor.c_str(), it->second.mzActor.size());
			for (auto it2 = it->second.mazModels.begin(); it2 != it->second.mazModels.end(); it2++) {
				crc = CRC32(crc, it2->c_str(), it2->size());
			}
		}
		return crc;
	}

};

#endif