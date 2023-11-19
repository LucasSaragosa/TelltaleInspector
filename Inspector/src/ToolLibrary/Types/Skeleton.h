// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _SKL
#define _SKL

#include "../Meta.hpp"
#include "DCArray.h"
#include "SArray.h"
#include <vector>
#include "TRange.h"
#include "ObjectSpaceIntrinsics.h"
#include "Map.h"

enum BoneType {
	eBoneType_Hinge = 0,
	eBoneType_Ball = 1
};

//look. i didnt spell this wrong. telltale did!
struct BoneContraints {
	BoneType mBoneType; 
	Vector3 mHingeAxis; 
	SArray<TRange<float>, 3> mAxisRange;

};

//.SKL files
struct Skeleton {

	struct Entry {
		Symbol mJointName; 
		Symbol mParentName; 
		int mParentIndex; 
		Symbol mMirrorBoneName; 
		int mMirrorBoneIndex; 
		float mBoneLength; 
		Vector3 mBoneDir;
		Quaternion mBoneRotationAdjust; 
		Vector3 mLocalPos;
		Quaternion mLocalQuat;
		Transform mRestXform;
		Vector3 mGlobalTranslationScale;
		Vector3 mLocalTranslationScale;
		Vector3 mAnimTranslationScale;
		Map<Symbol, float, Symbol::CompareCRC> mResourceGroupMembership;
		BoneContraints mConstraints;
		Flags mFlags;
	};

	bool mbRestTransformsComputed;
	DCArray<Skeleton::Entry> mEntries;//only member serialized
	std::map<Symbol,std::vector<std::pair<int,float> >,std::less<Symbol>> mResourceMembershipTable;
	std::vector<int> mBoneRemap;
	Skeleton *mpBaseSkeleton;
	unsigned int mSerial;

	Skeleton() {

	}

};

struct Node {

	Symbol mName;
	Flags mFlags;
	Transform mLocalXform, mGlobalXform;

};

struct SklNodeData {
	Node mNode;
};

#endif