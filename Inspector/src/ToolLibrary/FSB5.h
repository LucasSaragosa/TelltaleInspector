#ifndef _FMOD_SOUNDBANK_V5
#define _FMOD_SOUNDBANK_V5

#include "TelltaleToolLibrary.h"
#include "DataStream/DataStream.h"

struct FMODSample {

	std::string mName;

	int mFrequency, mChannels;

	u64 mDataOff;

	int mNumSamples, mRawSize;
	 
	u32 mVorbisCRC32;

	FMODSample(FMODSample&&) = default;
	FMODSample& operator=(FMODSample&&) = default;

	FMODSample(const FMODSample&) = default;
	FMODSample& operator=(const FMODSample&) = default;

};

struct VorbisHeader {
	u32 crc = 0;
	u32 data_size = 0;
	const void* pData;
};

extern VorbisHeader sVorbisHeaders[161];

//.BANK/.FSB5 FILES
class FMODSoundBank {

	std::vector<FMODSample> mSamples;
	DataStream* mpCachedStream;
	int mVersion = 0;
	bool bStripNameUUIDs = false;
	u64 base_off = 0;

	bool _OpenInternal(std::string& err);

public:

	inline void SetStripNameUUIDs(bool v) {
		bStripNameUUIDs = v;
	}

	DataStreamMemory* MakeAudioFile(int sampleIndex, std::string& err);

	inline int GetFSB5Version() const {
		return mVersion;
	}

	inline std::vector<FMODSample>& GetSamples() {
		return mSamples;
	}

	/*Takes ownership. Alloc with new. Must be FSB5*/
	bool Open(DataStream* pStream, std::string& error);

	void Reset();

	FMODSoundBank();

	~FMODSoundBank() {
		Reset();
	}

};

#endif