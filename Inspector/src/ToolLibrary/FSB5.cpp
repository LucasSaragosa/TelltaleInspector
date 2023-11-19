#include "FSB5.h"

#include "../ogg/ogg.h"
#include "../vorbis/vorbisenc.h"

static u16 U16(char* a, int i)
{
	return *((u16*)(a + i));
	//return ((u32)a[i] << 0) | ((u32)a[i + 1] << 8) | ((u32)a[i + 2] << 16) | ((u32)a[i + 3] << 24);
}

void write_packets(ogg_stream_state* O, DataStreamMemory* pOut, bool bFlush) {
	ogg_page pg{};
	if (bFlush) {
		while (ogg_stream_flush(O, &pg) != 0) {
			pOut->Serialize((char*)pg.header, pg.header_len);
			pOut->Serialize((char*)pg.body, pg.body_len);
		}
	}
	else {
		while (ogg_stream_pageout(O, &pg) != 0) {
			pOut->Serialize((char*)pg.header, pg.header_len);
			pOut->Serialize((char*)pg.body, pg.body_len);
		}
	}
}

DataStreamMemory* FMODSoundBank::MakeAudioFile(int sampleIndex, std::string& err)
{
	FMODSample& sample = mSamples[sampleIndex];
	if (sample.mVorbisCRC32 == 0)
		return nullptr;
	u32 header_size;
	const void* header = nullptr;
	for (int i = 0; i < ((sizeof(sVorbisHeaders) / sizeof(VorbisHeader))); i++) {
		if (sVorbisHeaders[i].crc == sample.mVorbisCRC32) {
			header = sVorbisHeaders[i].pData;
			header_size = sVorbisHeaders[i].data_size;
		}
	}
	if (header == nullptr) {
		err = "Could not locate the vorbis header from the CRC!";
		return nullptr;
	}

	vorbis_info inf{};
	vorbis_comment com{};
	ogg_stream_state O{};
	ogg_stream_init(&O, 1);
	vorbis_info_init(&inf);
	vorbis_comment_init(&com);

	unsigned char* idheader_buf;

	DataStreamMemory* pMem = new DataStreamMemory(sample.mRawSize);

	/*write the ID header*/
	ogg_packet idhead{};
	oggpack_buffer buf{};
	oggpack_writeinit(&buf);
	oggpack_write(&buf, 1, 8);
	oggpack_write(&buf, 118, 8);
	oggpack_write(&buf, 111, 8);
	oggpack_write(&buf, 114, 8);
	oggpack_write(&buf, 98, 8);
	oggpack_write(&buf, 105, 8);
	oggpack_write(&buf, 115, 8);
	oggpack_write(&buf, 0, 32);
	oggpack_write(&buf, sample.mChannels, 8);
	oggpack_write(&buf, sample.mFrequency, 32);
	oggpack_write(&buf, 0, 32);
	oggpack_write(&buf, 0, 32);
	oggpack_write(&buf, 0, 32);
	oggpack_write(&buf, 184, 8);
	oggpack_write(&buf, 1, 8);
	idhead.bytes = oggpack_bytes(&buf);
	idhead.packet = idheader_buf = new unsigned char[idhead.bytes];
	memcpy(idheader_buf, buf.buffer, idhead.bytes);
	idhead.b_o_s = 1;
	idhead.e_o_s = 0;
	idhead.granulepos = 0;
	idhead.packetno = 0;

	/*write the comment header*/
	ogg_packet commenthead{};
	ogg_packet_clear(&commenthead);
	com.vendor = "Telltale Inspector | Vunder - Lucas Saragosa";
	com.comments = 1;
	const char* comments_ = "Exported by The Telltale Inspector";
	int len_comm_ = 35;//len of above
	com.user_comments = (char**) & comments_;
	com.comment_lengths = &len_comm_;
	vorbis_commentheader_out(&com, &commenthead);

	/*write the setup header - with the crc header*/
	ogg_packet setuphead{};
	setuphead.b_o_s = 0;
	setuphead.e_o_s = 0;
	setuphead.packet = ( unsigned char*)header;
	setuphead.bytes = header_size;
	setuphead.packetno = 2;
	setuphead.granulepos = 0;

	/*write the header packets*/
	vorbis_synthesis_headerin(&inf, &com, &idhead);
	vorbis_synthesis_headerin(&inf, &com, &commenthead);
	vorbis_synthesis_headerin(&inf, &com, &setuphead);
	ogg_stream_packetin(&O, &idhead);
	write_packets(&O, pMem, false);
	ogg_stream_packetin(&O, &commenthead);
	write_packets(&O, pMem, false);
	ogg_stream_packetin(&O, &setuphead);
	write_packets(&O, pMem, false);
	write_packets(&O, pMem, true);
	u64 packetno = setuphead.packetno;
	u64 granulepos = 0;
	u64 prevblocksz = 0;
	
	mpCachedStream->SetPosition(base_off + sample.mDataOff, DataStreamSeekType::eSeekType_Begin);

	char tmp[2];
	unsigned char* temp = (unsigned char*)calloc(1, 0x10000);
	mpCachedStream->Serialize(tmp, 2);
	u16 packetsize = U16(tmp, 0);
	while (packetsize != 0) {
		packetno++;
		mpCachedStream->Serialize((char*)temp, packetsize);
		ogg_packet pkt{};
		pkt.bytes = packetsize;
		pkt.b_o_s = 0;
		pkt.packet = temp;
		pkt.e_o_s = packetsize == 0;
		pkt.packetno = packetno;
		mpCachedStream->Serialize(tmp, 2);
		packetsize = U16(tmp, 0);
		int bsz = vorbis_packet_blocksize(&inf, &pkt);
		granulepos = (packetsize != 0) ? (granulepos + ((bsz + prevblocksz) >> 2)) : 0;
		pkt.granulepos = granulepos;
		prevblocksz = bsz;
		ogg_stream_packetin(&O, &pkt);
		write_packets(&O, pMem, false);
	}

	delete[] temp;
	delete[] idheader_buf;
	return pMem;
}

