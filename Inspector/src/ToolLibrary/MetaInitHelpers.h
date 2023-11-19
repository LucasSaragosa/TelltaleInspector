#pragma once


//types that use no copy should not be used in .prop since these functions are needed. .scene files use this, so NO scene should be in a prop
#define DEFINETNOCOPY(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mpVTable[0] = MetaClassDescription_Typed<Ty>::New;\
meta_##name_.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name_.mpVTable[2] = MetaClassDescription_Typed<Ty>::Construct;\
meta_##name_.mpVTable[3] = NULL;\
meta_##name_.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

#define KEYOVERRIDE(t,mem,x) meta_##t##_##mem##.mpIntrinsicKeyOverride = x;

#define DEFINET(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mpVTable[0] = MetaClassDescription_Typed<Ty>::New;\
meta_##name_.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name_.mpVTable[2] = MetaClassDescription_Typed<Ty>::Construct;\
meta_##name_.mpVTable[3] = MetaClassDescription_Typed<Ty>::CopyConstruct;\
meta_##name_.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mClassAlign = alignof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

//USE ONLY IN WALNUT APP CPP
#define DEFINET_3(name_,Ty) }++sMetaTypesCount; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mpVTable[0] = MetaClassDescription_Typed<Ty>::New;\
meta_##name_.mpVTable[1] = MetaClassDescription_Typed<Ty>::Delete;\
meta_##name_.mpVTable[2] = MetaClassDescription_Typed<Ty>::Construct;\
meta_##name_.mpVTable[3] = MetaClassDescription_Typed<Ty>::CopyConstruct;\
meta_##name_.mpVTable[4] = MetaClassDescription_Typed<Ty>::Destroy;\
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mClassAlign = alignof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();\
meta_##name_##.Initialize(typeid(Ty))

#define DEFINET2NOCOPY(name,_Ty) DEFINETNOCOPY(name,_Ty);\
meta_##name##.Initialize(typeid(_Ty))

#define DEFINET2(name,_Ty) DEFINET(name,_Ty);\
meta_##name##.Initialize(typeid(_Ty))

#define ADD(name) meta_##name##.Insert()

#define EXT(name,e) meta_##name##.mpExt = #e;

