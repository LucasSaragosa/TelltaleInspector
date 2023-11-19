// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#ifndef _ANM
#define _ANM

#include "../Meta.hpp"
#include "ToolProps.h"
#include "DCArray.h"
#include "AnimatedValueInterface.h"

//.ANM FILES
struct Animation {

	long mVersion;
	Flags mFlags;
	Symbol mName;
	float mLength;
	float mAdditiveMask;
	DCArray<AnimationValueInterfaceBase*> mValues;//not serialized/see spec. 
	ToolProps mToolProps;
	u32 mValueDataBufferSize;//no ser
	void* mpValueDataBuffer;//no ser - a runtime buffer for animation data. size is serialized. might be needed for some value types

	void AddValue(AnimationValueInterfaceBase* i) {
		u16 version = i->mVersion;
		for (int x = 0; x < mValues.GetSize(); x++) {
			if (mValues[x]->mVersion != version)
				version = mValues[x]->mVersion;
		}
		i->mVersion = version;
		mValues.AddElement(0, NULL, &i);
	}

	void _DeleteData() {
		for (int i = 0; i < mValues.GetSize(); i++) {
			MetaClassDescription* c = mValues[i]->GetMetaClassDescription();
			c->Delete(mValues[i]);
		}
		mValues.ClearElements();
		if (mpValueDataBuffer)
			free(mpValueDataBuffer);
		mValueDataBufferSize = 0;
		mpValueDataBuffer = NULL;
	}

	Animation() {
		mValueDataBufferSize = 0;
		mpValueDataBuffer = NULL;
	}

	~Animation() {
		_DeleteData();
	}

