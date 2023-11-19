// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _IMAP
#define _IMAP

#include "../Meta.hpp"
#include "../TelltaleToolLibrary.h"
#include "DCArray.h"

//.IMAP FILES
struct InputMapper {

	struct RawEvent {
		int mKey;
		int mType;
		int mX;
		int mY;
		int mController;
		DCArray<InputMapper*> mIMAPFilter;
	};

	enum EventType {
		eBeginEvent = 0,
		eEndEvent = 1,
		eMouseMove = 2,
		eEitherEvent = 3,
		eForceDword = 0xFFFFFFFF
	};

	struct EventMapping {
		int mInputCode;
		EventType mEvent;
		String mScriptFunction;
		int mControllerIndexOverride;
	};

	String mName;
	DCArray<EventMapping> mMappedEvents;

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, MetaMemberDescription* c, void* pUserData) {
		return Meta::MetaOperation_SerializeAsync(pObj, pObjDesc, c, pUserData);//dont ask, telltale did it not me 
	}

};

struct PlatformInputMapper {

	struct EventMapping {
		int mPlatformInputCode;
		int mInputCode;
	};

	String mName;
	DCArray<EventMapping> mMappedEvents;
	TTBitArray<4096, unsigned __int64> mKeyFlags, mPlatformKeyFlags;//512 byte buf

	static MetaOpResult MetaOperation_SerializeAsync(void* pObj, MetaClassDescription* pObjDesc, MetaMemberDescription* c, void* pUserData) {
		return Meta::MetaOperation_SerializeAsync(pObj, pObjDesc, c, pUserData);//dont ask, telltale did it not me 
	}

};

#endif