static int GetFrequencyHz(int f)
{
	switch (f)
	{
	case 1:
		return 8000;
	case 2:
		return 11000;
	case 3:
		return 11025;
	case 4:
		return 16000;
	case 5:
		return 22050;
	case 6:
		return 24000;
	case 7:
		return 32000;
	case 8:
		return 44100;
	case 9:
		return 48000;
	default:
		break;
	}
	return 0;
}

enum fsb5_mode {
	PCM8 = 1,
	PCM16 = 2,
	PCM24 = 3,
	PCM32 = 4,
	PCMFLOAT = 5,
	GCADPCM = 6,
	IMAADPCM = 7,
	VAG = 8,
	HEVAG = 9,
	XMA = 10,
	MPEG = 11,
	CELT = 12,
	AT9 = 13,
	XWMA = 14,
	VORBIS = 15
};

static u32 U32(char* a, int i)
{
	return *((u32*)(a + i));
	//return ((u32)a[i] << 0) | ((u32)a[i + 1] << 8) | ((u32)a[i + 2] << 16) | ((u32)a[i + 3] << 24);
}

static u64 U64(char* a, int i)
{
	return *((u64*)(a + i));
	//return ((u64)a[i] << 0) | ((u64)a[i + 1] << 8) | ((u64)a[i + 2] << 16) | ((u64)a[i + 3] << 24) |
	//	((u64)a[i + 4] << 32) | ((u64)a[i + 5] << 40) | ((u64)a[i + 6] << 48) | ((u64)a[i + 7] << 56);
}

static u64 BitRange(u64 value, u64 start, u64 len)
{
	return (value & ((1llu << ((start + len))) - 1llu)) >> start;
}