	static METAOP_FUNC_IMPL__(SerializeAsync) {
		CAST_METAOP(Animation, anm);
		MetaOpResult r = Meta::MetaOperation_SerializeAsync(pObj,
			pObjDescription, pContextDescription, pUserData);
		if (r != eMetaOp_Succeed)
			return r;
		meta->BeginBlock();
		u32 totalNumOfInterfaces = anm->mValues.GetSize();
		meta->serialize_uint32(&totalNumOfInterfaces);
		if (meta->IsWrite()) {
			if (anm->mVersion > 5 || anm->mVersion < 4) {
				TelltaleToolLib_RaiseError(
					"Can only serialize animation versions 4&5", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			meta->serialize_uint32(&anm->mValueDataBufferSize);

			struct __serialize_td {
				MetaClassDescription* _type;
				u16 _version;
				u16 _num;
			};

			DCArray<__serialize_td> types{};
			__serialize_td s{};
			bool b = false;
			for (int i = 0; i < anm->mValues.GetSize(); i++) {
				b = false;
				AnimationValueInterfaceBase* iface = anm->mValues[i];
				for (int j = 0; j < types.GetSize(); j++) {
					__serialize_td* p = types.mpStorage + j;
					if (p->_type == iface->GetMetaClassDescription()) {
						++p->_num;
						if (iface->mVersion != p->_version) {
							TelltaleToolLib_RaiseError(
								"Detected trying to serialize corrupt animation value"
								": versions mismatch (use AddValue(..))!",
								ErrorSeverity::ERR);
							return eMetaOp_Fail;
						}
						b = true;
						break;
					}
				}
				if (!b) {
					s._num = 1;
					s._type = iface->GetMetaClassDescription();
					s._version = iface->mVersion;
					types.AddElement(0, NULL, &s);
				}
			}
			u32 size = types.GetSize();
			meta->serialize_uint32(&size);
			for (int i = 0; i < size; i++) {
				__serialize_td* p = types.mpStorage + i;
				meta->serialize_uint64(&p->_type->mHash);
				meta->serialize_uint16(&p->_num);
				meta->serialize_uint16(&p->_version);
			}
			for (int i = 0; i < size; i++) {
				__serialize_td* p = types.mpStorage + i;
				for (int x = 0; x < anm->mValues.GetSize(); x++) {
					AnimationValueInterfaceBase* iface = anm->mValues[x];
					if (iface->GetMetaClassDescription() == p->_type) {
						iface->SerializeOut(meta);
					}
				}
			}
			for (int i = 0; i < size; i++) {
				__serialize_td* p = types.mpStorage + i;
				for (int x = 0; x < anm->mValues.GetSize(); x++) {
					AnimationValueInterfaceBase* iface = anm->mValues[x];
					if (iface->GetMetaClassDescription() == p->_type) {
						meta->serialize_uint32(&iface->mFlags);
					}
				}
			}
			if (anm->mValues.GetSize() > 0) {
				u16 v = 0;
				meta->serialize_uint16(&v);
				u64 crc = 0;
				for (int i = 0; i < size; i++) {
					__serialize_td* p = types.mpStorage + i;
					for (int x = 0; x < anm->mValues.GetSize(); x++) {
						AnimationValueInterfaceBase* iface = anm->mValues[x];
						if (iface->GetMetaClassDescription() == p->_type) {
							crc = iface->mName.GetCRC();
							meta->serialize_uint64(&crc);
						}
					}
				}
			}
			meta->EndBlock();
			return eMetaOp_Succeed;
		}
		else {
			MetaClassDescription* interfaceDesc = 
				GetMetaClassDescription<AnimationValueInterfaceBase>();
			if (!interfaceDesc) {
				TelltaleToolLib_RaiseError("Meta not initialized: "
					"cannot read anm: no metaclass for value interface base", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
			anm->_DeleteData();
			anm->mValues.Resize(totalNumOfInterfaces);
			if (anm->mVersion >= 4) {
				u32 dataBufferSize = 0, animValueTypes = 0;
				meta->serialize_uint32(&dataBufferSize);
				meta->serialize_uint32(&animValueTypes);//anim value type count
				char* classDescriptions = (char*)calloc(1,16 * animValueTypes);
				char* typeCounts = classDescriptions + 8 * animValueTypes;
				char* typeVersions = classDescriptions + 12 * animValueTypes;
				u64 crc = 0,total=0;
				MetaClassDescription* desc = NULL;
				u16 valuesOfType = 0, typeVersion = 0;
				for (int i = 0; i < animValueTypes; i++) {
					meta->serialize_uint64(&crc);
					desc = TelltaleToolLib_FindMetaClassDescription_ByHash(crc);
					if (!desc) {
						static char temp[100];
						sprintf(temp, 
							"Could not find anm value description SYM: %llx", crc);
						TelltaleToolLib_RaiseError(temp, ErrorSeverity::ERR);
						meta->EndBlock();
						return eMetaOp_SymbolNotFound;
					}
					meta->serialize_uint16(&valuesOfType);
					meta->serialize_uint16(&typeVersion);
					memcpy(classDescriptions + 8 * i, &desc, 8);
					memcpy(typeCounts + 4 * i, &valuesOfType, 2);
					memcpy(typeVersions + 4 * i, &typeVersion, 2);
					total += desc->mClassSize * valuesOfType;
				}
				//if (total > 0) {
				//	anm->mValueBufferSize = total;
				//	anm->mpValueBuffer = (char*)malloc(total);
				//}
				if (dataBufferSize > 0) {
					anm->mpValueDataBuffer = (char*)malloc(dataBufferSize);
					anm->mValueDataBufferSize = dataBufferSize;
					if (!anm->mpValueDataBuffer) {
						free(classDescriptions);
						TelltaleToolLib_RaiseError("Could not allocate "
							"animation value data buffers", 
							ErrorSeverity::ERR);
						meta->EndBlock();
						return eMetaOp_OutOfMemory;
					}
				}
				if (animValueTypes > 0) {
					AnimationValueSerializeContext context{ 0 };
					context.mStream = meta;
					context.mBufferOffset = 0;
					context.mBufferSize = dataBufferSize;
					context.mpBuffer = (char*)anm->mpValueDataBuffer;
					AnimationValueInterfaceBase* base = NULL;
					for (int i = 0; i < animValueTypes; i++) {
						MetaClassDescription* desc = *((MetaClassDescription**)classDescriptions + i);
						int count = ((int*)typeCounts)[i];
						int ver = ((int*)typeVersions)[i];
						for (int j = 0; j < count; j++) {
							void* instance = desc->New();
							base = (AnimationValueInterfaceBase*)desc->CastToBase(instance, interfaceDesc);
							if (!base) {
								TelltaleToolLib_RaiseError("Animation value was not a "
									"subclass of AnimationValueInterfaceBase", ErrorSeverity::ERR);
								meta->EndBlock();
								free(classDescriptions);
								return eMetaOp_Fail;
							}
							context.BeginValue();
							//r = eMetaOp_Succeed;
							base->mVersion = (u16)ver;
							r=base->SerializeIn(&context, ver);
							if (r != eMetaOp_Succeed) {
								static char temp1[350];
								sprintf(temp1, "Animation value type %s serializein failed",desc->mpTypeInfoName);
								TelltaleToolLib_RaiseError(temp1, ErrorSeverity::ERR);
								meta->EndBlock();
								free(classDescriptions);
								return eMetaOp_Fail;
							}
							anm->mValues.AddElement(0, NULL, &base);
						}
					}
				}
				free(classDescriptions);
				if (totalNumOfInterfaces > 0) {//in case the implemented types of serializein dont call the base class serializer
					for (int i = 0; i < totalNumOfInterfaces; i++) {
						AnimationValueInterfaceBase* value = *(anm->mValues.mpStorage + i);
						meta->serialize_uint32(&value->mFlags);
						if (!(value->mFlags & (AnimationValueInterfaceBase::Flags::eRuntimeAnimation | 
							AnimationValueInterfaceBase::Flags::eTransientAnimation))) {
							value->mFlags |= AnimationValueInterfaceBase::Flags::eRuntimeAnimation;
							meta->mRuntimeFlags.mFlags |= MetaStream::RuntimeFlags::eWriteback;
						}
					}
					u16 v = 0;
					meta->serialize_uint16(&v);//?? make it a boolean ffs smh
					if (!v) {
						u64 crc = 0;
						for (int i = 0; i < totalNumOfInterfaces; i++) {
							AnimationValueInterfaceBase* value = *(anm->mValues.mpStorage + i);
							meta->serialize_uint64(&crc);
							value->mName.SetCRC(crc);
						}
					}
				}
			}
			else {
				TelltaleToolLib_RaiseError("Cannot serialize Animation: version"
					" has to be greater than 4 (old versions not supported)", ErrorSeverity::ERR);
				return eMetaOp_Fail;
			}
		}
		meta->EndBlock();
		return r;
	}

};

struct AnimationConstraintParameters
{
	float mUseAnimationConstraints;
	float mUseSpineEcho;
	float mLockToAnimLeftWrist;
	float mLockToAnimRightWrist;
	float mLockToNodeLeftWrist;
	float mLockToNodeRightWrist;
	__declspec(align(16)) Transform mLockToAnimLeftWristOffset;
	Transform mLockToAnimRightWristOffset;
	Transform mLockToNodeLeftWristOffset;
	Transform mLockToNodeRightWristOffset;
	float mUseLimitConstraints;
	float mUseBoneLengthConstraints;
	Ptr<Agent> mpAgent;
};


//.EYES FILES
struct ProceduralEyes : Animation {
	char __reserve;
};

struct InverseKinematicsDerived {
	char __reserve;
};

struct InverseKinematicsBase {
	char __reserve;
};

struct ParticleInverseKinematics : InverseKinematicsBase {

};

struct InverseKinematicsAttach : InverseKinematicsDerived {

};

struct InverseKinematics : InverseKinematicsDerived {
	HandleLock<Animation> mhTargetAnimation;
};

//KNOWN TYPES TO BE SERIALIZED IN ANM
/*
* CompressedSkeletonPoseKeys
* KeyframedValue<String>
* KeyframedValue<PhonemeKey>
* CompressedSkeletonPoseKeys2
* KeyframedValue<Transform>
* KeyframedValue<Bool>
* KeyframedValue<Vector3>
* KeyframedValue<Float>
*/

#endif