// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _REVERB
#define _REVERB

//.REVERB FILES
struct SoundReverbDefinition {

	bool mbEnabled;
	float mfRoomEffectLevel;
	float mfRoomEffectLevelHighFrequency;
	float mfRoomEffectLevelLowFrequency;
	float mfDecayTime;
	float mfDecayHighFrequencyRatio;
	float mfReflections;
	float mfReflectionsDelay;
	float mfReverb;
	float mfReverbDelay;
	float mfHighFrequencyReference;
	float mfLowFrequencyReference;
	float mfDiffusion;
	float mfDensity;

};

#endif