#define DEFINETABS(name_,Ty) }++sMetaTypesCount;static MetaClassDescription meta_##name_; \
if(!(meta_##name_.mFlags.mFlags & MetaFlag::Internal_MetaFlag_Initialized)){ \
meta_##name_.mClassSize = sizeof(Ty);\
meta_##name_.mpTypeInfoExternalName = typeid(Ty).name();

#define PARENT_OFFSET(parent, child) (char*)(parent*)(child*)0x100 - (char*)(child*)0x100

#define MKNAME(_var, _str) if(_var.mpTypeInfoName && _var.mbNameIsHeapAllocated){free((void*)##_var##.mpTypeInfoName);}\
_var.mpTypeInfoName = _str;\
_var.mbNameIsHeapAllocated = false;\
_var.mHash = CRC64_CaseInsensitive(0, _str);\


#define DEFINESARRAY(type,count) DEFINET(sarray_##type##_##count##, SArray<type SEP count>);\
meta_sarray_##type##_##count##.Initialize(typeid(SArray<type SEP count>));\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeAsync, SArray<type SEP count>::MetaOperation_SerializeAsync);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeMain, SArray<type SEP count>::MetaOperation_SerializeMain);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeMain);\
DEFINEM(sarray_##type##_##count##,baseclass);\
meta_sarray_##type##_##count##.mpFirstMember = &meta_sarray_##type##_##count##_baseclass;\
meta_sarray_##type##_##count##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_sarray_##type##_##count##_baseclass.mOffset = 0;\
meta_sarray_##type##_##count##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_sarray_##type##_##count##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_sarray_##type##_##count##.Insert();

#define DEFINEDCARRAY(type) DEFINET(DCArray_##type##, DCArray<type>);\
meta_DCArray_##type##.Initialize(typeid(DCArray<type>));\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeAsync, DCArray<type>::MetaOperation_SerializeAsync);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeMain, DCArray<type>::MetaOperation_SerializeMain);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeMain);\
DEFINEM(DCArray_##type##,baseclass);\
meta_DCArray_##type##.mpFirstMember = &meta_DCArray_##type##_baseclass;\
meta_DCArray_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_DCArray_##type##_baseclass.mOffset = 0;\
meta_DCArray_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_DCArray_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_DCArray_##type##.Insert();

#define DEFINEDCARRAY2(_Ty,type) DEFINET(DCArray_##type##, DCArray<_Ty>);\
meta_DCArray_##type##.Initialize(typeid(DCArray<_Ty>));\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeAsync, DCArray<_Ty>::MetaOperation_SerializeAsync);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(DCArray_##type##, eMetaOpSerializeMain, DCArray<_Ty>::MetaOperation_SerializeMain);\
meta_DCArray_##type##.InstallSpecializedMetaOperation(&meta_DCArray_##type##_eMetaOpSerializeMain);\
DEFINEM(DCArray_##type##,baseclass);\
meta_DCArray_##type##.mpFirstMember = &meta_DCArray_##type##_baseclass;\
meta_DCArray_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_DCArray_##type##_baseclass.mOffset = 0;\
meta_DCArray_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_DCArray_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_DCArray_##type##.Insert();

#define DEFINESCRIPTENUM(ENUM_) \
DEFINET(##ENUM_, ScriptEnum);\
meta_##ENUM_.Initialize(ScriptEnum::##ENUM_);\
ADDFLAGS(##ENUM_, MetaFlag::MetaFlag_ScriptEnum);\
FIRSTMEM2(##ENUM_, mCurValue, ScriptEnum, string, 0);\
ADD(##ENUM_);

#define DEFINESET(type) DEFINET(Set_##type##, Set<type>);\
meta_Set_##type##.Initialize(typeid(Set<type>));\
METAOP_CUSTOM(Set_##type##, eMetaOpSerializeAsync, Set<type>::MetaOperation_SerializeAsync);\
DEFINEM(Set_##type##,baseclass);\
meta_Set_##type##.mpFirstMember = &meta_Set_##type##_baseclass;\
meta_Set_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Set_##type##_baseclass.mOffset = 0;\
meta_Set_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Set_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Set_##type##.InstallSpecializedMetaOperation(&meta_Set_##type##_eMetaOpSerializeAsync);\
meta_Set_##type##.Insert();

#define DEFINESET_(_Ty,type) DEFINET(Set_##type##, Set<_Ty>);\
meta_Set_##type##.Initialize(typeid(Set<_Ty>));\
METAOP_CUSTOM(Set_##type##, eMetaOpSerializeAsync, Set<_Ty>::MetaOperation_SerializeAsync);\
DEFINEM(Set_##type##,baseclass);\
meta_Set_##type##.mpFirstMember = &meta_Set_##type##_baseclass;\
meta_Set_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Set_##type##_baseclass.mOffset = 0;\
meta_Set_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Set_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Set_##type##.InstallSpecializedMetaOperation(&meta_Set_##type##_eMetaOpSerializeAsync);\
meta_Set_##type##.Insert();

#define ADDFLAGS(name, flags) meta_##name.mFlags |= flags

#define DEFINESARRAY_(_Ty,type,count) DEFINET(sarray_##type##_##count##, SArray<_Ty SEP count>);\
meta_sarray_##type##_##count##.Initialize(typeid(SArray<_Ty SEP count>));\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeAsync, SArray<_Ty SEP count>::MetaOperation_SerializeAsync);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeAsync);\
METAOP_CUSTOM(sarray_##type##_##count, eMetaOpSerializeMain, SArray<_Ty SEP count>::MetaOperation_SerializeMain);\
meta_sarray_##type##_##count##.InstallSpecializedMetaOperation(&meta_sarray_##type##_##count##_eMetaOpSerializeMain);\
DEFINEM(sarray_##type##_##count##,baseclass);\
meta_sarray_##type##_##count##.mpFirstMember = &meta_sarray_##type##_##count##_baseclass;\
meta_sarray_##type##_##count##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_sarray_##type##_##count##_baseclass.mOffset = 0;\
meta_sarray_##type##_##count##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_sarray_##type##_##count##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_sarray_##type##_##count##.Insert();

#define DEFINELIST(type) DEFINET(List_##type##, List<type>);\
meta_List_##type##.Initialize(typeid(List<type>));\
METAOP_CUSTOM(List_##type##, eMetaOpSerializeAsync, List<type>::MetaOperation_SerializeAsync);\
DEFINEM(List_##type##,baseclass);\
meta_List_##type##.mpFirstMember = &meta_List_##type##_baseclass;\
meta_List_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_List_##type##_baseclass.mOffset = 0;\
meta_List_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_List_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_List_##type##.InstallSpecializedMetaOperation(&meta_List_##type##_eMetaOpSerializeAsync);\
meta_List_##type##.Insert();

#define DEFINELIST_(_Ty,type) DEFINET(List_##type##, List<_Ty>);\
meta_List_##type##.Initialize(typeid(List<_Ty>));\
METAOP_CUSTOM(List_##type##, eMetaOpSerializeAsync, List<_Ty>::MetaOperation_SerializeAsync);\
DEFINEM(List_##type##,baseclass);\
meta_List_##type##.mpFirstMember = &meta_List_##type##_baseclass;\
meta_List_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_List_##type##_baseclass.mOffset = 0;\
meta_List_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_List_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_List_##type##.InstallSpecializedMetaOperation(&meta_List_##type##_eMetaOpSerializeAsync);\
meta_List_##type##.Insert();

#define DEFINEDEQUE(type) DEFINET(Deque_##type##, Deque<type>);\
meta_Deque_##type##.Initialize(typeid(Deque<type>));\
METAOP_CUSTOM(Deque_##type##, eMetaOpSerializeAsync, Deque<type>::MetaOperation_SerializeAsync);\
DEFINEM(Deque_##type##,baseclass);\
meta_Deque_##type##.mpFirstMember = &meta_Deque_##type##_baseclass;\
meta_Deque_##type##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Deque_##type##_baseclass.mOffset = 0;\
meta_Deque_##type##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Deque_##type##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Deque_##type##.InstallSpecializedMetaOperation(&meta_Deque_##type##_eMetaOpSerializeAsync);\
meta_Deque_##type##.Insert();

#define FIRSTMEM1(parent, namestr, memberNameInStruct, memberAlias, pathToMember, typeDesc, flags) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberAlias);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##.mpFirstMember = &meta_##parent##_##memberNameInStruct##;

#define NEXTMEM1(parent, namestr, memberNameInStruct, memberAlias, pathToMember, typeDesc, flags, previousMember) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberAlias);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;


#define NEXTMEM4(parent, memberNameInStruct, pathToMember, typeDesc, flags, previousMember, minVersion, maxVersion) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = #memberNameInStruct;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;\
meta_##parent##_##memberNameInStruct##.mGameIndexVersionRange.min = minVersion;\
meta_##parent##_##memberNameInStruct##.mGameIndexVersionRange.max = maxVersion;

#define NEXTMEM(parent, namestr, memberNameInStruct, pathToMember, typeDesc, flags, previousMember) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##_##previousMember##.mpNextMember =& meta_##parent##_##memberNameInStruct##;\


#define FIRSTMEM(parent, namestr, memberNameInStruct, pathToMember, typeDesc, flags) \
DEFINEM(parent, memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpName = namestr;\
meta_##parent##_##memberNameInStruct##.mOffset = offsetof(pathToMember,memberNameInStruct);\
meta_##parent##_##memberNameInStruct##.mpMemberDesc = &meta_##typeDesc##;\
meta_##parent##_##memberNameInStruct##.mFlags |= flags;\
meta_##parent##.mpFirstMember = &meta_##parent##_##memberNameInStruct##;

#define NEXTMEM2(parent, memberNameInStruct, pathToMember, typeDesc, flags, previousMember) \
NEXTMEM(parent, #memberNameInStruct, memberNameInStruct, pathToMember, typeDesc, flags, previousMember)

#define FIRSTMEM2(parent, memberNameInStruct, pathToMember, typeDesc, flags) \
FIRSTMEM(parent, #memberNameInStruct, memberNameInStruct, pathToMember, typeDesc, flags)

#define DEFINEMAP(key,value,less) DEFINET(Map_##key##_##value##, Map<key SEP value SEP less>);\
meta_Map_##key##_##value##.Initialize(typeid(Map<key SEP value SEP less>));\
METAOP_CUSTOM(Map_##key##_##value##, eMetaOpSerializeAsync, Map<key SEP value SEP less>::MetaOperation_SerializeAsync);\
DEFINEM(Map_##key##_##value##,baseclass);\
meta_Map_##key##_##value##.mpFirstMember = &meta_Map_##key##_##value##_baseclass;\
meta_Map_##key##_##value##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Map_##key##_##value##_baseclass.mOffset = 0;\
meta_Map_##key##_##value##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Map_##key##_##value##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Map_##key##_##value##.InstallSpecializedMetaOperation(&meta_Map_##key##_##value##_eMetaOpSerializeAsync);\
meta_Map_##key##_##value##.Insert();

#define FIRSTENUM(parent,child,enumName, enumIndex, flags) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = #enumName;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##.mpEnumDescriptions = &meta_##parent##_##child##_##enumName##;

#define NEXTENUM(parent,child,enumName, enumIndex, flags, previous) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = #enumName;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##_##previous.mpNext = &meta_##parent##_##child##_##enumName##;

#define NEXTFLAG(parent,child,FlagNameStr, FlagName, FlagIndex, previous) static MetaFlagDescription meta_##parent##_##child##_##FlagName##;\
meta_##parent##_##child##_##FlagName##.mpFlagName = FlagNameStr;\
meta_##parent##_##child##_##FlagName##.mFlagValue = FlagIndex;\
meta_##parent##_##child##_##previous.mpNext = &meta_##parent##_##child##_##FlagName##;

#define FIRSTFLAG(parent,child,FlagNameStr, FlagName, FlagIndex) static MetaFlagDescription meta_##parent##_##child##_##FlagName##;\
meta_##parent##_##child##_##FlagName##.mpFlagName = FlagNameStr;\
meta_##parent##_##child##_##FlagName##.mFlagValue = FlagIndex;\
meta_##parent##_##child##.mpFlagDescriptions = &meta_##parent##_##child##_##FlagName##;

#define FIRSTENUM2(parent,child,enumNameStr, enumName, enumIndex, flags) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = enumNameStr;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##.mpEnumDescriptions = &meta_##parent##_##child##_##enumName##;

#define NEXTENUM2(parent,child,enumNameStr, enumName, enumIndex, flags, previous) static MetaEnumDescription meta_##parent##_##child##_##enumName##;\
meta_##parent##_##child##_##enumName##.mpEnumName = enumNameStr;\
meta_##parent##_##child##_##enumName##.mEnumIntValue = enumIndex;\
meta_##parent##_##child##_##enumName##.mFlags |= flags;\
meta_##parent##_##child##_##previous.mpNext = &meta_##parent##_##child##_##enumName##;

#define DEFINEMAP2(key,value,key_var_name, value_var_name,less) DEFINET(Map_##key_var_name##_##value_var_name##, Map<key SEP value SEP less>);\
meta_Map_##key_var_name##_##value_var_name##.Initialize(typeid(Map<key SEP value SEP less>));\
METAOP_CUSTOM(Map_##key_var_name##_##value_var_name##, eMetaOpSerializeAsync, Map<key SEP value SEP less>::MetaOperation_SerializeAsync);\
DEFINEM(Map_##key_var_name##_##value_var_name##,baseclass);\
meta_Map_##key_var_name##_##value_var_name##.mpFirstMember = &meta_Map_##key_var_name##_##value_var_name##_baseclass;\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mpName = "Baseclass_ContainerInterface";\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mOffset = 0;\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mpMemberDesc = &meta_cinterface;\
meta_Map_##key_var_name##_##value_var_name##_baseclass.mFlags |= (int)MetaFlag::MetaFlag_BaseClass;\
meta_Map_##key_var_name##_##value_var_name##.InstallSpecializedMetaOperation(&meta_Map_##key_var_name##_##value_var_name##_eMetaOpSerializeAsync);\
meta_Map_##key_var_name##_##value_var_name##.Insert();

#define DEFINEHANDLE(name_,Ty) DEFINET(Handle##name_, Handle<Ty>) \
meta_Handle##name_##.Initialize(typeid(Handle<Ty>));\
meta_Handle##name_##.mFlags |= 0x20004;\
DEFINEM(Handle##name_, handlebase);\
meta_Handle##name_##_handlebase.mpName = "Baseclass_HandleBase";\
meta_Handle##name_##_handlebase.mOffset = 0;\
meta_Handle##name_##_handlebase.mpMemberDesc = &meta_handlebase;\
meta_Handle##name_##_handlebase.mFlags |= 0x10;\
meta_Handle##name_##.mpFirstMember = &meta_Handle##name_##_handlebase;\
METAOP_CUSTOM(Handle##name_,eMetaOpSerializeAsync, Handle<Ty>::MetaOperation_SerializeAsync);\
meta_Handle##name_##.InstallSpecializedMetaOperation(&meta_Handle##name_##_eMetaOpSerializeAsync);\
meta_Handle##name_##.Insert();\
DEFINEHANDLELOCK(name_,Ty);\
DEFINEDCARRAY2(Handle<Ty>,meta_Handle##name_);\
MKNAME(meta_Handle##name_, "Handle<" #Ty ">");\
MKNAME(meta_DCArray_##meta_Handle##name_, "DCArray<Handle<" #Ty ">>");\

#define SERIALIZER(name,_Ty) \
METAOP_CUSTOM(name, eMetaOpSerializeAsync, _Ty##::MetaOperation_SerializeAsync);\
meta_##name##.InstallSpecializedMetaOperation(&meta_##name##_eMetaOpSerializeAsync)

#define DEFINEHANDLELOCK(name_,Ty) DEFINET(HandleLock##name_, HandleLock<Ty>) \
meta_HandleLock##name_##.Initialize(typeid(HandleLock<Ty>));\
meta_HandleLock##name_##.mFlags |= 0x20004;\
DEFINEM(HandleLock##name_, handlebase);\
meta_HandleLock##name_##_handlebase.mpName = "Baseclass_HandleBase";\
meta_HandleLock##name_##_handlebase.mOffset = 0;\
meta_HandleLock##name_##_handlebase.mpMemberDesc = &meta_handlebase;\
meta_HandleLock##name_##_handlebase.mFlags |= 0x10;\
meta_HandleLock##name_##.mpFirstMember = &meta_HandleLock##name_##_handlebase;\
METAOP_CUSTOM(HandleLock##name_,eMetaOpSerializeAsync, HandleLock<Ty>::MetaOperation_SerializeAsync);\
meta_HandleLock##name_##.InstallSpecializedMetaOperation(&meta_HandleLock##name_##_eMetaOpSerializeAsync);\
meta_HandleLock##name_##.Insert();\
MKNAME(meta_HandleLock##name_, "HandleLock<" #Ty ">");\
DEFINEDCARRAY2(HandleLock<Ty>,meta_HandleLock##name_);\
MKNAME(meta_DCArray_##meta_HandleLock##name_, "DCArray<HandleLock<" #Ty ">>");

#define DEFINEOP(name, opName,fid,fun)static MetaOperationDescription meta_##name##_##opName; meta_##name##_##opName.id = fid;\
meta_##name##_##opName.mpOpFn = fun;

#define DEFINEM(name,memberName)static MetaMemberDescription meta_##name##_##memberName

#define METAOP_CUSTOM(Parent,ID,Func) static MetaOperationDescription meta_##Parent##_##ID;meta_##Parent##_##ID.id = \
MetaOperationDescription::ID; meta_##Parent##_##ID.mpOpFn = Func; meta_##Parent##_##ID.mpNext = NULL;

#define SEP ,

#define DEFINEANMVALUEI(_name, _Ty) DEFINET2(anmi_##_name##, AnimatedValueInterface<_Ty>);\
DEFINEM(anmi_##_name##, base);\
meta_anmi_##_name##_base.mpName = "Baseclass_AnimationValueInterfaceBase";\
meta_anmi_##_name##_base.mpMemberDesc = &meta_anminterface;\
meta_anmi_##_name##_base.mFlags |= 0x10;\
meta_anmi_##_name##_base.mOffset = (i64)((AnimationValueInterfaceBase*)((AnimatedValueInterface<_Ty>*)NULL));\
meta_anmi_##_name##.mpFirstMember = &meta_anmi_##_name##_base;\
ADD(anmi_##_name##);

#define DEFINESINGLEVALUE(_name, _Ty) DEFINET2(sv_##_name##, SingleValue<_Ty>);\
DEFINEM(sv_##_name##, base);\
meta_sv_##_name##_base.mpName = "Baseclass_AnimationValueInterfaceBase";\
meta_sv_##_name##_base.mpMemberDesc = &meta_anminterface;\
meta_sv_##_name##_base.mFlags |= 0x10;\
meta_sv_##_name##_base.mOffset = (i64)((AnimationValueInterfaceBase*)((SingleValue<_Ty>*)NULL));\
meta_sv_##_name##.mpFirstMember = &meta_sv_##_name##_base;\
SERIALIZER(sv_##_name##, SingleValue<_Ty>)\
ADD(sv_##_name##);

#define DEFINEKEYFRAMEDVALUE(_name,_Ty,_TyMetaVar) \
DEFINET2(kfv_##_name##_sample, KeyframedValue<_Ty>::Sample);\
FIRSTMEM2(kfv_##_name##_sample, mTime, KeyframedValue<_Ty>::Sample, float, 0);\
NEXTMEM2(kfv_##_name##_sample, mbInterpolateToNextKey, KeyframedValue<_Ty>::Sample, bool, 0, mTime);\
NEXTMEM2(kfv_##_name##_sample, mTangentMode, KeyframedValue<_Ty>::Sample, long, 0, mbInterpolateToNextKey);\
FIRSTENUM(kfv_##_name##_sample, mTangentMode, eTangentUnknown, 0, 0);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentStepped, 1, 0, eTangentUnknown);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentKnot, 2, 0, eTangentStepped);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentSmooth, 3, 0, eTangentKnot);\
NEXTENUM(kfv_##_name##_sample, mTangentMode, eTangentFlat, 4, 0, eTangentSmooth);\
NEXTMEM2(kfv_##_name##_sample, mValue, KeyframedValue<_Ty>::Sample, _TyMetaVar, 0, mTangentMode);\
NEXTMEM2(kfv_##_name##_sample, mRecipTimeToNextSample, KeyframedValue<_Ty>::Sample, float, 0x2021, mValue);\
ADD(kfv_##_name##_sample);\
DEFINEDCARRAY2(KeyframedValue<_Ty>::Sample, kfv_##_name##_sample);\
DEFINEANMVALUEI(_name, _Ty);\
DEFINET2(kfv_##_name##, KeyframedValue<_Ty>);\
DEFINEM(kfv_##_name##, animatedvalueinterface);\
meta_kfv_##_name##_animatedvalueinterface.mpName = "Baseclass_AnimatedValueInterface<T>";\
meta_kfv_##_name##_animatedvalueinterface.mpMemberDesc = &meta_anmi_##_name##;\
meta_kfv_##_name##_animatedvalueinterface.mFlags |= 0x10;\
meta_kfv_##_name##_animatedvalueinterface.mOffset = (i64)((AnimatedValueInterface<_Ty>*)((KeyframedValue<_Ty>*)NULL));\
DEFINEM(kfv_##_name##, keyframedvalueinterface);\
meta_kfv_##_name##_keyframedvalueinterface.mpName = "Baseclass_KeyframedValueInterface";\
meta_kfv_##_name##_keyframedvalueinterface.mpMemberDesc = &meta_keyframedvalueinterface;\
meta_kfv_##_name##_keyframedvalueinterface.mFlags |= 0x10;\
meta_kfv_##_name##_keyframedvalueinterface.mOffset = (i64)((KeyframedValueInterface*)((KeyframedValue<_Ty>*)NULL));\
meta_kfv_##_name##_animatedvalueinterface.mpNextMember = &meta_kfv_##_name##_keyframedvalueinterface;\
meta_kfv_##_name##.mpFirstMember = &meta_kfv_##_name##_animatedvalueinterface;\
NEXTMEM2(kfv_##_name##, mMinVal, KeyframedValue<_Ty>, _TyMetaVar, 0, animatedvalueinterface);\
NEXTMEM2(kfv_##_name##, mMaxVal, KeyframedValue<_Ty>, _TyMetaVar, 0, mMinVal);\
NEXTMEM2(kfv_##_name##, mSamples, KeyframedValue<_Ty>, DCArray_kfv_##_name##_sample, 0, mMaxVal);\
ADD(kfv_##_name##);