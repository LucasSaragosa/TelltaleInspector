// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SOUNDDATA
#define _SOUNDDATA

#include "DCArray.h"
#include "../Blowfish.h"

//.BANK FILES (dummy, use FSB.h for these files)
struct SoundEventBankDummy {
	Symbol mResourceName;
};

//.WAV FILES : Mainly used as file references. Holds state data, not useful here
struct SoundData {};

//.AUD FILES
struct AudioData {
	String mFilename;
	float mLength;
};

//.VOX FILES
struct VoiceData {
	bool mbEncrypted;
	float mLength;
	int mAllPacketsSize;
	int mPacketSamples;
	int mSampleRate;
	int mMode;
	DCArray<int> mPacketPositions;
	//MUST BE A MEMORY STREAM, SIZE = mAllPacketsSize
	Ptr<DataStream> mpStreamFile;
	String mStreamFromFileName;
	int mStreamFromPacketStartOffset;

	VoiceData() = default;

	~VoiceData() {
		mpStreamFile.DeleteObject();
	}

	bool GetSpeexPacket(int packet, void** pOutBufferAllocatedWithNewCharArray, u32* pOutPacketSize) {
		int nextSize = 0;
		if (packet < mPacketPositions.mSize - 1)
			nextSize = mPacketPositions[packet + 1];
		else
			nextSize = mAllPacketsSize;
		int packetSize = nextSize - mPacketPositions[packet];
		*pOutPacketSize = packetSize;
		char* pBuffer = new char[packetSize];
		*(char**)(pOutBufferAllocatedWithNewCharArray) = pBuffer;
		mpStreamFile.mpData->SetPosition(mPacketPositions[packet], DataStreamSeekType::eSeekType_Begin);
		mpStreamFile.mpData->Serialize(pBuffer, packetSize);
		if (mbEncrypted && !(packet % 64) && packetSize > 8) {
			TelltaleToolLib_BlowfishDecrypt((unsigned char*)pBuffer, packetSize, 
				sBlowfishKeys[sSetKeyIndex].isNewEncryption, (unsigned char*)sBlowfishKeys[sSetKeyIndex].game_key);
		}
		return true;
	}

	static inline METAOP_FUNC_IMPL__(SerializeAsync) {
		MetaStream* pStream = (MetaStream*)pUserData;
		VoiceData* pThis = (VoiceData*)pObj;
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj, pObjDescription, pContextDescription, pUserData);
		if (r == eMetaOp_Succeed) {
			if (pStream->mMode == MetaStreamMode::eMetaStream_Read) {
				pThis->mpStreamFile.DeleteObject();
				DataStreamMemory* pMemoryStream = nullptr;
				pThis->mpStreamFile.mpData = pMemoryStream = new DataStreamMemory(pThis->mAllPacketsSize, pThis->mAllPacketsSize);
				pStream->serialize_bytes(pMemoryStream->mMemoryBuffer, pThis->mAllPacketsSize);
			}
			else {
				if (pThis->mpStreamFile.mpData) {
					DataStreamMemory* pMemoryStream = static_cast<DataStreamMemory*>(pThis->mpStreamFile.mpData);
					pStream->serialize_bytes(pMemoryStream->mMemoryBuffer, pThis->mAllPacketsSize);
				}
			}
		}
		return r;
	}

};

#endif