FMODSoundBank::FMODSoundBank() : mSamples{} {
	mpCachedStream = nullptr;
}

#define read32(var) mpCachedStream->Serialize(temp, 4); var = U32(temp, 0)
#define read64(var) mpCachedStream->Serialize(temp, 8); var = U64(temp, 0)

//reads the fsb5 section (if in riff, or plain fsb5), the payload of this class
bool FMODSoundBank::_OpenInternal(std::string& err)
{
	char temp[16];
	//fsb5 header read
	read32(mVersion);

	u32 numSamples, headerSize, ntSize;
	read32(numSamples);
	read32(headerSize);
	read32(ntSize);

	if (ntSize == 0) {
		err = "FSB5 does not contain the file names. Cannot open!";
		return false;
	}

	u32 sampleDataSize, mode;
	read32(sampleDataSize);
	read32(mode);

	//skip guids etc
	mpCachedStream->SetPosition(0x20 + (mVersion == 0 ? 4 : 0), DataStreamSeekType::eSeekType_Current);

	if (!(mode == VORBIS)) {
		err = "Only VORBIS sound banks (.ogg) are supported. The one you are opening is likely .wav/.mpeg etc. Not supported as of yet.";
		return false;
	}

	mSamples.reserve(numSamples);
	for (int i = 0; i < numSamples; i++) {
		FMODSample sample{};
		u64 sampleInf{};
		read64(sampleInf);
		bool bHasNext = BitRange(sampleInf, 0, 1) == 1;
		u32 frequency = GetFrequencyHz((int)BitRange(sampleInf, 1, 4));
		if (frequency == 0) {
			err = "Invalid frequency for sample in the bank. Likely corrupted.";
			return false;
		}
		u32 numChannels = BitRange(sampleInf, 5, 1) + 1;
		u64 off = BitRange(sampleInf, 6, 28) << 4;
		u32 numInnerSamples = BitRange(sampleInf, 34, 30);
		sample.mFrequency = frequency;
		sample.mDataOff = off;
		sample.mChannels = numChannels;
		sample.mNumSamples = numInnerSamples;
		while (bHasNext) {
			u32 chunkData{};
			read32(chunkData);
			bHasNext = BitRange(chunkData, 0, 1) == 1;
			u32 chunkSize = (u32)BitRange(chunkData, 1, 24);
			u32 chunkType = (u32)BitRange(chunkData, 25, 7);
			u32 vorbisCRC{ 0 };

			//type 11 is vorbis header hash, so thats *needed*, if the frequency is different (2) thats updated here
			if (chunkType == 11) {
				read32(vorbisCRC);
				mpCachedStream->SetPosition(chunkSize - 4, DataStreamSeekType::eSeekType_Current);//skip the rest
				sample.mVorbisCRC32 = vorbisCRC;
			}
			else if (chunkType == 2) {
				read32(frequency);
				sample.mFrequency = frequency;
				mpCachedStream->SetPosition(chunkSize - 4, DataStreamSeekType::eSeekType_Current);
			}
			else {
				mpCachedStream->SetPosition(chunkSize, DataStreamSeekType::eSeekType_Current);
			}
		}
		if (mode == 15 && sample.mVorbisCRC32 == 0) {
			err = "No vorbis header CRC32 was found in vorbis samples. Likely a corrupt sound bank.";
			return false;
		}
		mSamples.push_back(sample);
	}
	u32* tempOffsets = new u32[numSamples];
	for (int i = 0; i < numSamples; i++) {
		read32(tempOffsets[i]);
		tempOffsets[i] -= 4 * numSamples;
	}
	char* nameTab = new char[ntSize - 4 * numSamples];
	mpCachedStream->Serialize(nameTab, ntSize - 4 * numSamples);

	for (int i = 0; i < numSamples; i++) {
		mSamples[i].mName = nameTab + tempOffsets[i];
		if (bStripNameUUIDs) {
			size_t bracket = mSamples[i].mName.find_first_of('{');
			if (bracket != std::string::npos) {
				mSamples[i].mName = mSamples[i].mName.substr(0, bracket > 2 ? bracket - 1 : bracket );
			}
		}
		mSamples[i].mName += ".ogg";// TODO IF ADDING MORE FILES ENSURE TO CHANGE THIS !!!!
		u64 endoff = (i < numSamples - 1) ? mSamples[i + 1].mDataOff : mSamples[i].mDataOff + sampleDataSize;
		mSamples[i].mRawSize = (u32)(endoff - mSamples[i].mDataOff);
	}

	delete[] nameTab;
	delete[] tempOffsets;
	base_off = mpCachedStream->GetPosition();

	return true;
}

//reads the .bank file riff header
bool FMODSoundBank::Open(DataStream* pStream, std::string& err)
{
	Reset();

	const char* riff = "RIFF";
	const char* fsb5 = "FSB5";
	const char* fev = "FEV FMT ";
	const char* snd = "SND ";
	const char* lst = "LIST";

	char temp[16];
	pStream->Serialize(temp, 4);
	if (memcmp(temp, riff, 4)) {
		if (memcmp(temp, fsb5, 4)) {
			delete pStream;
			err = "Not a valid fmod sound bank, or not FSB5!";
			return false;
		}
		else {
			mpCachedStream = pStream;
			bool result = _OpenInternal(err);
			if (!result)
				Reset();
			return result;
		}
	}

	pStream->SetPosition(4, DataStreamSeekType::eSeekType_Current);// total bank RIFF size

	//skip the format chunk
	pStream->Serialize(temp, 8);
	if (memcmp(temp, fev, 8)) {
		delete pStream;
		err = "The RIFF file given is not an FMOD bank - does not contain format chunk 2nd.";
		return false;
	}
	u32 fmtChunkZ{};
	pStream->Serialize(temp, 4);
	fmtChunkZ = U32(temp, 0);
	pStream->SetPosition(fmtChunkZ, DataStreamSeekType::eSeekType_Current);

	//skip the file list chunk (hashes, guid?)
	pStream->Serialize(temp, 4);
	if (memcmp(temp, lst, 4)) {
		err = "The RIFF file given does not contain the list chunk 3rd";
		delete pStream;
		return false;
	}
	u32 lstChunkZ{};
	pStream->Serialize(temp, 4);
	lstChunkZ = U32(temp, 0);
	//if its a strings bank (contains no sound data, just ignore and say it had no files)
	if (lstChunkZ + pStream->GetPosition() >= pStream->GetSize()) {
		delete pStream;
		return true;
	}
	pStream->SetPosition(lstChunkZ, DataStreamSeekType::eSeekType_Current);


	//sound chunk, which contains the bank embedded
	pStream->Serialize(temp, 4);
	if (memcmp(temp, snd, 4)) {
		delete pStream;
		err = "The RIFF file given does not contain a sound chunk";
		return false;
	}
	u32 sndChunkZ{};
	pStream->Serialize(temp, 4);
	sndChunkZ = U32(temp, 0);
	//offset the 32 padding bytes
	pStream->SetPosition(0x20llu - (pStream->GetPosition() % 0x20), DataStreamSeekType::eSeekType_Current);

	pStream->Serialize(temp, 4);
	if (memcmp(temp, fsb5, 4)) {
		err = "Sound chunk does not contain FSB5, it may be an older version (FSB4,FSB3)!";
		delete pStream;
		return false;
	}

	mpCachedStream = pStream;
	bool result = _OpenInternal(err);
	if (!result) {
		delete mpCachedStream;
		mpCachedStream = nullptr;
	}
	return result;
}

void FMODSoundBank::Reset()
{
	if (mpCachedStream)
		delete mpCachedStream;
	mpCachedStream = nullptr;
	mSamples.clear();
	base_off = 0;
}
