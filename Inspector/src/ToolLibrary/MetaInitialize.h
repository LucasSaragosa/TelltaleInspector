// This file was written by Lucas Saragosa. The code derives from Telltale Games' Engine.
// I do not intend to take credit for it, however; Im the author of this interpretation of 
// the engine and require that if you use this code or library, you give credit to me and
// the amazing Telltale Games.

#pragma warning (disable : 4018 4244 4267 4554 6387 4099)

#include "Meta.hpp"
#include "Types/TypeIncludes.h"
#include "TelltaleToolLibrary.h"
#include "LegacyTypes/D3DMeshLegacy.h"
#include "MetaInitHelpers.h"

namespace MetaInit {

	void Initialize2();
	void Initialize3();
	void Initialize4();

	//ONLY TO BE CALLED BY LIBRARY, USE LIBTELLTALETOOL_INIT
	void Initialize() {
		{
			DEFINET(char, char)
				meta_char.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_char.Initialize("int8");
			meta_char.mpTypeInfoExternalName = typeid(char).name();
			meta_char.mbIsIntrinsic = true;
			METAOP_CUSTOM(char, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint8);
			meta_char.InstallSpecializedMetaOperation(&meta_char_eMetaOpSerializeAsync);
			meta_char.Insert();
			DEFINET(ushort, u16)
				meta_ushort.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_ushort.Initialize("uint16");
			METAOP_CUSTOM(ushort, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint16);
			meta_ushort.InstallSpecializedMetaOperation(&meta_ushort_eMetaOpSerializeAsync);
			meta_ushort.Insert();
			meta_ushort.mbIsIntrinsic = true;
			DEFINET(short, i16)
				meta_short.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_short.Initialize("int16");
			METAOP_CUSTOM(short, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint16);
			meta_short.InstallSpecializedMetaOperation(&meta_short_eMetaOpSerializeAsync);
			meta_short.Insert();
			meta_short.mbIsIntrinsic = true;
			DEFINET(int, i32)
				meta_int.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_int.Initialize("int");
			METAOP_CUSTOM(int, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint32);
			meta_int.InstallSpecializedMetaOperation(&meta_int_eMetaOpSerializeAsync);
			meta_int.Insert();
			meta_int.mbIsIntrinsic = true;
			DEFINET(long, long)
				meta_long.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled | MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_long.Initialize("long");
			METAOP_CUSTOM(long, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint32);
			meta_long.InstallSpecializedMetaOperation(&meta_long_eMetaOpSerializeAsync);
			meta_long.Insert();
			meta_long.mbIsIntrinsic = true;
			DEFINET(u64, u64)
				meta_u64.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_u64.Initialize("uint64");
			METAOP_CUSTOM(u64, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint64);
			meta_u64.InstallSpecializedMetaOperation(&meta_u64_eMetaOpSerializeAsync);
			meta_u64.Insert();
			meta_u64.mbIsIntrinsic = true;

			DEFINET(float, float)
				meta_float.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled | MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_float.Initialize("float");
			METAOP_CUSTOM(float, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncfloat);
			meta_float.InstallSpecializedMetaOperation(&meta_float_eMetaOpSerializeAsync);
			meta_float.Insert();
			meta_float.mbIsIntrinsic = true;
			DEFINET(double, double)
				meta_double.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_double.Initialize("double");
			METAOP_CUSTOM(double, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncdouble);
			meta_double.InstallSpecializedMetaOperation(&meta_double_eMetaOpSerializeAsync);
			meta_double.Insert();
			meta_double.mbIsIntrinsic = true;
			DEFINET(__int64, __int64)
				meta___int64.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___int64.Initialize("int64");
			METAOP_CUSTOM(__int64, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint64);
			meta___int64.InstallSpecializedMetaOperation(&meta___int64_eMetaOpSerializeAsync);
			meta___int64.Insert();
			meta___int64.mbIsIntrinsic = true;
			DEFINET(__uint32, unsigned __int32)
				meta___uint32.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___uint32.Initialize("uint32");
			METAOP_CUSTOM(__uint32, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint32);
			meta___uint32.InstallSpecializedMetaOperation(&meta___uint32_eMetaOpSerializeAsync);
			meta___uint32.Insert();
			meta___uint32.mbIsIntrinsic = true;
			DEFINET(__int32, __int32)
				meta___int32.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___int32.Initialize("int32");
			METAOP_CUSTOM(__int32, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncint32);
			meta___int32.InstallSpecializedMetaOperation(&meta___int32_eMetaOpSerializeAsync);
			meta___int32.Insert();
			meta___int32.mbIsIntrinsic = true;
			DEFINET(__uint8, unsigned __int8)
				meta___uint8.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta___uint8.Initialize("uint8");
			METAOP_CUSTOM(__uint8, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncuint8);
			meta___uint8.InstallSpecializedMetaOperation(&meta___uint8_eMetaOpSerializeAsync);
			meta___uint8.Insert();
			meta___uint8.mbIsIntrinsic = true;
			DEFINET(symbol, Symbol)
				meta_symbol.mFlags = MetaFlag::MetaFlag_MetaSerializeNonBlockedVariableSize;
			meta_symbol.Initialize("Symbol");
			METAOP_CUSTOM(symbol, eMetaOpSerializeAsync, Symbol::MetaOperation_SerializeAsync);
			meta_symbol.InstallSpecializedMetaOperation(&meta_symbol_eMetaOpSerializeAsync);
			meta_symbol.Insert();
			DEFINET(flags, Flags)
				meta_flags.Initialize(typeid(Flags));
			DEFINEM(flags, mFlags);
			meta_flags_mFlags.mpName = "mFlags";
			meta_flags_mFlags.mFlags |= MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_flags_mFlags.mpMemberDesc = &meta_long;
			meta_flags_mFlags.mOffset = offsetof(Flags, mFlags);
			meta_flags.mpFirstMember = &meta_flags_mFlags;
			meta_flags.mFlags.mFlags |= MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_flags.Insert();
			DEFINET(bool, bool)
				meta_bool.mFlags = MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			meta_bool.Initialize("bool");
			METAOP_CUSTOM(bool, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncbool);
			meta_bool.InstallSpecializedMetaOperation(&meta_bool_eMetaOpSerializeAsync);
			meta_bool.Insert();
			meta_bool.mbIsIntrinsic = true;
			DEFINET(ztest, ZTestFunction)
				meta_ztest.mFlags |= MetaFlag_EnumWrapperClass;
			meta_ztest.Initialize(typeid(ZTestFunction));
			DEFINEM(ztest, mZTestType);
			meta_ztest_mZTestType.mpName = "mZTestType";
			meta_ztest_mZTestType.mpMemberDesc = &meta_long;
			meta_ztest_mZTestType.mOffset = offsetof(ZTestFunction, mZTestType);
			meta_ztest.mpFirstMember = &meta_ztest_mZTestType;
			meta_ztest.Insert();
			DEFINET(uidowner, UID::Owner)
				meta_uidowner.Initialize(typeid(UID::Owner));
			DEFINEM(uidowner, miUniqueID);
			meta_uidowner_miUniqueID.mpName = "miUniqueID";
			meta_uidowner_miUniqueID.mpMemberDesc = &meta_long;
			meta_uidowner_miUniqueID.mOffset = offsetof(UID::Owner, miUniqueID);
			meta_uidowner.mFlags |= MetaFlag_EditorHide;
			meta_uidowner.mpFirstMember = &meta_uidowner_miUniqueID;
			meta_uidowner.Insert();

			//UID::Generator

			DEFINET(uidgen, UID::Generator)
				meta_uidgen.Initialize(typeid(UID::Generator));
			DEFINEM(uidgen, miNextUniqueID);
			meta_uidgen_miNextUniqueID.mpName = "miNextUniqueID";
			meta_uidgen_miNextUniqueID.mpMemberDesc = &meta_long;
			meta_uidgen_miNextUniqueID.mOffset = offsetof(UID::Generator, miNextUniqueID);
			meta_uidgen.mpFirstMember = &meta_uidgen_miNextUniqueID;
			meta_uidgen.Insert();

			//T3VertexSampleDataBase

			DEFINET(t3vsdb, T3VertexSampleDataBase);
			meta_t3vsdb.Initialize(typeid(T3VertexSampleDataBase));

			DEFINEOP(t3vsdb, serialize, MetaOperationDescription::sIDs::eMetaOpSerializeAsync, NULL);
			meta_t3vsdb.InstallSpecializedMetaOperation(&meta_t3vsdb_serialize);

			DEFINEM(t3vsdb, numverts);
			meta_t3vsdb.mpFirstMember = &meta_t3vsdb_numverts;
			meta_t3vsdb_numverts.mpName = "mNumVerts";
			meta_t3vsdb_numverts.mOffset = offsetof(T3VertexSampleDataBase, mNumVerts);
			meta_t3vsdb_numverts.mpMemberDesc = &meta_long;

			DEFINEM(t3vsdb, vertsize);
			meta_t3vsdb_vertsize.mpName = "mVertSize";
			meta_t3vsdb_vertsize.mOffset = offsetof(T3VertexSampleDataBase, mVertSize);
			meta_t3vsdb_vertsize.mpMemberDesc = &meta_long;
			meta_t3vsdb_numverts.mpNextMember = &meta_t3vsdb_vertsize;

			meta_t3vsdb.Insert();

			DEFINET(string, String);
			meta_string.Initialize("String");
			METAOP_CUSTOM(string, eMetaOpSerializeAsync, MetaOperation_SerializeIntrinsicAsyncString);
			meta_string.InstallSpecializedMetaOperation(&meta_string_eMetaOpSerializeAsync);
			meta_string.mFlags.mFlags |= (int)MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_string.Insert();
			meta_string.mbIsIntrinsic = true;

			//ContainerInterface
			DEFINETABS(cinterface, ContainerInterface);
			meta_cinterface.Initialize("ContainerInterface");
			//Override operatoins: collecttyped, objectstate, scriptunlock, scriptlock,
			meta_cinterface.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_BaseClass;
			meta_cinterface.Insert();

			//Vector2
			DEFINET(vec2, Vector2);
			meta_vec2.Initialize(typeid(Vector2));
			meta_vec2.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_PlaceInAddPropMenu;
			DEFINEM(vec2, y);
			meta_vec2_y.mpName = "y";
			meta_vec2_y.mOffset = 4;
			meta_vec2_y.mpMemberDesc = &meta_float;
			DEFINEM(vec2, x);
			meta_vec2.mpFirstMember = &meta_vec2_x;
			meta_vec2_x.mpName = "x";
			meta_vec2_x.mOffset = 0;
			meta_vec2_x.mpMemberDesc = &meta_float;
			meta_vec2_x.mpNextMember = &meta_vec2_y;
			meta_vec2.Insert();

			DEFINETABS(handlebase, HandleBase);
			meta_handlebase.Initialize(typeid(HandleBase));
			METAOP_CUSTOM(handlebase, eMetaOpSerializeAsync, HandleBase::MetaOperation_SerializeAsync);
			meta_handlebase.InstallSpecializedMetaOperation(&meta_handlebase_eMetaOpSerializeAsync);
			meta_handlebase.Insert();

			DEFINETABS(handleun, HandleUncached);
			meta_handleun.Initialize(typeid(HandleUncached));
			METAOP_CUSTOM(handlebase1, eMetaOpSerializeAsync, HandleBase::MetaOperation_SerializeAsync);
			meta_handleun.InstallSpecializedMetaOperation(&meta_handlebase1_eMetaOpSerializeAsync);
			meta_handleun.Insert();

			//TRange<uint>
			DEFINET(rangeuint, TRange<unsigned int>);
			meta_rangeuint.Initialize(typeid(TRange<unsigned int>));
			meta_rangeuint.mFlags |= 6;//blocking disabled, add to prop menu
			DEFINEM(rangeuint, max);
			meta_rangeuint_max.mpName = "max";
			meta_rangeuint_max.mOffset = offsetof(TRange<unsigned int>, max);
			meta_rangeuint_max.mpMemberDesc = &meta___uint32;
			DEFINEM(rangeuint, min);
			meta_rangeuint_min.mpName = "min";
			meta_rangeuint_min.mOffset = offsetof(TRange<unsigned int>, min);
			meta_rangeuint_min.mpMemberDesc = &meta___uint32;
			meta_rangeuint_min.mpNextMember = &meta_rangeuint_max;
			meta_rangeuint.mpFirstMember = &meta_rangeuint_min;
			meta_rangeuint.Insert();
			//TRange<float>
			DEFINET(rangef, TRange<float>);
			meta_rangef.Initialize(typeid(TRange<float>));
			meta_rangef.mFlags |= 6;//blocking disabled, add to prop menu
			DEFINEM(rangef, max);
			meta_rangef_max.mpName = "max";
			meta_rangef_max.mOffset = offsetof(TRange<float>, max);
			meta_rangef_max.mpMemberDesc = &meta_float;
			DEFINEM(rangef, min);
			meta_rangef_min.mpName = "min";
			meta_rangef_min.mOffset = offsetof(TRange<float>, min);
			meta_rangef_min.mpMemberDesc = &meta_float;
			meta_rangef_min.mpNextMember = &meta_rangef_max;
			meta_rangef.mpFirstMember = &meta_rangef_min;
			meta_rangef.Insert();
			
			//Vector3
			DEFINET(vec3, Vector3);
			meta_vec3.Initialize(typeid(Vector3));
			meta_vec3.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_PlaceInAddPropMenu;
			DEFINEM(vec3, z);
			meta_vec3_z.mpName = "z";
			meta_vec3_z.mpMemberDesc = &meta_float;
			meta_vec3_z.mOffset = 8;
			DEFINEM(vec3, y);
			meta_vec3_y.mpName = "y";
			meta_vec3_y.mpMemberDesc = &meta_float;
			meta_vec3_y.mOffset = 4;
			meta_vec3_y.mpNextMember = &meta_vec3_z;
			DEFINEM(vec3, x);
			meta_vec3.mpFirstMember = &meta_vec3_x;
			meta_vec3_x.mpName = "x";
			meta_vec3_x.mpMemberDesc = &meta_float;
			meta_vec3_x.mOffset = 0;
			meta_vec3_x.mpNextMember = &meta_vec3_y;
			meta_vec3.Insert();
			METAOP_CUSTOM(vec3, eMetaOpSerializeAsync, Vector3::MetaOperation_SerializeAsync);
			meta_vec3.InstallSpecializedMetaOperation(&meta_vec3_eMetaOpSerializeAsync);

			//Vector4
			DEFINET(vec4, Vector4);
			meta_vec4.Initialize(typeid(Vector4));
			meta_vec4.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled;
			DEFINEM(vec4, w);
			meta_vec4_w.mpName = "w";
			meta_vec4_w.mpMemberDesc = &meta_float;
			meta_vec4_w.mOffset = 12;
			DEFINEM(vec4, z);
			meta_vec4_z.mpName = "z";
			meta_vec4_z.mpMemberDesc = &meta_float;
			meta_vec4_z.mOffset = 8;
			meta_vec4_z.mpNextMember = &meta_vec4_w;
			DEFINEM(vec4, y);
			meta_vec4_y.mpName = "y";
			meta_vec4_y.mpMemberDesc = &meta_float;
			meta_vec4_y.mOffset = 4;
			meta_vec4_y.mpNextMember = &meta_vec4_z;
			DEFINEM(vec4, x);
			meta_vec4.mpFirstMember = &meta_vec4_x;
			meta_vec4_x.mpName = "x";
			meta_vec4_x.mpMemberDesc = &meta_float;
			meta_vec4_x.mOffset = 0;
			meta_vec4_x.mpNextMember = &meta_vec4_y;
			meta_vec4.Insert();
			METAOP_CUSTOM(vec4, eMetaOpSerializeAsync, Vector4::MetaOperation_SerializeAsync);
			meta_vec4.InstallSpecializedMetaOperation(&meta_vec4_eMetaOpSerializeAsync);

			//Quaternion
			DEFINET(quat, Quaternion);
			meta_quat.Initialize(typeid(Quaternion));
			meta_quat.mFlags.mFlags |= (int)MetaFlag_MetaSerializeBlockingDisabled | (int)MetaFlag_PlaceInAddPropMenu;
			DEFINEM(quat, w);
			meta_quat_w.mpName = "w";
			meta_quat_w.mpMemberDesc = &meta_float;
			meta_quat_w.mOffset = 12;
			DEFINEM(quat, z);
			meta_quat_z.mpName = "z";
			meta_quat_z.mpMemberDesc = &meta_float;
			meta_quat_z.mOffset = 8;
			meta_quat_z.mpNextMember = &meta_quat_w;
			DEFINEM(quat, y);
			meta_quat_y.mpName = "y";
			meta_quat_y.mpMemberDesc = &meta_float;
			meta_quat_y.mOffset = 4;
			meta_quat_y.mpNextMember = &meta_quat_z;
			DEFINEM(quat, x);
			meta_quat.mpFirstMember = &meta_quat_x;
			meta_quat_x.mpName = "x";
			meta_quat_x.mpMemberDesc = &meta_float;
			meta_quat_x.mOffset = 0;
			meta_quat_x.mpNextMember = &meta_quat_y;
			meta_quat.Insert();
			METAOP_CUSTOM(quat, eMetaOpSerializeAsync, Quaternion::MetaOperation_SerializeAsync);
			meta_quat.InstallSpecializedMetaOperation(&meta_quat_eMetaOpSerializeAsync);

			DEFINET(transform, Transform);
			meta_transform.Initialize(typeid(Transform));
			DEFINEM(transform, trans);
			meta_transform_trans.mpName = "mTrans";
			meta_transform_trans.mpMemberDesc = &meta_vec3;
			meta_transform_trans.mOffset = offsetof(Transform, mTrans);
			DEFINEM(transform, rot);
			meta_transform_rot.mpName = "mRot";
			meta_transform_rot.mpMemberDesc = &meta_quat;
			meta_transform_rot.mOffset = offsetof(Transform, mRot);
			meta_transform_rot.mpNextMember = &meta_transform_trans;
			meta_transform.mpFirstMember = &meta_transform_rot;
			meta_transform.Insert();

			DEFINET(rect, Rect);
			meta_rect.Initialize(typeid(Rect));
			meta_rect.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeBlockingDisabled;
			DEFINEM(rect, bottom);
			meta_rect_bottom.mpName = "bottom";
			meta_rect_bottom.mpMemberDesc = &meta_long;
			meta_rect_bottom.mOffset = offsetof(Rect, bottom);
			DEFINEM(rect, top);
			meta_rect_top.mpName = "top";
			meta_rect_top.mpMemberDesc = &meta_long;
			meta_rect_top.mOffset = offsetof(Rect, top);
			meta_rect_top.mpNextMember = &meta_rect_bottom;
			DEFINEM(rect, right);
			meta_rect_right.mpName = "right";
			meta_rect_right.mpMemberDesc = &meta_long;
			meta_rect_right.mOffset = offsetof(Rect, right);
			meta_rect_right.mpNextMember = &meta_rect_top;
			DEFINEM(rect, left);
			meta_rect_left.mpName = "left";
			meta_rect_left.mpMemberDesc = &meta_long;
			meta_rect_left.mOffset = offsetof(Rect, left);
			meta_rect.mpFirstMember = &meta_rect_left;
			meta_rect_left.mpNextMember = &meta_rect_right;
			meta_rect.Insert();

			DEFINET(sphere, Sphere);
			meta_sphere.Initialize(typeid(Sphere));
			DEFINEM(sphere, radius);
			meta_sphere_radius.mOffset = offsetof(Sphere, mRadius);
			meta_sphere_radius.mpName = "mRadius";
			meta_sphere_radius.mpMemberDesc = &meta_float;
			DEFINEM(sphere, center);
			meta_sphere_center.mOffset = offsetof(Sphere, mCenter);
			meta_sphere_center.mpName = "mCenter";
			meta_sphere_center.mpMemberDesc = &meta_vec3;
			meta_sphere_center.mpNextMember = &meta_sphere_radius;
			meta_sphere.mpFirstMember = &meta_sphere_center;
			meta_sphere.Insert();

			DEFINET(color, Color);
			meta_color.mFlags = 0x6;
			meta_color.Initialize(typeid(Color));
			DEFINEM(color, alpha);
			meta_color_alpha.mpName = "a";
			meta_color_alpha.mOffset = offsetof(Color, a);
			meta_color_alpha.mpMemberDesc = &meta_float;
			DEFINEM(color, b);
			meta_color_b.mpName = "b";
			meta_color_b.mOffset = offsetof(Color, b);
			meta_color_b.mpMemberDesc = &meta_float;
			meta_color_b.mpNextMember = &meta_color_alpha;
			DEFINEM(color, g);
			meta_color_g.mpName = "g";
			meta_color_g.mOffset = offsetof(Color, g);
			meta_color_g.mpMemberDesc = &meta_float;
			meta_color_g.mpNextMember = &meta_color_b;
			DEFINEM(color, r);
			meta_color_r.mpName = "r";
			meta_color_r.mOffset = offsetof(Color, r);
			meta_color_r.mpMemberDesc = &meta_float;
			meta_color_r.mpNextMember = &meta_color_g;
			meta_color.mpFirstMember = &meta_color_r;
			meta_color.Insert();
			DEFINEHANDLE(propset, PropertySet);
			DEFINESARRAY(u32, 3);
			DEFINESARRAY(u8, 32);
			DEFINESARRAY(i32, 4);
			DEFINESARRAY(i32, 3);
			DEFINESARRAY(float, 9);
			DEFINESARRAY(float, 3);
			DEFINESARRAY_(TRange<float>, rangefloat, 3);
			DEFINEDCARRAY(i32);
			DEFINEDCARRAY(u16);
			DEFINEDCARRAY(u64);
			DEFINEDCARRAY(u32);
			DEFINEDCARRAY(u8);
			DEFINEDCARRAY(float);
			DEFINEDCARRAY(Symbol);
			DEFINEDCARRAY(bool);
			DEFINEDCARRAY(String);
			MKNAME(meta_sarray_u32_3, "SArray<unsigned int,3>");
			MKNAME(meta_sarray_i32_4, "SArray<int,4>");
			MKNAME(meta_sarray_i32_3, "SArray<int,3>");
			MKNAME(meta_sarray_float_3, "SArray<float,3>");
			MKNAME(meta_sarray_float_9, "SArray<float,9>");
			MKNAME(meta_DCArray_float, "DCArray<float>");
			MKNAME(meta_DCArray_String, "DCArray<String>");
			MKNAME(meta_DCArray_bool, "DCArray<bool>");
			MKNAME(meta_sarray_rangefloat_3, "SArray<TRange<float>,3>");
			if (meta_DCArray_String.mbNameIsHeapAllocated) {
				free((void*)meta_DCArray_String.mpTypeInfoName);
				meta_DCArray_String.mbNameIsHeapAllocated = false;
			}
			meta_DCArray_String.mpTypeInfoName = "DCArray<String>";//string is std::basic_string<...> etc
			meta_DCArray_String.mHash = CRC64_CaseInsensitive(0, meta_DCArray_String.mpTypeInfoName);
			DEFINESET(i32);
			DEFINESET(String);
			DEFINESET(u32);
			DEFINESET(u64);
			DEFINELIST(Symbol);
			DEFINELIST(i32);
			DEFINELIST(String);
			DEFINEDEQUE(i32);
			DEFINEDEQUE(String);
			MKNAME(meta_Set_i32, "Set<int,less<int>>");
			MKNAME(meta_Set_String, "Set<String,Less<String>>");
			MKNAME(meta_Set_u32, "Set<uint,less<uint>>");
			MKNAME(meta_Set_u64, "Set<uint64,less<uint64>>");
			MKNAME(meta_List_Symbol, "List<Symbol>");
			MKNAME(meta_List_i32, "List<int>");
			MKNAME(meta_List_String, "List<String>");
			MKNAME(meta_Deque_i32, "Deque<int>");
			MKNAME(meta_Deque_String, "Deque<String>");
			DEFINEMAP(Symbol, String, Symbol::CompareCRC);
			DEFINEMAP(Symbol, Symbol, Symbol::CompareCRC);
			DEFINEMAP(Symbol, float, Symbol::CompareCRC);
			DEFINEMAP(Symbol, int, Symbol::CompareCRC);
			DEFINEMAP(String, int, std::less<String>);
			DEFINEMAP(int, Symbol, std::less<int>);
			DEFINEMAP(int, String, std::less<int>);
			DEFINEMAP(int, int, std::less<int>);
			DEFINEMAP(int, float, std::less<int>);
			DEFINEMAP2(Symbol, Set<Symbol>, Symbol, setsymbol, Symbol::CompareCRC);
			DEFINEMAP2(String, DCArray<String>, String, dcarraystring, Symbol::CompareCRC);
			DEFINEMAP2(int, Map<int SEP Map<Symbol SEP float SEP Symbol::CompareCRC>>, int, mapintmapsymbolfloat, std::less<int>);
			DEFINEMAP2(int, Map<int SEP int SEP std::less<int>>, int, mapintint, std::less<int>);
			DEFINEMAP(String, String, std::less<String>);
			DEFINEMAP2(String, PropertySet, String, PropertySet, std::less<String>);
			DEFINEMAP2(Symbol, PropertySet, Symbol, PropertySet, std::less<String>);
			DEFINEMAP(int, bool, std::less<int>);
			MKNAME(meta_Map_String_PropertySet, "Map<String,PropertySet,Less<String>>");
			MKNAME(meta_Map_Symbol_PropertySet, "Map<Symbol,PropertySet,Less<Symbol>>");
			MKNAME(meta_Map_int_String, "Map<int,String,Less<int>>");
			MKNAME(meta_Map_Symbol_String, "Map<Symbol,String,Less<Symbol>>");
			MKNAME(meta_Map_Symbol_Symbol, "Map<Symbol,Symbol,Less<Symbol>>");
			MKNAME(meta_Map_Symbol_float , "Map<Symbol,Float,Less<Symbol>>");
			MKNAME(meta_Map_Symbol_int   , "Map<Symbol,int,Less<Symbol>>");
			MKNAME(meta_Map_String_int   , "Map<String,int,Less<String>>");
			MKNAME(meta_Map_int_Symbol, "Map<int,Symbol,Less<int>>");
			MKNAME(meta_Map_int_int, "Map<int,int,Less<int>>");
			MKNAME(meta_Map_int_float, "Map<int,float,Less<int>>");
			MKNAME(meta_Map_Symbol_setsymbol, "Map<Symbol,Set<Symbol,less<Symbol>>,Less<Symbol>>");
			MKNAME(meta_Map_String_dcarraystring, "Map<String,DCArray<String>,Less<String>>");
			MKNAME(meta_Map_int_bool, "Map<int,bool,less<int>>");
			MKNAME(meta_Map_String_String, "Map<String,String,Less<String>>");
			MKNAME(meta_Map_int_mapintint, "Map<int,Map<int,int,less<int>>,less<int>>");
			MKNAME(meta_Map_int_mapintmapsymbolfloat, "Map<int,Map<int,Map<Symbol,float,less<Symbol>>,less<int>>,less<int>>");
			DEFINESET_(PropertySet::KeyInfo, keyinfo);
			DEFINET(prop, PropertySet);
			meta_prop.Initialize(typeid(PropertySet));
			meta_prop.mpExt = "prop";
			METAOP_CUSTOM(prop, eMetaOpSerializeAsync, PropertySet::MetaOperation_SerializeAsync);
			meta_prop.InstallSpecializedMetaOperation(&meta_prop_eMetaOpSerializeAsync);
			DEFINEM(prop, version);
			meta_prop_version.mpName = "mPropVersion";
			meta_prop_version.mOffset = offsetof(PropertySet, mPropVersion);
			meta_prop_version.mpMemberDesc = &meta_long;
			meta_prop.mpFirstMember = &meta_prop_version;
			DEFINEM(prop, flags);
			meta_prop_flags.mpName = "mPropertyFlags";
			meta_prop_flags.mpMemberDesc = &meta_flags;
			meta_prop_flags.mOffset = offsetof(PropertySet, mPropertyFlags);
			meta_prop_version.mpNextMember = &meta_prop_flags;
			DEFINEM(prop, keymap);
			meta_prop_keymap.mpName = "mKeyMap";
			meta_prop_keymap.mpMemberDesc = &meta_long;//! no serialize
			meta_prop_keymap.mFlags |= (int)MetaFlag::MetaFlag_MetaSerializeDisable;
			meta_prop_keymap.mOffset = offsetof(PropertySet, mKeyMap);
			meta_prop_flags.mpNextMember = &meta_prop_keymap;
			meta_prop.Insert();

			DEFINET(bc, BoneContraints);
			meta_bc.Initialize(typeid(BoneContraints));
			DEFINEM(bc, btype);
			meta_bc_btype.mOffset = offsetof(BoneContraints, mBoneType);
			meta_bc_btype.mpName = "mBoneType";
			meta_bc_btype.mpMemberDesc = &meta_long;
			meta_bc_btype.mFlags |= MetaFlag::MetaFlag_EnumIntType;
			static MetaEnumDescription meta_bce_hinge, meta_bce_joint;//joint=ball oops
			meta_bce_hinge.mpEnumName = "eBoneType_Hinge";
			meta_bce_hinge.mEnumIntValue = 0;
			meta_bce_hinge.mpNext = &meta_bce_joint;
			meta_bce_joint.mpEnumName = "eBoneType_Ball";
			meta_bce_joint.mEnumIntValue = 1;
			meta_bc_btype.mpEnumDescriptions = &meta_bce_hinge;
			DEFINEM(bc, axis);
			meta_bc_btype.mpNextMember = &meta_bc_axis;
			meta_bc_axis.mpName = "mHingeAxis";
			meta_bc_axis.mOffset = offsetof(BoneContraints, mHingeAxis);
			meta_bc_axis.mFlags |= MetaFlag::MetaFlag_PlaceInAddPropMenu;
			meta_bc_axis.mpMemberDesc = &meta_vec3;
			DEFINEM(bc, range);
			meta_bc_axis.mpNextMember = &meta_bc_range;
			meta_bc_range.mpName = "mAxisRange";
			meta_bc_range.mOffset = offsetof(BoneContraints, mAxisRange);
			meta_bc_range.mpMemberDesc = &meta_sarray_rangefloat_3;
			meta_bc.mpFirstMember = &meta_bc_btype;
			meta_bc.Insert();

			DEFINET(sklentry, Skeleton::Entry);
			meta_sklentry.Initialize(typeid(Skeleton::Entry));
			DEFINEM(sklentry, flags);
			meta_sklentry_flags.mpName = "mFlags";
			meta_sklentry_flags.mpMemberDesc = &meta_flags;
			meta_sklentry_flags.mOffset = offsetof(Skeleton::Entry, mFlags);
			DEFINEM(sklentry, constraints);
			meta_sklentry_constraints.mpName = "mConstraints";
			meta_sklentry_constraints.mpMemberDesc = &meta_bc;
			meta_sklentry_constraints.mOffset = offsetof(Skeleton::Entry, mConstraints);
			meta_sklentry_constraints.mpNextMember = &meta_sklentry_flags;
			DEFINEM(sklentry, rgm);
			meta_sklentry_rgm.mpName = "mResourceGroupMembership";
			meta_sklentry_rgm.mOffset = offsetof(Skeleton::Entry, mResourceGroupMembership);
			meta_sklentry_rgm.mpNextMember = &meta_sklentry_constraints;
			meta_sklentry_rgm.mpMemberDesc = &meta_Map_Symbol_float;
			DEFINEM(sklentry, ats);
			meta_sklentry_ats.mpName = "mAnimTranslationScale";
			meta_sklentry_ats.mOffset = offsetof(Skeleton::Entry, mAnimTranslationScale);
			meta_sklentry_ats.mpNextMember = &meta_sklentry_rgm;
			meta_sklentry_ats.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, lts);
			meta_sklentry_lts.mpName = "mLocalTranslationScale";
			meta_sklentry_lts.mOffset = offsetof(Skeleton::Entry, mLocalTranslationScale);
			meta_sklentry_lts.mpNextMember = &meta_sklentry_ats;
			meta_sklentry_lts.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, gts);
			meta_sklentry_gts.mpName = "mGlobalTranslationScale";
			meta_sklentry_gts.mOffset = offsetof(Skeleton::Entry, mGlobalTranslationScale);
			meta_sklentry_gts.mpNextMember = &meta_sklentry_lts;
			meta_sklentry_gts.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, restx);
			meta_sklentry_restx.mpName = "mRestXForm";
			meta_sklentry_restx.mOffset = offsetof(Skeleton::Entry, mRestXform);
			meta_sklentry_restx.mpNextMember = &meta_sklentry_gts;
			meta_sklentry_restx.mpMemberDesc = &meta_transform;
			DEFINEM(sklentry, localq);
			meta_sklentry_localq.mpName = "mLocalQuat";
			meta_sklentry_localq.mOffset = offsetof(Skeleton::Entry, mLocalQuat);
			meta_sklentry_localq.mpNextMember = &meta_sklentry_restx;
			meta_sklentry_localq.mpMemberDesc = &meta_quat;
			DEFINEM(sklentry, localp);
			meta_sklentry_localp.mpName = "mLocalPos";
			meta_sklentry_localp.mOffset = offsetof(Skeleton::Entry, mLocalPos);
			meta_sklentry_localp.mpNextMember = &meta_sklentry_localq;
			meta_sklentry_localp.mpMemberDesc = &meta_vec3;
			DEFINEM(sklentry, blen);
			meta_sklentry_blen.mpName = "mBoneLength";
			meta_sklentry_blen.mOffset = offsetof(Skeleton::Entry, mBoneLength);
			meta_sklentry_blen.mpNextMember = &meta_sklentry_localp;
			meta_sklentry_blen.mpMemberDesc = &meta_float;
			meta_sklentry_blen.mFlags |= MetaFlag::MetaFlag_MetaSerializeDisable;
			DEFINEM(sklentry, mblen);
			meta_sklentry_mblen.mpName = "mMirrorBoneIndex";
			meta_sklentry_mblen.mOffset = offsetof(Skeleton::Entry, mMirrorBoneIndex);
			meta_sklentry_mblen.mpNextMember = &meta_sklentry_blen;
			meta_sklentry_mblen.mpMemberDesc = &meta_long;
			DEFINEM(sklentry, mbn);
			meta_sklentry_mbn.mpName = "mMirrorBoneName";
			meta_sklentry_mbn.mOffset = offsetof(Skeleton::Entry, mMirrorBoneName);
			meta_sklentry_mbn.mpNextMember = &meta_sklentry_mblen;
			meta_sklentry_mbn.mpMemberDesc = &meta_symbol;
			DEFINEM(sklentry, pi);
			meta_sklentry_pi.mpName = "mParentIndex";
			meta_sklentry_pi.mOffset = offsetof(Skeleton::Entry, mParentIndex);
			meta_sklentry_pi.mpNextMember = &meta_sklentry_mbn;
			meta_sklentry_pi.mpMemberDesc = &meta_long;
			DEFINEM(sklentry, pn);
			meta_sklentry_pn.mpName = "mParentName";
			meta_sklentry_pn.mOffset = offsetof(Skeleton::Entry, mParentName);
			meta_sklentry_pn.mpNextMember = &meta_sklentry_pi;
			meta_sklentry_pn.mpMemberDesc = &meta_symbol;
			DEFINEM(sklentry, jn);
			meta_sklentry_jn.mpName = "mJointName";
			meta_sklentry_jn.mOffset = offsetof(Skeleton::Entry, mJointName);
			meta_sklentry_jn.mpNextMember = &meta_sklentry_pn;
			meta_sklentry_jn.mpMemberDesc = &meta_symbol;
			meta_sklentry.mpFirstMember = &meta_sklentry_jn;
			meta_sklentry.Insert();
			MKNAME(meta_sklentry, "Skeleton::Entry");

			DEFINEDCARRAY2(Skeleton::Entry, sklentry);

			DEFINET(skl, Skeleton);
			meta_skl.Initialize(typeid(Skeleton));
			meta_skl.mpExt = "skl";
			DEFINEM(skl, entries);
			meta_skl_entries.mpName = "mEntries";
			meta_skl_entries.mOffset = offsetof(Skeleton, mEntries);
			meta_skl_entries.mpMemberDesc = &meta_DCArray_sklentry;
			meta_skl.mpFirstMember = &meta_skl_entries;
			meta_skl.Insert();
			MKNAME(meta_skl, "Skeleton");

			DEFINET(aam, ActorAgentMapper);
			meta_aam.Initialize(typeid(ActorAgentMapper));
			meta_aam.mpExt = "aam";
			DEFINEM(aam, aam);
			meta_aam_aam.mpName = "mActorAgentMap";
			meta_aam_aam.mpMemberDesc = &meta_prop;
			meta_aam_aam.mOffset = offsetof(ActorAgentMapper, mActorAgentMap);
			meta_aam.mpFirstMember = &meta_aam_aam;
			DEFINEM(aam, aa);
			meta_aam_aa.mpName = "mActionActors";
			meta_aam_aa.mpMemberDesc = &meta_Set_String;
			meta_aam_aa.mOffset = offsetof(ActorAgentMapper, mActionActors);
			meta_aam_aam.mpNextMember = &meta_aam_aa;
			meta_aam.Insert();
			MKNAME(meta_aam, "ActorAgentMapper");

			DEFINET(amape, AgentMap::AgentMapEntry);
			meta_amape.Initialize(typeid(AgentMap::AgentMapEntry));
			DEFINEM(amape, name);
			meta_amape_name.mpName = "mzName";
			meta_amape_name.mOffset = offsetof(AgentMap::AgentMapEntry, mzName);
			meta_amape_name.mpMemberDesc = &meta_string;

			DEFINEM(amape, Actor);
			meta_amape_Actor.mpName = "mzActor";
			meta_amape_Actor.mOffset = offsetof(AgentMap::AgentMapEntry, mzActor);
			meta_amape_Actor.mpMemberDesc = &meta_string;
			meta_amape_name.mpNextMember = &meta_amape_Actor;

			DEFINEM(amape, name2);
			meta_amape_name2.mpName = "mazModels";
			meta_amape_name2.mOffset = offsetof(AgentMap::AgentMapEntry, mazModels);
			meta_amape_name2.mpMemberDesc = &meta_Set_String;
			meta_amape_Actor.mpNextMember = &meta_amape_name2;

			DEFINEM(amape, name3);
			meta_amape_name3.mpName = "mazGuides";
			meta_amape_name3.mOffset = offsetof(AgentMap::AgentMapEntry, mazGuides);
			meta_amape_name3.mpMemberDesc = &meta_Set_String;
			meta_amape_name2.mpNextMember = &meta_amape_name3;

			DEFINEM(amape, name4);
			meta_amape_name4.mpName = "mazStyleIdles";
			meta_amape_name4.mOffset = offsetof(AgentMap::AgentMapEntry, mazStyleIdles);
			meta_amape_name4.mpMemberDesc = &meta_Set_String;
			meta_amape_name3.mpNextMember = &meta_amape_name4;

			meta_amape.mpFirstMember = &meta_amape_name;
			meta_amape.Insert();
			MKNAME(meta_amape, "AgentMap::AgentMapEntry");

			DEFINEMAP2(String, AgentMap::AgentMapEntry, str, amape, std::less<String>);
			MKNAME(meta_Map_str_amape, "Map<String,AgentMap::AgentMapEntry,less<String>>");

			DEFINET(amap, AgentMap);
			meta_amap.Initialize(typeid(AgentMap));
			meta_amap.mpExt = "amap";
			DEFINEM(amap, agents);
			meta_amap_agents.mpName = "maAgents";
			meta_amap_agents.mOffset = offsetof(AgentMap, maAgents);
			meta_amap_agents.mpMemberDesc = &meta_Map_str_amape;
			meta_amap.mpFirstMember = &meta_amap_agents;
			meta_amap.Insert();
			MKNAME(meta_amap, "AgentMap");

			DEFINET(acol, AssetCollection);
			meta_acol.Initialize(typeid(AssetCollection));
			meta_acol.mpExt = "acol";
			DEFINEM(acol, inc);
			DEFINEM(acol, exl);
			DEFINEM(acol, pre);
			meta_acol_inc.mpMemberDesc = &meta_DCArray_String;
			meta_acol_exl.mpMemberDesc = &meta_DCArray_String;
			meta_acol_pre.mpMemberDesc = &meta_string;
			meta_acol_inc.mOffset = offsetof(AssetCollection, mIncludeMasks);
			meta_acol_exl.mOffset = offsetof(AssetCollection, mExcludeMasks);
			meta_acol_pre.mOffset = offsetof(AssetCollection, mPreFilter);
			meta_acol_inc.mpName = "mIncludeMasks";
			meta_acol_exl.mpName = "mExcludeMasks";
			meta_acol_pre.mpName = "mPreFilter";
			meta_acol_pre.mFlags |= MetaFlag::MetaFlag_EditorHide;
			meta_acol.mpFirstMember = &meta_acol_inc;
			meta_acol_inc.mpNextMember = &meta_acol_exl;
			meta_acol_exl.mpNextMember = &meta_acol_pre;
			meta_acol.Insert();
			MKNAME(meta_acol, "AssetCollection");

			DEFINEMAP2(String, SoundBusSystem::BusDescription, str, bd, std::less<String>);//we can define since it doesnt ref the mcd yet
			MKNAME(meta_Map_str_bd, "Map<String,SoundBusSystem::BusDescription,less<String>>");
			DEFINET(busd, SoundBusSystem::BusDescription);
			meta_busd.Initialize(typeid(SoundBusSystem::BusDescription));
			FIRSTMEM(busd, "fVolumedB", fVolumedB, SoundBusSystem::BusDescription, float, 0);
			NEXTMEM(busd, "fReverbWetLeveldB", fReverbWetLeveldB, SoundBusSystem::BusDescription, float, 0, fVolumedB);
			NEXTMEM(busd, "fLowPassFrequencyCutoff", fLowPassFrequencyCutoff, SoundBusSystem::BusDescription, float, 0, fReverbWetLeveldB);
			NEXTMEM(busd, "fHighPassFrequencyCutoff", fHighPassFrequencyCutoff, SoundBusSystem::BusDescription, float, 0, fLowPassFrequencyCutoff);
			NEXTMEM(busd, "bEnableLowPass", bEnableLowPass, SoundBusSystem::BusDescription, bool, 0, fHighPassFrequencyCutoff);
			NEXTMEM(busd, "bEnableHighPass", bEnableHighPass, SoundBusSystem::BusDescription, bool, 0, bEnableLowPass);
			NEXTMEM(busd, "children", children, SoundBusSystem::BusDescription, Map_str_bd, 0, bEnableHighPass);
			NEXTMEM(busd, "autoAssignPatternCollection", autoAssignPatternCollection, SoundBusSystem::BusDescription, acol, 0, children);
			meta_busd.Insert();

			DEFINET(bus, SoundBusSystem::BusHolder);
			meta_bus.Initialize(typeid(SoundBusSystem::BusHolder));
			meta_bus.mpExt = "audiobus";
			DEFINEM(bus, mbus);
			meta_bus_mbus.mpName = "masterBus";
			meta_bus_mbus.mOffset = offsetof(SoundBusSystem::BusHolder, masterBus);
			meta_bus_mbus.mpMemberDesc = &meta_busd;
			DEFINEM(bus, am);
			meta_bus_am.mpName = "assetMap";
			meta_bus_mbus.mpNextMember = &meta_bus_am;
			meta_bus_am.mOffset = offsetof(SoundBusSystem::BusHolder, assetMap);
			meta_bus_am.mpMemberDesc = &meta_Map_Symbol_Symbol;
			meta_bus_am.mFlags |= MetaFlag::MetaFlag_EditorHide;
			meta_bus.mpFirstMember = &meta_bus_mbus;
			meta_bus.Insert();

			DEFINET2(imapm, InputMapper::EventMapping);
			FIRSTMEM(imapm, "mInputCode", mInputCode, InputMapper::EventMapping, long, MetaFlag::MetaFlag_EnumIntType);
			NEXTMEM2(imapm, mEvent, InputMapper::EventMapping, long, MetaFlag::MetaFlag_EnumIntType, mInputCode);
			NEXTMEM2(imapm, mScriptFunction, InputMapper::EventMapping, string, 0, mEvent);
			NEXTMEM2(imapm, mControllerIndexOverride, InputMapper::EventMapping, long, 0, mScriptFunction);
			ADD(imapm);

			DEFINEDCARRAY2(InputMapper*, imapptr);//telltale made a mistake! would not need to serialize ptrs? has overriden metaop tho

			DEFINET2(imapr, InputMapper::RawEvent);
			FIRSTMEM2(imapr, mKey, InputMapper::RawEvent, long, MetaFlag::MetaFlag_EnumIntType);
			NEXTMEM2(imapr, mType, InputMapper::RawEvent, long, MetaFlag::MetaFlag_EnumIntType, mKey);
			NEXTMEM2(imapr, mX, InputMapper::RawEvent, float, 0, mType);
			NEXTMEM2(imapr, mY, InputMapper::RawEvent, float, 0, mX);
			NEXTMEM2(imapr, mController, InputMapper::RawEvent, long, 0, mY);
			NEXTMEM2(imapr, mIMAPFilter, InputMapper::RawEvent, DCArray_imapptr, MetaFlag::MetaFlag_NoPanelCaption | MetaFlag::MetaFlag_BaseClass, mController);
			ADD(imapr);

			DEFINEDCARRAY2(InputMapper::EventMapping, eventmapping);

			DEFINET2(imap, InputMapper);
			EXT(imap, imap);
			METAOP_CUSTOM(imap, eMetaOpSerializeAsync, InputMapper::MetaOperation_SerializeAsync);
			meta_imap.InstallSpecializedMetaOperation(&meta_imap_eMetaOpSerializeAsync);
			FIRSTMEM2(imap, mName, InputMapper, string, 0);
			NEXTMEM2(imap, mMappedEvents, InputMapper, DCArray_eventmapping, 0, mName);
			ADD(imap);

			DEFINET2(pmapm, PlatformInputMapper::EventMapping);
			FIRSTMEM2(pmapm, mPlatformInputCode, PlatformInputMapper::EventMapping, long, 0);
			NEXTMEM2(pmapm, mInputCode, PlatformInputMapper::EventMapping, long, 0, mPlatformInputCode);
			ADD(pmapm);

			DEFINEHANDLE(scene, Scene);
			DEFINEDCARRAY2(HandleLock<Scene>, hlscene);

			DEFINET2(sceneagent, Scene::AgentInfo);
			FIRSTMEM2(sceneagent, mAgentName, Scene::AgentInfo, string, 0);
			NEXTMEM2(sceneagent, mAgentSceneProps, Scene::AgentInfo, prop, 0, mAgentName);
			ADD(sceneagent);

			DEFINELIST_(Scene::AgentInfo*, agentinfoptr);

			DEFINET2NOCOPY(scene, Scene);
			METAOP_CUSTOM(scene, eMetaOpSerializeAsync, Scene::MetaOperation_SerializeAsync);
			meta_scene.InstallSpecializedMetaOperation(&meta_scene_eMetaOpSerializeAsync);
			FIRSTMEM2(scene, mTimeScale, Scene, float, 1);
			NEXTMEM2(scene, mbActive, Scene, bool, 1, mTimeScale);
			NEXTMEM2(scene, mbHidden, Scene, bool, 0, mbActive);
			NEXTMEM2(scene, mName, Scene, string, 0, mbHidden);
			NEXTMEM2(scene, mAgentList, Scene, List_agentinfoptr, 1, mName);
			NEXTMEM2(scene, mReferencedScenes, Scene, DCArray_hlscene, 0, mAgentList);
			EXT(scene, scene);
			ADD(scene);

			DEFINET2(dlgprops1, DlgObjectPropsMap);
			SERIALIZER(dlgprops1, DlgObjectPropsMap);
			ADD(dlgprops1);

			DEFINET2(dlgdef, DlgObjectPropsMap::GroupDefinition);
			SERIALIZER(dlgdef, DlgObjectPropsMap::GroupDefinition);
			ADDFLAGS(dlgdef, MetaFlag::MetaFlag_NoPanelCaption);
			DEFINEM(dlgdef, uidowner);
			meta_dlgdef_uidowner.mpName = "Baseclass_UID::Owner";
			meta_dlgdef_uidowner.mpMemberDesc = &meta_uidowner;
			meta_dlgdef.mpFirstMember = &meta_dlgdef_uidowner;
			meta_dlgdef_uidowner.mOffset = PARENT_OFFSET(UID::Owner, DlgObjectPropsMap::GroupDefinition);
			NEXTMEM2(dlgdef, mGroupCat, DlgObjectPropsMap::GroupDefinition, long, MetaFlag::MetaFlag_EditorHide, uidowner);
			NEXTMEM2(dlgdef, mVer, DlgObjectPropsMap::GroupDefinition, long, MetaFlag::MetaFlag_EditorHide, mGroupCat);
			NEXTMEM2(dlgdef, mhProps, DlgObjectPropsMap::GroupDefinition, Handlepropset, MetaFlag::MetaFlag_EditorHide, mVer);

			ADD(dlgdef);

			DEFINET2(dss, DlgSystemSettings);
			EXT(dss, dss);
			FIRSTMEM2(dss, mPropsMapUser, DlgSystemSettings, dlgprops1, 0);
			NEXTMEM2(dss, mPropsMapProduction, DlgSystemSettings, dlgprops1, 0, mPropsMapUser);
			ADD(dss);

			DEFINET2(bb, BinaryBuffer);
			FIRSTMEM2(bb, mDataSize, BinaryBuffer, long, 0);
			SERIALIZER(bb, BinaryBuffer);
			ADD(bb);

			DEFINET2(enlsig, EnlightenSignature);
			FIRSTMEM2(enlsig, mSignature, EnlightenSignature, __int64, 0);
			NEXTMEM2(enlsig, mMagicNumber, EnlightenSignature, long, 0, mSignature);
			NEXTMEM2(enlsig, mVersion, EnlightenSignature, long, 0, mMagicNumber);
			ADD(enlsig);

			DEFINET2(enlprobe, EnlightenProbeData);
			FIRSTMEM2(enlprobe, mEnvTileName, EnlightenProbeData, symbol, 0);
			NEXTMEM2(enlprobe, mRadProbeSetCore, EnlightenProbeData, bb, 0, mEnvTileName);
			ADD(enlprobe);

			DEFINET2(enlsys, EnlightenSystemData);
			FIRSTMEM2(enlsys, mName, EnlightenSystemData, symbol, 0);
			NEXTMEM2(enlsys, mEnvTileName, EnlightenSystemData, symbol, 0, mName);
			NEXTMEM2(enlsys, mRadSystemCore, EnlightenSystemData, bb, 0, mEnvTileName);
			NEXTMEM2(enlsys, mInputWorkspace, EnlightenSystemData, bb, 0, mRadSystemCore);
			NEXTMEM2(enlsys, mClusterAlbedoWorkspaceMaterial, EnlightenSystemData, bb, 0, mInputWorkspace);
			NEXTMEM2(enlsys, mPrecomputedVisibility, EnlightenSystemData, bb, 0, mClusterAlbedoWorkspaceMaterial);
			ADD(enlsys);

			DEFINEDCARRAY(EnlightenSystemData);
			DEFINEDCARRAY(EnlightenProbeData);

			DEFINET2(enl, EnlightenData);
			EXT(enl, enl);
			FIRSTMEM2(enl, mSignature, EnlightenData, enlsig, 0);
			NEXTMEM2(enl, mName, EnlightenData, string, 0, mSignature);
			NEXTMEM2(enl, mSystemData, EnlightenData, DCArray_EnlightenSystemData, 0, mName);
			NEXTMEM2(enl, mProbeData, EnlightenData, DCArray_EnlightenProbeData, 0, mSystemData);
			//no need for overriden metaops since they just call meta::x
			ADD(enl);

			DEFINET2(reverb, SoundReverbDefinition);
			EXT(reverb, reverb);
			FIRSTMEM2(reverb, mbEnabled, SoundReverbDefinition, bool, 0);
			NEXTMEM2(reverb, mfRoomEffectLevel, SoundReverbDefinition, float, 0, mbEnabled);
			NEXTMEM2(reverb, mfRoomEffectLevelHighFrequency, SoundReverbDefinition, float, 0, mfRoomEffectLevel);
			NEXTMEM2(reverb, mfRoomEffectLevelLowFrequency, SoundReverbDefinition, float, 0, mfRoomEffectLevelHighFrequency);
			NEXTMEM2(reverb, mfDecayTime, SoundReverbDefinition, float, 0, mfRoomEffectLevelLowFrequency);
			NEXTMEM2(reverb, mfDecayHighFrequencyRatio, SoundReverbDefinition, float, 0, mfDecayTime);
			NEXTMEM2(reverb, mfReflections, SoundReverbDefinition, float, 0, mfDecayHighFrequencyRatio);
			NEXTMEM2(reverb, mfReflectionsDelay, SoundReverbDefinition, float, 0, mfReflections);
			NEXTMEM2(reverb, mfReverb, SoundReverbDefinition, float, 0, mfReflectionsDelay);
			NEXTMEM2(reverb, mfReverbDelay, SoundReverbDefinition, float, 0, mfReverb);
			NEXTMEM2(reverb, mfHighFrequencyReference, SoundReverbDefinition, float, 0, mfReverbDelay);
			NEXTMEM2(reverb, mfLowFrequencyReference, SoundReverbDefinition, float, 0, mfHighFrequencyReference);
			NEXTMEM2(reverb, mfDiffusion, SoundReverbDefinition, float, 0, mfLowFrequencyReference);
			NEXTMEM2(reverb, mfDensity, SoundReverbDefinition, float, 0, mfDiffusion);
			ADD(reverb);

			DEFINET2(anminterface, AnimationValueInterfaceBase);
			FIRSTMEM2(anminterface, mName, AnimationValueInterfaceBase, symbol, 0);
			NEXTMEM2(anminterface, mFlags, AnimationValueInterfaceBase, long, 0, mName);
			ADD(anminterface);

			DEFINET2(keyframedvalueinterface, KeyframedValueInterface);
			ADDFLAGS(keyframedvalueinterface, MetaFlag::MetaFlag_MetaSerializeDisable | MetaFlag::MetaFlag_SkipObjectState);
			ADD(keyframedvalueinterface);

			DEFINET2(sampler, T3SamplerStateBlock);
			FIRSTMEM2(sampler, mData, T3SamplerStateBlock, long, 0);
			ADD(sampler);

			DEFINET2(pt, EnumPlatformType);
			FIRSTMEM2(pt, mVal, EnumPlatformType, long, 0);
			ADDFLAGS(pt, MetaFlag::MetaFlag_NoPanelCaption | MetaFlag::MetaFlag_EnumWrapperClass);
			FIRSTENUM(pt, mVal, ePlatform_None, 0, 0);
			NEXTENUM(pt, mVal, ePlatform_All, 1, 0, ePlatform_None);
			NEXTENUM(pt, mVal, ePlatform_PC, 2, 0, ePlatform_All);
			NEXTENUM(pt, mVal, ePlatform_Wii, 3, 0, ePlatform_PC);
			NEXTENUM(pt, mVal, ePlatform_Xbox, 4, 0, ePlatform_Wii);
			NEXTENUM(pt, mVal, ePlatform_PS3, 5, 0, ePlatform_Xbox);
			NEXTENUM(pt, mVal, ePlatform_Mac, 6, 0, ePlatform_PS3);
			NEXTENUM(pt, mVal, ePlatform_iPhone, 7, 0, ePlatform_Mac);
			NEXTENUM(pt, mVal, ePlatform_Android, 8, 0, ePlatform_iPhone);
			NEXTENUM(pt, mVal, ePlatform_Vita, 9, 0, ePlatform_Android);
			NEXTENUM(pt, mVal, ePlatform_Linux, 10, 0, ePlatform_Vita);
			NEXTENUM(pt, mVal, ePlatform_PS4, 11, 0, ePlatform_Linux);
			NEXTENUM(pt, mVal, ePlatform_XBOne, 12, 0, ePlatform_PS4);
			NEXTENUM(pt, mVal, ePlatform_WiiU, 13, 0, ePlatform_XBOne);
			NEXTENUM(pt, mVal, ePlatform_Win10, 14, 0, ePlatform_WiiU);
			NEXTENUM(pt, mVal, ePlatform_NX, 15, 0, ePlatform_Win10);
			ADDFLAGS(pt_mVal, MetaFlag::MetaFlag_EnumIntType);
			ADD(pt);

			DEFINET2(tp, ToolProps);
			SERIALIZER(tp, ToolProps);
			ADDFLAGS(tp, MetaFlag::MetaFlag_MetaSerializeNonBlockedVariableSize);
			FIRSTMEM2(tp, mbHasProps, ToolProps, bool, 0);
			ADD(tp);

			DEFINET2(t3gr, T3ToonGradientRegion);
			FIRSTMEM2(t3gr, mColor, T3ToonGradientRegion, color, 0);
			NEXTMEM2(t3gr, mSize, T3ToonGradientRegion, float, 0, mColor);
			NEXTMEM2(t3gr, mGradientSize, T3ToonGradientRegion, float, 0, mSize);
			ADD(t3gr);

			DEFINEDCARRAY(T3ToonGradientRegion);

			DEFINET2(swizzle, RenderSwizzleParams);
			SERIALIZER(swizzle, RenderSwizzleParams);
			DEFINEM(RenderSwizzleParams, s0);
			size_t start = offsetof(RenderSwizzleParams, mSwizzle);
			meta_RenderSwizzleParams_s0.mOffset = start + 0;
			meta_RenderSwizzleParams_s0.mpName = "mSwizzle[0]";
			meta_RenderSwizzleParams_s0.mpMemberDesc = &meta_char;
			meta_swizzle.mpFirstMember = &meta_RenderSwizzleParams_s0;
			DEFINEM(RenderSwizzleParams, s1);
			meta_RenderSwizzleParams_s1.mOffset = start + 1;
			meta_RenderSwizzleParams_s1.mpName = "mSwizzle[1]";
			meta_RenderSwizzleParams_s1.mpMemberDesc = &meta_char;
			meta_RenderSwizzleParams_s0.mpNextMember = &meta_RenderSwizzleParams_s1;
			DEFINEM(RenderSwizzleParams, s2);
			meta_RenderSwizzleParams_s2.mOffset = start + 2;
			meta_RenderSwizzleParams_s2.mpName = "mSwizzle[2]";
			meta_RenderSwizzleParams_s2.mpMemberDesc = &meta_char;
			meta_RenderSwizzleParams_s1.mpNextMember = &meta_RenderSwizzleParams_s2;
			DEFINEM(RenderSwizzleParams, s3);
			meta_RenderSwizzleParams_s3.mOffset = start + 3;
			meta_RenderSwizzleParams_s3.mpName = "mSwizzle[3]";
			meta_RenderSwizzleParams_s3.mpMemberDesc = &meta_char;
			meta_RenderSwizzleParams_s2.mpNextMember = &meta_RenderSwizzleParams_s3;
			ADD(swizzle);

			DEFINET2(tex, T3Texture);
			EXT(tex, d3dtx);
			FIRSTMEM2(tex, mVersion, T3Texture, long, MetaFlag::MetaFlag_EditorHide);
			NEXTMEM2(tex, mSamplerState, T3Texture, sampler, 0, mVersion);
			NEXTMEM2(tex, mPlatform, T3Texture, pt, 0, mSamplerState);
			NEXTMEM2(tex, mName, T3Texture, string, 0, mPlatform);
			NEXTMEM2(tex, mImportName, T3Texture, string, 0, mName);
			NEXTMEM2(tex, mImportScale, T3Texture, float, 0, mImportName);
			NEXTMEM4(tex, mImportSpecularPower, T3Texture, float, 0, mImportScale, -1, TelltaleToolLib_GetGameKeyIndex("MCSM"));
			NEXTMEM2(tex, mToolProps, T3Texture, tp, MetaFlag::MetaFlag_EditorHide, mImportSpecularPower);
			NEXTMEM2(tex, mNumMipLevels, T3Texture, long, 0, mToolProps);
			NEXTMEM2(tex, mWidth, T3Texture, long, 0, mNumMipLevels);
			NEXTMEM2(tex, mHeight, T3Texture, long, 0, mWidth);
			NEXTMEM4(tex, mDepth, T3Texture, long, 0, mHeight, TelltaleToolLib_GetGameKeyIndex("BATMAN"), -1);
			NEXTMEM4(tex, mArraySize, T3Texture, long, 0, mDepth, TelltaleToolLib_GetGameKeyIndex("BATMAN"), -1);
			NEXTMEM2(tex, mSurfaceFormat, T3Texture, long, 0, mArraySize);
			FIRSTENUM(tex, mSurfaceFormat, eSurface_Unknown, -1, 0);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB8, 0, 0, eSurface_Unknown);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB16, 0x1, 0, eSurface_ARGB8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGB565, 0x2, 0, eSurface_ARGB16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB1555, 0x3, 0, eSurface_RGB565);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB4, 0x4, 0, eSurface_ARGB1555);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ARGB2101010, 0x5, 0, eSurface_ARGB4);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R16, 0x6, 0, eSurface_ARGB2101010);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16, 0x7, 0, eSurface_R16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA16, 0x8, 0, eSurface_RG16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG8, 0x9, 0, eSurface_RGBA16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA8, 0xA, 0, eSurface_RG8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R32, 0xB, 0, eSurface_RGBA8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG32, 0xC, 0, eSurface_R32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA32, 0xD, 0, eSurface_RG32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA8S, 0xF, 0, eSurface_RGBA32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_A8, 0x10, 0, eSurface_RGBA8S);
			NEXTENUM(tex, mSurfaceFormat, eSurface_L8, 0x11, 0, eSurface_A8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_AL8, 0x12, 0, eSurface_L8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_L16, 0x13, 0, eSurface_AL8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16S, 0x14, 0, eSurface_L16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA16S, 0x15, 0, eSurface_RG16S);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R16UI, 0x16, 0, eSurface_RGBA16S);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16UI, 0x17, 0, eSurface_R16UI);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R16F, 0x20, 0, eSurface_RG16UI);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG16F, 0x21, 0, eSurface_R16F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA16F, 0x22, 0, eSurface_RG16F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R32F, 0x23, 0, eSurface_RGBA16F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RG32F, 0x24, 0, eSurface_R32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA32F, 0x25, 0, eSurface_RG32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGBA1010102F, 0x26, 0, eSurface_RGBA32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGB111110F, 0x27, 0, eSurface_RGBA1010102F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_RGB9E5F, 0x28, 0, eSurface_RGB111110F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DepthPCF16, 0x30, 0, eSurface_RGB9E5F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DepthPCF24, 0x31, 0, eSurface_DepthPCF16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth16, 0x32, 0, eSurface_DepthPCF24);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth24, 0x33, 0, eSurface_Depth16);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DepthStencil32, 0x34, 0, eSurface_Depth24);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth32F, 0x35, 0, eSurface_DepthStencil32);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth32F_Stencil8, 0x36, 0, eSurface_Depth32F);
			NEXTENUM(tex, mSurfaceFormat, eSurface_Depth24F_Stencil8, 0x37, 0, eSurface_Depth32F_Stencil8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT1, 64, 0, eSurface_Depth24F_Stencil8);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT3, 65, 0, eSurface_DXT1);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT5, 66, 0, eSurface_DXT3);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXT5A, 67, 0, eSurface_DXT5);
			NEXTENUM(tex, mSurfaceFormat, eSurface_DXTN, 68, 0, eSurface_DXT5A);
			NEXTENUM(tex, mSurfaceFormat, eSurface_CTX1, 69, 0, eSurface_DXTN);
			NEXTENUM(tex, mSurfaceFormat, eSurface_BC6, 70, 0, eSurface_CTX1);
			NEXTENUM(tex, mSurfaceFormat, eSurface_BC7, 71, 0, eSurface_BC6);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC2, 80, 0, eSurface_BC7);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC4, 81, 0, eSurface_PVRTC2);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC2a, 82, 0, eSurface_PVRTC4);
			NEXTENUM(tex, mSurfaceFormat, eSurface_PVRTC4a, 83, 0, eSurface_PVRTC2a);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATC_RGB, 96, 0, eSurface_PVRTC4a);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATC_RGB1A, 97, 0, eSurface_ATC_RGB);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATC_RGBA, 98, 0, eSurface_ATC_RGB1A);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC1_RGB, 112, 0, eSurface_ATC_RGBA);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RGB, 113, 0, eSurface_ETC1_RGB);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RGB1A, 114, 0, eSurface_ETC2_RGB);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RGBA, 115, 0, eSurface_ETC2_RGB1A);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_R, 116, 0, eSurface_ETC2_RGBA);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ETC2_RG, 117, 0, eSurface_ETC2_R);
			NEXTENUM(tex, mSurfaceFormat, eSurface_ATSC_RGBA_4x4, 128, 0, eSurface_ETC2_RG);
			NEXTENUM(tex, mSurfaceFormat, eSurface_R8, 14, 0, eSurface_ATSC_RGBA_4x4);
			ADDFLAGS(tex_mSurfaceFormat, MetaFlag::MetaFlag_EnumIntType);
			NEXTMEM2(tex, mTextureLayout, T3Texture, long, 0, mSurfaceFormat);
			ADDFLAGS(tex_mTextureLayout, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mTextureLayout, eTextureLayout_Unknown, -1, 0);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_2D, 0, 0, eTextureLayout_Unknown);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_Cube, 1, 0, eTextureLayout_2D);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_3D, 2, 0, eTextureLayout_Cube);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_2DArray, 3, 0, eTextureLayout_3D);
			NEXTENUM(tex, mTextureLayout, eTextureLayout_CubeArray, 4, 0, eTextureLayout_2DArray);
			NEXTMEM2(tex, mSurfaceGamma, T3Texture, long, 0, mTextureLayout);
			ADDFLAGS(tex_mSurfaceGamma, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mSurfaceGamma, eSurfaceGamma_Unknown, -1, 0);
			NEXTENUM(tex, mSurfaceGamma, eSurfaceGamma_Linear, 0, 0, eSurfaceGamma_Unknown);
			NEXTENUM(tex, mSurfaceGamma, eSurfaceGamma_sRGB, 1, 0, eSurfaceGamma_Linear);
			NEXTMEM4(tex, mSurfaceMultisample, T3Texture, long, 0, mSurfaceGamma, TelltaleToolLib_GetGameKeyIndex("BATMAN"), -1);
			ADDFLAGS(tex_mSurfaceMultisample, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_None, 0, 0);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_2x, 1, 0, eSurfaceMultisample_None);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_4x, 2, 0, eSurfaceMultisample_None);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_8x, 3, 0, eSurfaceMultisample_None);
			NEXTENUM(tex, mSurfaceMultisample, eSurfaceMultisample_16x, 4, 0, eSurfaceMultisample_None);
			NEXTMEM2(tex, mResourceUsage, T3Texture, long, 0, mSurfaceMultisample);
			ADDFLAGS(tex_mResourceUsage, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mResourceUsage, eResourceUsage_Static, 0, 0);
			NEXTENUM(tex, mResourceUsage, eResourceUsage_Dynamic, 1, 0, eResourceUsage_Static);
			NEXTENUM(tex, mResourceUsage, eResourceUsage_System, 2, 0, eResourceUsage_Dynamic);
			NEXTMEM2(tex, mType, T3Texture, long, 0, mResourceUsage);
			ADDFLAGS(tex_mType, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM(tex, mType, eTxUnknown, 0, 0);
			NEXTENUM(tex, mType, eTxLightmap_V0, 0x1, 0, eTxUnknown);
			NEXTENUM(tex, mType, eTxBumpmap, 0x2, 0, eTxLightmap_V0);
			NEXTENUM(tex, mType, eTxNormalMap, 0x3, 0, eTxBumpmap);
			NEXTENUM(tex, mType, eTxUNUSED1, 0x4, 0, eTxNormalMap);
			NEXTENUM(tex, mType, eTxUNUSED0, 0x5, 0, eTxUNUSED1);
			NEXTENUM(tex, mType, eTxSubsurfaceScatteringMap_V0, 0x6, 0, eTxUNUSED0);
			NEXTENUM(tex, mType, eTxSubsurfaceScatteringMap, 0x7, 0, eTxSubsurfaceScatteringMap_V0);
			NEXTENUM(tex, mType, eTxDetailMap, 0x8, 0, eTxSubsurfaceScatteringMap);
			NEXTENUM(tex, mType, eTxStaticShadowMap, 0x9, 0, eTxDetailMap);
			NEXTENUM(tex, mType, eTxLightmapHDR, 0xA, 0, eTxStaticShadowMap);
			NEXTENUM(tex, mType, eTxSDFDetailMap, 0xB, 0, eTxLightmapHDR);
			NEXTENUM(tex, mType, eTxEnvMap, 0xC, 0, eTxSDFDetailMap);
			NEXTENUM(tex, mType, eTxSpecularColor, 0xD, 0, eTxEnvMap);
			NEXTENUM(tex, mType, eTxToonLookup, 0xE, 0, eTxSpecularColor);
			NEXTENUM(tex, mType, eTxStandard, 0xF, 0, eTxToonLookup);
			NEXTENUM(tex, mType, eTxOutlineDiscontinuity, 0x10, 0, eTxStandard);
			NEXTENUM(tex, mType, eTxLightmapHDRScaled, 0x11, 0, eTxOutlineDiscontinuity);
			NEXTENUM(tex, mType, eTxEmissiveMap, 0x12, 0, eTxLightmapHDRScaled);
			NEXTENUM(tex, mType, eTxParticleProperties, 0x13, 0, eTxEmissiveMap);
			NEXTENUM(tex, mType, eTxBrushNormalMap, 0x14, 0, eTxParticleProperties);
			NEXTENUM(tex, mType, eTxUNUSED2, 0x15, 0, eTxBrushNormalMap);
			NEXTENUM(tex, mType, eTxNormalGlossMap, 0x16, 0, eTxUNUSED2);
			NEXTENUM(tex, mType, eTxLookup, 0x17, 0, eTxNormalGlossMap);
			NEXTENUM(tex, mType, eTxAmbientOcclusion, 0x18, 0, eTxLookup);
			NEXTENUM(tex, mType, eTxPrefilteredEnvCubeMapHDR, 0x19, 0, eTxAmbientOcclusion);
			NEXTENUM(tex, mType, eTxBrushLookupMap, 0x1A, 0, eTxPrefilteredEnvCubeMapHDR);
			NEXTENUM(tex, mType, eTxVector2Map, 0x1B, 0, eTxBrushLookupMap);
			NEXTENUM(tex, mType, eTxNormalDxDyMap, 0x1C, 0, eTxVector2Map);
			NEXTENUM(tex, mType, eTxPackedSDFDetailMap, 0x1D, 0, eTxNormalDxDyMap);
			NEXTENUM(tex, mType, eTxSingleChannelSDFDetailMap, 0x1E, 0, eTxPackedSDFDetailMap);
			NEXTENUM(tex, mType, eTxLightmapDirection, 0x1F, 0, eTxSingleChannelSDFDetailMap);
			NEXTENUM(tex, mType, eTxLightmapStaticShadows, 0x20, 0, eTxLightmapDirection);
			NEXTENUM(tex, mType, eTxLightStaticShadowMapAtlas, 0x21, 0, eTxLightmapStaticShadows);
			NEXTENUM(tex, mType, eTxLightStaticShadowMap, 0x22, 0, eTxLightStaticShadowMapAtlas);
			NEXTENUM(tex, mType, eTxPrefilteredEnvCubeMapHDRScaled, 0x23, 0, eTxLightStaticShadowMap);
			NEXTENUM(tex, mType, eTxLightStaticShadowVolume, 0x24, 0, eTxPrefilteredEnvCubeMapHDRScaled);
			NEXTENUM(tex, mType, eTxLightmapAtlas, 0x25, 0, eTxLightStaticShadowVolume);
			NEXTENUM(tex, mType, eTxNormalXYMap, 0x26, 0, eTxLightmapAtlas);
			NEXTENUM(tex, mType, eTxLightmapFlatAtlas, 0x27, 0, eTxNormalXYMap);
			NEXTENUM(tex, mType, eTxLookupXY, 0x28, 0, eTxLightmapFlatAtlas);
			NEXTENUM(tex, mType, eTxObjectNormalMap, 0x29, 0, eTxLookupXY);
			NEXTMEM4(tex, mSwizzle, T3Texture, swizzle, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled, mType,TelltaleToolLib_GetGameKeyIndex("MICHONNE"), -1);
			NEXTMEM4(tex, mNormalMapFormat, T3Texture, int, 0, mSwizzle, -1, TelltaleToolLib_GetGameKeyIndex("MCSM"));
			NEXTMEM2(tex, mSpecularGlossExponent, T3Texture, float, 0, mNormalMapFormat);
			NEXTMEM2(tex, mHDRLightmapScale, T3Texture, float, 0, mSpecularGlossExponent);
			NEXTMEM2(tex, mToonGradientCutoff, T3Texture, float, 0, mHDRLightmapScale);
			NEXTMEM2(tex, mAlphaMode, T3Texture, long, 0, mToonGradientCutoff);
			FIRSTENUM(tex, mAlphaMode, eTxAlphaUnkown, -1, 0);
			NEXTENUM(tex, mAlphaMode, eTxNoAlpha, 0, 0, eTxAlphaUnkown);
			NEXTENUM(tex, mAlphaMode, eTxAlphaTest, 1, 0, eTxNoAlpha);
			NEXTENUM(tex, mAlphaMode, eTxAlphaBlend, 2, 0, eTxAlphaTest);
			NEXTMEM2(tex, mColorMode, T3Texture, long, 0, mAlphaMode);
			FIRSTENUM(tex, mColorMode, eTxColorUnknown, -1, 0);
			NEXTENUM(tex, mColorMode, eTxColorFull, 0, 0, eTxColorUnknown);
			NEXTENUM(tex, mColorMode, eTxColorBlack, 1, 0, eTxColorFull);
			NEXTENUM(tex, mColorMode, eTxColorGrayscale, 2, 0, eTxColorBlack);
			NEXTENUM(tex, mColorMode, eTxColorGrayscaleAlpha, 3, 0, eTxColorGrayscale);
			NEXTMEM2(tex, mUVOffset, T3Texture, vec2, 0, mColorMode);
			NEXTMEM2(tex, mUVScale, T3Texture, vec2, 0, mUVOffset);
			NEXTMEM2(tex, mNumMipLevelsAllocated, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable | MetaFlag::MetaFlag_EditorHide, mUVScale);
			NEXTMEM2(tex, mNumSurfacesRequested, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable | MetaFlag::MetaFlag_EditorHide, mNumMipLevelsAllocated);
			NEXTMEM2(tex, mNumSurfacesRequired, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable | MetaFlag::MetaFlag_EditorHide, mNumSurfacesRequested);
			NEXTMEM2(tex, mNumSurfacesLoaded, T3Texture, long, MetaFlag::MetaFlag_MetaSerializeDisable | MetaFlag::MetaFlag_EditorHide, mNumSurfacesRequired);
			NEXTMEM4(tex, mArrayFrameNames, T3Texture, DCArray_Symbol, 0, mNumSurfacesLoaded, TelltaleToolLib_GetGameKeyIndex("BATMAN"),-1);
			NEXTMEM2(tex, mToonRegions, T3Texture, DCArray_T3ToonGradientRegion, MetaFlag::MetaFlag_EditorHide, mArrayFrameNames);
			SERIALIZER(tex, T3Texture);
			ADD(tex);

			DEFINET2(theader, T3Texture::StreamHeader);
			FIRSTMEM2(theader, mRegionCount, T3Texture::StreamHeader, long, 0);
			NEXTMEM2(theader, mAuxDataCount, T3Texture::StreamHeader, long, 0, mRegionCount);
			NEXTMEM2(theader, mTotalDataSize, T3Texture::StreamHeader, long, 0, mAuxDataCount);
			ADD(theader);

			DEFINET2(rsh, T3Texture::RegionStreamHeader);
			FIRSTMEM2(rsh, mFaceIndex, T3Texture::RegionStreamHeader, long, 0);
			NEXTMEM2(rsh, mMipIndex, T3Texture::RegionStreamHeader, long, 0, mFaceIndex);
			NEXTMEM2(rsh, mMipCount, T3Texture::RegionStreamHeader, long, 0, mMipIndex);
			NEXTMEM2(rsh, mDataSize, T3Texture::RegionStreamHeader, long, 0, mMipCount);
			NEXTMEM2(rsh, mPitch, T3Texture::RegionStreamHeader, long, 0, mDataSize);
			NEXTMEM4(rsh, mSlicePitch, T3Texture::RegionStreamHeader, long, 0, mPitch, TelltaleToolLib_GetGameKeyIndex("BATMAN"),-1);
			ADD(rsh);

			DEFINET2(aux, T3Texture::AuxilaryData);
			FIRSTMEM2(aux, mType, T3Texture::AuxilaryData, symbol, 0);
			NEXTMEM2(aux, mData, T3Texture::AuxilaryData, bb, 0, mType);
			ADD(aux);

			DEFINET2(locreg, LocalizationRegistry);
			EXT(locreg, locreg);
			FIRSTMEM2(locreg, mFlagIndexMap, LocalizationRegistry, Map_Symbol_int, 0);
			NEXTMEM2(locreg, mFlagIndexMapReverse, LocalizationRegistry, Map_int_Symbol, 0, mFlagIndexMap);
			NEXTMEM2(locreg, mToolProps, LocalizationRegistry, tp, 0, mFlagIndexMapReverse);
			meta_locreg_mToolProps.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("wdc");
			ADD(locreg);

			DEFINEHANDLE(dlg, Dlg);

			DEFINET2(dlgset, LanguageLookupMap::DlgIDSet);
			FIRSTMEM2(dlgset, mIDRange, LanguageLookupMap::DlgIDSet, rangeuint, 0);
			NEXTMEM2(dlgset, mAdditionalIDs, LanguageLookupMap::DlgIDSet, Set_u32, 0, mIDRange);
			NEXTMEM2(dlgset, mhDlg, LanguageLookupMap::DlgIDSet, Handledlg, 0, mAdditionalIDs);
			ADD(dlgset);

			DEFINEDCARRAY2(LanguageLookupMap::DlgIDSet, dlgsetarr);

			DEFINET2(llm, LanguageLookupMap);
			EXT(llm, llm);
			FIRSTMEM2(llm, mIDSets, LanguageLookupMap, DCArray_dlgsetarr, 0);
			ADD(llm);

			DEFINEKEYFRAMEDVALUE(float, float, float);

			DEFINET2(trm, TransitionRemapper);
			FIRSTMEM2(trm, mRemapKeys, TransitionRemapper, kfv_float, 0);
			ADD(trm);

			DEFINET2(tmi, TransitionMap::TransitionMapInfo);
			FIRSTMEM2(tmi, mRemapper, TransitionMap::TransitionMapInfo, trm, 0);
			ADD(tmi);

			DEFINEMAP2(Symbol, TransitionMap::TransitionMapInfo, symbol, tmapinfo, Symbol::CompareCRC);
			MKNAME(meta_Map_symbol_tmapinfo, "Map<Symbol,TransitionMap::TransitionMapInfo,less<Symbol>>");

			DEFINET2(tmap, TransitionMap);
			EXT(tmap, tmap);
			FIRSTMEM2(tmap, mTransitionRemappers, TransitionMap, Map_symbol_tmapinfo, 0);
			ADD(tmap);

			DEFINET2(enumbase, EnumBase);
			ADDFLAGS(enumbase, 0x21);
			ADD(enumbase);

			DEFINET2(soundmat, SoundFootsteps::EnumMaterial);
			ADDFLAGS(soundmat, 0x8008);
			FIRSTMEM2(soundmat, mVal, SoundFootsteps::EnumMaterial, long, 0x40);
			FIRSTENUM(soundmat, mVal, Default, SoundFootsteps::Default, 0);
			NEXTENUM(soundmat, mVal, Carpet, 2, 0, Default);
			NEXTENUM(soundmat, mVal, Concrete, 3, 0, Carpet);
			NEXTENUM2(soundmat, mVal, "Concrete (Wet)", Concrete_Wet, 4, 0, Concrete);
			NEXTENUM2(soundmat, mVal, "Dirt", Dirt, 5, 0, Concrete_Wet);
			NEXTENUM2(soundmat, mVal, "Grass", Grass, 6, 0, Dirt);
			NEXTENUM2(soundmat, mVal, "Grass (Tall)", Grass_Tall, 7, 0, Grass);
			NEXTENUM2(soundmat, mVal, "Gravel", Gravel, 8, 0, Grass_Tall);
			NEXTENUM2(soundmat, mVal, "Leaves", Leaves, 9, 0, Gravel);
			NEXTENUM2(soundmat, mVal, "Linoleum", Linoleum, 10, 0, Leaves);
			NEXTENUM2(soundmat, mVal, "Metal (Thick)", Metal_Thick, 11, 0, Linoleum);
			NEXTENUM2(soundmat, mVal, "Metal (Thin)", Metal_Thin, 12, 0, Metal_Thick);
			NEXTENUM2(soundmat, mVal, "Mud", Mud, 13, 0, Metal_Thin);
			NEXTENUM2(soundmat, mVal, "Puddle", Puddle, 14, 0, Mud);
			NEXTENUM2(soundmat, mVal, "Sand", Sand, 15, 0, Puddle);
			NEXTENUM2(soundmat, mVal, "Snow", Snow, 16, 0, Sand);
			NEXTENUM2(soundmat, mVal, "Tile (Hard)", Tile_Hard, 17, 0, Snow);
			NEXTENUM2(soundmat, mVal, "Wood", Wood, 18, 0, Tile_Hard);
			DEFINEM(soundmat, base);
			meta_soundmat_base.mpName = "Baseclass_EnumBase";
			meta_soundmat_base.mOffset = PARENT_OFFSET(EnumBase, SoundFootsteps::EnumMaterial);
			meta_soundmat_base.mpMemberDesc = &meta_enumbase;
			meta_soundmat_mVal.mpNextMember = &meta_soundmat_base;
			ADD(soundmat);

			DEFINET2(wboxedge, WalkBoxes::Edge);
			FIRSTMEM2(wboxedge, mV1, WalkBoxes::Edge, long, 0);
			NEXTMEM2(wboxedge, mV2, WalkBoxes::Edge, long, 0, mV1);
			NEXTMEM2(wboxedge, mEdgeDest, WalkBoxes::Edge, long, 0, mV2);
			NEXTMEM2(wboxedge, mEdgeDestEdge, WalkBoxes::Edge, long, 0, mEdgeDest);
			NEXTMEM2(wboxedge, mEdgeDir, WalkBoxes::Edge, long, 0, mEdgeDestEdge);
			NEXTMEM2(wboxedge, mMaxRadius, WalkBoxes::Edge, float, 0, mEdgeDir);
			ADD(wboxedge);

			DEFINET2(wboxquad, WalkBoxes::Quad);
			FIRSTMEM2(wboxquad, mVerts, WalkBoxes::Quad, sarray_i32_4, 0);
			ADD(wboxquad);

			DEFINET2(wboxvert, WalkBoxes::Vert);
			FIRSTMEM2(wboxvert, mFlags, WalkBoxes::Vert, flags, 0);
			NEXTMEM2(wboxvert, mPos, WalkBoxes::Vert, vec3, 0, mFlags);
			ADD(wboxvert);

			DEFINESARRAY_(WalkBoxes::Edge, wboxedge, 3);

			DEFINET2(wboxtri, WalkBoxes::Tri);
			FIRSTMEM2(wboxtri, mFootstepMaterial, WalkBoxes::Tri, soundmat, 0);
			NEXTMEM2(wboxtri, mFlags, WalkBoxes::Tri, flags, 0, mFootstepMaterial);
			NEXTMEM2(wboxtri, mNormal, WalkBoxes::Tri, long, 0, mFlags);
			NEXTMEM2(wboxtri, mQuadBuddy, WalkBoxes::Tri, long, 0, mNormal);
			NEXTMEM2(wboxtri, mMaxRadius, WalkBoxes::Tri, float, 0, mQuadBuddy);
			NEXTMEM2(wboxtri, mVerts, WalkBoxes::Tri, sarray_i32_3, 0, mMaxRadius);
			NEXTMEM2(wboxtri, mEdgeInfo, WalkBoxes::Tri, sarray_wboxedge_3, 0, mVerts);
			NEXTMEM2(wboxtri, mVertOffsets, WalkBoxes::Tri, sarray_i32_3, 0, mEdgeInfo);
			NEXTMEM2(wboxtri, mVertScales, WalkBoxes::Tri, sarray_float_3, 0, mVertOffsets);
			ADD(wboxtri);

			DEFINEDCARRAY2(WalkBoxes::Tri, tri);
			DEFINEDCARRAY2(WalkBoxes::Vert, vert);
			DEFINEDCARRAY2(WalkBoxes::Quad, quad);
			DEFINEDCARRAY2(Vector3, vec3);

			DEFINET2(wbox, WalkBoxes);
			EXT(wbox, wbox);
			SERIALIZER(wbox, WalkBoxes);
			FIRSTMEM2(wbox, mName, WalkBoxes, string, 0);
			NEXTMEM2(wbox, mTris, WalkBoxes, DCArray_tri, 0, mName);
			NEXTMEM2(wbox, mVerts, WalkBoxes, DCArray_vert, 0, mTris);
			NEXTMEM2(wbox, mNormals, WalkBoxes, DCArray_vec3, 0, mVerts);
			NEXTMEM2(wbox, mQuads, WalkBoxes, DCArray_quad, 0x20, mNormals);
			ADD(wbox);

			DEFINET2(sebmp, SoundEventBankMap);
			EXT(sebmp, soundeventbankmap);
			FIRSTMEM2(sebmp, mBankMap, SoundEventBankMap, Map_String_dcarraystring, 0);
			NEXTMEM2(sebmp, mbLoadAllBanksGlobally, SoundEventBankMap, bool, 0, mBankMap);
			ADD(sebmp);

			DEFINET2(sedd, SoundEventData);
			ADDFLAGS(sedd, MetaFlag::MetaFlag_DontAsyncLoad | MetaFlag::MetaFlag_VirtualResource);
			ADD(sedd);

			DEFINET2(sedd1, SoundEventSnapshotData);
			ADDFLAGS(sedd1, MetaFlag::MetaFlag_DontAsyncLoad | MetaFlag::MetaFlag_VirtualResource);
			ADD(sedd1);

			DEFINEHANDLE(seddh, SoundEventData);
			DEFINEHANDLE(sedd1h, SoundEventSnapshotData);

			DEFINEHANDLE(hanm, Animation);
			DEFINEHANDLE(hchore, Chore);

			DEFINET2(animorchore, AnimOrChore);
			ADDFLAGS(animorchore, MetaFlag::MetaFlag_PlaceInAddPropMenu);
			FIRSTMEM2(animorchore, mhAnim, AnimOrChore, Handlehanm, 0);
			NEXTMEM2(animorchore, mhChore, AnimOrChore, Handlehchore, 0, mhAnim);
			ADD(animorchore);

			DEFINET2(resgroups, ResourceGroups);
			ADDFLAGS(resgroups, MetaFlag::MetaFlag_EditorHide);
			FIRSTMEM2(resgroups, mGroups, ResourceGroups, Map_Symbol_float, 0);
			ADD(resgroups);

			DEFINET2(propo, ActingOverridablePropOwner);
			FIRSTMEM2(propo, mSerializationFlags, ActingOverridablePropOwner, flags, 0);
			SERIALIZER(propo, ActingOverridablePropOwner);
			ADD(propo);

			DEFINET2(tanmode, EnumeTangentModes);
			FIRSTMEM2(tanmode, mVal, EnumeTangentModes, long, 0);
			FIRSTENUM(tanmode, mVal, eTangentUnknown, eTangentUnknown, 0);
			NEXTENUM(tanmode, mVal, eTangentStepped, eTangentStepped, 0, eTangentUnknown);
			NEXTENUM(tanmode, mVal, eTangentKnot, eTangentKnot, 0, eTangentStepped);
			NEXTENUM(tanmode, mVal, eTangentSmooth, eTangentSmooth, 0, eTangentKnot);
			NEXTENUM(tanmode, mVal, eTangentFlat, eTangentFlat, 0, eTangentSmooth);
			NEXTMEM1(tanmode, "Baseclass_EnumBase", BASE_CLASS, mVal,
				EnumeTangentModes, enumbase, 0, mVal);
			ADD(tanmode);

			DEFINET2(actres, ActingResource);
			FIRSTMEM(actres, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingResource, propo, 0);
			NEXTMEM2(actres, mResource, ActingResource, animorchore, MetaFlag::MetaFlag_EditorHide, mSerializationFlags);
			NEXTMEM2(actres, mValidIntensityRange, ActingResource, rangef, 0, mResource);
			ADD(actres);

			DEFINET2(actdur, ActingPalette::EnumActiveDuring);
			ADDFLAGS(actdur, 0x8008);
			FIRSTMEM2(actdur, mVal, ActingPalette::EnumActiveDuring, long, 0);
			FIRSTENUM(actdur, mVal, always, ActingPalette::ActiveDuring::always, 0);
			NEXTENUM(actdur, mVal, talking, ActingPalette::ActiveDuring::talking, 0, always);
			NEXTENUM(actdur, mVal, listening, ActingPalette::ActiveDuring::listening, 0, talking);
			NEXTMEM1(actdur, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumActiveDuring, enumbase, 0x10, mVal);
			ADD(actdur);

			DEFINET2(actrun1, ActingAccentPalette::EnumOverrun);
			ADDFLAGS(actrun1, 0x8008);
			FIRSTMEM2(actrun1, mVal, ActingAccentPalette::EnumOverrun, long, 0);
			FIRSTENUM(actrun1, mVal, disallowed, ActingAccentPalette::Overrun::disallowed, 0);
			NEXTENUM(actrun1, mVal, allowed, ActingAccentPalette::Overrun::allowed, 0, disallowed);
			NEXTMEM1(actrun1, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingAccentPalette::EnumOverrun, enumbase, 0x10, mVal);
			ADD(actrun1);

			DEFINET2(actrun, ActingPalette::EnumOverrun);
			ADDFLAGS(actrun, 0x8008);
			FIRSTMEM2(actrun, mVal, ActingPalette::EnumOverrun, long, 0);
			FIRSTENUM(actrun, mVal, disallowed, ActingPalette::Overrun::disallowed, 0);
			NEXTENUM(actrun, mVal, allowed, ActingPalette::Overrun::allowed, 0, disallowed);
			NEXTMEM1(actrun, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumOverrun, enumbase, 0x10, mVal);
			ADD(actrun);

			DEFINET2(actrel, ActingPalette::EnumEndRelativeTo);
			ADDFLAGS(actrel, 0x8008);
			FIRSTMEM2(actrel, mVal, ActingPalette::EnumEndRelativeTo, long, 0);
			FIRSTENUM(actrel, mVal, beginning, ActingPalette::EndRelativeTo::beginning, 0);
			NEXTENUM(actrel, mVal, end, ActingPalette::EndRelativeTo::end, 0, beginning);
			NEXTENUM(actrel, mVal, transition, ActingPalette::EndRelativeTo::transition, 0, end);
			NEXTMEM1(actrel, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumEndRelativeTo, enumbase, 0x10, mVal);
			ADD(actrel);

			DEFINET2(aresp, ActingResource*);
			ADD(aresp);
			DEFINEDCARRAY2(ActingResource*, actresp);

			DEFINET2(actp, ActingPalette);
			FIRSTMEM(actp, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingPalette, propo, 0x10);
			NEXTMEM(actp, "Baseclass_UID::Owner", miUniqueID, ActingPalette, uidowner, 0x10, mSerializationFlags);
			SERIALIZER(actp, ActingPalette);
			NEXTMEM2(actp, mName, ActingPalette, string, 0x20, miUniqueID);
			NEXTMEM2(actp, mActiveDuring, ActingPalette, actdur, 0, mName);
			NEXTMEM2(actp, mTimeBetweenActions, ActingPalette, rangef, 0, mActiveDuring);
			NEXTMEM2(actp, mFirstActionDelayRange, ActingPalette, rangef, 0, mTimeBetweenActions);
			NEXTMEM2(actp, mSpilloutBufPreRange, ActingPalette, rangef, 0x20, mFirstActionDelayRange);
			NEXTMEM2(actp, mSpilloutBufPostRange, ActingPalette, rangef, 0, mSpilloutBufPreRange);
			NEXTMEM2(actp, mLatestStartOffsetRange, ActingPalette, rangef, 0, mSpilloutBufPostRange);
			NEXTMEM2(actp, mValidIntensityRange, ActingPalette, rangef, 0, mLatestStartOffsetRange);
			NEXTMEM2(actp, mResourcePtrs, ActingPalette, DCArray_actresp, 0x21, mValidIntensityRange);
			NEXTMEM2(actp, mGroupMembershipUID, ActingPalette, long, 0x20, mResourcePtrs);
			NEXTMEM2(actp, mFlags, ActingPalette, flags, 0x20, mGroupMembershipUID);
			NEXTMEM2(actp, mFirstActionEndRel, ActingPalette, actrel, 0, mFlags);
			NEXTMEM2(actp, mEndOffsetRel, ActingPalette, actrel, 0, mFirstActionEndRel);
			NEXTMEM2(actp, mLatestStartOffsetRel, ActingPalette, actrel, 0, mEndOffsetRel);
			NEXTMEM2(actp, mOverrunAllowed, ActingPalette, actrun, 0, mLatestStartOffsetRel);
			NEXTMEM2(actp, mMoodOverrunAllowed, ActingPalette, actrun, 0, mOverrunAllowed);
			NEXTMEM2(actp, mDisableAct, ActingPalette, bool, 0, mMoodOverrunAllowed);
			NEXTMEM2(actp, mJunket, ActingPalette, long, 0, mDisableAct);
			ADD(actp);


			DEFINET2(actap, ActingAccentPalette);
			ADDFLAGS(actap, 0x40);
			SERIALIZER(actap, ActingAccentPalette);
			FIRSTMEM(actap, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingAccentPalette, propo, 0x10);
			NEXTMEM(actap, "Baseclass_UID::Owner", miUniqueID, ActingAccentPalette, uidowner, 0x10, mSerializationFlags);
			NEXTMEM2(actap, mName, ActingAccentPalette, string, 0, miUniqueID);
			NEXTMEM2(actap, mStartOffsetRange, ActingAccentPalette, rangef, 0, mName);
			NEXTMEM2(actap, mMoodOverrunAllowed, ActingAccentPalette, actrun1, 0, mStartOffsetRange);
			NEXTMEM2(actap, mDisableAct, ActingAccentPalette, bool, 0, mMoodOverrunAllowed);
			NEXTMEM2(actap, mValidIntensityRange, ActingAccentPalette, rangef, 0, mDisableAct);
			NEXTMEM2(actap, mSpilloutBufPostRange, ActingAccentPalette, rangef, 0, mValidIntensityRange);
			NEXTMEM2(actap, mRandomChance, ActingAccentPalette, float, 0, mSpilloutBufPostRange);
			NEXTMEM2(actap, mTrackID, ActingAccentPalette, long, 0, mRandomChance);
			FIRSTENUM2(actap, mTrackID, "Body", Body, 1, 0);
			NEXTENUM2(actap, mTrackID, "Face", Face, 2, 0, Body);
			NEXTENUM2(actap, mTrackID, "Head 1", Head1, 3, 0, Face);
			NEXTENUM2(actap, mTrackID, "Head 2", Head2, 4, 0, Head1);
			NEXTMEM2(actap, mResourcePtrs, ActingAccentPalette, DCArray_actresp, 0x21, mTrackID);
			NEXTMEM2(actap, mGroupMembershipUID, ActingAccentPalette, long, 0x20, mResourcePtrs);
			NEXTMEM2(actap, mFlags, ActingAccentPalette, flags, 0x20, mGroupMembershipUID);
			NEXTMEM2(actap, mVersion, ActingAccentPalette, long, 0x20, mFlags);
			ADD(actap);

			DEFINEHANDLE(tmap, TransitionMap);

			DEFINET2(idlet, ActingPaletteGroup::EnumIdleTransition);
			ADDFLAGS(idlet, 0x8008);
			FIRSTMEM(idlet, "mVal", mVal, ActingPaletteGroup::EnumIdleTransition, long, 0);
			FIRSTENUM(idlet, mVal, transitionLinear, 1, 0);
			NEXTENUM(idlet, mVal, transitionEaseInOut, 2, 0, transitionLinear);
			NEXTENUM(idlet, mVal, transitionUnused, 3, 0, transitionEaseInOut);
			NEXTMEM1(idlet, "Baseclass_EnumBase", BASE_CLASS, mVal,
				ActingPalette::EnumEndRelativeTo, enumbase, 0x10, mVal);
			ADD(idlet);

			DEFINET2(actpt, ActingPaletteGroup::ActingPaletteTransition);
			FIRSTMEM2(actpt, mTransition, ActingPaletteGroup::ActingPaletteTransition, string, 0);
			NEXTMEM2(actpt, mTransitionIn, ActingPaletteGroup::ActingPaletteTransition, animorchore, 0, mTransition);
			NEXTMEM2(actpt, mCenterOffset, ActingPaletteGroup::ActingPaletteTransition, float, 0, mTransitionIn);
			NEXTMEM2(actpt, mPreDelay, ActingPaletteGroup::ActingPaletteTransition, float, 0, mCenterOffset);
			NEXTMEM2(actpt, mPostDelay, ActingPaletteGroup::ActingPaletteTransition, float, 0, mPreDelay);
			NEXTMEM2(actpt, mFadeTime, ActingPaletteGroup::ActingPaletteTransition, float, 0, mPostDelay);
			ADD(actpt);

			DEFINELIST_(ActingPaletteGroup::ActingPaletteTransition, acttrans);

			DEFINET2(actg, ActingPaletteGroup);
			SERIALIZER(actg, ActingPaletteGroup);
			FIRSTMEM(actg, "Baseclass_UID::Owner", miUniqueID, ActingPaletteGroup, uidowner, 0x10, mSerializationFlags);
			NEXTMEM2(actg, mName, ActingPaletteGroup, string, 0x20, miUniqueID);
			NEXTMEM2(actg, mIdle, ActingPaletteGroup, animorchore, 0, mName);
			NEXTMEM2(actg, mTalkingIdle, ActingPaletteGroup, animorchore, 0x20, mIdle);
			NEXTMEM2(actg, mMumbleMouth, ActingPaletteGroup, animorchore, 0x20, mTalkingIdle);
			NEXTMEM2(actg, mWeight, ActingPaletteGroup, float, 0x20, mMumbleMouth);
			NEXTMEM2(actg, mTransitionIn, ActingPaletteGroup, animorchore, 0x20, mWeight);
			NEXTMEM2(actg, mTransitionOut, ActingPaletteGroup, animorchore, 0x20, mTransitionIn);
			NEXTMEM2(actg, mTransitions, ActingPaletteGroup, List_acttrans, 0x20, mTransitionOut);
			NEXTMEM2(actg, mIdleTransitionTimeOverride, ActingPaletteGroup, float, 0, mTransitions);
			NEXTMEM2(actg, mhIdleTransitionMap, ActingPaletteGroup, Handletmap, 0, mIdleTransitionTimeOverride);
			meta_actg_mhIdleTransitionMap.mMinMetaVersion = 6;//only in new
			NEXTMEM2(actg, mIdleTransitionKind, ActingPaletteGroup, idlet, 0, mhIdleTransitionMap);
			NEXTMEM2(actg, mRandomAutoMin, ActingPaletteGroup, float, 0, mIdleTransitionKind);
			NEXTMEM2(actg, mRandomAutoMax, ActingPaletteGroup, float, 0, mRandomAutoMin);
			ADD(actg);

			DEFINET2(actpp, ActingPalette*);
			ADD(actpp);

			DEFINEDCARRAY2(ActingPalette*, actpp);

			DEFINET2(actapp, ActingAccentPalette*);
			ADD(actapp);

			DEFINEDCARRAY2(ActingAccentPalette*, actapp);

			DEFINET2(actppg, ActingPaletteGroup*);
			ADD(actppg);

			DEFINEDCARRAY2(ActingPaletteGroup*, actppg);

			DEFINET2(actc, ActingPaletteClass);
			SERIALIZER(actc, ActingPaletteClass);
			FIRSTMEM(actc, "Baseclass_UID::Generator", miNextUniqueID, ActingPaletteClass, uidgen, 0x10);
			NEXTMEM(actc, "Baseclass_UID::Owner", miUniqueID, ActingPaletteClass, uidowner, 0x10, miNextUniqueID);
			NEXTMEM(actc, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, ActingPaletteClass, propo, 0x10, miUniqueID);
			NEXTMEM2(actc, mName, ActingPaletteClass, string, 0, mSerializationFlags);
			NEXTMEM2(actc, mPalettePtrs, ActingPaletteClass, DCArray_actpp, 0x21, mName);
			NEXTMEM2(actc, mAccentPalettePtrs, ActingPaletteClass, DCArray_actapp, 0x21, mPalettePtrs);
			NEXTMEM2(actc, mPaletteGroupPtrs, ActingPaletteClass, DCArray_actppg, 0x21, mAccentPalettePtrs);
			NEXTMEM2(actc, mAlternateNames, ActingPaletteClass, DCArray_String, 0, mPaletteGroupPtrs);
			NEXTMEM2(actc, mDefaultPaletteGroupID, ActingPaletteClass, long, 0x20, mAlternateNames);
			NEXTMEM2(actc, mFlags, ActingPaletteClass, flags, 0x20, mDefaultPaletteGroupID);
			NEXTMEM2(actc, mInstantChange, ActingPaletteClass, bool, 0x20, mFlags);
			ADD(actc);

			DEFINEDCARRAY2(ActingPaletteClass*, actcp);
			DEFINEDCARRAY2(ActingPaletteClass, actc);
			DEFINET2(sg, StyleGuide);
			SERIALIZER(sg, StyleGuide);
			EXT(sg, style);
			FIRSTMEM(sg, "Baseclass_UID::Generator", miNextUniqueID, StyleGuide, uidgen, 0x10);
			NEXTMEM(sg, "Baseclass_ActingOverridablePropOwner", mSerializationFlags, StyleGuide, propo, 0x10, miNextUniqueID);
			NEXTMEM2(sg, mDefPaletteClassID, StyleGuide, long, 0, mSerializationFlags);
			NEXTMEM2(sg, mbGeneratesLookAts, StyleGuide, bool, 0, mDefPaletteClassID);
			NEXTMEM2(sg, mPaletteClassPtrs, StyleGuide, DCArray_actcp, 1, mbGeneratesLookAts);
			NEXTMEM2(sg, mFlags, StyleGuide, flags, 0, mPaletteClassPtrs);
			NEXTMEM2(sg, mPaletteClasses, StyleGuide, DCArray_actc, 0x20, mFlags);
			NEXTMEM2(sg, mDefPaletteClassIndex, StyleGuide, long, 0x20, mPaletteClasses);
			ADD(sg);

			DEFINEHANDLE(sound, SoundData);

			DEFINET(ptrbase, void*);
			meta_ptrbase.Initialize("Ptr<PtrBase>");
			meta_ptrbase.mFlags |= 1u;
			ADD(ptrbase);

			DEFINET2(lr, LanguageResource);
			SERIALIZER(lr, LanguageResource);
			EXT(lr, langres);
			FIRSTMEM2(lr, mId, LanguageResource, long, 0);
			NEXTMEM2(lr, mPrefix, LanguageResource, string, 0, mId);
			NEXTMEM2(lr, mText, LanguageResource, string, 0, mPrefix);
			NEXTMEM2(lr, mhAnimation, LanguageResource, Handlehanm, 0, mText);
			NEXTMEM2(lr, mhVoiceData, LanguageResource, Handlesound, 0, mhAnimation);
			NEXTMEM2(lr, mShared, LanguageResource, bool, 0, mhVoiceData);
			NEXTMEM2(lr, mAllowSharing, LanguageResource, bool, 0, mShared);
			NEXTMEM2(lr, mbNoAnim, LanguageResource, bool, 0, mAllowSharing);
			NEXTMEM2(lr, mpLipsyncAnimation, LanguageResource, ptrbase, 0, mbNoAnim);
			NEXTMEM2(lr, mFlags, LanguageResource, flags, 0, mpLipsyncAnimation);
			static MetaFlagDescription synth;
			synth.mpFlagName = "Synthesized";
			synth.mFlagValue = 1;
			static MetaFlagDescription synth1;
			synth1.mpFlagName = "Lipsync ignore text";
			synth1.mFlagValue = 2;
			synth1.mpNext = &synth;
			static MetaFlagDescription synth2;
			synth2.mpFlagName = "Hide Subtitles";
			synth2.mFlagValue = 4;
			synth2.mpNext = &synth1;
			static MetaFlagDescription synth3;
			synth3.mpFlagName = "Is Voiced";
			synth3.mFlagValue = 8;
			synth3.mpNext = &synth2;
			static MetaFlagDescription synth4;
			synth4.mpFlagName = "Is Silent";
			synth4.mFlagValue = 16;
			synth4.mpNext = &synth3;
			meta_lr_mFlags.mpFlagDescriptions = &synth4;
			meta_lr_mFlags.mFlags = MetaFlag::MetaFlag_FlagType;
			ADD(lr);

			DEFINEMAP(int, LanguageResource, std::less<int>);
			MKNAME(meta_Map_int_LanguageResource, "Map<int,LanguageResource,less<int>>");

			DEFINET2(langdb, LanguageDatabase);
			EXT(langdb, langdb);
			SERIALIZER(langdb, LanguageDatabase);
			FIRSTMEM2(langdb, mLanguageResources, LanguageDatabase,
				Map_int_LanguageResource, 0);
			NEXTMEM2(langdb, mName, LanguageDatabase, string, 0, mLanguageResources);
			ADD(langdb);

			DEFINET2(locali, LocalizeInfo);
			FIRSTMEM2(locali, mFlags, LocalizeInfo, flags, 0);
			ADD(locali);

			DEFINET2(langresl, LanguageResLocal);
			ADDFLAGS(langresl, MetaFlag::MetaFlag_NoPanelCaption);
			FIRSTMEM2(langresl, mPrefix, LanguageResLocal, string, 0);
			NEXTMEM2(langresl, mText, LanguageResLocal, string, 0, mPrefix);
			NEXTMEM2(langresl, mLocalInfo, LanguageResLocal, locali, 0x20, mText);
			ADD(langresl);

			DEFINEDCARRAY2(LanguageResLocal, lresl);

			DEFINET2(pidp, ProjectDatabaseIDPair);
			FIRSTMEM2(pidp, mProjectID, ProjectDatabaseIDPair, long, 0);
			NEXTMEM2(pidp, mDBID, ProjectDatabaseIDPair, long, 0, mProjectID);
			ADD(pidp);

			DEFINET2(recs, RecordingUtils::EnumRecordingStatus);
			FIRSTMEM2(recs, mVal, RecordingUtils::EnumRecordingStatus, long, 0);
			FIRSTENUM2(recs, mVal, "Not Recorded", notrec, 0, 0);
			NEXTENUM2(recs, mVal, "Sent To Studio", sent, 0, 1, notrec);
			NEXTENUM2(recs, mVal, "Recorded", rec, 0, 2, sent);
			NEXTENUM2(recs, mVal, "Delivered", del, 0, 3, rec);
			NEXTMEM1(recs, "Baseclass_EnumBase", BASE_CLASS, mVal,
				RecordingUtils::EnumRecordingStatus, enumbase, 0x10, mVal);
			ADD(recs);

			DEFINET2(lres, LanguageRes);
			FIRSTMEM2(lres, mResName, LanguageRes, symbol, 0);
			NEXTMEM2(lres, mID, LanguageRes, long, 0x20, mResName);
			NEXTMEM2(lres, mIDAlias, LanguageRes, long, 0, mID);
			NEXTMEM2(lres, mhAnimation, LanguageRes, Handlehanm, 0, mIDAlias);
			NEXTMEM2(lres, mhVoiceData, LanguageRes, Handlesound, 0, mhAnimation);
			NEXTMEM2(lres, mLocalData, LanguageRes, DCArray_lresl, 0x20, mhVoiceData);
			NEXTMEM2(lres, mLengthOverride, LanguageRes, float, 0x20, mLocalData);
			NEXTMEM2(lres, mResolvedLocalData, LanguageRes, langresl, 0x20, mLengthOverride);
			NEXTMEM2(lres, mRecordingStatus, LanguageRes, recs, 0x20, mResolvedLocalData);
			NEXTMEM2(lres, mFlags, LanguageRes, flags, 0x20, mRecordingStatus);
			static MetaFlagDescription asynth;
			asynth.mpFlagName = "Synthesized";
			asynth.mFlagValue = 1;
			static MetaFlagDescription asynth1;
			asynth1.mpFlagName = "Lipsync ignore text";
			asynth1.mFlagValue = 2;
			asynth1.mpNext = &asynth;
			static MetaFlagDescription asynth2;
			asynth2.mpFlagName = "Hide Subtitles";
			asynth2.mFlagValue = 4;
			asynth2.mpNext = &asynth1;
			static MetaFlagDescription asynth3;
			asynth3.mpFlagName = "Is Voiced";
			asynth3.mFlagValue = 8;
			asynth3.mpNext = &asynth2;
			static MetaFlagDescription asynth4;
			asynth4.mpFlagName = "Is Silent";
			asynth4.mFlagValue = 16;
			asynth4.mpNext = &asynth3;
			static MetaFlagDescription asynth5;
			asynth5.mpFlagName = "No Anim";
			asynth5.mFlagValue = 32;
			asynth5.mpNext = &asynth4;
			meta_lres_mFlags.mpFlagDescriptions = &asynth5;
			meta_lres_mFlags.mFlags = MetaFlag::MetaFlag_FlagType;
			EXT(lres, lang);
			ADD(lres);

			DEFINET2(lanreg, LanguageRegister);
			FIRSTMEM(lanreg, "Baseclass_UID::Generator", miNextUniqueID, LanguageRegister, uidgen, 0x10);
			EXT(lanreg, lanreg);
			ADD(lanreg);

			DEFINEDCARRAY2(ProjectDatabaseIDPair, pdbidp);

			DEFINEMAP2(unsigned int, LanguageRes, uint, langres, std::less<unsigned int>);
			MKNAME(meta_Map_uint_langres, "Map<unsignedint,LanguageRes,less<unsignedint>>");

			DEFINET2(landb, LanguageDB);
			EXT(landb, landb);
			SERIALIZER(landb,LanguageDB);
			FIRSTMEM(landb, "Baseclass_UID::Owner", miUniqueID, LanguageDB, uidowner, 0x10);
			NEXTMEM(landb, "Baseclass_UID::Generator", miNextUniqueID, LanguageDB, uidgen, 0x10, miUniqueID);
			NEXTMEM2(landb, mLanguageResources, LanguageDB, Map_uint_langres, 0, miNextUniqueID);
			NEXTMEM2(landb, mRegistry, LanguageDB, locreg, 0, mLanguageResources);
			NEXTMEM2(landb, mFlags, LanguageDB, flags, 0, mRegistry);
			NEXTMEM2(landb, mProjectID, LanguageDB, long, 0, mFlags);
			NEXTMEM2(landb, mExpandedIDRanges, LanguageDB, DCArray_pdbidp, 0, mProjectID);
			ADD(landb);

			DEFINEHANDLE(sprite, ParticleSprite);
			DEFINEHANDLE(tex, T3Texture);
			DEFINEHANDLE(font, Font);

			DEFINET2(ovsp, T3OverlaySpriteParams);
			FIRSTMEM2(ovsp, mhSprite, T3OverlaySpriteParams, Handlesprite, 0);
			NEXTMEM2(ovsp, mInitialPosition, T3OverlaySpriteParams, vec2, 0, mhSprite);
			NEXTMEM2(ovsp, mSize, T3OverlaySpriteParams, vec2, 0, mInitialPosition);
			NEXTMEM2(ovsp, mAnimation, T3OverlaySpriteParams, symbol, 0, mSize);
			NEXTMEM2(ovsp, mAnimationSpeed, T3OverlaySpriteParams, float, 0, mAnimation);
			NEXTMEM2(ovsp, mFlags, T3OverlaySpriteParams, flags, MetaFlag::MetaFlag_FlagType, mAnimationSpeed);
			static MetaFlagDescription ovspf;
			ovspf.mpFlagName = "eOverlaySprite_AnimationLoop";
			ovspf.mFlagValue = 1;
			ovspf.mpNext = NULL;
			meta_ovsp_mFlags.mpFlagDescriptions = &ovspf;
			ADD(ovsp);

			DEFINET2(oodtp, T3OverlayTextParams);
			FIRSTMEM2(oodtp, mhFont, T3OverlayTextParams, Handlefont, 0);
			NEXTMEM2(oodtp, mhDlg, T3OverlayTextParams, Handledlg, 0, mhFont);
			NEXTMEM2(oodtp, mDlgNodeName, T3OverlayTextParams, symbol, 0, mhDlg);
			NEXTMEM2(oodtp, mText, T3OverlayTextParams, string, 0, mDlgNodeName);
			NEXTMEM2(oodtp, mInitialPosition, T3OverlayTextParams, vec2, 0, mText);
			ADD(oodtp);

			DEFINET2(oods, T3OverlayObjectData_Sprite);
			FIRSTMEM2(oods, mName, T3OverlayObjectData_Sprite, symbol, 0);
			NEXTMEM2(oods, mParams, T3OverlayObjectData_Sprite, ovsp, 0, mName);
			ADD(oods);

			DEFINET2(oodt, T3OverlayObjectData_Text);
			FIRSTMEM2(oodt, mName, T3OverlayObjectData_Text, symbol, 0);
			NEXTMEM2(oodt, mParams, T3OverlayObjectData_Text, oodtp, 0, mName);
			ADD(oodt);

			DEFINET2(op, T3OverlayParams);
			FIRSTMEM2(op, mhBackgroundTexture, T3OverlayParams, Handletex, 0);
			NEXTMEM2(op, mhChore, T3OverlayParams, Handlehchore, 0, mhBackgroundTexture);
			NEXTMEM2(op, mMinDisplayTime, T3OverlayParams, float, 0, mhChore);
			NEXTMEM2(op, mFadeTime, T3OverlayParams, float, 0, mMinDisplayTime);
			ADD(op);

			DEFINEDCARRAY2(T3OverlayObjectData_Text, overlaytext);
			DEFINEDCARRAY2(T3OverlayObjectData_Sprite, overlaysprite);

			DEFINET2(overlay, T3OverlayData);
			EXT(overlay, overlay);
			FIRSTMEM2(overlay, mName, T3OverlayData, string, 0x20);
			NEXTMEM2(overlay, mSpriteObjects, T3OverlayData, DCArray_overlaysprite, 0, mName);
			NEXTMEM2(overlay, mTextObjects, T3OverlayData, DCArray_overlaytext, 0, mSpriteObjects);
			NEXTMEM2(overlay, mParams, T3OverlayData, op, 0, mTextObjects);
			ADD(overlay);

			DEFINET2(blendmode, BlendMode);
			FIRSTMEM2(blendmode, mVal, BlendMode, long, MetaFlag::MetaFlag_EnumIntType);
			FIRSTENUM2(blendmode, mVal, "Normal", normal, T3BlendMode::eBlendModeNormal, 0);
			NEXTENUM2(blendmode, mVal, "Default", default, T3BlendMode::eBlendModeDefault, 0, normal);
			NEXTENUM2(blendmode, mVal, "Alpha", alpha, 1, 0, default);
			NEXTENUM2(blendmode, mVal, "Alpha Alpha Test", aat, 2, 0, alpha);
			NEXTENUM2(blendmode, mVal, "Alpha Test", at, 3, 0, aat);
			NEXTENUM2(blendmode, mVal, "Inverse Alpha Test", iat, 4, 0, at);
			NEXTENUM2(blendmode, mVal, "Add", add, 5, 0, iat);
			NEXTENUM2(blendmode, mVal, "Multiply", mul, 6, 0, add);
			NEXTENUM2(blendmode, mVal, "InverseMultiply", imul, 7, 0, mul);
			NEXTENUM2(blendmode, mVal, "Alpha Add", aadd, 8, 0, imul);
			NEXTENUM2(blendmode, mVal, "Alpha Subtract", asub, 9, 0, aadd);
			NEXTENUM2(blendmode, mVal, "Alpha Inverse Alpha Test", aiat, 10, 0, asub);
			NEXTENUM2(blendmode, mVal, "Add Alpha Test", aat1,11, 0, aiat);
			NEXTENUM2(blendmode, mVal, "Add Inverse Alpha Test", aaiat, 12, 0, aat1);
			NEXTENUM2(blendmode, mVal, "Multiply Alpha Test", mat, 13, 0, aaiat);
			NEXTENUM2(blendmode, mVal, "Multiply Inverse Alpha Test", miat, 14, 0,mat);
			NEXTMEM1(blendmode, "Baseclass_EnumBase", BASE_CLASS, mVal, BlendMode, enumbase, MetaFlag::MetaFlag_BaseClass, mVal);
			ADD(blendmode);

			DEFINESARRAY_( Handle<T3Texture>, handletex, 1);

			DEFINET2(spriteanm, ParticleSprite::Animation);
			FIRSTMEM2(spriteanm, mName, ParticleSprite::Animation, symbol, 0);
			NEXTMEM2(spriteanm, mStartFrame, ParticleSprite::Animation, long, 0, mName);
			NEXTMEM2(spriteanm, mFrameCount, ParticleSprite::Animation, long, 0, mStartFrame);
			ADD(spriteanm);

			DEFINEDCARRAY2(ParticleSprite::Animation, spriteanm);

			DEFINET2(sprite, ParticleSprite);
			EXT(sprite, sprite);
			SERIALIZER(sprite, ParticleSprite);
			FIRSTMEM2(sprite, mName, ParticleSprite, string, 0x20);
			NEXTMEM2(sprite, mhTexture, ParticleSprite, sarray_handletex_1, 0, mName);
			NEXTMEM2(sprite, mTextureX, ParticleSprite, long, 0, mhTexture);
			NEXTMEM2(sprite, mTextureY, ParticleSprite, long, 0, mTextureX);
			NEXTMEM2(sprite, mSpriteSize, ParticleSprite, vec2, 0, mTextureY);
			NEXTMEM2(sprite, mBlendMode, ParticleSprite, blendmode, 0, mSpriteSize);
			NEXTMEM2(sprite, mAnimations, ParticleSprite, DCArray_spriteanm, 0, mBlendMode);
			ADD(sprite);

			DEFINET2(rectf, TRect<float>);
			ADDFLAGS(rectf, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			FIRSTMEM2(rectf, left, TRect<float>, float, 0);
			NEXTMEM2(rectf, right, TRect<float>, float, 0,left);
			NEXTMEM2(rectf, top, TRect<float>, float, 0, right);
			NEXTMEM2(rectf, bottom, TRect<float>, float, 0, top);
			ADD(rectf);

			DEFINET2(glyph, Font::GlyphInfo);
			FIRSTMEM2(glyph, mTexturePage, Font::GlyphInfo, long, 0);
			NEXTMEM2(glyph, mChannel, Font::GlyphInfo, long, 0, mTexturePage);
			NEXTMEM2(glyph, mGlyph, Font::GlyphInfo, rectf, 0, mChannel);
			NEXTMEM2(glyph, mWidth, Font::GlyphInfo, float, 0, mGlyph);
			NEXTMEM2(glyph, mHeight, Font::GlyphInfo, float, 0, mWidth);
			NEXTMEM2(glyph, mXOffset, Font::GlyphInfo, float, 0, mHeight);
			NEXTMEM2(glyph, mYOffset, Font::GlyphInfo, float, 0, mXOffset);
			NEXTMEM2(glyph, mXAdvance, Font::GlyphInfo, float, 0, mYOffset);
			NEXTMEM4(glyph, mGradientSize, Font::GlyphInfo, float, 0, mXAdvance, -1, TelltaleToolLib_GetGameKeyIndex("MICHONNE"));
			ADD(glyph);

			DEFINEMAP2(unsigned int, Font::GlyphInfo, uint,
				glyphinf, std::less<unsigned int>);

			DEFINEDCARRAY(T3Texture);
			DEFINEDCARRAY2(unsigned int, uint);
			MKNAME(meta_DCArray_T3Texture, "DCArray<T3Texture>");
			MKNAME(meta_DCArray_uint, "DCArray<uint>");
			MKNAME(meta_Map_uint_glyphinf, "Map<uint,Font::GlyphInfo,less<uint>>");

			DEFINET2(font, Font);
			ADDFLAGS(font, MetaFlag::MetaFlag_RenderResource);
			EXT(font, font);
			SERIALIZER(font, Font);
			FIRSTMEM2(font, mName, Font, string, 0);
			NEXTMEM2(font, mbUnicode, Font, bool, 0, mName);
			NEXTMEM2(font, mHeight, Font, float, 0, mbUnicode);
			NEXTMEM2(font, mBase, Font, float, 0, mHeight);
			//WII SCALE, FONT SCALE
			NEXTMEM4(font, mWiiScale, Font, float, 0, mBase, -1, TelltaleToolLib_GetGameKeyIndex("BATMAN2"));
			NEXTMEM4(font, mFontScale, Font, float, 0, mWiiScale, -1, TelltaleToolLib_GetGameKeyIndex("BATMAN2"));
			//-------
			NEXTMEM2(font, mGlyphInfo, Font, Map_uint_glyphinf, 0, mFontScale);
			NEXTMEM2(font, mTexturePages, Font, DCArray_T3Texture, 0, mGlyphInfo);
			NEXTMEM2(font, mIsDistanceField, Font, bool, 0, mTexturePages);
			//NEWER GAMES
			NEXTMEM4(font, mIsRuntime, Font, bool, 0, mIsDistanceField, TelltaleToolLib_GetGameKeyIndex("WD4"),-1);
			NEXTMEM4(font, mIsFiltered, Font, bool, 0, mIsRuntime, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(font, mTtfData, Font, bb, 0, mIsFiltered, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(font, mBasePointSize, Font, float, 0, mTtfData, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(font, mPreferredPointSizes, Font, DCArray_uint, 0, mBasePointSize, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			//------
			ADD(font);

			DEFINET2(dlgid, DlgObjID);
			FIRSTMEM2(dlgid, mID, DlgObjID, symbol, 0);
			ADD(dlgid);

			DEFINET2(startnodeoff, PreloadPackage::StartNodeOffset);
			FIRSTMEM2(startnodeoff, mStartNodeChain, PreloadPackage::StartNodeOffset, dlgid, 0);
			NEXTMEM2(startnodeoff, fStartTimeSeconds, PreloadPackage::StartNodeOffset, float, 0,mStartNodeChain);
			NEXTMEM2(startnodeoff, mfMinDurationToPreload, PreloadPackage::StartNodeOffset, float, 0, fStartTimeSeconds);
			ADD(startnodeoff);

			DEFINET2(idandnodeoff, PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset);
			FIRSTMEM2(idandnodeoff, mID, PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset, dlgid, 0);
			NEXTMEM2(idandnodeoff, mOffset, PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset, startnodeoff, 0, mID);
			ADD(idandnodeoff);

			DEFINESET_(Symbol SEP Symbol::CompareCRC, symbol);
			MKNAME(meta_Set_symbol, "Set<Symbol,Less<Symbol>>");

			DEFINET2(seen, PreloadPackage::ResourceSeenTimes);
			FIRSTMEM2(seen, mfEarliest, PreloadPackage::ResourceSeenTimes, float, 0);
			NEXTMEM2(seen, mfLatest, PreloadPackage::ResourceSeenTimes, float, 0, mfEarliest);
			NEXTMEM2(seen, mAdditionalScenes, PreloadPackage::ResourceSeenTimes, Set_symbol,0, mfLatest);
			ADD(seen);

			DEFINET2(bitsetbase3, BitSetBase<3>);
			SERIALIZER(bitsetbase3, BitSetBase<3>);
			ADD(bitsetbase3);

			DEFINET2(bitsetbase2, BitSetBase<2>);
			SERIALIZER(bitsetbase2, BitSetBase<2>);
			ADD(bitsetbase2);

			DEFINET2(bitsetbase1, BitSetBase<1>);
			SERIALIZER(bitsetbase1, BitSetBase<1>);
			ADD(bitsetbase1);

			DEFINET2(reskey, PreloadPackage::ResourceKey);
			FIRSTMEM2(reskey, mResourceName, PreloadPackage::ResourceKey, symbol, 0);
			NEXTMEM2(reskey, mMetaClassDescriptionCrc, PreloadPackage::ResourceKey, __int64, 0, mResourceName);
			//NEWER GAMES
			NEXTMEM4(reskey, mRenderQualities, PreloadPackage::ResourceKey, bitsetbase1, 0x20, mMetaClassDescriptionCrc
				,TelltaleToolLib_GetGameKeyIndex("BATMAN"),-1);
			NEXTMEM4(reskey, mVisible, PreloadPackage::ResourceKey, bool, 
				0, mRenderQualities, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			NEXTMEM4(reskey, mPrefix, PreloadPackage::ResourceKey, string,
				0, mVisible, TelltaleToolLib_GetGameKeyIndex("WD4"), -1);
			//--
			ADD(reskey);

			DEFINEDCARRAY2(PreloadPackage::ResourceKey, reskey);

			DEFINET2(ppkgs, PreloadPackage::RuntimeDataScene);
			EXT(ppkgs, preloadpackagerts);
			FIRSTMEM2(ppkgs, mResources, PreloadPackage::RuntimeDataScene, DCArray_reskey, 0);
			ADD(ppkgs);

			DEFINET2(dlgresinf, PreloadPackage::RuntimeDataDialog::DialogResourceInfo);
			FIRSTMEM2(dlgresinf, mResourceKey, PreloadPackage::RuntimeDataDialog::DialogResourceInfo, reskey, 0);
			NEXTMEM2(dlgresinf, mResourceSeenTimes, PreloadPackage::RuntimeDataDialog::DialogResourceInfo, seen, 0, mResourceKey);
			ADD(dlgresinf);

			DEFINEDCARRAY2(PreloadPackage::RuntimeDataDialog::DialogResourceInfo, resinfodlg);

			DEFINET2(idandvec, PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector);
			FIRSTMEM2(idandvec, mID, PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector, dlgid, 0);
			NEXTMEM2(idandvec, mVector, PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector, DCArray_resinfodlg, 0, mID);
			ADD(idandvec);

			DEFINEDCARRAY2(PreloadPackage::RuntimeDataDialog::DlgObjIdAndResourceVector, resvec);
			DEFINEDCARRAY2(PreloadPackage::RuntimeDataDialog::DlgObjIdAndStartNodeOffset, startnodeoffandid);

			DEFINET2(ppkgd, PreloadPackage::RuntimeDataDialog);
			EXT(ppkgd, preloadpackagertd);
			FIRSTMEM2(ppkgd, mDialogResourceVectors, PreloadPackage::RuntimeDataDialog, DCArray_resvec, 0);
			NEXTMEM2(ppkgd, mStartNodeOffsets, PreloadPackage::RuntimeDataDialog, DCArray_startnodeoffandid, 0, mDialogResourceVectors);
			ADD(ppkgd);

			DEFINET2(sounddata, SoundData);
			EXT(sounddata, wav);
			ADD(sounddata);

			DEFINET2(pathbase, PathBase);
			ADDFLAGS(pathbase, MetaFlag::MetaFlag_Memberless);
			ADD(pathbase);

			DEFINET2(ldb, LocomotionDB::AnimationInfo);
			FIRSTMEM2(ldb, mu64TimeStamp, LocomotionDB::AnimationInfo, u64, 0);
			NEXTMEM2(ldb, mzName, LocomotionDB::AnimationInfo, string, 0, mu64TimeStamp);
			NEXTMEM2(ldb, meCategory, LocomotionDB::AnimationInfo, long, MetaFlag::MetaFlag_EnumIntType, mzName);
			FIRSTENUM2(ldb, meCategory, "Category_Idle", a, 0, 0);
			NEXTENUM2(ldb, meCategory, "Category_Start", b, 1, 0, a);
			NEXTENUM2(ldb, meCategory, "Category_Move", c, 2, 0, b);
			NEXTENUM2(ldb, meCategory, "Category_Stop", d, 3, 0, c);
			NEXTENUM2(ldb, meCategory, "Category_Turn", e, 4, 0, d);
			NEXTENUM2(ldb, meCategory, "Category_TurnAndStart", f, 5, 0, e);
			NEXTENUM2(ldb, meCategory, "Category_MoveAndTurn", g, 6, 0, f);
			NEXTENUM2(ldb, meCategory, "Category_StopAndTurn", h, 7, 0, g);
			NEXTENUM2(ldb, meCategory, "Category_Unknown", i, 8, 0, h);
			NEXTENUM2(ldb, meCategory, "Category_Number", j, 9, 0, i);
			NEXTENUM2(ldb, meCategory, "Category_None", k , -1, 0, j);
			NEXTMEM2(ldb, mfDuration, LocomotionDB::AnimationInfo, float, 0, meCategory);
			NEXTMEM2(ldb, mfStartSpeed, LocomotionDB::AnimationInfo, float, 0, mfDuration);
			NEXTMEM2(ldb, mfEndSpeed, LocomotionDB::AnimationInfo, float, 0, mfStartSpeed);
			NEXTMEM2(ldb, mfTurnAngle, LocomotionDB::AnimationInfo, float, 0, mfEndSpeed);
			NEXTMEM2(ldb, mfMoveDistance, LocomotionDB::AnimationInfo, float, 0, mfTurnAngle);
			NEXTMEM2(ldb, mbMoveStart, LocomotionDB::AnimationInfo, bool, 0, mfMoveDistance);
			NEXTMEM2(ldb, mbMoveStop, LocomotionDB::AnimationInfo, bool, 0, mbMoveStart);
			NEXTMEM2(ldb, mbMove, LocomotionDB::AnimationInfo, bool, 0, mbMoveStop);
			NEXTMEM2(ldb, mbTurnLeft, LocomotionDB::AnimationInfo, bool, 0, mbMove);
			NEXTMEM2(ldb, mbTurnRight, LocomotionDB::AnimationInfo, bool, 0, mbTurnLeft);
			NEXTMEM2(ldb, mbTurn, LocomotionDB::AnimationInfo, bool, 0, mbTurnRight);
			ADD(ldb);

			DEFINEMAP2(String, LocomotionDB::AnimationInfo, string, ldbanim, std::less<String>);

			DEFINET2(dbl, LocomotionDB);
			EXT(dbl, ldb);
			FIRSTMEM2(dbl, mAnimInfoList, LocomotionDB, Map_string_ldbanim, 0);
			ADD(dbl);

			DEFINEHANDLE(dbl, LocomotionDB);

			DEFINEMAP(Symbol, bool, Symbol::CompareCRC);
			MKNAME(meta_Map_Symbol_bool, "Map<Symbol,bool,less<Symbol>>");

			DEFINET2(litem, LogicGroup::LogicItem);
			FIRSTMEM(litem, "Baseclass_PropertySet", mPropVersion,LogicGroup::LogicItem, prop, 0);
			meta_litem_mPropVersion.mOffset = PARENT_OFFSET(PropertySet, LogicGroup::LogicItem);
			NEXTMEM2(litem, mName, LogicGroup::LogicItem, string, 0, mPropVersion);
			NEXTMEM2(litem, mKeyNegateList, LogicGroup::LogicItem, Map_Symbol_bool, 0, mName);
			NEXTMEM2(litem, mKeyComparisonList, LogicGroup::LogicItem, Map_Symbol_int, 0, mKeyNegateList);
			NEXTMEM2(litem, mKeyActionList , LogicGroup::LogicItem, Map_Symbol_int, 0, mKeyComparisonList);
			NEXTMEM2(litem, mReferenceKeyList, LogicGroup::LogicItem, DCArray_String, 0, mKeyActionList);
			meta_litem_mReferenceKeyList.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			ADD(litem);

			DEFINEDCARRAY(LogicGroup);
			DEFINEMAP2(String, LogicGroup::LogicItem, string, logicitem, std::less<String>);
			MKNAME(meta_Map_string_logicitem, "Map<String,LogicGroup::LogicItem,less<String>>");

			DEFINET2(lgroup, LogicGroup);
			FIRSTMEM2(lgroup, mOperator, LogicGroup, long, 0);
			NEXTMEM2(lgroup, mItems, LogicGroup, Map_string_logicitem, 0, mOperator);
			NEXTMEM2(lgroup, mLogicGroups, LogicGroup,DCArray_LogicGroup, 0, mItems);
			NEXTMEM2(lgroup, mGroupOperator, LogicGroup, long, 0, mLogicGroups);
			NEXTMEM2(lgroup, mType, LogicGroup, long, 0, mGroupOperator);
			NEXTMEM2(lgroup, mName, LogicGroup, string, 0, mType);
			ADD(lgroup);

			DEFINET2(rule, Rule);
			ADDFLAGS(rule, MetaFlag::MetaFlag_ScriptTransient | MetaFlag::MetaFlag_PlaceInAddPropMenu);
			SERIALIZER(rule, Rule);
			FIRSTMEM2(rule, mName, Rule, string, 0);
			NEXTMEM2(rule, mRuntimePropName, Rule, string, 0, mName);
			NEXTMEM2(rule, mFlags, Rule, flags, 0, mRuntimePropName);
			NEXTMEM2(rule, mConditions, Rule, lgroup, 0, mFlags);
			NEXTMEM2(rule, mActions, Rule, lgroup, 0, mConditions);
			NEXTMEM2(rule, mElse, Rule, lgroup, 0, mActions);
			NEXTMEM2(rule, mAgentCategory, Rule, string, 0, mElse);
			ADD(rule);

			DEFINEMAP2(String, Rule*, string, ruleptr, std::less<String>);

			DEFINET2(rules, Rules);
			SERIALIZER(rules, Rules);
			EXT(rules, rules);
			FIRSTMEM2(rules, mFlags, Rules, flags, 0);
			NEXTMEM2(rules, mhLogicProps, Rules, Handlepropset, 0, mFlags);
			NEXTMEM2(rules, mRuleMap, Rules, Map_string_ruleptr, MetaFlag::MetaFlag_MetaSerializeDisable, mhLogicProps);
			ADD(rules);

			DEFINET2(eloge, EventLoggerEvent);
			FIRSTMEM2(eloge, mEventID, EventLoggerEvent, long, 0);
			NEXTMEM2(eloge, mMaxSeverity, EventLoggerEvent, long, 0, mEventID);
			ADD(eloge);

			DEFINET2(epage, EventStoragePage);
			SERIALIZER(epage, EventStoragePage);
			EXT(epage, epage);
			FIRSTMEM2(epage, mVersion, EventStoragePage, long, 0);
			NEXTMEM2(epage, mSessionID,EventStoragePage, __int64, 0, mVersion);
			NEXTMEM4(epage, mFlushedNameOnDisk, EventStoragePage, string, 0, mSessionID, TelltaleToolLib_GetGameKeyIndex("BORDERLANDS"), -1);
			ADD(epage);

			DEFINEDCARRAY2(EventStorage::PageEntry, estoreentry);
			DEFINEHANDLE(estorepage, EventStoragePage);
			

			DEFINET2(estoree, EventStorage::PageEntry);
			ADDFLAGS(estoree, MetaFlag::MetaFlag_Handle | MetaFlag::MetaFlag_PlaceInAddPropMenu);
			FIRSTMEM2(estoree, mhPage, EventStorage::PageEntry, Handleestorepage, 0);
			NEXTMEM2(estoree, mMaxEventID, EventStorage::PageEntry, long, 0, mhPage);
			ADD(estoree);

			DEFINET2(estore, EventStorage);
			EXT(estore, estore);
			SERIALIZER(estore, EventStorage);
			FIRSTMEM2(estore, mVersion, EventStorage, long, 0);
			NEXTMEM2(estore, mSessionID, EventStorage, __int64, 0, mVersion);
			NEXTMEM2(estore, mPages, EventStorage, DCArray_estoreentry, 0, mSessionID);
			NEXTMEM2(estore, mName, EventStorage, string, 0, mPages);
			NEXTMEM2(estore, mLastEventID, EventStorage, long, 0, mName);
			NEXTMEM2(estore, mEventStoragePageSize, EventStorage, long, 0, mLastEventID);
			ADD(estore);

			DEFINET2(pentry, PhonemeTable::PhonemeEntry);
			FIRSTMEM2(pentry, mAnimation, PhonemeTable::PhonemeEntry, animorchore, 0);
			NEXTMEM2(pentry, mContributionScalar, PhonemeTable::PhonemeEntry, float, 0, mAnimation);
			NEXTMEM2(pentry, mTimeScalar, PhonemeTable::PhonemeEntry, float, 0, mContributionScalar);
			ADD(pentry);

			DEFINEMAP2(Symbol, PhonemeTable::PhonemeEntry, symbol, pentry, Symbol::CompareCRC);
			MKNAME(meta_Map_symbol_pentry, "Map<Symbol,PhonemeTable::PhonemeEntry,Less<Symbol>>");

			DEFINET2(ptable, PhonemeTable);
			EXT(ptable, ptable);
			SERIALIZER(ptable, PhonemeTable);
			FIRSTMEM2(ptable, mName, PhonemeTable, string, 0);
			NEXTMEM2(ptable, mContributionScaler, PhonemeTable, float, 0, mName);
			NEXTMEM2(ptable, mAnimations, PhonemeTable, Map_symbol_pentry, 0, mContributionScaler);
			ADD(ptable);

			DEFINET2(cstate, Procedural_LookAt::EnumLookAtComputeStage);
			FIRSTMEM(cstate, "Baseclass_EnumBase", mVal, 
				Procedural_LookAt::EnumLookAtComputeStage, enumbase, 0);
			NEXTMEM1(cstate, "mVal", ALAIS,mVal,Procedural_LookAt::EnumLookAtComputeStage,
				long, 0, mVal);
			FIRSTENUM2(cstate, ALAIS, "Idle Look At", idlelook, 0,0);
			NEXTENUM2(cstate, ALAIS, "Dialog Chore Look At", dlglook, 1, 0, idlelook);
			NEXTENUM2(cstate, ALAIS, "Final Look At", flook, 2, 0, dlglook);
			ADD(cstate);

			DEFINET2(lookat, Procedural_LookAt);
			ADDFLAGS(lookat, MetaFlag::MetaFlag_Memberless);
			EXT(lookat, look);
			FIRSTMEM2(lookat, mHostNode, Procedural_LookAt, string, 1);
			NEXTMEM2(lookat, mTargetAgent, Procedural_LookAt, string, 1, mHostNode);
			NEXTMEM2(lookat, mTargetNode, Procedural_LookAt, string, 1, mTargetAgent);
			NEXTMEM2(lookat, mTargetOffset, Procedural_LookAt, vec3, 1, mTargetNode);
			NEXTMEM2(lookat, mbUsePrivateNode, Procedural_LookAt, bool, 1, mTargetOffset);
			NEXTMEM2(lookat, mhXAxisChore, Procedural_LookAt, animorchore, 1, mbUsePrivateNode);
			NEXTMEM2(lookat, mhYAxisChore, Procedural_LookAt, animorchore, 1, mhXAxisChore);
			NEXTMEM2(lookat, mbRotateHostNode, Procedural_LookAt, bool, 1, mhYAxisChore);
			NEXTMEM2(lookat, mLastLRAngle, Procedural_LookAt, float, 1, mbRotateHostNode);
			NEXTMEM2(lookat, mLastUDAngle, Procedural_LookAt, float, 1, mLastLRAngle);
			NEXTMEM2(lookat, mLastLRWeight, Procedural_LookAt, float, 1, mLastUDAngle);
			NEXTMEM2(lookat,mLastUDWeight, Procedural_LookAt, float, 1, mLastLRWeight);
			NEXTMEM2(lookat, mLookAtComputeStage, Procedural_LookAt, cstate, 1,mLastUDWeight);
			ADD(lookat);

			DEFINET2(pbase, PathBase);
			ADDFLAGS(pbase, MetaFlag::MetaFlag_Memberless);
			ADD(pbase);

			DEFINET2(pseg, PathSegment);
			FIRSTMEM(pseg, "Baseclass_PathBase", mStart, PathSegment, pbase, 0x10);
			NEXTMEM1(pseg, "mStart", ALAIS, mStart, PathSegment, vec3, 0, mStart);
			NEXTMEM2(pseg, mEnd, PathSegment, vec3, 0, ALAIS);
			NEXTMEM2(pseg, mStartNodeId, PathSegment, long, 0, mEnd);
			NEXTMEM2(pseg, mEndNodeId, PathSegment, long, 0, mStartNodeId);
			SERIALIZER(pseg, PathSegment);
			ADD(pseg);

			DEFINET2(psegh, HermiteCurvePathSegment);
			FIRSTMEM(psegh, "Baseclass_PathBase", mStart, HermiteCurvePathSegment, pbase, 0x10);
			NEXTMEM1(psegh, "mStart", ALAIS, mStart, HermiteCurvePathSegment, vec3, 0, mStart);
			NEXTMEM2(psegh, mEnd, HermiteCurvePathSegment, vec3, 0, ALAIS);
			NEXTMEM2(psegh, mStartDir, HermiteCurvePathSegment, vec3, 0, mEnd);
			NEXTMEM2(psegh, mEndDir, HermiteCurvePathSegment, vec3, 0, mStartDir);
			NEXTMEM2(psegh, mStartNodeId, HermiteCurvePathSegment, long, 0, mEndDir);
			NEXTMEM2(psegh, mEndNodeId, HermiteCurvePathSegment, long, 0, mStartNodeId);
			SERIALIZER(psegh, HermiteCurvePathSegment);
			ADD(psegh);

			DEFINET2(cstate1, AnimationDrivenPathSegment::EnumAnimatedPathSegmentType);
			FIRSTMEM(cstate1, "Baseclass_EnumBase", mVal,
				AnimationDrivenPathSegment::EnumAnimatedPathSegmentType, enumbase, 0);
			NEXTMEM1(cstate1, "mVal", ALAIS, mVal, AnimationDrivenPathSegment::EnumAnimatedPathSegmentType,
				long, 0, mVal);
			FIRSTENUM2(cstate1, ALAIS, "eStartType", idlelook, 0, 0);
			NEXTENUM2(cstate1, ALAIS, "eLoopType", dlglook, 1, 0, idlelook);
			NEXTENUM2(cstate1, ALAIS, "eStopType", flook, 2, 0, dlglook);
			ADD(cstate1);

			DEFINET2(psegha, AnimationDrivenPathSegment);
			FIRSTMEM(psegha, "Baseclass_PathBase", mStart, AnimationDrivenPathSegment, pbase, 0x10);
			NEXTMEM1(psegha, "mStart", ALAIS, mStart, AnimationDrivenPathSegment, vec3, 0, mStart);
			NEXTMEM2(psegha, mEnd, AnimationDrivenPathSegment, vec3, 0, ALAIS);
			NEXTMEM2(psegha, mStartDirection, AnimationDrivenPathSegment, vec3, 0, mEnd);
			NEXTMEM2(psegha, mEndDirection, AnimationDrivenPathSegment, vec3, 0, mStartDirection);
			NEXTMEM2(psegha, mAnimType, AnimationDrivenPathSegment, cstate1, 0, mEndDirection);
			SERIALIZER(psegha, AnimationDrivenPathSegment);
			ADD(psegha);

			DEFINET2(wpath, WalkPath);
			FIRSTMEM2(wpath, mName, WalkPath, string, 0);
			SERIALIZER(wpath, WalkPath);
			ADD(wpath);

			DEFINET2(dloader, DependencyLoader<1>);
			SERIALIZER(dloader, DependencyLoader<1>);
			ADDFLAGS(dloader, MetaFlag::MetaFlag_Memberless);
			ADD(dloader);

			DEFINET2(aas, AutoActStatus);
			FIRSTMEM2(aas, m_Status, AutoActStatus, 
				long, MetaFlag::MetaFlag_EnumIntType);
			ADD(aas);

			DEFINET2(aab, ActorAgentBinding);
			FIRSTMEM2(aab, mActorName, ActorAgentBinding, string, 0);
			ADD(aab);

			DEFINEMAP(Symbol, WalkPath, Symbol::CompareCRC);

			DEFINET2(chore, Chore);
			EXT(chore, chore);
			SERIALIZER(chore, Chore);
			FIRSTMEM2(chore, mName, Chore, string, 0);
			NEXTMEM2(chore, mFlags, Chore, flags, 0, mName);
			NEXTMEM2(chore, mLength, Chore, float, 0, mFlags);
			NEXTMEM2(chore, mNumResources, Chore, long, 0, mLength);
			NEXTMEM2(chore, mNumAgents, Chore, long, 0, mNumResources);
			NEXTMEM2(chore, mEditorProps, Chore, prop, 
				MetaFlag::MetaFlag_SkipObjectState, mNumAgents);
			NEXTMEM2(chore, mChoreSceneFile, Chore, string, 0, mEditorProps);
			NEXTMEM2(chore, mRenderDelay, Chore, long, MetaFlag::MetaFlag_SkipObjectState,
				mChoreSceneFile);
			NEXTMEM2(chore, mSynchronizedToLocalization, Chore, locali, 0, mRenderDelay);
			NEXTMEM2(chore, mDependencies, Chore, dloader,
				MetaFlag::MetaFlag_SkipObjectState, mSynchronizedToLocalization);
			NEXTMEM2(chore, mToolProps, Chore, tp, MetaFlag::MetaFlag_SkipObjectState
				| MetaFlag::MetaFlag_EditorHide, mDependencies);
			NEXTMEM2(chore, mWalkPaths, Chore, Map_Symbol_WalkPath, 0, mToolProps);
			ADD(chore);

			DEFINET2(cattach, ChoreAgent::Attachment);
			FIRSTMEM2(cattach, mbDoAttach, ChoreAgent::Attachment, bool, 0);
			NEXTMEM2(cattach, mAttachTo, ChoreAgent::Attachment, string, MetaFlag::MetaFlag_SelectAgentType,mbDoAttach);
			NEXTMEM2(cattach, mAttachToNode, ChoreAgent::Attachment, string, 0, mAttachTo);
			NEXTMEM2(cattach, mAttachPos, ChoreAgent::Attachment, vec3, 0, mAttachToNode);
			NEXTMEM2(cattach, mAttachQuat, ChoreAgent::Attachment, quat, 0, mAttachPos);
			NEXTMEM2(cattach, mbAttachPreserveWorldPos, ChoreAgent::Attachment, bool, 0, mAttachQuat);
			NEXTMEM2(cattach, mbLeaveAttachedWhenComplete, ChoreAgent::Attachment, bool, 0, mbAttachPreserveWorldPos);
			ADD(cattach);

			DEFINET2(cagent, ChoreAgent);
			SERIALIZER(cagent, ChoreAgent);
			FIRSTMEM2(cagent, mpChore, ChoreAgent, ptrbase, 0);
			NEXTMEM2(cagent, mAgentName, ChoreAgent, string, 0, mpChore);
			NEXTMEM2(cagent, mFlags, ChoreAgent, flags, 0, mAgentName);
			NEXTMEM2(cagent, mResources, ChoreAgent, DCArray_i32, 0, mFlags);
			NEXTMEM2(cagent, mAttachment, ChoreAgent, cattach, 0, mResources);
			NEXTMEM2(cagent, mAABinding, ChoreAgent, aab, 0, mAttachment);
			NEXTMEM2(cagent, mAgentEnabledRule, ChoreAgent, rule, 0, mAABinding);
			ADD(cagent);

			DEFINET2(cblock, ChoreResource::Block);
			FIRSTMEM2(cblock, mStartTime, ChoreResource::Block, float, 0);
			NEXTMEM2(cblock, mEndTime, ChoreResource::Block, float, 0, mStartTime);
			NEXTMEM2(cblock, mbLoopingBlock, ChoreResource::Block, bool, 0, mEndTime);
			NEXTMEM2(cblock, mScale, ChoreResource::Block, float, 0, mbLoopingBlock);
			NEXTMEM2(cblock, mbSelected, ChoreResource::Block, bool, 1, mScale);
			ADD(cblock);

			DEFINEDCARRAY2(ChoreResource::Block, choreblock);

			DEFINEDCARRAY2(AnimationValueInterfaceBase*, anminterfacebase);

			DEFINET2(anm, Animation);
			EXT(anm, anm);
			SERIALIZER(anm, Animation);
			FIRSTMEM2(anm, mVersion, Animation, long, 0);
			NEXTMEM2(anm, mFlags, Animation, long, 0, mVersion);
			NEXTMEM2(anm, mName, Animation, symbol, 0, mFlags);
			NEXTMEM2(anm, mLength, Animation, float, 0, mName);
			NEXTMEM2(anm, mAdditiveMask, Animation, float, 0, mLength);
			NEXTMEM2(anm, mValues, Animation, DCArray_anminterfacebase, MetaFlag::MetaFlag_MetaSerializeDisable, mAdditiveMask);
			NEXTMEM2(anm, mToolProps, Animation, tp, MetaFlag::MetaFlag_EditorHide | MetaFlag::MetaFlag_SkipObjectState, mValues);
			ADD(anm);

			DEFINET2(cres, ChoreResource);
			SERIALIZER(cres, ChoreResource);
			FIRSTMEM2(cres, mpChore, ChoreResource, ptrbase, 0);
			NEXTMEM2(cres, mVersion, ChoreResource, long, 0, mpChore);
			NEXTMEM2(cres, mResName, ChoreResource, symbol, 0, mVersion);
			NEXTMEM2(cres, mResLength, ChoreResource, float, 0, mResName);
			NEXTMEM2(cres, mPriority, ChoreResource, long, 0, mResLength);
			NEXTMEM2(cres, mFlags, ChoreResource, flags, 0, mPriority);
			NEXTMEM2(cres, mResourceGroup, ChoreResource, string, 0, mFlags);
			NEXTMEM2(cres, mhObject, ChoreResource, handlebase, 0, mResourceGroup);
			NEXTMEM2(cres, mControlAnimation, ChoreResource, anm, 0, mhObject);
			NEXTMEM2(cres, mBlocks, ChoreResource, DCArray_choreblock, 0, mControlAnimation);
			NEXTMEM2(cres, mbNoPose, ChoreResource, bool, 0, mBlocks);
			NEXTMEM2(cres, mbEmbedded, ChoreResource, bool, 0, mbNoPose);
			NEXTMEM2(cres, mbEnabled, ChoreResource, bool, 0, mbEmbedded);
			NEXTMEM2(cres, mbIsAgentResource, ChoreResource, bool, 0, mbEnabled);
			NEXTMEM2(cres, mbViewGraphs, ChoreResource, bool,0, mbIsAgentResource);
			NEXTMEM2(cres, mbViewEmptyGraphs, ChoreResource, bool, 0, mbViewGraphs);
			NEXTMEM2(cres, mbViewProperties, ChoreResource, bool, 0, mbViewEmptyGraphs);
			NEXTMEM2(cres, mbViewResourceGroups, ChoreResource, bool, 0, mbViewProperties);
			NEXTMEM2(cres, mResourceProperties, ChoreResource, prop, 0, mbViewResourceGroups);
			NEXTMEM2(cres, mResourceGroupInclude, ChoreResource, Map_Symbol_float, 0, mResourceProperties);
			NEXTMEM2(cres, mAAStatus, ChoreResource, aas, 0, mResourceGroupInclude);
			NEXTMEM1(cres, "mAAStatus", ALIAS, mAAStatus,
				ChoreResource, long, 0, mAAStatus);
			meta_cres_ALIAS.mGameIndexVersionRange.max = 
				TelltaleToolLib_GetGameKeyIndex("MCSM");
			meta_cres_mAAStatus.mGameIndexVersionRange.min = 
				TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			ADD(cres);

			DEFINET2(rsin, ResourceGroupInfo);
			ADDFLAGS(rsin, MetaFlag::MetaFlag_PlaceInAddPropMenu);
			FIRSTMEM2(rsin, mColor, ResourceGroupInfo, color, 0);
			NEXTMEM2(rsin, mPriority, ResourceGroupInfo, long, 0, mColor);
			ADD(rsin);

			DEFINET2(pkey, PhonemeKey);
			FIRSTMEM2(pkey, mPhoneme, PhonemeKey, symbol, 0);
			NEXTMEM2(pkey, mFadeInTime, PhonemeKey, float, 0, mPhoneme);
			NEXTMEM2(pkey, mHoldTime, PhonemeKey, float, 0, mFadeInTime);
			NEXTMEM2(pkey, mFadeOutTime, PhonemeKey, float, 0, mHoldTime);
			NEXTMEM2(pkey, mTargetContribution, PhonemeKey, float, 0, mFadeOutTime);
			SERIALIZER(pkey, PhonemeKey);
			ADD(pkey);

			

			DEFINEKEYFRAMEDVALUE(Vector3,Vector3,vec3);
			DEFINEKEYFRAMEDVALUE(Transform, Transform, transform);
			DEFINEKEYFRAMEDVALUE(bool, bool, bool);
			DEFINEKEYFRAMEDVALUE(String, String, string);
			DEFINEKEYFRAMEDVALUE(PhonemeKey, PhonemeKey, pkey);
			DEFINEKEYFRAMEDVALUE(HCHORE, Handle<Chore>, Handlehchore);

			if (meta_kfv_String_sample.mbNameIsHeapAllocated) {
				free((void*)meta_kfv_String_sample.mpTypeInfoName);
				meta_kfv_String_sample.mbNameIsHeapAllocated = false;
			}
			meta_kfv_String_sample.mpTypeInfoName = "KeyframedValue<String>::Sample";//typedef string
			meta_kfv_String_sample.mHash = CRC64_CaseInsensitive(0, 
				meta_kfv_String_sample.mpTypeInfoName);
			
			if (meta_anmi_String.mbNameIsHeapAllocated) {
				free((void*)meta_anmi_String.mpTypeInfoName);
				meta_anmi_String.mbNameIsHeapAllocated = false;
			}
			meta_anmi_String.mpTypeInfoName = "AnimatedValueInterface<String>";//typedef string
			meta_anmi_String.mHash = CRC64_CaseInsensitive(0, meta_anmi_String.mpTypeInfoName);


			if (meta_kfv_String.mbNameIsHeapAllocated) {
				free((void*)meta_kfv_String.mpTypeInfoName);
				meta_kfv_String.mbNameIsHeapAllocated = false;
			}
			meta_kfv_String.mpTypeInfoName = "KeyframedValue<String>";//typedef string
			meta_kfv_String.mHash = CRC64_CaseInsensitive(0, meta_kfv_String.mpTypeInfoName);
			DEFINET2(spose, SkeletonPose);
			ADD(spose);

			DEFINET2(sklk, CompressedSkeletonPoseKeys);
			FIRSTMEM(sklk, "Baseclass_AnimationValueInterfaceBase", mName, 
				CompressedSkeletonPoseKeys, anminterface, MetaFlag::MetaFlag_BaseClass);
			meta_sklk_mName.mOffset = (i64)((AnimationValueInterfaceBase*)((CompressedSkeletonPoseKeys*)NULL));
			NEXTMEM2(sklk, mDataSize, CompressedSkeletonPoseKeys, long, 0, mName);
			ADD(sklk);

			DEFINET2(sklk2, CompressedSkeletonPoseKeys2);
			FIRSTMEM(sklk2, "Baseclass_AnimationValueInterfaceBase", mName,
				CompressedSkeletonPoseKeys2, anminterface, MetaFlag::MetaFlag_BaseClass);
			meta_sklk2_mName.mOffset = (i64)((AnimationValueInterfaceBase*)((CompressedSkeletonPoseKeys2*)NULL));
			NEXTMEM2(sklk2, mDataSize, CompressedSkeletonPoseKeys2, long, 0, mName);
			ADD(sklk2);

			DEFINET2(sbentry, SkeletonPoseValue::BoneEntry);
			FIRSTMEM2(sbentry, mName, SkeletonPoseValue::BoneEntry, symbol, 0);
			NEXTMEM2(sbentry, mFlags, SkeletonPoseValue::BoneEntry, long, 0, mName);
			ADD(sbentry);
			
			DEFINEDCARRAY(Transform);

			DEFINET2(sample, SkeletonPoseValue::Sample);
			FIRSTMEM2(sample, mTime, SkeletonPoseValue::Sample, float, 0);
			NEXTMEM2(sample, mRecipTimeToNextSample, SkeletonPoseValue::Sample,
				float, 0, mTime);
			NEXTMEM2(sample, mValues, SkeletonPoseValue::Sample, DCArray_Transform,
				0, mRecipTimeToNextSample);
			NEXTMEM2(sample, mTangents, SkeletonPoseValue::Sample, DCArray_i32, 0,
				mValues);
			ADD(sample);

			DEFINEDCARRAY2(SkeletonPoseValue::Sample, sample);
			DEFINEDCARRAY2(SkeletonPoseValue::BoneEntry, bentry);

			DEFINET2(pose, SkeletonPoseValue);
			FIRSTMEM(pose, "Baseclass_AnimationValueInterfaceBase", mName,
				SkeletonPoseValue , anminterface, MetaFlag::MetaFlag_BaseClass);
			meta_pose_mName.mOffset = (i64)((AnimationValueInterfaceBase*)
				((SkeletonPoseValue*)NULL));
			NEXTMEM2(pose, mBones, SkeletonPoseValue, DCArray_bentry, 0, mName);
			NEXTMEM2(pose, mSamples, SkeletonPoseValue, DCArray_sample, 0, mBones);
			ADD(pose);

			DEFINET2(dlgidowner, DlgObjIDOwner);
			FIRSTMEM2(dlgidowner, mDlgObjID, DlgObjIDOwner, dlgid, 0x20);
			ADD(dlgidowner);

			DEFINET2(dlgprops, DlgObjectProps);
			ADDFLAGS(dlgprops, MetaFlag::MetaFlag_NoPanelCaption);
			SERIALIZER(dlgprops, DlgObjectProps);
			FIRSTMEM2(dlgprops, mFlags, DlgObjectProps, flags, 0x20);
			ADD(dlgprops);

			DEFINET2(dlgpropo, DlgObjectPropsOwner);
			FIRSTMEM2(dlgpropo, mDlgObjectProps, DlgObjectPropsOwner, dlgprops, 8);
			ADD(dlgpropo);

			DEFINET2(dlglink, DlgNodeLink);
			FIRSTMEM(dlglink, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgNodeLink, dlgidowner, 0x10);
			meta_dlglink_mDlgObjID.mOffset = PARENT_OFFSET(DlgObjIDOwner, DlgNodeLink);
			NEXTMEM2(dlglink, mRequiredCCType, DlgNodeLink, long, 0, mDlgObjID);
			ADD(dlglink);

			DEFINET2(dlghead, DlgChainHead);
			FIRSTMEM(dlghead, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgChainHead, dlgidowner, 0x10);
			meta_dlghead_mDlgObjID.mOffset = PARENT_OFFSET(DlgObjIDOwner, DlgChainHead);
			NEXTMEM2(dlghead, mLink, DlgChainHead, dlglink, 0, mDlgObjID);
			ADD(dlghead);

			DEFINET2(dlgdown, DlgDownstreamVisibilityConditions);
			FIRSTMEM2(dlgdown, mNodeTypeFlags, DlgDownstreamVisibilityConditions, flags, 0);
			NEXTMEM2(dlgdown, mMaxNumNodeEvals, DlgDownstreamVisibilityConditions, long, 0, mNodeTypeFlags);
			ADD(dlgdown);

			DEFINET2(dlgvcond, DlgVisibilityConditions);
			FIRSTMEM2(dlgvcond, mbDiesOff, DlgVisibilityConditions, bool, 0);
			NEXTMEM2(dlgvcond, mFlags, DlgVisibilityConditions, flags, 0, mbDiesOff);
			NEXTMEM2(dlgvcond, mDownstreamVisCond, DlgVisibilityConditions,dlgdown, 0, mFlags);
			NEXTMEM2(dlgvcond, mScriptVisCond, DlgVisibilityConditions, string, 0, mDownstreamVisCond);
			SERIALIZER(dlgvcond, DlgVisibilityConditions);
			ADD(dlgvcond);

			DEFINET2(dlgvowner, DlgVisibilityConditionsOwner);
			ADDFLAGS(dlgvowner, 8);
			FIRSTMEM2(dlgvowner, mVisCond, DlgVisibilityConditionsOwner, dlgvcond, 0);
			ADD(dlgvowner);

			DEFINET2(dlgchild, DlgChild);
			ADDFLAGS(dlgchild, 8);
			FIRSTMEM(dlgchild, "Baseclass_DlgChainHead", mLink, DlgChainHead, dlghead, 0x10);
			NEXTMEM2(dlgchild, mName, DlgChild, symbol, 0, mLink);
			meta_dlgchild_mLink.mOffset = PARENT_OFFSET(DlgChainHead, DlgChild);
			NEXTMEM(dlgchild, "Baseclass_DlgVisibilityConditionsOwner", mVisCond, DlgChild, dlgvowner, 0x10, mName);
			NEXTMEM(dlgchild, "Baseclass_DlgObjectPropsOwner", 
				mDlgObjectProps, DlgChild, dlgpropo, 0x10, mVisCond);
			meta_dlgchild_mVisCond.mOffset = PARENT_OFFSET(DlgVisibilityConditionsOwner, DlgChild);
			meta_dlgchild_mDlgObjectProps.mOffset = PARENT_OFFSET(DlgObjectPropsOwner, DlgChild);
			NEXTMEM2(dlgchild, mParent, DlgChild, dlglink, 0x20, mDlgObjectProps);
			ADD(dlgchild);

			DEFINEDCARRAY2(DlgChild*, childptr);

			DEFINET2(dlgchildset, DlgChildSet);
			FIRSTMEM2(dlgchildset, mChildren, DlgChildSet, DCArray_childptr, 0x20);
			NEXTMEM2(dlgchildset, mParent, DlgChildSet, dlglink, 0x20, mChildren);
			SERIALIZER(dlgchildset, DlgChildSet);
			ADD(dlgchildset);

			DEFINET2(dfolder, DlgFolder);
			ADDFLAGS(dfolder, 8);
			FIRSTMEM(dfolder, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgFolder, dlgidowner, 0x30);
			meta_dfolder_mDlgObjID.mOffset = PARENT_OFFSET(DlgObjIDOwner, DlgFolder);
			NEXTMEM(dfolder, "Baseclass_DlgObjectPropsOwner", mDlgObjectProps, DlgFolder, dlgpropo, 0x10, mDlgObjID);
			meta_dfolder_mDlgObjectProps.mOffset = PARENT_OFFSET(DlgObjectPropsOwner, DlgFolder);
			NEXTMEM(dfolder, "Baseclass_DlgChildSet", mChildren, DlgFolder, dlgchildset, 0x10, mDlgObjectProps);
			meta_dfolder_mChildren.mOffset = PARENT_OFFSET(DlgChildSet, DlgFolder);
			NEXTMEM(dfolder, "Baseclass_UID::Owner", miUniqueID, DlgFolder, uidowner, 0x10, mChildren);
			meta_dfolder_miUniqueID.mOffset = PARENT_OFFSET(UID::Owner, DlgFolder);
			meta_dfolder_miUniqueID.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(dfolder, mName, DlgFolder, symbol, 0, miUniqueID);
			NEXTMEM2(dfolder, mProdReportProps, DlgFolder, prop, 0, mName);
			meta_dfolder_mProdReportProps.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BORDERLANDS");
			ADD(dfolder);

			DEFINET2(dnode, DlgNode);
			ADDFLAGS(dnode, 8);
			FIRSTMEM(dnode, "Baseclass_DlgObjIDOwner", mDlgObjID, DlgNode, dlgidowner, 0x30);
			meta_dnode_mDlgObjID.mOffset = PARENT_OFFSET(DlgObjIDOwner, DlgNode);
			NEXTMEM(dnode, "Baseclass_DlgVisibilityConditionsOwner", mVisCond, DlgNode, dlgvowner, 0x10, mDlgObjID);
			meta_dnode_mVisCond.mOffset = PARENT_OFFSET(DlgVisibilityConditionsOwner, DlgNode);
			NEXTMEM(dnode, "Baseclass_DlgObjectPropsOwner", mDlgObjectProps, DlgNode, dlgpropo, 0x10, mVisCond);
			meta_dnode_mDlgObjectProps.mOffset = PARENT_OFFSET(DlgObjectPropsOwner, DlgNode);
			NEXTMEM(dnode, "Baseclass_UID::Owner", miUniqueID, DlgNode, uidowner, 0x10, mDlgObjectProps);
			meta_dnode_miUniqueID.mOffset = PARENT_OFFSET(UID::Owner, DlgNode);
			meta_dnode_miUniqueID.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(dnode, mPrev, DlgNode, dlglink, 0x20, miUniqueID);
			NEXTMEM2(dnode, mNext, DlgNode, dlglink, 0x20, mPrev);
			NEXTMEM2(dnode, mName, DlgNode, symbol, 0x20, mNext);
			NEXTMEM2(dnode, mFlags, DlgNode, flags, 0x20, mName);
			NEXTMEM2(dnode, mChainContextTypeID, DlgNode, long, 0x20, mFlags);
			ADD(dnode);

			DEFINET2(date, DateStamp);
			FIRSTMEM2(date, mSec, DateStamp, __uint8, 0);
			NEXTMEM2(date, mMin, DateStamp, __uint8, 0, mSec);
			NEXTMEM2(date, mHour, DateStamp, __uint8, 0, mMin);
			NEXTMEM2(date, mMday, DateStamp, __uint8, 0, mHour);
			NEXTMEM2(date, mMon, DateStamp, __uint8, 0, mMday);
			NEXTMEM2(date, mYear, DateStamp, __uint8, 0, mMon);
			NEXTMEM2(date, mWday, DateStamp, __uint8, 0, mYear);
			NEXTMEM2(date, mYday, DateStamp, ushort, 0, mWday);
			NEXTMEM2(date, mIsdst, DateStamp, __uint8, 0, mYday);
			ADD(date);

			DEFINET2(notee, Note::Entry);
			FIRSTMEM(notee, "Baseclass_UID::Owner", miUniqueID, Note::Entry, uidowner, 0x10);
			meta_notee_miUniqueID.mOffset = PARENT_OFFSET(UID::Owner, Note::Entry);
			NEXTMEM(notee, "Baseclass_DlgObjIDOwner", mDlgObjID, Note::Entry, dlgidowner, 0x10, miUniqueID);
			meta_notee_mDlgObjID.mOffset = PARENT_OFFSET(DlgObjIDOwner, Note::Entry);
			NEXTMEM2(notee, mAuthor, Note::Entry, string, 0, mDlgObjID);
			NEXTMEM2(notee, mStamp, Note::Entry, date, 0, mAuthor);
			NEXTMEM2(notee, mCategory, Note::Entry, string, 0, mStamp);
			NEXTMEM2(notee, mText, Note::Entry, string, 0, mCategory);
			ADD(notee);

			DEFINEDCARRAY2(Note::Entry*, noteentryp);

			DEFINET2(note, Note);
			SERIALIZER(note, Note);
			EXT(note, note);
			FIRSTMEM2(note, mEntries, Note, DCArray_noteentryp, 1);
			NEXTMEM2(note, mName, Note, string, 0, mEntries);
			ADD(note);

			DEFINEMAP2(int, Note*, int, notep, std::less<int>);

			DEFINET2(notec, NoteCollection);
			FIRSTMEM(notec, "Baseclass_UID::Generator", miNextUniqueID, NoteCollection, uidgen, 0x10);
			meta_notec_miNextUniqueID.mOffset = PARENT_OFFSET(UID::Generator, NoteCollection);
			NEXTMEM2(notec, mNotes, NoteCollection, Map_int_notep, 1, miNextUniqueID);
			SERIALIZER(notec, NoteCollection);
			ADD(notec);

			DEFINET2(navcamm, NavCam::EnumMode);
			FIRSTMEM1(navcamm, "Baseclass_EnumBase", ALAIS, mVal,
				NavCam::EnumMode, enumbase, 0x10);
			NEXTMEM2(navcamm, mVal, NavCam::EnumMode, long, 0, ALAIS);
			FIRSTENUM2(navcamm, mVal, "eNone", r, 1, 0);
			NEXTENUM2(navcamm, mVal, "eLookAt", f, 2, 0, r);
			NEXTENUM2(navcamm, mVal, "eOrbit", a, 3, 0, f);
			NEXTENUM2(navcamm, mVal, "eAnimation_Track", b, 4, 0, a);
			NEXTENUM2(navcamm, mVal, "eAnimation_Time", c, 5, 0, b);
			NEXTENUM2(navcamm, mVal, "eAnimation_Pos_ProceduralLookAt", d, 6, 0, c);
			NEXTENUM2(navcamm, mVal, "eScenePosition", e, 7, 0, d);
			NEXTENUM2(navcamm, mVal, "eDynamicConversationCamera", g, 8, 0, e);
			ADDFLAGS(navcamm, 0x8008);
			ADD(navcamm);

			DEFINET2(testt, DlgNodeCriteria::EnumTestT);
			FIRSTMEM1(testt, "Baseclass_EnumBase", ALAIS, mVal,
				DlgNodeCriteria::EnumTestT, enumbase, 0x10);
			NEXTMEM2(testt, mVal, DlgNodeCriteria::EnumTestT, long, 0, ALAIS);
			FIRSTENUM2(testt, mVal, "eRequired", r, 1, 0);
			NEXTENUM2(testt, mVal, "eForbidden", f, 2, 0, r);
			ADDFLAGS(testt, 0x8008);
			ADD(testt);

			DEFINET2(thresht, DlgNodeCriteria::EnumThresholdT);
			FIRSTMEM1(thresht, "Baseclass_EnumBase", ALAIS, mVal,
				DlgNodeCriteria::EnumThresholdT, enumbase, 0x10);
			NEXTMEM2(thresht, mVal, DlgNodeCriteria::EnumThresholdT, long, 0, ALAIS);
			FIRSTENUM2(thresht, mVal, "eAny", r, 1, 0);
			NEXTENUM2(thresht, mVal, "eAll", f, 2, 0, r);
			ADDFLAGS(thresht, 0x8008);
			ADD(thresht);

			DEFINET2(deft, DlgNodeCriteria::EnumDefaultResultT);
			FIRSTMEM1(deft, "Baseclass_EnumBase", ALAIS, mVal,
				DlgNodeCriteria::EnumDefaultResultT, enumbase, 0x10);
			NEXTMEM2(deft, mVal, DlgNodeCriteria::EnumDefaultResultT, long, 0, ALAIS);
			FIRSTENUM2(deft, mVal, "eDefaultToPass", r, 1, 0);
			NEXTENUM2(deft, mVal, "eDefaultToNotPass", f, 2, 0, r);
			NEXTENUM2(deft, mVal, "eDefaultToNotPassUnlessTransparent", g, 3, 0, f);
			ADDFLAGS(deft, 0x8008);
			ADD(deft);

			DEFINET2(crit, DlgNodeCriteria);
			FIRSTMEM2(crit, mTestType, DlgNodeCriteria, testt, 0);
			NEXTMEM2(crit, mFlagsThreshold, DlgNodeCriteria, thresht, 0, mTestType);
			NEXTMEM2(crit, mCriteriaThreshold, DlgNodeCriteria, thresht, 0, mFlagsThreshold);
			NEXTMEM2(crit, mDefaultResult, DlgNodeCriteria, deft, 0, mCriteriaThreshold);
			NEXTMEM2(crit, mClassFlags, DlgNodeCriteria, flags, 0, mDefaultResult);
			NEXTMEM2(crit, mClassIDs, DlgNodeCriteria, Set_i32, 0, mClassFlags);
			ADD(crit);

			DEFINET2(res, LanguageResProxy);
			FIRSTMEM2(res, mID, LanguageResProxy, long, 0);
			ADD(res);

			DEFINET2(jrecord, JiraRecord);
			ADD(jrecord);

			DEFINEMAP2(String, JiraRecord*, String, jrecord, std::less<String>);

			DEFINET2(jira, JiraRecordManager);
			SERIALIZER(jira, JiraRecordManager);
			FIRSTMEM2(jira, mRecords, JiraRecordManager, Map_String_jrecord, MetaFlag_EditorHide);
			ADD(jira);

			DEFINET2(line, DlgLine);
			FIRSTMEM1(line, "Baseclass_UID::Owner", ALAISOWNER, miUniqueID,
				DlgLine, uidowner, 0x10);
			meta_line_ALAISOWNER.mOffset = PARENT_OFFSET(UID::Owner, DlgLine);
			NEXTMEM1(line, "Baseclass_DlgObjIDOwner", ALAISDLG, mDlgObjID,
				DlgLine, dlgidowner, 0x30, ALAISOWNER);
			meta_line_ALAISDLG.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BORDERLANDS");
			meta_line_ALAISDLG.mOffset = PARENT_OFFSET(DlgObjIDOwner, DlgLine);
			NEXTMEM2(line, mLangResProxy, DlgLine, res, 0, ALAISDLG);
			ADD(line);

			DEFINEMAP(int, DlgLine, std::less<int>);
			MKNAME(meta_Map_int_DlgLine, "Map<int,DlgLine,less<int>>");

			DEFINET2(lcol, DlgLineCollection);
			FIRSTMEM1(lcol, "Baseclass_UID::Generator", ALAISGEN, miNextUniqueID,
				DlgLineCollection, uidgen, 0x10);
			meta_lcol_ALAISGEN.mOffset = PARENT_OFFSET(UID::Generator, DlgLineCollection);
			NEXTMEM2(lcol, mLines, DlgLineCollection, Map_int_DlgLine, 0,
				ALAISGEN);
			ADD(lcol);

			DEFINET2(folderc, DlgFolderChild);
			FIRSTMEM1(folderc, "Baseclass_DlgChild", CHILDALAIS,mName, 
				DlgChild, dlgchild, 0x10);
			meta_folderc_CHILDALAIS.mOffset = PARENT_OFFSET(DlgChild, DlgFolderChild);
			ADD(folderc);

			DEFINET2(dlg, Dlg);
			SERIALIZER(dlg,Dlg);
			FIRSTMEM1(dlg, "Baseclass_DlgObjIDOwner", BASE, mDlgObjID,
				Dlg, dlgidowner, 0x10);
			meta_dlg_BASE.mOffset = PARENT_OFFSET(DlgObjIDOwner,Dlg);
			NEXTMEM1(dlg, "Baseclass_UID::Generator", BASE1,
				miNextUniqueID, Dlg, uidgen, 0x10, BASE);
			meta_dlg_BASE1.mOffset = PARENT_OFFSET(UID::Generator, Dlg);
			meta_dlg_BASE1.mGameIndexVersionRange.max = 
				TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(dlg, mName, Dlg, string, 0x20, BASE1);
			NEXTMEM2(dlg, mVersion, Dlg, long, 0x20, mName);
			NEXTMEM2(dlg, mDefFolderID, Dlg, dlgid, 0x20, mVersion);
			NEXTMEM2(dlg, mLangDB, Dlg, landb, 0x20, mDefFolderID);
			NEXTMEM2(dlg, mProjectID, Dlg, long, 0x20, mLangDB);
			NEXTMEM2(dlg, mResourceLocationID, Dlg, symbol, 0x20, mProjectID);
			NEXTMEM2(dlg, mChronology, Dlg, long, 0, mResourceLocationID);
			NEXTMEM2(dlg, mFlags, Dlg, flags, 0x20, mChronology);
			NEXTMEM2(dlg, mDependencies, Dlg, dloader, 0x20, mFlags);
			NEXTMEM2(dlg, mProdReportProps, Dlg, prop, 0, mDependencies);
			meta_dlg_mProdReportProps.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BORDERLANDS");
			NEXTMEM2(dlg, mJiraRecordManager, Dlg, jira, 0x20, mProdReportProps);
			meta_dlg_mJiraRecordManager.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BORDERLANDS");
			NEXTMEM2(dlg, mbHasToolOnlyData, Dlg, bool, 0x20, mJiraRecordManager);
			meta_dlg_mbHasToolOnlyData.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BORDERLANDS");
			EXT(dlg, dlog);
			ADD(dlg);

			DEFINET2(nstart, DlgNodeStart);
			FIRSTMEM1(nstart, "Baseclass_DlgNode", BASE, mName, DlgNodeStart, dnode, 0x10);
			meta_nstart_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeStart);
			ADDFLAGS(nstart, 8);
			NEXTMEM2(nstart, mProdReportProps, DlgNodeStart, prop, 0, BASE);
			meta_nstart_mProdReportProps.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BORDERLANDS");
			ADD(nstart);

			DEFINET2(eentry, DlgNodeExchange::Entry);
			FIRSTMEM2(eentry, mID, DlgNodeExchange::Entry, long, 0);
			NEXTMEM2(eentry, mType, DlgNodeExchange::Entry, long, MetaFlag::MetaFlag_EnumIntType, mID);
			ADD(eentry);

			DEFINEDCARRAY2(DlgNodeExchange::Entry, eentry);

			DEFINESCRIPTENUM(ENUM_TEXT_COLOUR_STYLE);
			DEFINESCRIPTENUM(ENUM_LIGHT_COMPOSER_NODE_LOCATION);
			DEFINESCRIPTENUM(ENUM_GAMEPAD_BUTTON);
			DEFINESCRIPTENUM(ENUM_LIGHT_COMPOSER_LIGHT_SOURCE_QUADRANT);
			DEFINESCRIPTENUM(ENUM_LIGHT_COMPOSER_CAMERA_ZONE);
			DEFINESCRIPTENUM(ENUM_AI_DUMMY_POSITION);
			DEFINESCRIPTENUM(ENUM_BLEND_TYPE);
			DEFINESCRIPTENUM(ENUM_RETICLE_ACTIONS);
			DEFINESCRIPTENUM(ENUM_STRUGGLE_TYPE);
			DEFINESCRIPTENUM(ENUM_AI_PATROL_TYPE);
			DEFINESCRIPTENUM(ENUM_MENU_ALIGN);
			DEFINESCRIPTENUM(ENUM_MENU_VERTICAL_ALIGN);
			DEFINESCRIPTENUM(ENUM_UI_COLOUR);
			DEFINESCRIPTENUM(ENUM_CHASE_FORWARD_VECTOR);
			DEFINESCRIPTENUM(ENUM_RETICLE_DISPLAY_MODE);
			DEFINESCRIPTENUM(ENUM_CONTROLLER_BUTTONS);
			DEFINESCRIPTENUM(ENUM_DIALOG_MODE);
			DEFINESCRIPTENUM(ENUM_USEABLE_TYPE);
			DEFINESCRIPTENUM(ENUM_AI_AGENT_STATE);
			DEFINESCRIPTENUM(ENUM_QUICK_TIME_EVENT_TYPE);

			DEFINET2(dex, DlgNodeExchange);
			SERIALIZER(dex, DlgNodeExchange);
			ADDFLAGS(dex, 8);
			FIRSTMEM2(dex, mPriority, DlgNodeExchange, float, 0);
			NEXTMEM2(dex, mhChore, DlgNodeExchange, Handlehchore, 0, mPriority);
			NEXTMEM1(dex, "Baseclass_DlgNode", BASE, mName, DlgNodeExchange, dnode, 0x10, mhChore);
			meta_dex_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeExchange);
			NEXTMEM2(dex, mEntries, DlgNodeExchange, DCArray_eentry, 0x20, BASE);
			ADD(dex);

			DEFINET2(cohort, DlgNodeStats::DlgChildSetCohort);
			FIRSTMEM1(cohort, "Baseclass_DlgChildSet", BASE, mChildren,
				DlgNodeStats::DlgChildSetCohort,
				dlgchildset, 0x10);
			meta_cohort_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgNodeStats::DlgChildSetCohort);
			ADD(cohort);

			DEFINET2(dccase, DlgChildSetConditionalCase);
			FIRSTMEM1(dccase, "Baseclass_DlgChildSet", BASE, mChildren, 
				DlgChildSetConditionalCase,
				dlgchildset, 0x10);
			meta_dccase_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgChildSetConditionalCase);
			ADD(dccase);

			DEFINET2(dcond, DlgNodeConditional);
			ADDFLAGS(dcond, 8);
			FIRSTMEM1(dcond, "Baseclass_DlgNode", BASE, mName, DlgNodeConditional,
				dnode, 0x10);
			meta_dcond_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeConditional);
			NEXTMEM2(dcond, mCases, DlgNodeConditional, dccase, 0x20, BASE);
			ADD(dcond);

			DEFINET2(dcase, DlgConditionalCase);
			ADDFLAGS(dcase, 8);
			FIRSTMEM1(dcase, "Baseclass_DlgChild", BASE, mName, DlgConditionalCase,
				dlgchild, 0x10);
			meta_dcase_BASE.mOffset = PARENT_OFFSET(DlgChild,DlgConditionalCase);
			ADD(dcase);

			DEFINET2(ddcohorti, DlgNodeStats::Cohort);
			ADDFLAGS(ddcohorti, 8);
			FIRSTMEM1(ddcohorti, "Baseclass_DlgChild", BASE, mName, DlgNodeStats::Cohort,
				dlgchild, 0x10);
			meta_ddcohorti_BASE.mOffset = PARENT_OFFSET(DlgChild, DlgNodeStats::Cohort);
			NEXTMEM2(ddcohorti, mhImage, DlgNodeStats::Cohort, Handletex, 0, BASE);
			NEXTMEM2(ddcohorti, mDisplayText1, DlgNodeStats::Cohort, res, 0, mhImage);
			NEXTMEM2(ddcohorti, mDisplayText2, DlgNodeStats::Cohort, res, 0, mDisplayText1);
			NEXTMEM2(ddcohorti, mLayout, DlgNodeStats::Cohort, string, 0, mDisplayText2);
			NEXTMEM2(ddcohorti, mSummaryDisplayText, DlgNodeStats::Cohort, res, 0, mLayout);
			ADD(ddcohorti);

			DEFINET2(dccase3, DlgChildSetChoicesChildPost);
			FIRSTMEM1(dccase3, "Baseclass_DlgChildSet", BASE, mChildren,
				DlgChildSetChoicesChildPost,
				dlgchildset, 0x10);
			meta_dccase3_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgChildSetChoicesChildPost);
			ADD(dccase3);

			DEFINET2(dccase2, DlgChildSetChoicesChildPre);
			FIRSTMEM1(dccase2, "Baseclass_DlgChildSet", BASE, mChildren,
				DlgChildSetChoicesChildPre,
				dlgchildset, 0x10);
			meta_dccase2_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgChildSetChoicesChildPre);
			ADD(dccase2);

			DEFINET2(dccase1, DlgChildSetChoice);
			FIRSTMEM1(dccase1, "Baseclass_DlgChildSet", BASE, mChildren,
				DlgChildSetChoice,
				dlgchildset, 0x10);
			meta_dccase1_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgChildSetChoice);
			ADD(dccase1);

			DEFINET2(dccase7, DlgNodeParallel::DlgChildSetElement);
			FIRSTMEM1(dccase7, "Baseclass_DlgChildSet", BASE, mChildren,
				DlgNodeParallel::DlgChildSetElement,
				dlgchildset, 0x10);
			meta_dccase7_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgNodeParallel::DlgChildSetElement);
			ADD(dccase7);

			DEFINET2(dccase5, DlgNodeSequence::DlgChildSetElement);
			FIRSTMEM1(dccase5, "Baseclass_DlgChildSet", BASE, mChildren,
				DlgNodeSequence::DlgChildSetElement,
				dlgchildset, 0x10);
			meta_dccase5_BASE.mOffset = PARENT_OFFSET(DlgChildSet,
				DlgNodeSequence::DlgChildSetElement);
			ADD(dccase5);

			DEFINET2(dchoices, DlgNodeChoices);
			ADDFLAGS(dchoices, 8);
			FIRSTMEM1(dchoices, "Baseclass_DlgNode", BASE, mName, DlgNodeChoices,
				dnode, 0x10);
			meta_dchoices_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeChoices);
			NEXTMEM2(dchoices, mChoices, DlgNodeChoices, dccase1, 0x20, BASE);
			NEXTMEM2(dchoices, mPreChoice, DlgNodeChoices, dccase2, 0x20, mChoices);
			NEXTMEM2(dchoices, mPostChoice, DlgNodeChoices, dccase3, 0x20, mPreChoice);
			ADD(dchoices);

			DEFINET2(dcondset, DlgConditionSet);
			SERIALIZER(dcondset, DlgConditionSet);
			ADD(dcondset);

			DEFINET2(dscript, DlgNodeScript);
			ADDFLAGS(dscript, 8);
			FIRSTMEM1(dscript, "Baseclass_DlgNode", BASE, mName, DlgNodeScript,
				dnode, 0x10);
			meta_dscript_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeScript);
			NEXTMEM2(dscript, mScriptText, DlgNodeScript, string, 0x20, BASE);
			NEXTMEM2(dscript, mbBlocking, DlgNodeScript, bool, 0, mScriptText);
			NEXTMEM2(dscript, mbExecuteOnInstanceRetire,DlgNodeScript, bool, 0, mbBlocking);
			ADD(dscript);

			DEFINET2(dlogic, DlgNodeLogic);
			ADDFLAGS(dlogic, 8);
			FIRSTMEM1(dlogic, "Baseclass_DlgNode", BASE, mName, DlgNodeLogic,
				dnode, 0x10);
			meta_dlogic_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeLogic);
			NEXTMEM2(dlogic, mRule, DlgNodeLogic, rule, 0, BASE);
			ADD(dlogic);

			DEFINET2(djump, DlgNodeJump);
			ADDFLAGS(djump, 8);
			FIRSTMEM1(djump, "Baseclass_DlgNode", BASE, mName, DlgNodeJump,
				dnode, 0x10);
			meta_djump_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeJump);
			NEXTMEM2(djump, mJumpToLink, DlgNodeJump, dlglink, 0x20, BASE);
			NEXTMEM2(djump, mJumpToName, DlgNodeJump, symbol, 0, mJumpToLink);
			NEXTMEM2(djump, mJumpTargetClass, DlgNodeJump, long, 0x40, mJumpToName);
			FIRSTENUM2(djump, mJumpTargetClass, "eToNodeAfterParentWaitNode", a, 3,0);
			NEXTENUM2(djump, mJumpTargetClass, "eToParent", b, 2, 0, a);
			NEXTENUM2(djump, mJumpTargetClass, "eToName", c, 1, 0, b);
			NEXTMEM2(djump, mJumpBehaviour, DlgNodeJump, long, 0x40, mJumpTargetClass);
			FIRSTENUM2(djump, mJumpBehaviour, "eJumpAndExecute", a, 1, 0);
			NEXTENUM2(djump, mJumpBehaviour, "eJumpExecuteAndReturn", b, 2, 0, a);
			NEXTENUM2(djump, mJumpBehaviour, "eReturn", c, 3, 0, b);
			NEXTMEM2(djump, mChoiceTransparency, DlgNodeJump, long, 0x0, mJumpBehaviour);
			NEXTMEM2(djump, mVisibilityBehaviour, DlgNodeJump, long, 0x40, mChoiceTransparency);
			FIRSTENUM2(djump, mVisibilityBehaviour, "eIgnoreVisibility", a, 1, 0);
			NEXTENUM2(djump, mVisibilityBehaviour, "eObeyVisibility", b, 2, 0, a);
			NEXTENUM2(djump, mVisibilityBehaviour, "eReturn", c, 3, 0, b);//shouldnt be here, tellale u made a mistake :P
			NEXTMEM2(djump, mhJumpToDlg, DlgNodeJump, Handledlg, 0, mVisibilityBehaviour);
			ADD(djump);

			DEFINET2(didle, DlgNodeIdle);
			ADDFLAGS(didle, 8);
			FIRSTMEM1(didle, "Baseclass_DlgNode", BASE, mName, DlgNodeIdle,
				dnode, 0x10);
			meta_didle_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeIdle);
			NEXTMEM2(didle, mhIdle, DlgNodeIdle, Handlehchore, 0, BASE);
			NEXTMEM2(didle, mOverrideOptionTime, DlgNodeIdle, long, 0, mhIdle);
			FIRSTENUM2(didle, mOverrideOptionTime, "eUseDefaults", def, 1, 0);
			NEXTENUM2(didle, mOverrideOptionTime, "eOverride", ov, 2, 0, def);
			NEXTMEM2(didle, mOverrideOptionStyle, DlgNodeIdle, long, 0x40, mOverrideOptionTime);
			FIRSTENUM2(didle, mOverrideOptionStyle, "eUseDefaults", def, 1, 0);
			NEXTENUM2(didle, mOverrideOptionStyle, "eOverride", ov, 2, 0, def);
			NEXTMEM2(didle, mTransitionTimeOverride, DlgNodeIdle, float, 0, mOverrideOptionStyle);
			NEXTMEM2(didle, mTransitionStyleOverride, DlgNodeIdle, long, 0x20, mTransitionTimeOverride);
			NEXTMEM2(didle, mIdleSlot, DlgNodeIdle, long, 0x20, mTransitionStyleOverride);
			ADD(didle);

			DEFINET2(dcase8, DlgNodeParallel::PElement);
			ADDFLAGS(dcase8, 8);
			FIRSTMEM1(dcase8, "Baseclass_DlgChild", BASE, mName, DlgNodeParallel::PElement,
				dlgchild, 0x10);
			meta_dcase8_BASE.mOffset = PARENT_OFFSET(DlgChild, DlgNodeParallel::PElement);
			ADD(dcase8);

			DEFINET2(dcase6, DlgNodeSequence::Element);
			ADDFLAGS(dcase6, 8);
			FIRSTMEM1(dcase6, "Baseclass_DlgChild", BASE, mName, DlgNodeSequence::Element,
				dlgchild, 0x10);
			meta_dcase6_BASE.mOffset = PARENT_OFFSET(DlgChild, DlgNodeSequence::Element);
			NEXTMEM2(dcase6, mRepeat, DlgNodeSequence::Element, long, 0x40, BASE);
			FIRSTENUM2(dcase6, mRepeat, "eMaxPlusOne", a, 8, 0);
			NEXTENUM2(dcase6, mRepeat, "eIndefinitely", b, 1, 0, a);
			NEXTENUM2(dcase6, mRepeat, "eSix", c, 7, 0, b);
			NEXTENUM2(dcase6, mRepeat, "eFive", d, 6, 0, c);
			NEXTENUM2(dcase6, mRepeat, "eFour", e, 5, 0, d);
			NEXTENUM2(dcase6, mRepeat, "eThree", f, 4, 0, e);
			NEXTENUM2(dcase6, mRepeat, "eTwo", g, 3, 0, f);
			NEXTENUM2(dcase6, mRepeat, "eOne", h, 2, 0, g);
			NEXTMEM2(dcase6, mPlayPosition, DlgNodeSequence::Element, long, 0x40, mRepeat);
			FIRSTENUM2(dcase6, mPlayPosition, "eLast", a, 3, 0);
			NEXTENUM2(dcase6, mPlayPosition, "eFirst", b, 2, 0, a);
			NEXTENUM2(dcase6, mPlayPosition, "eUnspecified", c, 1, 0, b);
			ADD(dcase6);

			DEFINET2(dsequence, DlgNodeSequence);
			ADDFLAGS(dsequence, 8);
			FIRSTMEM1(dsequence, "Baseclass_DlgNode", BASE, mName, DlgNodeSequence,
				dnode, 0x10);
			meta_dsequence_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeSequence);
			NEXTMEM2(dsequence, mElements, DlgNodeSequence, dccase5, 0x20, BASE);
			NEXTMEM2(dsequence, mPlaybackMode, DlgNodeSequence, long, 0x40, mElements);
			FIRSTENUM2(dsequence, mPlaybackMode, "eShuffle", a, 2, 0);
			NEXTENUM2(dsequence, mPlaybackMode, "eSequential", b, 1, 0, a);
			NEXTMEM2(dsequence, mLifetimeMode, DlgNodeSequence, long, 0x40, mPlaybackMode);
			FIRSTENUM2(dsequence, mLifetimeMode, "eSingleSequenceRepeatFinal", a, 3, 0);
			NEXTENUM2(dsequence, mLifetimeMode, "eSingleSequence", b, 2, 0, a);
			NEXTENUM2(dsequence, mLifetimeMode, "eLooping", c, 1, 0, b);
			NEXTMEM2(dsequence, mElemUseCriteria, DlgNodeSequence, crit, 0x20, mLifetimeMode);
			ADD(dsequence);

			DEFINET2(dparallel, DlgNodeParallel);
			ADDFLAGS(dparallel, 8);
			FIRSTMEM1(dparallel, "Baseclass_DlgNode", BASE, mName, DlgNodeParallel,
				dnode, 0x10);
			meta_dparallel_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeParallel);
			NEXTMEM2(dparallel, mPElements, DlgNodeParallel, dccase7, 0x20, BASE);
			NEXTMEM2(dparallel, mElemUseCriteria, DlgNodeParallel, crit, 0x20, mPElements);
			ADD(dparallel);

			DEFINET2(dNote, DlgNodeNotes);
			ADDFLAGS(dNote, 8);
			FIRSTMEM1(dNote, "Baseclass_DlgNode", BASE, mName, DlgNodeNotes,
				dnode, 0x10);
			meta_dNote_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeNotes);
			NEXTMEM2(dNote, mNoteText, DlgNodeNotes, string, 0x20, BASE);
			ADD(dNote);

			DEFINET2(dtext, DlgNodeText);
			ADDFLAGS(dtext, 8);
			FIRSTMEM1(dtext, "Baseclass_DlgNode", BASE, mName, DlgNodeText,
				dnode, 0x10);
			meta_dtext_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeText);
			NEXTMEM2(dtext, mLangResProxy, DlgNodeText, res, 0x20, BASE);
			ADD(dtext);

			DEFINET2(dchore, DlgNodeChore);
			ADDFLAGS(dchore, 8);
			FIRSTMEM2(dchore, mChore, DlgNodeChore, Handlehchore, 0);
			NEXTMEM2(dchore, mPriority, DlgNodeChore, long, 0, mChore);
			NEXTMEM2(dchore, mLooping, DlgNodeChore, bool, 0, mPriority);
			NEXTMEM1(dchore, "Baseclass_DlgNode", BASE, mName, DlgNodeChore,
				dnode, 0x10,mLooping);
			meta_dchore_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeChore);
			ADD(dchore);

			DEFINET2(dchoices4, DlgNodeMarker);
			ADDFLAGS(dchoices4, 8);
			FIRSTMEM1(dchoices4, "Baseclass_DlgNode", BASE, mName, DlgNodeMarker,
				dnode, 0x10);
			meta_dchoices4_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeMarker);
			ADD(dchoices4);

			DEFINET2(dchoices3, DlgNodeWait);
			ADDFLAGS(dchoices3, 8);
			FIRSTMEM1(dchoices3, "Baseclass_DlgNode", BASE, mName, DlgNodeWait,
				dnode, 0x10);
			meta_dchoices3_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeWait);
			NEXTMEM1(dchoices3, "Baseclass_DlgConditionSet", BASE1, mName, DlgNodeWait, dcondset, 0x10, BASE);
			meta_dchoices3_BASE1.mOffset = PARENT_OFFSET(DlgConditionSet, DlgNodeWait);
			ADD(dchoices3);

			DEFINET2(dcancel, DlgNodeCancelChoices);
			ADDFLAGS(dcancel, 8);
			FIRSTMEM1(dcancel, "Baseclass_DlgNode", BASE, mName, DlgNodeCancelChoices,
				dnode, 0x10);
			meta_dcancel_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeCancelChoices);
			NEXTMEM2(dcancel, mCancelGroup, DlgNodeCancelChoices, long, 0x20, BASE);
			ADD(dcancel);

			DEFINET2(dboard, DlgNodeStoryBoard);
			ADDFLAGS(dboard, 8);
			FIRSTMEM1(dboard, "Baseclass_DlgNode", BASE, mName, DlgNodeStoryBoard,
				dnode, 0x10);
			meta_dboard_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeStoryBoard);
			NEXTMEM2(dboard, mStoryBoardImage, DlgNodeStoryBoard, symbol, 0, BASE);
			ADD(dboard);

			DEFINET2(dstats, DlgNodeStats);
			ADDFLAGS(dstats, 8);
			FIRSTMEM1(dstats, "Baseclass_DlgNode", BASE, mName, DlgNodeStats,
				dnode, 0x10);
			meta_dstats_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeStats);
			NEXTMEM2(dstats, mStatsType, DlgNodeStats, long, 0x40, BASE);
			FIRSTENUM2(dstats, mStatsType, "kChoices", a, 1, 0);
			NEXTENUM2(dstats, mStatsType, "kExtended", b, 2, 0, a);
			NEXTENUM2(dstats, mStatsType, "kCrowdPlay", c, 3, 0, b);
			NEXTENUM2(dstats, mStatsType, "kRelationships", d, 4, 0, c);
			NEXTMEM2(dstats, mCohorts, DlgNodeStats, cohort, 0, mStatsType);
			NEXTMEM2(dstats, mhImage, DlgNodeStats, Handletex, 0, mCohorts);
			NEXTMEM2(dstats, mDisplayText, DlgNodeStats, res, 0, mhImage);
			ADD(dstats);

			DEFINET2(dExit, DlgNodeExit);
			ADDFLAGS(dExit, 8);
			FIRSTMEM1(dExit, "Baseclass_DlgNode", BASE, mName, DlgNodeExit,
				dnode, 0x10);
			meta_dExit_BASE.mOffset = PARENT_OFFSET(DlgNode, DlgNodeExit);
			ADD(dExit);

			DEFINET2(dlgcond, DlgCondition);
			ADDFLAGS(dlgcond,8);
			FIRSTMEM1(dlgcond, "Baseclass_DlgObjIDOwner", BASE, mDlgObjID, DlgCondition,
				dlgidowner, 0x10);
			meta_dlgcond_BASE.mOffset = PARENT_OFFSET(DlgObjIDOwner, DlgCondition);
			ADD(dlgcond);

			DEFINET2(dcase1, DlgChoice);
			ADDFLAGS(dcase1, 8);
			FIRSTMEM1(dcase1, "Baseclass_DlgChild", BASE, mName, DlgChoice,
				dlgchild, 0x10);
			meta_dcase1_BASE.mOffset = PARENT_OFFSET(DlgChild, DlgChoice);
			NEXTMEM1(dcase1, "Baseclass_DlgConditionSet", BASE1, mName,DlgChoice,
				dcondset, 0x10, BASE);
			meta_dcase1_BASE1.mOffset = PARENT_OFFSET(DlgConditionSet, DlgChoice);
			ADD(dcase1);

			DEFINET2(dcase3, DlgChoicesChildPost);
			ADDFLAGS(dcase3, 8);
			FIRSTMEM1(dcase3, "Baseclass_DlgChild", BASE, mName, DlgChoicesChildPost,
				dlgchild, 0x10);
			meta_dcase3_BASE.mOffset = PARENT_OFFSET(DlgChild, DlgChoicesChildPost);
			ADD(dcase3);

			DEFINET2(dcase2, DlgChoicesChildPre);
			ADDFLAGS(dcase2, 8);
			FIRSTMEM1(dcase2, "Baseclass_DlgChild", BASE, mName, DlgChoicesChildPre,
				dlgchild, 0x10);
			meta_dcase2_BASE.mOffset = PARENT_OFFSET(DlgChild, DlgChoicesChildPre);
			ADD(dcase2);

			DEFINET2(dtime, DlgConditionTime);
			FIRSTMEM1(dtime, "Baseclass_DlgCondition", BASE, mDlgObjID, DlgConditionTime, dlgcond, 0x10);
			meta_dtime_BASE.mOffset = PARENT_OFFSET(DlgCondition, DlgConditionTime);
			NEXTMEM2(dtime, mDurationClass, DlgConditionTime, long, 0x40, BASE);
			NEXTMEM2(dtime, mSeconds, DlgConditionTime, float, 0x20, mDurationClass);
			ADD(dtime);

			DEFINET2(dinput1, DlgConditionRule);
			FIRSTMEM1(dinput1, "Baseclass_DlgCondition", BASE, mDlgObjID, DlgConditionRule, dlgcond, 0x10);
			meta_dinput1_BASE.mOffset = PARENT_OFFSET(DlgCondition, DlgConditionRule);
			NEXTMEM2(dinput1, mRule, DlgConditionRule, rule, 0, BASE);
			ADD(dinput1);

			DEFINET2(dinput, DlgConditionInput);
			FIRSTMEM1(dinput, "Baseclass_DlgCondition", BASE, mDlgObjID, DlgConditionInput, dlgcond, 0x10);
			meta_dinput_BASE.mOffset = PARENT_OFFSET(DlgCondition, DlgConditionInput);
			ADD(dinput);

			//all types with file extension handles that would be referenced (handle and handlelock)
			DEFINEHANDLE(wbox, WalkBoxes);
			DEFINEHANDLE(aam, ActorAgentMapper);
			DEFINEHANDLE(amap, AgentMap);
			DEFINEHANDLE(audiobus, SoundBusSystem::BusHolder);
			DEFINEHANDLE(dss, DlgSystemSettings);
			DEFINEHANDLE(enl, EnlightenData);
			DEFINEHANDLE(imap, InputMapper);
			DEFINEHANDLE(landb, LanguageDB);
			DEFINEHANDLE(langdb, LanguageDatabase);
			DEFINEHANDLE(llm, LanguageLookupMap);
			DEFINEHANDLE(locreg, LocalizationRegistry);
			DEFINEHANDLE(overlay, T3OverlayData);
			DEFINEHANDLE(ppkgd, PreloadPackage::RuntimeDataDialog);
			DEFINEHANDLE(ppkgs, PreloadPackage::RuntimeDataScene);
			DEFINEHANDLE(probe, LightProbeData);
			DEFINEHANDLE(ptable, PhonemeTable);
			DEFINEHANDLE(rules, Rules);
			DEFINEHANDLE(reverb, SoundReverbDefinition);
			DEFINEHANDLE(skl, Skeleton);
			DEFINEHANDLE(style, StyleGuide);

			DEFINET2(cp112, EnumRenderMaskWrite);
			FIRSTMEM2(cp112, mVal, EnumRenderMaskWrite, long, MetaFlag::MetaFlag_EnumWrapperClass | MetaFlag::MetaFlag_NoPanelCaption);
			FIRSTENUM2(cp112, mVal, "eRenderMaskWrite_None", a, 1, 0);
			NEXTENUM2(cp112, mVal, "eRenderMaskWrite_Set", b, 2, 0, a);
			NEXTENUM2(cp112, mVal, "eRenderMaskWrite_Clear", c, 3, 0, b);
			ADD(cp112);

			DEFINET2(cp1123, EnumRenderMaskTest);
			FIRSTMEM2(cp1123, mVal, EnumRenderMaskTest, long, MetaFlag::MetaFlag_EnumWrapperClass | MetaFlag::MetaFlag_NoPanelCaption);
			FIRSTENUM2(cp1123, mVal, "eRenderMaskTest_None", a, 1, 0);
			NEXTENUM2(cp1123, mVal, "eRenderMaskTest_Set", b, 2, 0, a);
			NEXTENUM2(cp1123, mVal, "eRenderMaskTest_Clear", c, 3, 0, b);
			ADD(cp1123);

			DEFINET2(cp11, EnumT3LightEnvGroup);
			FIRSTMEM2(cp11, mVal, EnumT3LightEnvGroup, long, MetaFlag::MetaFlag_EnumWrapperClass | MetaFlag::MetaFlag_NoPanelCaption);
			FIRSTENUM2(cp11, mVal, "eLightEnvGroup_None", a, -2, 0);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Default", b, -1, 0,a);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group0", c, 0, 0, b);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group1", d, 1, 0, c);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group2", e, 2, 0, d);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group3", f, 3, 0, e);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group4", g, 4, 0, f);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group5", h, 5, 0, g);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group6", i, 6, 0, h);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_Group7", j, 7, 0, i);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_AmbientGroup0", k, 16, 0, j);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_AmbientGroup1", l, 17, 0, k);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_AmbientGroup2", m, 18, 0, l);
			NEXTENUM2(cp11, mVal, "eLightEnvGroup_AmbientGroup3", n, 19, 0, m);
			ADD(cp11);

			DEFINET2(cp1, FlagsT3LightEnvGroupSet);
			FIRSTMEM2(cp1, mFlags, FlagsT3LightEnvGroupSet, flags, MetaFlag::MetaFlag_FlagType);
			FIRSTFLAG(cp1, mFlags, "Group0", a, 1);
			NEXTFLAG(cp1, mFlags, "Group1", b, 2, a);
			NEXTFLAG(cp1, mFlags, "Group2", c, 4, b);
			NEXTFLAG(cp1, mFlags, "Group3", d, 8, c);
			NEXTFLAG(cp1, mFlags, "Group4", e, 16, d);
			NEXTFLAG(cp1, mFlags, "Group5", f, 32, e);
			NEXTFLAG(cp1, mFlags, "Group6", g, 64, f);
			NEXTFLAG(cp1, mFlags, "Group7", h, 128, g);
			NEXTFLAG(cp1, mFlags, "AmbientGroup0", i, 256, h);
			NEXTFLAG(cp1, mFlags, "AmbientGroup1", j, 512, i);
			NEXTFLAG(cp1, mFlags, "AmbientGroup2", k, 1024, j);
			NEXTFLAG(cp1, mFlags, "AmbientGroup3", l, 2048, k);
			ADD(cp1);


			DEFINET2(cp, CorrespondencePoint);
			FIRSTMEM2(cp, mFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType);
			FIRSTFLAG(cp, mFlags, "Left Foot Down", a, 2);
			NEXTFLAG(cp, mFlags, "Right Foot Down", b, 4, a);
			NEXTFLAG(cp, mFlags, "Force End", c, 8, b);
			NEXTFLAG(cp, mFlags, "Start Position Lerp", d, 16, c);
			NEXTFLAG(cp, mFlags, "End Position Lerp", e, 32, d);
			NEXTMEM2(cp, mEaseOutStartFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType, mFlags);
			FIRSTFLAG(cp, mEaseOutStartFlags, "Ease Out A Open", a, 1);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out B Open", b, 2, a);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out C Open", c, 4, b);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out D Open", d, 8, c);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out E Open", e, 16, d);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out F Open", f, 32, e);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out G Open", g, 64, f);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out H Open", h, 128, g);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out I Open", i, 256, h);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out J Open", j, 512, i);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out K Open", k, 1024, j);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out L Open", l, 2048, k);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out M Open", m, 4096, l);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out N Open", n, 8192, m);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out O Open", o, 0x4000, n);
			NEXTFLAG(cp, mEaseOutStartFlags, "Ease Out P Open", p, 0x8000, o);
			NEXTMEM2(cp, mEaseOutEndFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType, mEaseOutStartFlags);
			FIRSTFLAG(cp, mEaseOutEndFlags, "Ease Out A Close", a, 1);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out B Close", b, 2, a);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out C Close", c, 4, b);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out D Close", d, 8, c);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out E Close", e, 16, d);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out F Close", f, 32, e);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out G Close", g, 64, f);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out H Close", h, 128, g);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out I Close", i, 256, h);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out J Close", j, 512, i);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out K Close", k, 1024, j);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out L Close", l, 2048, k);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out M Close", m, 4096, l);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out N Close", n, 8192, m);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out O Close", o, 0x4000, n);
			NEXTFLAG(cp, mEaseOutEndFlags, "Ease Out P Close", p, 0x8000, o);

			NEXTMEM2(cp, mEaseInStartFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType, mEaseOutEndFlags);
			FIRSTFLAG(cp, mEaseInStartFlags, "Ease In A Open", a, 1);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In B Open", b, 2, a);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In C Open", c, 4, b);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In D Open", d, 8, c);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In E Open", e, 16, d);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In F Open", f, 32, e);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In G Open", g, 64, f);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In H Open", h, 128, g);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In I Open", i, 256, h);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In J Open", j, 512, i);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In K Open", k, 1024, j);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In L Open", l, 2048, k);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In M Open", m, 4096, l);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In N Open", n, 8192, m);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In O Open", o, 0x4000, n);
			NEXTFLAG(cp, mEaseInStartFlags, "Ease In P Open", p, 0x8000, o);
			NEXTMEM2(cp, mEaseInEndFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType, mEaseInStartFlags);
			FIRSTFLAG(cp, mEaseInEndFlags, "Ease In A Close", a, 1);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In B Close", b, 2, a);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In C Close", c, 4, b);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In D Close", d, 8, c);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In E Close", e, 16, d);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In F Close", f, 32, e);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In G Close", g, 64, f);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In H Close", h, 128, g);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In I Close", i, 256, h);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In J Close", j, 512, i);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In K Close", k, 1024, j);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In L Close", l, 2048, k);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In M Close", m, 4096, l);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In N Close", n, 8192, m);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In O Close", o, 0x4000, n);
			NEXTFLAG(cp, mEaseInEndFlags, "Ease In P Close", p, 0x8000, o);
			meta_cp_mEaseOutStartFlags.mGameIndexVersionRange.max  = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_cp_mEaseInStartFlags.mGameIndexVersionRange.max  = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_cp_mEaseOutEndFlags.mGameIndexVersionRange.max  = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_cp_mEaseInEndFlags.mGameIndexVersionRange.max  = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_cp_mEaseOutStartFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MC2");
			meta_cp_mEaseInStartFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MC2");
			meta_cp_mEaseOutEndFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MC2");
			meta_cp_mEaseInEndFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MC2");
			NEXTMEM2(cp, mSteeringFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType, mEaseInEndFlags);
			FIRSTFLAG(cp, mSteeringFlags, "Manual Steering On", a, 1);
			NEXTFLAG(cp, mSteeringFlags, "Manual Steering Off", b, 2, a);
			NEXTMEM2(cp, mTransitionFlags, CorrespondencePoint, flags, MetaFlag::MetaFlag_FlagType, mSteeringFlags);
			FIRSTFLAG(cp, mTransitionFlags, "Transition Window Open", a, 1);
			meta_cp_mTransitionFlags.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			NEXTFLAG(cp, mTransitionFlags, "Transition Window Closed", b, 2, a);
			NEXTMEM2(cp, mfTime, CorrespondencePoint, float, 0, mTransitionFlags);
			NEXTMEM2(cp, mComment, CorrespondencePoint, string, 0, mfTime);
			ADD(cp);

			DEFINEDCARRAY(CorrespondencePoint);

			DEFINET2(be, BlendEntry);
			FIRSTMEM2(be, mParameterValues, BlendEntry, vec3, 0);
			NEXTMEM2(be, mAnimOrChore, BlendEntry, animorchore, 0, mParameterValues);
			NEXTMEM2(be, mCorrespondencePoints, BlendEntry, DCArray_CorrespondencePoint, 0, mAnimOrChore);
			NEXTMEM2(be, mfAnimOrChoreLength, BlendEntry, float, 0x20, mCorrespondencePoints);
			NEXTMEM2(be, mComment, BlendEntry, string, 0, mfAnimOrChoreLength);
			ADD(be);

			DEFINEDCARRAY(BlendEntry);

			DEFINET2(bgt, BlendGraph::EnumBlendGraphType);
			FIRSTMEM2(bgt, mVal, BlendGraph::EnumBlendGraphType, long, 0x40);
			FIRSTENUM2(bgt, mVal, "eBlendgraph_Looping", a, 0, 0);
			ADDFLAGS(bgt, 0x8008);
			NEXTENUM2(bgt, mVal, "eBlendgraph_NonLooping", b, 1, 0, a);
			NEXTMEM1(bgt, "Baseclass_EnumBase", BASE, mVal, BlendGraph::EnumBlendGraphType, enumbase, 0x10,mVal);
			ADD(bgt);

			DEFINEKEYFRAMEDVALUE(int, int, long);
			DEFINEMAP2(float, KeyframedValue<int>, float, kfvi, std::less<float>);
			DEFINEMAP2(float, Map<float SEP KeyframedValue<int>>, float, map_f_kfv, std::less<float>);
			MKNAME(meta_kfv_int, "KeyframedValue<int>");
			MKNAME(meta_Map_float_kfvi, "Map<float,KeyframedValue<int>,less<float>>");
			MKNAME(meta_Map_float_map_f_kfv, "Map<float,Map<float,KeyframedValue<int>,less<float>>,less<float>>");

			DEFINET2(bg, BlendGraph);
			EXT(bg, bgh);
			SERIALIZER(bg, BlendGraph);
			FIRSTMEM1(bg, "mNumDimensions", ALAIS, mNumDimensions, BlendGraph, long, 0x20);
			meta_bg_ALAIS.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(bg, mParameters, BlendGraph, DCArray_Symbol, 0, ALAIS);
			NEXTMEM2(bg, mFrozenParameterNames, BlendGraph, DCArray_Symbol, 0, mParameters);
			//NEXTMEM1(bg, "mFrozenParameterNames", WD3FUCKUP, mFrozenParameterNames, BlendGraph, DCArray_Symbol, 0, mFrozenParameterNames);
			//meta_bg_mFrozenParameterNames.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			//meta_bg_WD3FUCKUP.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_bg_mFrozenParameterNames.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			NEXTMEM2(bg, mDampeningConstants, BlendGraph, DCArray_float, 0, mFrozenParameterNames);
			NEXTMEM2(bg, mEntries, BlendGraph, DCArray_BlendEntry, 0, mDampeningConstants);
			NEXTMEM2(bg, mBlendGraphType, BlendGraph, bgt, 0, mEntries);
			NEXTMEM2(bg, mDampen, BlendGraph, bool, 0, mBlendGraphType);
			NEXTMEM2(bg, mfTimeScale, BlendGraph, float, 0, mDampen);
			NEXTMEM2(bg, mComment, BlendGraph, string, 0, mfTimeScale);
			NEXTMEM2(bg, mhBlendGraphAuxiliaryChore, BlendGraph, Handlehchore, 0, mComment);
			meta_bg_mhBlendGraphAuxiliaryChore.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			NEXTMEM2(bg, mbInvertParameters, BlendGraph, long, 0x20, mhBlendGraphAuxiliaryChore);
			meta_bg_mbInvertParameters.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(bg, mVersion, BlendGraph, long, 0x20, mbInvertParameters);
			NEXTMEM2(bg, mNumGeometryDimensions, BlendGraph,long, 0x20, mVersion);
			NEXTMEM1(bg, "mNumDimensions", ALAIS2, mNumDimensions, BlendGraph, long, 0x20, mNumGeometryDimensions);
			NEXTMEM2(bg, mParameterOrder, BlendGraph, DCArray_i32, 0x20, ALAIS2);
			meta_bg_mParameterOrder.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_bg_ALAIS2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			ADD(bg);

			DEFINET2(bgm, BlendGraphManager);
			EXT(bgm, bgm);
			FIRSTMEM2(bgm, mfTransitionTime, BlendGraphManager, float, 0);
			NEXTMEM2(bgm, mIdleAnimOrChore, BlendGraphManager, animorchore, 0, mfTransitionTime);
			NEXTMEM2(bgm, mbUseAnimationMoverData, BlendGraphManager, bool, 0, mIdleAnimOrChore);
			meta_bgm_mbUseAnimationMoverData.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(bgm, mhFreewalkStartGraph, BlendGraphManager, Handlehchore, 0, mbUseAnimationMoverData);
			NEXTMEM2(bgm, mhFreewalkLoopGraph, BlendGraphManager, Handlehchore, 0, mhFreewalkStartGraph);
			NEXTMEM2(bgm, mhFreewalkStopGraph, BlendGraphManager, Handlehchore, 0, mhFreewalkLoopGraph);
			NEXTMEM2(bgm, mhTurnToFaceGraph, BlendGraphManager, Handlehchore, 0, mhFreewalkStopGraph);
			NEXTMEM2(bgm, mhChoredMovementStartGraph, BlendGraphManager, Handlehchore, 0, mhTurnToFaceGraph);
			NEXTMEM2(bgm, mhChoredMovementLoopGraph, BlendGraphManager, Handlehchore, 0, mhChoredMovementStartGraph);
			NEXTMEM2(bgm, mhChoredMovementStopGraph, BlendGraphManager, Handlehchore, 0, mhChoredMovementLoopGraph);
			meta_bgm_mhChoredMovementStartGraph.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mhChoredMovementStopGraph.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mhChoredMovementLoopGraph.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mhChoredMovementStartGraph.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_bgm_mhChoredMovementStopGraph.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_bgm_mhChoredMovementLoopGraph.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_bgm_mhTurnToFaceGraph.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(bgm, mVersion, BlendGraphManager, long, 0, mhChoredMovementStopGraph);
			NEXTMEM2(bgm, mbUseAlgorithmicHeadTurn, BlendGraphManager, bool, 0, mVersion);
			NEXTMEM2(bgm, mfMaxManualSteeringVelocityInDegrees, BlendGraphManager, float, 0, mbUseAlgorithmicHeadTurn);
			NEXTMEM2(bgm, mfMinManualSteeringVelocityInDegrees, BlendGraphManager, float, 0, mfMaxManualSteeringVelocityInDegrees);
			NEXTMEM2(bgm, mfMaxLeanInPercentVelocity, BlendGraphManager, float, 0, mfMinManualSteeringVelocityInDegrees);
			NEXTMEM2(bgm, mfMinLeanInPercentVelocity, BlendGraphManager, float, 0, mfMaxLeanInPercentVelocity);
			NEXTMEM2(bgm, mfWalkSpeedScale, BlendGraphManager, float, 0, mfMinLeanInPercentVelocity);
			NEXTMEM2(bgm, mfRunSpeedScale, BlendGraphManager, float, 0, mfWalkSpeedScale);
			meta_bgm_mfMaxManualSteeringVelocityInDegrees.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mfMinManualSteeringVelocityInDegrees.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mfMaxLeanInPercentVelocity.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mfMinLeanInPercentVelocity.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mfWalkSpeedScale.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mfRunSpeedScale.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_bgm_mbUseAlgorithmicHeadTurn.mSkipVersion = TelltaleToolLib_GetGameKeyIndex("WD3");
			ADD(bgm);

			DEFINET2(omesh, T3OcclusionMeshBatch);
			FIRSTMEM2(omesh, mFlags, T3OcclusionMeshBatch, long, 0);
			NEXTMEM2(omesh, mStartIndex, T3OcclusionMeshBatch, long, 0, mFlags);
			NEXTMEM2(omesh, mNumTriangles, T3OcclusionMeshBatch, long, 0, mStartIndex);
			ADD(omesh);

			DEFINET2(msed, MeshSceneEnlightenData);
			FIRSTMEM2(msed, mSystemName, MeshSceneEnlightenData, symbol, 0);
			NEXTMEM2(msed, mUVTransform, MeshSceneEnlightenData, vec4, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled, mSystemName);
			NEXTMEM2(msed, mFlags, MeshSceneEnlightenData, flags, MetaFlag::MetaFlag_FlagType, mUVTransform);
			FIRSTFLAG(msed, mFlags, "eMeshSceneEnlightenFlag_ProbeLit", probe, 1);
			ADD(msed);

			DEFINET2(lmde, MeshSceneLightmapData::Entry);
			FIRSTMEM2(lmde, mMeshName, MeshSceneLightmapData::Entry, symbol, 0);
			NEXTMEM2(lmde, mLODIndex, MeshSceneLightmapData::Entry, long, 0, mMeshName);
			NEXTMEM2(lmde, mLightQuality, MeshSceneLightmapData::Entry, long, 0, mLODIndex);
			NEXTMEM2(lmde, mTextureScale, MeshSceneLightmapData::Entry, vec2, 0, mLightQuality);
			NEXTMEM2(lmde, mTextureOffset, MeshSceneLightmapData::Entry, vec2, 0, mTextureScale);
			NEXTMEM2(lmde, mTexturePage, MeshSceneLightmapData::Entry, long, 0, mTextureOffset);
			ADD(lmde);

			DEFINEDCARRAY2(MeshSceneLightmapData::Entry, lmde);
			DEFINEDCARRAY2(unsigned short, ushort);

			DEFINET2(lmd, MeshSceneLightmapData);
			FIRSTMEM2(lmd, mEntries, MeshSceneLightmapData, DCArray_lmde, 0);
			NEXTMEM2(lmd, mStationaryLightIndices, MeshSceneLightmapData, DCArray_ushort, 0, mEntries);
			NEXTMEM2(lmd, mFlags, MeshSceneLightmapData, flags, 0, mStationaryLightIndices);
			ADD(lmd);

			DEFINEDCARRAY(T3OcclusionMeshBatch);

			DEFINET2(bbox, BoundingBox);
			ADDFLAGS(bbox, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			FIRSTMEM2(bbox, mMin, BoundingBox, vec3, 0);
			NEXTMEM2(bbox, mMax, BoundingBox, vec3, 0, mMin);
			ADD(bbox);

			DEFINET2(mesho, T3OcclusionMeshData);
			SERIALIZER(mesho, T3OcclusionMeshData);
			FIRSTMEM2(mesho, mData, T3OcclusionMeshData, bb, 0);
			NEXTMEM2(mesho, mBoundingBox, T3OcclusionMeshData, bbox, 0, mData);
			NEXTMEM2(mesho, mBoundingSphere, T3OcclusionMeshData, sphere, 0, mBoundingBox);
			NEXTMEM2(mesho, mBatches, T3OcclusionMeshData, DCArray_T3OcclusionMeshBatch, 0, mBoundingSphere);
			NEXTMEM2(mesho, mVertexCount, T3OcclusionMeshData, long, 0, mBatches);
			ADD(mesho);

			DEFINET2(texind, T3MeshTextureIndices);
			SERIALIZER(texind, T3MeshTextureIndices);
			ADD(texind);

			DEFINET2(batch, T3MeshBatch);
			FIRSTMEM2(batch, mBoundingBox, T3MeshBatch, bbox, 0);
			NEXTMEM2(batch, mBoundingSphere, T3MeshBatch, sphere, 0, mBoundingBox);
			NEXTMEM2(batch, mBatchUsage, T3MeshBatch, flags, 0, mBoundingSphere);
			NEXTMEM2(batch, mMinVertIndex, T3MeshBatch, long, 0, mBatchUsage);
			NEXTMEM2(batch, mMaxVertIndex, T3MeshBatch, long, 0, mMinVertIndex);
			NEXTMEM2(batch, mBaseIndex, T3MeshBatch, long, 0, mMaxVertIndex);
			NEXTMEM2(batch, mStartIndex, T3MeshBatch, long, 0, mBaseIndex);
			NEXTMEM2(batch, mNumPrimitives, T3MeshBatch, long, 0, mStartIndex);
			NEXTMEM2(batch, mNumIndices, T3MeshBatch, long, 0, mNumPrimitives);
			NEXTMEM2(batch, mTextureIndices, T3MeshBatch, texind, 0, mNumIndices);
			NEXTMEM2(batch, mMaterialIndex, T3MeshBatch, long, 0, mTextureIndices);
			NEXTMEM2(batch, mBonePaletteIndex, T3MeshBatch, long, 0, mMaterialIndex);
			NEXTMEM2(batch, mLocalTransformIndex, T3MeshBatch, long, 0, mBonePaletteIndex);
			NEXTMEM2(batch, mAdjacencyStartIndex, T3MeshBatch, long, 0, mLocalTransformIndex);
			meta_batch_mBonePaletteIndex.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_batch_mLocalTransformIndex.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_batch_mAdjacencyStartIndex.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_batch_mBaseIndex.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_batch_mNumIndices.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			ADD(batch);

			DEFINEDCARRAY(T3MeshBatch);

			DEFINET2(lod, T3MeshLOD);
			FIRSTMEM(lod, "mBatches[0]", mBatches, T3MeshLOD, DCArray_T3MeshBatch, 0);
			NEXTMEM1(lod, "mBatches[1]", mBatches1, mBatches, T3MeshLOD, DCArray_T3MeshBatch, 0, mBatches);
			meta_lod_mBatches.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_lod_mBatches1.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_lod_mBatches1.mOffset += sizeof(DCArray<T3MeshBatch>);
			NEXTMEM1(lod, "mBatches", mBatchesM, mBatchesM, T3MeshLOD, DCArray_T3MeshBatch, 0, mBatches1);
			meta_lod_mBatchesM.mOffset = offsetof(T3MeshLOD, mBatchesM);
			meta_lod_mBatchesM.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			NEXTMEM2(lod, mVertexStreams, T3MeshLOD, bitsetbase1,0, mBatchesM);
			NEXTMEM2(lod, mBoundingBox, T3MeshLOD, bbox, 0, mVertexStreams);
			NEXTMEM2(lod, mBoundingSphere, T3MeshLOD, sphere, 0, mBoundingBox);
			NEXTMEM2(lod, mFlags, T3MeshLOD, flags, 0, mBoundingSphere);
			NEXTMEM2(lod, mVertexStateIndex, T3MeshLOD, long,0, mFlags);
			NEXTMEM2(lod, mNumPrimitives, T3MeshLOD, long, 0, mVertexStateIndex);
			NEXTMEM2(lod, mNumBatches, T3MeshLOD, long, 0, mNumPrimitives);
			NEXTMEM2(lod, mVertexStart, T3MeshLOD, long, 0, mNumBatches);
			NEXTMEM2(lod, mVertexCount, T3MeshLOD, long, 0, mVertexStart);
			NEXTMEM2(lod, mTextureAtlasWidth, T3MeshLOD, long, 0, mVertexCount);
			NEXTMEM2(lod, mTextureAtlasHeight, T3MeshLOD, long, 0, mTextureAtlasWidth);
			NEXTMEM2(lod, mPixelSize, T3MeshLOD, float, 0, mTextureAtlasHeight);
			NEXTMEM2(lod, mDistance, T3MeshLOD, float, 0, mPixelSize);
			meta_lod_mNumBatches.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_lod_mNumPrimitives.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_lod_mVertexStart.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_lod_mVertexCount.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_lod_mFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_lod_mDistance.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_lod_mTextureAtlasWidth.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_lod_mTextureAtlasHeight.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(lod, mBones, T3MeshLOD, DCArray_Symbol, 0, mDistance);
			meta_lod_mBones.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			ADD(lod);

			DEFINET2(mesht, T3MeshTexture);
			FIRSTMEM2(mesht, mTextureType, T3MeshTexture, long, 0);
			NEXTMEM2(mesht, mhTexture, T3MeshTexture, Handletex, 0, mTextureType);
			NEXTMEM2(mesht, mNameSymbol, T3MeshTexture, symbol, 0, mhTexture);
			NEXTMEM2(mesht, mBoundingBox, T3MeshTexture, bbox, 0, mNameSymbol);
			NEXTMEM2(mesht, mBoundingSphere, T3MeshTexture, sphere, 0, mBoundingBox);
			NEXTMEM2(mesht, mMaxObjAreaPerUVArea, T3MeshTexture, float, 0, mBoundingSphere);
			NEXTMEM2(mesht, mAverageObjAreaPerUVArea, T3MeshTexture, float, 0, mMaxObjAreaPerUVArea);
			ADD(mesht);

			DEFINET2(meshm, T3MeshMaterial);
			FIRSTMEM2(meshm, mhMaterial, T3MeshMaterial, Handlepropset, 0);
			NEXTMEM2(meshm, mBaseMaterialName, T3MeshMaterial, symbol, 0, mhMaterial);
			NEXTMEM2(meshm, mLegacyRenderTextureProperty, T3MeshMaterial, symbol, 0, mBaseMaterialName);
			NEXTMEM2(meshm, mBoundingBox, T3MeshMaterial, bbox, 0, mLegacyRenderTextureProperty);
			NEXTMEM2(meshm, mBoundingSphere, T3MeshMaterial, sphere, 0, mBoundingBox);
			NEXTMEM2(meshm, mFlags, T3MeshMaterial, flags, 0, mBoundingSphere);
			meta_meshm_mLegacyRenderTextureProperty.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			ADD(meshm);

			DEFINET2(over, T3MeshMaterialOverride);
			FIRSTMEM2(over, mhOverrideMaterial, T3MeshMaterialOverride, Handlepropset, 0);
			NEXTMEM2(over, mMaterialIndex, T3MeshMaterialOverride, long, 0, mhOverrideMaterial);
			ADD(over);

			DEFINET2(bone, T3MeshBoneEntry);
			FIRSTMEM2(bone, mBoneName, T3MeshBoneEntry, symbol, 0);
			NEXTMEM2(bone, mBoundingBox, T3MeshBoneEntry, bbox, 0, mBoneName);
			NEXTMEM2(bone, mBoundingSphere, T3MeshBoneEntry, sphere, 0, mBoundingBox);
			NEXTMEM2(bone, mNumVerts, T3MeshBoneEntry,long, 0, mBoundingSphere);
			ADD(bone);

			DEFINET2(tle, T3MeshLocalTransformEntry);
			FIRSTMEM2(tle, mTransform, T3MeshLocalTransformEntry, transform, 0);
			NEXTMEM2(tle, mCameraFacingType, T3MeshLocalTransformEntry, long, 0, mTransform);
			ADD(tle);

			DEFINET2(df, T3MeshEffectPreloadDynamicFeatures);
			FIRSTMEM2(df, mDynamicFeatures, T3MeshEffectPreloadDynamicFeatures, bitsetbase1, 0);
			NEXTMEM2(df, mPriority, T3MeshEffectPreloadDynamicFeatures, long, 0, mDynamicFeatures);
			ADD(df);

			DEFINEDCARRAY(T3MeshEffectPreloadDynamicFeatures);

			DEFINET2(fxe, T3MeshEffectPreloadEntry);
			FIRSTMEM2(fxe, mEffectType, T3MeshEffectPreloadEntry, long, 0);
			NEXTMEM2(fxe, mStaticEffectFeatures, T3MeshEffectPreloadEntry, bitsetbase3, 0, mEffectType);
			NEXTMEM2(fxe, mMaterialCRC, T3MeshEffectPreloadEntry, __int64, 0, mStaticEffectFeatures);
			NEXTMEM2(fxe, mDynamicEffectFeatures, T3MeshEffectPreloadEntry,
				DCArray_T3MeshEffectPreloadDynamicFeatures, 0, mMaterialCRC);
			ADD(fxe);

			DEFINEDCARRAY(T3MeshEffectPreloadEntry);

			DEFINET2(fx, T3MeshEffectPreload);
			FIRSTMEM2(fx, mEffectQuality, T3MeshEffectPreload, long, 0);
			NEXTMEM2(fx, mEntries, T3MeshEffectPreload, DCArray_T3MeshEffectPreloadEntry, 0, mEffectQuality);
			NEXTMEM2(fx, mTotalEffectCount, T3MeshEffectPreload, long, 0, mEntries);
			ADD(fx);

			DEFINET2(ct, T3MaterialChannelType);
			static MetaMemberDescription meta_ct_ctm{};
			meta_ct.mpFirstMember = &meta_ct_ctm;
			meta_ct_ctm.mpName = "mVal";
			meta_ct_ctm.mpNextMember = 0;
			meta_ct_ctm.mFlags = MetaFlag_EnumIntType;
			meta_ct_ctm.mpMemberDesc = &meta_long;
			meta_ct_ctm.mOffset = 0;
			FIRSTENUM2(ct, ctm, "SurfaceNormal", 00, 0x00, 0);
			NEXTENUM2(ct, ctm, "DiffuseColor", 01, 0x01, 0, 00);
			NEXTENUM2(ct, ctm, "SpecularColor", 02, 0x02, 0, 01);
			NEXTENUM2(ct, ctm, "EmissiveColor", 03, 0x03, 0, 02);
			NEXTENUM2(ct, ctm, "VertexNormal", 04, 0x04, 0, 03);
			NEXTENUM2(ct, ctm, "InvertShadow", 05, 0x05, 0, 04);
			NEXTENUM2(ct, ctm, "AlphaLegacy", 06, 0x06, 0, 05);
			NEXTENUM2(ct, ctm, "Gloss", 07, 0x07, 0, 06);
			NEXTENUM2(ct, ctm, "AmbientOcclusion", 08, 0x08, 0, 07);
			NEXTENUM2(ct, ctm, "Glow", 09, 0x09, 0, 08);
			NEXTENUM2(ct, ctm, "OutlineSize", 10, 0x0A, 0, 09);
			NEXTENUM2(ct, ctm, "VertexOffset", 11, 0x0B, 0, 10);
			NEXTENUM2(ct, ctm, "VertexColor0", 12, 0x0C, 0, 11);
			NEXTENUM2(ct, ctm, "VertexColor1", 13, 0x0D, 0, 12);
			NEXTENUM2(ct, ctm, "TexCoord0", 14, 0x0E, 0, 13);
			NEXTENUM2(ct, ctm, "TexCoord1", 15, 0x0F, 0, 14);
			NEXTENUM2(ct, ctm, "TexCoord2", 16, 0x10, 0, 15);
			NEXTENUM2(ct, ctm, "TexCoord3", 17, 0x11, 0, 16);
			NEXTENUM2(ct, ctm, "NPR_Hatching", 18, 0x18, 0, 17);
			NEXTENUM2(ct, ctm, "LineColor", 19, 0x19, 0, 18);
			NEXTENUM2(ct, ctm, "LineVisibility", 20, 0x1A, 0, 19);
			NEXTENUM2(ct, ctm, "LineGenerationStyle", 21, 0x1B, 0, 20);
			NEXTENUM2(ct, ctm, "LineWidth", 22, 0x1C, 0, 21);
			NEXTENUM2(ct, ctm, "Reserved1", 23, 0x1D, 0, 22);
			NEXTENUM2(ct, ctm, "Reserved2", 24, 0x1E, 0, 23);
			NEXTENUM2(ct, ctm, "Reserved3", 25, 0x1F, 0, 24);
			NEXTENUM2(ct, ctm, "Reserved4", 26, 0x20, 0, 25);
			NEXTENUM2(ct, ctm, "DetailColor", 27, 0x21, 0, 26);
			NEXTENUM2(ct, ctm, "DetailAlpha", 28, 0x22, 0, 27);
			NEXTENUM2(ct, ctm, "DiffuseAlbedoColor", 29, 0x23, 0, 28);
			NEXTENUM2(ct, ctm, "FinalColor", 30, 0x24, 0, 29);
			NEXTENUM2(ct, ctm, "LineAlpha", 31, 0x25, 0, 30);
			NEXTENUM2(ct, ctm, "Opacity", 32, 0x26, 0, 31);
			NEXTENUM2(ct, ctm, "OpacityMask", 33, 0x27, 0, 32);
			NEXTENUM2(ct, ctm, "SpecularAlbedoColor", 34, 0x28, 0, 33);
			NEXTENUM2(ct, ctm, "Anisotropy", 35, 0x29, 0, 34);
			NEXTENUM2(ct, ctm, "SmoothSurfaceNormal", 36, 0x2A, 0, 35);
			NEXTENUM2(ct, ctm, "AnisotropyMask", 37, 0x2B, 0, 36);
			NEXTENUM2(ct, ctm, "AnisotropyTangent", 38, 0x2C, 0, 37);
			NEXTENUM2(ct, ctm, "SecondarySpecularAlbedoColor", 39, 0x2D, 0, 38);
 			ADD(ct);

			DEFINET2(bitsetreq, BitSet<enum T3MaterialChannelType SEP 46 SEP 0>);
			SERIALIZER(bitsetreq, BitSet<enum T3MaterialChannelType SEP 46 SEP 0>);
			ADD(bitsetreq);

			DEFINET2(reqs, T3MaterialRequirements);
			FIRSTMEM2(reqs, mPasses, T3MaterialRequirements, bitsetbase1, 0);
			NEXTMEM2(reqs, mChannels, T3MaterialRequirements, bitsetreq, 0, mPasses);
			NEXTMEM2(reqs, mChannels2, T3MaterialRequirements, bitsetbase1, 0, mChannels);
			NEXTMEM2(reqs, mInputs, T3MaterialRequirements, bitsetbase3, 0, mChannels2);
			NEXTMEM2(reqs, mInputs2, T3MaterialRequirements, bitsetbase2, 0, mInputs);
			NEXTMEM2(reqs, mInputs3, T3MaterialRequirements, bitsetbase1, 0, mInputs2);
			meta_reqs_mChannels2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			meta_reqs_mChannels.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_reqs_mInputs2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_reqs_mInputs3.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			meta_reqs_mInputs2.mpName = meta_reqs_mInputs3.mpName = "mInputs";
			meta_reqs_mInputs.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_reqs_mInputs2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			ADD(reqs);

			DEFINET2(skin, T3MeshCPUSkinningData);
			FIRSTMEM2(skin, mPositionFormat, T3MeshCPUSkinningData, long, 0);
			NEXTMEM2(skin, mWeightFormat, T3MeshCPUSkinningData, long, 0, mPositionFormat);
			NEXTMEM2(skin, mNormalFormat, T3MeshCPUSkinningData, long, 0, mWeightFormat);
			NEXTMEM2(skin, mVertexStreams, T3MeshCPUSkinningData, bitsetbase1, 0, mNormalFormat);
			NEXTMEM2(skin, mNormalCount, T3MeshCPUSkinningData, long, 0, mVertexStreams);
			NEXTMEM2(skin, mWeightOffset, T3MeshCPUSkinningData, long, 0, mNormalCount);
			NEXTMEM2(skin, mVertexSize, T3MeshCPUSkinningData, long, 0, mWeightOffset);
			NEXTMEM2(skin, mWeightSize, T3MeshCPUSkinningData, long, 0, mVertexSize);
			NEXTMEM2(skin, mData, T3MeshCPUSkinningData, bb, 0, mWeightSize);
			meta_skin_mWeightOffset.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_skin_mWeightSize.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			ADD(skin);

			DEFINET2(tct, T3MeshTexCoordTransform);
			FIRSTMEM2(tct, mScale, T3MeshTexCoordTransform, vec2, 0);
			NEXTMEM2(tct, mOffset, T3MeshTexCoordTransform, vec2, 0, mScale);
			ADD(tct);

			DEFINET2(lmtype, EnumRenderLightmapUVGenerationType);
			ADDFLAGS(lmtype , MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			FIRSTMEM2(lmtype, mVal, EnumRenderLightmapUVGenerationType, long, 0x40);
			FIRSTENUM2(lmtype, mVal, "eRenderLightmapUVGeneration_Default", a, 0, 0);
			NEXTENUM2(lmtype, mVal, "eRenderLightmapUVGeneration_Auto", b, 1, 0, a);
			NEXTENUM2(lmtype, mVal, "eRenderLightmapUVGeneration_UV0", c, 2, 0, b);
			NEXTENUM2(lmtype, mVal, "eRenderLightmapUVGeneration_UV1", d, 3, 0, c);
			NEXTENUM2(lmtype, mVal, "eRenderLightmapUVGeneration_UV2", e, 4, 0, d);
			NEXTENUM2(lmtype, mVal, "eRenderLightmapUVGeneration_UV3", f, 5, 0, e);
			NEXTMEM1(lmtype, "Baseclass_EnumBase", ALAIS, mVal, EnumRenderLightmapUVGenerationType, enumbase, 0, mVal);
			ADD(lmtype);

			DEFINET2(mt, EnumT3MaterialLightModelType);
			FIRSTMEM2(mt, mVal, EnumT3MaterialLightModelType, long, 0x40);
			FIRSTENUM2(mt, mVal, "eMaterialLightModel_Default", a, -1, 0);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Unlit", b, 0, 0, a);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_VertexDiffuse", c, 1, 0, b);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Diffuse", d, 2, 0, c);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Phong", e, 3, 0, d);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_PhongGloss", f, 4, 0, e);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Toon", g, 5, 0, f);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_NPR_Depreceated", h, 6, 0, g);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_PBS", i, 7, 0, h);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Cloth", j, 8, 0, i);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Hair", k, 9, 0, j);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_Skin", l, 10, 0, k);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_HybridCloth", m, 11, 0, l);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_DiffuseNoDirection", n, 12, 0, m);
			NEXTENUM2(mt, mVal, "eMaterialLightModel_HybridHair", o, 13, 0, n);
			NEXTMEM1(mt, "Baseclass_EnumBase", ALAIS, mVal, EnumT3MaterialLightModelType, enumbase, 0, mVal);
			ADD(mt);

			DEFINET2(state, T3GFXVertexState);
			SERIALIZER(state, T3GFXVertexState);
			FIRSTMEM2(state, mVertexCountPerInstance, T3GFXVertexState, long, 0);
			NEXTMEM2(state, mIndexBufferCount, T3GFXVertexState, long, 0, mVertexCountPerInstance);
			NEXTMEM2(state, mVertexBufferCount, T3GFXVertexState, long, 0, mIndexBufferCount);
			NEXTMEM2(state, mAttributeCount, T3GFXVertexState, long, 0, mVertexBufferCount);
			meta_state_mIndexBufferCount.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			ADD(state);

			DEFINET2(params, GFXPlatformAttributeParams);
			FIRSTMEM2(params, mAttribute, GFXPlatformAttributeParams, long, 0);
			NEXTMEM2(params, mFormat, GFXPlatformAttributeParams, long, 0, mAttribute);
			NEXTMEM2(params, mAttributeIndex, GFXPlatformAttributeParams, long, 0, mFormat);
			NEXTMEM2(params, mBufferIndex, GFXPlatformAttributeParams, long, 0, mAttributeIndex);
			NEXTMEM2(params, mBufferOffset, GFXPlatformAttributeParams, long, 0, mBufferIndex);
			ADD(params);

			DEFINET2(buffer, T3GFXBuffer);
			SERIALIZER(buffer, T3GFXBuffer);
			FIRSTMEM2(buffer, mResourceUsage, T3GFXBuffer, long, 0);
			NEXTMEM2(buffer, mBufferFormat, T3GFXBuffer, long, 0, mResourceUsage);
			NEXTMEM2(buffer, mBufferUsage, T3GFXBuffer, long, 0, mBufferFormat);
			NEXTMEM2(buffer, mCount, T3GFXBuffer, long, 0, mBufferUsage);
			NEXTMEM2(buffer, mStride, T3GFXBuffer, long, 0, mCount);
			KEYOVERRIDE(buffer, mResourceUsage, "Resource Usage");
			ADD(buffer);

			DEFINEDCARRAY(T3MeshLOD);
			DEFINEDCARRAY(T3MeshMaterial);
			DEFINEDCARRAY(T3MeshMaterialOverride);
			DEFINEDCARRAY(T3MeshTexture);
			DEFINEDCARRAY(T3MeshBoneEntry);
			DEFINEDCARRAY(T3MeshLocalTransformEntry);
			DEFINEDCARRAY(T3MeshEffectPreload);

			DEFINET2(data, T3MeshData);
			SERIALIZER(data, T3MeshData);
			FIRSTMEM2(data, mLODs, T3MeshData, DCArray_T3MeshLOD, 0);
			NEXTMEM2(data, mTextures, T3MeshData, DCArray_T3MeshTexture, 0, mLODs);
			NEXTMEM2(data, mMaterials, T3MeshData, DCArray_T3MeshMaterial, 0, mTextures);
			NEXTMEM2(data, mMaterialOverrides, T3MeshData, DCArray_T3MeshMaterialOverride, 0, mMaterials);
			NEXTMEM2(data, mBones, T3MeshData, DCArray_T3MeshBoneEntry, 0, mMaterialOverrides);
			NEXTMEM2(data, mLocalTransforms, T3MeshData, DCArray_T3MeshLocalTransformEntry, 0, mBones);
			NEXTMEM2(data, mMaterialRequirements, T3MeshData, reqs, 0, mLocalTransforms);
			NEXTMEM2(data, mVertexStreams, T3MeshData, bitsetbase1, 0, mMaterialRequirements);
			NEXTMEM2(data, mBoundingBox, T3MeshData, bbox, 0, mVertexStreams);
			NEXTMEM2(data, mBoundingSphere, T3MeshData, sphere, 0, mBoundingBox);
			NEXTMEM2(data, mEndianType, T3MeshData, long, 0, mBoundingSphere);
			NEXTMEM2(data, mPositionScale, T3MeshData, vec3, 0, mEndianType);
			NEXTMEM2(data, mPositionWScale, T3MeshData, vec3, 0, mPositionScale);
			NEXTMEM2(data, mPositionOffset, T3MeshData, vec3, 0, mPositionWScale);
			meta_data_mPositionScale.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_data_mPositionWScale.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_data_mPositionOffset.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			NEXTMEM2(data, mLightmapTexelAreaPerSurfaceArea, T3MeshData, float, 0, mPositionOffset);
			NEXTMEM2(data, mPropertyKeyBase, T3MeshData, symbol, 0, mLightmapTexelAreaPerSurfaceArea);
			NEXTMEM2(data, mVertexCount, T3MeshData, long, 0, mPropertyKeyBase);
			NEXTMEM2(data, mFlags, T3MeshData, flags, 0, mVertexCount);
			NEXTMEM2(data, mMeshPreload, T3MeshData, DCArray_T3MeshEffectPreload, 0, mFlags);
			meta_data_mMeshPreload.mGameIndexVersionRange.min =
				TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_data_mLightmapTexelAreaPerSurfaceArea.mGameIndexVersionRange.min =
				TelltaleToolLib_GetGameKeyIndex("BATMAN");
			ADD(data);

			DEFINEDCARRAY(HandleBase);

			DEFINET2(dbgt, EnumMeshDebugRenderType);
			ADDFLAGS(dbgt, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			FIRSTMEM2(dbgt, mVal, EnumMeshDebugRenderType, long, 0x40);
			FIRSTENUM2(dbgt, mVal, "eMeshRender_Solid", a, 1, 0);
			NEXTENUM2(dbgt, mVal, "eMeshRender_Wireframe", b, 2, 0, a);
			NEXTMEM1(dbgt, "Baseclass_EnumBase", ALAIS, mVal, EnumMeshDebugRenderType, enumbase, 0, mVal);
			ADD(dbgt);

			DEFINET2(mesh, D3DMesh);
			SERIALIZER(mesh, D3DMesh);
			EXT(mesh, d3dmesh);
			ADDFLAGS(mesh, MetaFlag::MetaFlag_RenderResource);
			FIRSTMEM2(mesh, mName, D3DMesh, string, 0);
			NEXTMEM2(mesh, mVersion, D3DMesh, long, 0, mName);
			NEXTMEM2(mesh, mMeshData, D3DMesh, data, 1, mVersion);
			NEXTMEM2(mesh, mInternalResources, D3DMesh, DCArray_HandleBase, 1, mMeshData);
			NEXTMEM2(mesh, mToolProps, D3DMesh, tp,0, mInternalResources);
			NEXTMEM2(mesh, mLightmapUVGenerationType, D3DMesh, lmtype, 0, mToolProps);
			NEXTMEM2(mesh, mLightmapTexelAreaPerSurfaceArea, D3DMesh, float, 0, mLightmapUVGenerationType);
			NEXTMEM2(mesh, mLightmapGlobalScale, D3DMesh, float, 0, mLightmapTexelAreaPerSurfaceArea);
			NEXTMEM2(mesh, mLightmapTexCoordVersion, D3DMesh, long, 0, mLightmapGlobalScale);
			NEXTMEM2(mesh, mLightmapTextureWidth, D3DMesh, long, 0, mLightmapTexCoordVersion);
			NEXTMEM2(mesh, mLightmapTextureHeight, D3DMesh, long, 0, mLightmapTextureWidth);
			meta_mesh_mLightmapGlobalScale.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mesh_mLightmapTexCoordVersion.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mesh_mLightmapTextureWidth.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mesh_mLightmapUVGenerationType.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mesh_mLightmapTextureHeight.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mesh_mLightmapTexelAreaPerSurfaceArea.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mesh_mLightmapTextureWidth.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			meta_mesh_mLightmapUVGenerationType.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			meta_mesh_mLightmapTextureHeight.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			meta_mesh_mLightmapTexelAreaPerSurfaceArea.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			NEXTMEM2(mesh, mLODParamCRC, D3DMesh, __int64, 0, mLightmapTextureHeight);
			meta_mesh_mLODParamCRC.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			ADD(mesh);

			DEFINET2(runtp, T3MaterialRuntimeProperty);
			FIRSTMEM2(runtp, mName, T3MaterialRuntimeProperty, symbol, 0);
			NEXTMEM2(runtp, mRuntimeName, T3MaterialRuntimeProperty, symbol, 0, mName);
			ADD(runtp);

			DEFINEDCARRAY(T3MaterialRuntimeProperty);

			DEFINET2(matp, T3MaterialParameter);
			FIRSTMEM2(matp, mName, T3MaterialParameter, symbol, 0);
			NEXTMEM2(matp, mPropertyType, T3MaterialParameter, long, 0, mName);
			NEXTMEM2(matp, mValueType, T3MaterialParameter, long, 0, mPropertyType);
			NEXTMEM2(matp, mFlags, T3MaterialParameter, long, 0, mValueType);
			FIRSTENUM2(matp, mValueType, "Float1", f1, 0, 0);
			NEXTENUM2(matp, mValueType, "Float2", f2, 1, 0, f1);
			NEXTENUM2(matp, mValueType, "Float3", f3, 2, 0, f2);
			NEXTENUM2(matp, mValueType, "Float4", f4, 3, 0, f3);
			NEXTENUM2(matp, mValueType, "Channels", f5, 4, 0, f4);
			NEXTENUM2(matp, mValueType, "None", f6, 0xFFFFFFFF, 0, f5);
			NEXTMEM2(matp, mScalarOffset, T3MaterialParameter, long, 0, mFlags);
			NEXTMEM1(matp, "mScalarOffset[0]", OFF0, mScalarOffset, T3MaterialParameter, long, 0, mScalarOffset);
			NEXTMEM1(matp, "mScalarOffset[1]", OFF1, mScalarOffset, T3MaterialParameter, long, 0,OFF0);
			NEXTMEM1(matp, "mScalarOffset[2]", OFF2, mScalarOffset, T3MaterialParameter, long, 0, OFF1);
			NEXTMEM1(matp, "mScalarOffset[3]", OFF3, mScalarOffset, T3MaterialParameter, long, 0, OFF2);
			NEXTMEM2(matp, mPreShaderScalarOffset, T3MaterialParameter, long, 0, OFF3);
			meta_matp_OFF1.mOffset += 4;
			meta_matp_OFF2.mOffset += 8;
			meta_matp_OFF3.mOffset += 12;
			meta_matp_OFF0.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matp_OFF1.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matp_OFF2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matp_mScalarOffset.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_matp_OFF3.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			NEXTMEM2(matp, mNestedMaterialIndex, T3MaterialParameter, long, 0, mPreShaderScalarOffset);
			ADD(matp);
			meta_matp_OFF2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_matp_OFF3.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");

			DEFINET2(mapt, T3MaterialTransform2D);
			FIRSTMEM2(mapt, mParameterPrefix, T3MaterialTransform2D, symbol, 0);
			NEXTMEM2(mapt, mFlags, T3MaterialTransform2D, flags, 0, mParameterPrefix);
			NEXTMEM2(mapt, mScalarOffset0, T3MaterialTransform2D, long, 0, mFlags);
			NEXTMEM1(mapt, "mScalarOffset0[0]", OFF0, mScalarOffset0, T3MaterialTransform2D, long, 0, mScalarOffset0);
			NEXTMEM1(mapt, "mScalarOffset0[1]", OFF1, mScalarOffset0, T3MaterialTransform2D, long, 0, OFF0);
			NEXTMEM1(mapt, "mScalarOffset0[2]", OFF2, mScalarOffset0, T3MaterialTransform2D, long, 0, OFF1);
			NEXTMEM1(mapt, "mScalarOffset0[3]", OFF3, mScalarOffset0, T3MaterialTransform2D, long, 0, OFF2);
			meta_mapt_OFF0.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OFF1.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OFF2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OFF3.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OFF1.mOffset += 4;
			meta_mapt_OFF2.mOffset += 8;
			meta_mapt_OFF3.mOffset += 12;
			NEXTMEM2(mapt, mScalarOffset1, T3MaterialTransform2D, long, 0, OFF3);
			NEXTMEM1(mapt, "mScalarOffset1[0]", OOFF0, mScalarOffset1, T3MaterialTransform2D, long, 0, mScalarOffset1);
			NEXTMEM1(mapt, "mScalarOffset1[1]", OOFF1, mScalarOffset1, T3MaterialTransform2D, long, 0, OOFF0);
			NEXTMEM1(mapt, "mScalarOffset1[2]", OOFF2, mScalarOffset1, T3MaterialTransform2D, long, 0, OOFF1);
			NEXTMEM1(mapt, "mScalarOffset1[3]", OOFF3, mScalarOffset1, T3MaterialTransform2D, long, 0, OOFF2);
			meta_mapt_mScalarOffset0.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_mapt_mScalarOffset1.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_mapt_mFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_mapt_OOFF0.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OOFF1.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OOFF2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OOFF3.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_mapt_OOFF1.mOffset += 4;
			meta_mapt_OOFF2.mOffset += 8;
			meta_mapt_OOFF3.mOffset += 12;
			meta_mapt_OOFF2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mapt_OOFF3.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mapt_OFF2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_mapt_OFF3.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");

			NEXTMEM2(mapt, mPreShaderScalarOffset0, T3MaterialTransform2D, long,0, OOFF3);
			NEXTMEM2(mapt, mPreShaderScalarOffset1, T3MaterialTransform2D, long, 0, mPreShaderScalarOffset0);
			NEXTMEM2(mapt, mNestedMaterialIndex, T3MaterialTransform2D, long, 0, mPreShaderScalarOffset1);
			ADD(mapt);

			DEFINET2(matt, T3MaterialTextureParam);
			FIRSTMEM2(matt, mParamType, T3MaterialTextureParam, long, 0);
			NEXTMEM2(matt, mValueType, T3MaterialTextureParam, long, 0, mParamType);
			FIRSTENUM2(matt, mValueType, "Float1", f1, 0, 0);
			NEXTENUM2(matt, mValueType, "Float2", f2, 1, 0, f1);
			NEXTENUM2(matt, mValueType, "Float3", f3, 2, 0, f2);
			NEXTENUM2(matt, mValueType, "Float4", f4, 3, 0, f3);
			NEXTENUM2(matt, mValueType, "Channels", f5, 4, 0, f4);
			NEXTENUM2(matt, mValueType, "None", f6, 0xFFFFFFFF, 0, f5);
			NEXTMEM2(matt, mFlags, T3MaterialTextureParam, long, 0, mValueType);
			NEXTMEM2(matt, mScalarOffset, T3MaterialTextureParam, long, 0, mFlags);
			ADD(matt);

			DEFINET2(mats, T3MaterialStaticParameter);
			FIRSTMEM2(mats, mName, T3MaterialStaticParameter, symbol, 0);
			NEXTMEM2(mats, mNestedMaterialIndex, T3MaterialStaticParameter, long, 0, mName);
			ADD(mats);

			DEFINET2(matsh, T3MaterialPreShader);
			FIRSTMEM2(matsh, mValueType, T3MaterialPreShader, long, 0);
			FIRSTENUM2(matsh, mValueType, "Float1", f1, 0, 0);
			NEXTENUM2(matsh, mValueType, "Float2", f2, 1, 0, f1);
			NEXTENUM2(matsh, mValueType, "Float3", f3, 2, 0, f2);
			NEXTENUM2(matsh, mValueType, "Float4", f4, 3, 0, f3);
			NEXTENUM2(matsh, mValueType, "Channels", f5, 4, 0, f4);
			NEXTENUM2(matsh, mValueType, "None", f6, 0xFFFFFFFF, 0, f5);
			NEXTMEM2(matsh, mFlags, T3MaterialPreShader, long, 0, mValueType);
			meta_matsh_mFlags.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			NEXTMEM2(matsh, mPreShaderOffset, T3MaterialPreShader, long, 0, mFlags);
			NEXTMEM2(matsh, mScalarParameterOffset, T3MaterialPreShader, long, 0, mPreShaderOffset);
			NEXTMEM1(matsh, "mScalarParameterOffset[0]", OFF0, mScalarParameterOffset, T3MaterialPreShader, long, 0, mScalarParameterOffset);
			NEXTMEM1(matsh, "mScalarParameterOffset[1]", OFF1, mScalarParameterOffset, T3MaterialPreShader, long, 0, OFF0);
			NEXTMEM1(matsh, "mScalarParameterOffset[2]", OFF2, mScalarParameterOffset, T3MaterialPreShader, long, 0, OFF1);
			NEXTMEM1(matsh, "mScalarParameterOffset[3]", OFF3, mScalarParameterOffset, T3MaterialPreShader, long, 0, OFF2);
			meta_matsh_mScalarParameterOffset.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("MARVEL");
			meta_matsh_OFF0.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matsh_OFF1.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matsh_OFF2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matsh_OFF3.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_matsh_OFF2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_matsh_OFF3.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");

			meta_matsh_OFF1.mOffset += 4;
			meta_matsh_OFF2.mOffset += 8;
			meta_matsh_OFF3.mOffset += 12;
			ADD(matsh);

			DEFINET2(matpass, T3MaterialPassData);
			FIRSTMEM2(matpass, mPassType, T3MaterialPassData, long, 0);
			FIRSTENUM2(matpass, mPassType, "Main", a, 0, 0);
			NEXTENUM2(matpass, mPassType, "PreZ", q, 1, 0, a);
			NEXTENUM2(matpass, mPassType, "GBuffer", w, 2, 0, q);
			NEXTENUM2(matpass, mPassType, "GBufferLines", e, 3, 0, w);
			NEXTENUM2(matpass, mPassType, "GBufferOutline", r, 4, 0, e);
			NEXTENUM2(matpass, mPassType, "Glow", t, 5, 0, r);
			NEXTENUM2(matpass, mPassType, "ShadowMap", y, 6, 0, t);
			NEXTENUM2(matpass, mPassType, "Outline", u, 7, 0, y);
			NEXTENUM2(matpass, mPassType, "LinearDepth", i, 8, 0, u);
			NEXTENUM2(matpass, mPassType, "LinesCS_Generate", o, 9, 0, i);
			NEXTENUM2(matpass, mPassType, "LinesCS_Rasterize", p, 10, 0, o);
			NEXTENUM2(matpass, mPassType, "LinesCS_ForceLinearDepth", s, 11, 0, p);
			NEXTENUM2(matpass, mPassType, "FirstParticle", d, 12, 0, s);
			NEXTENUM2(matpass, mPassType, "ParticleGlow", f, 13, 0, d);
			NEXTENUM2(matpass, mPassType, "ParticleLinearDepth", g, 14, 0, f);
			NEXTENUM2(matpass, mPassType, "DecalGBuffer", h, 15, 0, g);
			NEXTENUM2(matpass, mPassType, "DecalEmissive", j, 16, 0, h);
			NEXTENUM2(matpass, mPassType, "DecalGlow", k, 17, 0, j);
			NEXTENUM2(matpass, mPassType, "Post", l, 18, 0, k);
			NEXTENUM2(matpass, mPassType, "MayaShader", z, 19, 0, l);
			NEXTENUM2(matpass, mPassType, "LightBake", x, 20, 0, z);
			NEXTMEM2(matpass, mBlendMode, T3MaterialPassData, long, 0, mPassType);
			FIRSTENUM2(matpass, mBlendMode, "eBlendModeAlphaInvAlphaTest", M_8K69rklPGt, 10, 0);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeAlphaSubtract", M_fUR2IZ2mB1, 9, 0, M_8K69rklPGt);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeAlphaAdd", M_S7NLt2nQfj, 8, 0, M_fUR2IZ2mB1);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeInvMultiply", M_3SL3XByImc, 7, 0, M_S7NLt2nQfj);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeMultiply", M_iQTqVaAUnJ, 6, 0, M_3SL3XByImc);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeAdd", M_vUmHVQGsTm, 5, 0, M_iQTqVaAUnJ);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeInvAlphaTest", M_vHeuSEBMSP, 4, 0, M_vUmHVQGsTm);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeAlphaTest", M_h2Q6dQAjy4, 3, 0, M_vHeuSEBMSP);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeAlphaAlphaTest", M_KjsfSXF4YP, 2, 0, M_h2Q6dQAjy4);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeAlpha", M_8arF3SLLny, 1, 0, M_KjsfSXF4YP);
			NEXTENUM2(matpass, mBlendMode, "eBlendModeNormal", M_30KKziHtRb, 0, 0, M_8arF3SLLny);
			NEXTMEM2(matpass, mMaterialCrc, T3MaterialPassData, u64, Internal_MetaFlag_ShowAsHex, mBlendMode);
			ADD(matpass);

			DEFINET2(matn, T3MaterialNestedMaterial);
			FIRSTMEM2(matn, mhMaterial, T3MaterialNestedMaterial, Handlepropset, 0);
			ADD(matn);

			DEFINET2(mattex, T3MaterialTexture);
			FIRSTMEM2(mattex, mName, T3MaterialTexture, symbol, 0);
			NEXTMEM2(mattex, mTextureName, T3MaterialTexture, symbol, 0, mName);
			NEXTMEM2(mattex, mTextureNameWithoutExtension, T3MaterialTexture, symbol, 0, mTextureName);
			NEXTMEM2(mattex, mLayout, T3MaterialTexture, long, 0, mTextureNameWithoutExtension);
			NEXTMEM2(mattex, mPropertyType, T3MaterialTexture, long, 0, mLayout);
			meta_mattex_mPropertyType.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(mattex, mTextureTypes, T3MaterialTexture, bitsetbase1, 0, mPropertyType);
			NEXTMEM2(mattex, mFirstParamIndex, T3MaterialTexture, long, 0, mTextureTypes);
			NEXTMEM2(mattex, mParamCount, T3MaterialTexture, long, 0, mFirstParamIndex);
			NEXTMEM2(mattex, mTextureIndex, T3MaterialTexture, long, 0, mParamCount);
			NEXTMEM2(mattex, mNestedMaterialIndex, T3MaterialTexture, long, 0,mTextureIndex);
			ADD(mattex);

			DEFINET2(pcp, T3MaterialEnlightenPrecomputeParams);
			FIRSTMEM2(pcp, mIndirectReflectivity, T3MaterialEnlightenPrecomputeParams, float, 0);
			NEXTMEM2(pcp, mIndirectTransparency, T3MaterialEnlightenPrecomputeParams, float, 0, mIndirectReflectivity);
			ADD(pcp);

			DEFINEDCARRAY(T3MaterialParameter);
			DEFINEDCARRAY(T3MaterialTexture);
			DEFINEDCARRAY(T3MaterialTransform2D);
			DEFINEDCARRAY(T3MaterialNestedMaterial);
			DEFINEDCARRAY(T3MaterialPreShader);
			DEFINEDCARRAY(T3MaterialStaticParameter);
			DEFINEDCARRAY(T3MaterialTextureParam);
			DEFINEDCARRAY(T3MaterialPassData);

			DEFINET2(com, T3MaterialCompiledData);
			FIRSTMEM2(com, mParameters, T3MaterialCompiledData, DCArray_T3MaterialParameter, 0);
			NEXTMEM2(com, mTextures, T3MaterialCompiledData, DCArray_T3MaterialTexture, 0, mParameters);
			NEXTMEM2(com, mTransforms, T3MaterialCompiledData, DCArray_T3MaterialTransform2D, 0, mTextures);
			NEXTMEM2(com, mNestedMaterials, T3MaterialCompiledData, DCArray_T3MaterialNestedMaterial, 0, mTransforms);
			NEXTMEM2(com, mPreShaders, T3MaterialCompiledData, DCArray_T3MaterialPreShader, 0, mNestedMaterials);
			NEXTMEM2(com, mStaticParameters, T3MaterialCompiledData, DCArray_T3MaterialStaticParameter, 0, mPreShaders);
			NEXTMEM2(com, mTextureParams, T3MaterialCompiledData, DCArray_T3MaterialTextureParam, 0, mStaticParameters);
			NEXTMEM2(com, mPasses, T3MaterialCompiledData, DCArray_T3MaterialPassData, 0, mTextureParams);
			NEXTMEM2(com, mMaterialQuality, T3MaterialCompiledData, long, 0, mPasses);
			meta_com_mMaterialQuality.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(com, mMaterialBlendModes, T3MaterialCompiledData, bitsetbase1, 0, mMaterialQuality);
			NEXTMEM2(com, mMaterialPasses, T3MaterialCompiledData, bitsetbase1, 0, mMaterialBlendModes);
			NEXTMEM2(com, mMaterialChannels, T3MaterialCompiledData, bitsetreq, 0, mMaterialPasses);
			NEXTMEM1(com, "mMaterialChannels", c2,mMaterialChannels2, T3MaterialCompiledData, bitsetbase1, 0, mMaterialChannels);
			NEXTMEM2(com, mShaderInputs, T3MaterialCompiledData, bitsetbase3, 0, c2);
			NEXTMEM2(com, mShaderInputs2, T3MaterialCompiledData, bitsetbase2, 0, mShaderInputs);
			NEXTMEM2(com, mShaderInputs3, T3MaterialCompiledData, bitsetbase1, 0, mShaderInputs2);
			meta_com_mShaderInputs2.mpName = "mShaderInputs";
			NEXTMEM2(com, mSceneTextures, T3MaterialCompiledData, bitsetbase1, 0, mShaderInputs3);
			meta_com_mShaderInputs3.mpName = "mShaderInputs";
			meta_com_c2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			meta_com_mMaterialChannels.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_com_mSceneTextures.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_com_mShaderInputs.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			meta_com_mShaderInputs2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("BATMAN2");
			meta_com_mShaderInputs2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_com_mShaderInputs3.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("MICHONNE");
			NEXTMEM2(com, mOptionalPropertyTypes, T3MaterialCompiledData, bitsetbase1, 0, mSceneTextures);
			NEXTMEM2(com, mPreShaderBuffer, T3MaterialCompiledData, bb, 0, mOptionalPropertyTypes);
			NEXTMEM2(com, mFlags, T3MaterialCompiledData, flags, 0, mPreShaderBuffer);
			NEXTMEM(com, "mParameterBufferScalarSize[0]", mParameterBufferScalarSize,
				T3MaterialCompiledData, long, 0, mFlags);
			NEXTMEM1(com, "mParameterBufferScalarSize[1]", ALAIS,mParameterBufferScalarSize,
				T3MaterialCompiledData, long, 0, mParameterBufferScalarSize);
			NEXTMEM1(com, "mParameterBufferScalarSize[2]", ALAIS1, mParameterBufferScalarSize,
				T3MaterialCompiledData, long, 0, ALAIS);
			NEXTMEM1(com, "mParameterBufferScalarSize[3]", ALAIS2, mParameterBufferScalarSize,
				T3MaterialCompiledData, long, 0, ALAIS1);
			meta_com_ALAIS.mOffset = offsetof(T3MaterialCompiledData, mParameterBufferScalarSize) + 4;
			meta_com_ALAIS1.mOffset = offsetof(T3MaterialCompiledData, mParameterBufferScalarSize) + 8;
			meta_com_ALAIS2.mOffset = offsetof(T3MaterialCompiledData, mParameterBufferScalarSize) + 12;
			meta_com_ALAIS1.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_com_ALAIS2.mGameIndexVersionRange.max = TelltaleToolLib_GetGameKeyIndex("WD3");
			meta_com_ALAIS1.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			meta_com_ALAIS2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("BATMAN");
			NEXTMEM2(com, mPreShaderParameterBufferScalarSize, T3MaterialCompiledData, long, 0, ALAIS2);
			ADD(com);

			DEFINEDCARRAY(T3MaterialCompiledData);

			DEFINET2(lightt, LightType);
			ADDFLAGS(lightt, MetaFlag::MetaFlag_MetaSerializeBlockingDisabled);
			FIRSTMEM2(lightt, mLightType, LightType, long, 0x40);
			FIRSTENUM2(lightt, mLightType, "Directional", a, 0, 0);
			NEXTENUM2(lightt, mLightType, "Point Light", b, 1, 0, a);
			NEXTENUM2(lightt, mLightType, "Local Ambient Light", b1, 2, 0, b);
			NEXTENUM2(lightt, mLightType, "Directional Shadows", b2, 3, 0, b1);
			NEXTENUM2(lightt, mLightType, "Point shadows", b3, 4, 0, b2);
			NEXTENUM2(lightt, mLightType, "Gobo Shadows", b4, 5, 0, b3);
			NEXTENUM2(lightt, mLightType, "Gobo Color", b5, 6, 0, b4);
			NEXTENUM2(lightt, mLightType, "Point Light Simple", b6, 7, 0, b5);
			NEXTENUM2(lightt, mLightType, "Local Ambient Falloff", b7, 8, 0, b6);
			NEXTENUM2(lightt, mLightType, "Directional scene shadows", b8, 9, 0, b7);
			NEXTENUM2(lightt, mLightType, "Lightmap Specular", b9, 10, 0, b8);
			ADD(lightt);

			DEFINET2(matdata, T3MaterialData);
			SERIALIZER(matdata, T3MaterialData);
			ADDFLAGS(matdata, 0x20);
			FIRSTMEM2(matdata, mMaterialName, T3MaterialData, symbol, 0);
			NEXTMEM2(matdata, mRuntimePropertiesName, T3MaterialData, symbol, 0, mMaterialName);
			NEXTMEM2(matdata, mLegacyRenderTextureProperty, T3MaterialData, symbol, 0, mRuntimePropertiesName);
			NEXTMEM2(matdata, mLegacyBlendModeRuntimeProperty, T3MaterialData, symbol, 0, mLegacyRenderTextureProperty);
			NEXTMEM2(matdata, mDomain, T3MaterialData, long, 0, mLegacyBlendModeRuntimeProperty);
			meta_matdata_mDomain.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			NEXTMEM2(matdata, mRuntimeProperties, T3MaterialData, DCArray_T3MaterialRuntimeProperty, 0, mDomain);
			NEXTMEM2(matdata, mFlags, T3MaterialData, flags, 0, mRuntimeProperties);
			NEXTMEM2(matdata, mRuntimeFlags, T3MaterialData, flags, 1, mFlags);
			NEXTMEM2(matdata, mVersion, T3MaterialData, long, 0, mRuntimeFlags);
			NEXTMEM2(matdata, mCompiledData2, T3MaterialData, DCArray_T3MaterialCompiledData, 0, mVersion);
			meta_matdata_mCompiledData2.mGameIndexVersionRange.min = TelltaleToolLib_GetGameKeyIndex("WD4");
			ADD(matdata);

			DEFINET2(ldata, LocationInfo);
			FIRSTMEM2(ldata, mAttachedAgent, LocationInfo, string, 0);
			NEXTMEM2(ldata, mAttachedNode, LocationInfo, symbol, 0, mAttachedAgent);
			NEXTMEM2(ldata, mInitialLocalTransform, LocationInfo, transform, 0, mAttachedNode);
			ADD(ldata);

			DEFINEHANDLE(mesh, D3DMesh);

			DEFINET2(vtalign, EnumVTextAlignmentType);
			ADDFLAGS(vtalign, MetaFlag_NoPanelCaption | MetaFlag_EnumWrapperClass);
			FIRSTMEM2(vtalign, mVal, EnumVTextAlignmentType, long, MetaFlag_EnumIntType);
			FIRSTENUM2(vtalign, mVal, "None", a, 0, 0);
			NEXTENUM2(vtalign, mVal, "Top", b, 1, 0, a);
			NEXTENUM2(vtalign, mVal, "Middle", c, 2, 0, b);
			NEXTENUM2(vtalign, mVal, "Bottom", d, 3, 0, c);
			ADD(vtalign);

			DEFINET2(htalign, EnumHTextAlignmentType);
			ADDFLAGS(htalign, MetaFlag_NoPanelCaption | MetaFlag_EnumWrapperClass);
			FIRSTMEM2(htalign, mVal, EnumHTextAlignmentType, long, MetaFlag_EnumIntType);
			FIRSTENUM2(htalign, mVal, "None", a, 0, 0);
			NEXTENUM2(htalign, mVal, "Left Justified", b, 1, 0, a);
			NEXTENUM2(htalign, mVal, "Centered", c, 2, 0, b);
			NEXTENUM2(htalign, mVal, "Right Justified", d, 3, 0, c);
			ADD(htalign);

			DEFINET2(htttalign, EnumTonemapType);
			ADDFLAGS(htttalign, MetaFlag_NoPanelCaption | MetaFlag_EnumWrapperClass);
			FIRSTMEM2(htttalign, mVal, EnumTonemapType, long, MetaFlag_EnumIntType);
			FIRSTENUM2(htttalign, mVal, "eTonemapType_Default", a, 1, 0);
			NEXTENUM2(htttalign, mVal, "eTonemapType_Filmic", b, 2, 0, a);
			ADD(htttalign);

			DEFINET2(httt1align, CameraFacingTypes);
			ADDFLAGS(httt1align, MetaFlag_NoPanelCaption | MetaFlag_EnumWrapperClass);
			FIRSTMEM2(httt1align, mCameraFacingType, CameraFacingTypes, long, MetaFlag_EnumIntType);
			FIRSTENUM2(httt1align, mCameraFacingType, "eFacing", a, 0, 0);
			NEXTENUM2(httt1align, mCameraFacingType, "eFacingY", b, 1, 0, a);
			NEXTENUM2(httt1align, mCameraFacingType, "eFacingLocalY", c, 2, 0, b);
			ADD(httt1align);

			DEFINET2(ht1ttalign, EnumT3LightEnvType);
			ADDFLAGS(ht1ttalign, MetaFlag_NoPanelCaption | MetaFlag_EnumWrapperClass);
			FIRSTMEM2(ht1ttalign, mVal, EnumTonemapType, long, MetaFlag_EnumIntType);
			FIRSTENUM2(ht1ttalign, mVal, "eLightEnvType_Point", a, 0, 0);
			NEXTENUM2(ht1ttalign, mVal, "eLightEnvType_Spot", b, 1, 0, a);
			NEXTENUM2(ht1ttalign, mVal, "eLightEnvType_DirectionalKey", c, 2, 0, b);
			NEXTENUM2(ht1ttalign, mVal, "eLightEnvType_Ambient", d, 3, 0, c);
			NEXTENUM2(ht1ttalign, mVal, "eLightEnvType_DirectionalAmbient", e, 4, 0, d);
			ADD(ht1ttalign);

			DEFINEKEYFRAMEDVALUE(LocationInfo, LocationInfo, ldata);
			DEFINEKEYFRAMEDVALUE(hPropertySet, Handle<PropertySet>, Handlepropset);
			MKNAME(meta_kfv_hPropertySet, "KeyframedValue<Handle<PropertySet>>");
			DEFINEKEYFRAMEDVALUE(ui64, unsigned __int64, u64);
			MKNAME(meta_kfv_ui64, "KeyframedValue<unsigned__int64>");

			DEFINET2(senb, SoundEventNameBase);
			FIRSTMEM2(senb, mEventGuid, SoundEventNameBase, symbol, 0);
			NEXTMEM2(senb, mEventDisplayName, SoundEventNameBase, symbol, 0, mEventGuid);
			ADD(senb);

			DEFINET2(sen0, SoundEventName<SoundEventNameBase::NT_DEFAULT>);
			FIRSTMEM1(sen0, "Baseclass_SoundEventNameBase", ALIAS, mEventGuid, SoundEventName<SoundEventNameBase::NT_DEFAULT>, senb, 0);
			ADD(sen0);

			DEFINET2(sen1, SoundEventName<SoundEventNameBase::NT_SNAPSHOT>);
			FIRSTMEM1(sen1, "Baseclass_SoundEventNameBase", ALIAS, mEventGuid, SoundEventName<SoundEventNameBase::NT_SNAPSHOT>, senb, 0);
			ADD(sen1);

			DEFINET2(sen2, SoundEventName<SoundEventNameBase::NT_DIALOG>);
			FIRSTMEM1(sen2, "Baseclass_SoundEventNameBase", ALIAS, mEventGuid, SoundEventName<SoundEventNameBase::NT_DIALOG>, senb, 0);
			ADD(sen2);

			DEFINET2(evctx, SoundAmbience::EventContext);
			MKNAME(meta_evctx, "SoundAmbience::EventContext");
			FIRSTMEM1(evctx, "Baseclass_SoundEventNameBase", ALIAS, mEventGuid, SoundAmbience::EventContext, senb, 0);
			NEXTMEM2(evctx, mEventname, SoundAmbience::EventContext, sen0, 0, ALIAS);
			NEXTMEM2(evctx, mPlayChance, SoundAmbience::EventContext, float, 0, mEventname);
			NEXTMEM2(evctx, mSilentTimeRange, SoundAmbience::EventContext, rangef, 0, mPlayChance);
			NEXTMEM2(evctx, mPlayTimeRange, SoundAmbience::EventContext, rangef, 0, mSilentTimeRange);
			NEXTMEM2(evctx, mVolumeRangedB, SoundAmbience::EventContext, rangef, 0, mPlayTimeRange);
			NEXTMEM2(evctx, mVolumeFadeTimeRange, SoundAmbience::EventContext, rangef, 0, mVolumeRangedB);
			ADD(evctx);

			DEFINEDCARRAY2(SoundAmbience::EventContext, ambiencectx);

			DEFINET2(ambience, SoundAmbience::AmbienceDefinition);
			EXT(ambience, ambience);
			FIRSTMEM2(ambience, mEvents, SoundAmbience::AmbienceDefinition, DCArray_ambiencectx, 0);
			ADD(ambience);

			DEFINEANMVALUEI(animorchore, AnimOrChore);
			DEFINEKEYFRAMEDVALUE(kfvanc, AnimOrChore, animorchore);

			DEFINET2(plk, Procedural_LookAt::Constraint);
			FIRSTMEM2(plk, mMaxLeftRight, Procedural_LookAt::Constraint, float, 0);
			NEXTMEM2(plk, mMinLeftRight, Procedural_LookAt::Constraint, float, 0, mMaxLeftRight);
			NEXTMEM2(plk, mMaxUp, Procedural_LookAt::Constraint, float, 0, mMinLeftRight);
			NEXTMEM2(plk, mMinUp, Procedural_LookAt::Constraint, float, 0, mMaxUp);
			NEXTMEM2(plk, mLeftRightFixedOffsset, Procedural_LookAt::Constraint, float, 0, mMinUp);
			NEXTMEM2(plk, mUpDownFixedOffsset, Procedural_LookAt::Constraint, float, 0, mLeftRightFixedOffsset);
			ADD(plk);
			
			DEFINEDCARRAY2(Procedural_LookAt::Constraint, plk);

			DEFINEHANDLELOCK(langr, LanguageRes);
			DEFINEHANDLELOCK(langfres, LanguageResource);

			DEFINET2(diid, DialogInstance::InstanceID);
			FIRSTMEM2(diid, mID, DialogInstance::InstanceID, int, 0);
			ADD(diid);

			DEFINEKEYFRAMEDVALUE(kcol, Color, color);
			DEFINEANMVALUEI(kcoli, Color);
			DEFINEKEYFRAMEDVALUE(hsound, Handle<SoundData>, Handlesound);
			DEFINEKEYFRAMEDVALUE(htexx, Handle<T3Texture>, tex);
			DEFINEANMVALUEI(hsoundi, Handle<SoundData>);
			DEFINEANMVALUEI(htexxx, Handle<T3Texture>);
			
			DEFINEKEYFRAMEDVALUE(hphnt, Handle<PhonemeTable>, Handleptable);
			DEFINEANMVALUEI(hptanm, Handle<PhonemeTable>);
			DEFINEHANDLE(hambb, SoundAmbience::AmbienceDefinition);

			DEFINET2(pat1, EnumParticleGeometryType);
			FIRSTMEM2(pat1, mVal, EnumParticleGeometryType, int, MetaFlag_EnumIntType);
			NEXTMEM1(pat1, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumParticleGeometryType, enumbase, MetaFlag_BaseClass, mVal);
			FIRSTENUM2(pat1, mVal, "eParticleGeometryType_Sprite", a, 1, 0);
			NEXTENUM2(pat1, mVal, "eParticleGeometryType_Quad", b, 2, 0, a);
			NEXTENUM2(pat1, mVal, "eParticleGeometryType_Streak", c, 3, 0, b);
			NEXTENUM2(pat1, mVal, "eParticleGeometryType_Strip", d, 4, 0, c);
			NEXTENUM2(pat1, mVal, "eParticleGeometryType_StripFacing", e, 5, 0, d);
			NEXTENUM2(pat1, mVal, "eParticleGeometryType_None", f, 6, 0, e);
			ADD(pat1);

			DEFINET2(pat, EnumParticleAffectorType);
			FIRSTMEM2(pat, mVal, EnumParticleAffectorType, int, MetaFlag_EnumIntType);
			NEXTMEM1(pat, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumParticleAffectorType, enumbase, MetaFlag_BaseClass, mVal);
			FIRSTENUM2(pat, mVal, "eParticleAffectorType_Force", a, 1, 0);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_Attractor", b, 2, 0, a);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_KillPlane", c, 3, 0, b);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_KillBox", d, 4, 0, c);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_CollisionPlane", e, 5, 0, d);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_CollisionSphere", f, 6, 0, e);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_CollisionBox", g, 7, 0, f);
			NEXTENUM2(pat, mVal, "eParticleAffectorType_CollisionCylinder", h, 8, 0, g);
			ADD(pat);

			DEFINET2(po1, PhysicsObject::EnumePhysicsCollisionType);
			FIRSTMEM2(po1, mVal, PhysicsObject::EnumePhysicsCollisionType, int, MetaFlag_EnumIntType);
			NEXTMEM1(po1, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, PhysicsObject::EnumePhysicsCollisionType, enumbase, MetaFlag_BaseClass, mVal);
			FIRSTENUM2(po1, mVal, "PhysicsCollisionType_InterAgent", a, 0, 0);
			NEXTENUM2(po1, mVal, "PhysicsCollisionType_Raycast", b, 1, 0, a);
			ADD(po1);

			DEFINET2(pos2, PhysicsObject::EnumePhysicsBoundingVolumeType);
			FIRSTMEM2(pos2, mVal, PhysicsObject::EnumePhysicsBoundingVolumeType, int, MetaFlag_EnumIntType);
			NEXTMEM1(pos2, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, PhysicsObject::EnumePhysicsBoundingVolumeType, enumbase, MetaFlag_BaseClass, mVal);
			FIRSTENUM2(pos2, mVal, "PhysicsBoundingVolumeType_Cylinder", a, 0, 0);
			NEXTENUM2(pos2, mVal, "PhysicsBoundingVolumeType_Box", b, 1, 0, a);
			NEXTENUM2(pos2, mVal, "PhysicsBoundingVolumeType_Sphere", c, 2, 0, b);
			ADD(pos2);
			
			DEFINEMAP2(SoundFootsteps::EnumMaterial, SoundEventName<SoundEventNameBase::NT_DEFAULT>, soundmat, sen0, std::less<SoundFootsteps::EnumMaterial>);

			DEFINET2(f2b, Footsteps2::FootstepBank);
			FIRSTMEM2(f2b, mEventName, Footsteps2::FootstepBank, sen0, 0);
			NEXTMEM2(f2b, mMaterialMap, Footsteps2::FootstepBank, Map_soundmat_sen0, 0, mEventName);
			ADD(f2b);

			DEFINET2(abb, EnumEmitterTriggerEnable);
			FIRSTMEM2(abb, mVal, EnumEmitterTriggerEnable, int, MetaFlag_EnumIntType);
			NEXTMEM1(abb, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterTriggerEnable, enumbase, MetaFlag_BaseClass, mVal);
			FIRSTENUM2(abb, mVal, "eEmitterTriggerEnable_None", a, 1, 0);
			NEXTENUM2(abb, mVal, "eEmitterTriggerEnable_AgentVis", b, 2, 0, a);
			NEXTENUM2(abb, mVal, "eEmitterTriggerEnable_AgentVisInvert", c, 3, 0, b);
			NEXTENUM2(abb, mVal, "eEmitterTriggerEnable_Default", d, 4, 0, c);
			ADD(abb);

			DEFINET2(acc, EnumEmittersEnableType);
			FIRSTMEM2(acc, mVal, EnumEmittersEnableType, int, MetaFlag_EnumIntType);
			NEXTMEM1(acc, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmittersEnableType, enumbase, MetaFlag_BaseClass, mVal);
			FIRSTENUM2(acc, mVal, "eEmittersEnableType_All", a, 1, 0);
			NEXTENUM2(acc, mVal, "eEmittersEnableType_Random", b, 2, 0, a);
			NEXTENUM2(acc, mVal, "eEmittersEnableType_Sequential", c, 3, 0, b);
			ADD(acc);

			DEFINET2(rootk, RootKey);
			FIRSTMEM2(rootk, mTranslationConstraint__Enabled, RootKey, bool, 0);
			NEXTMEM2(rootk, mTranslationConstraint_MaxVelocity, RootKey, float, 0, mTranslationConstraint__Enabled);
			NEXTMEM2(rootk, mTranslationConstraint_MaxAcceleration, RootKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(rootk, mRotationConstraint__Enabled, RootKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(rootk, mRotationConstraint_MaxBendAngularVelocity, RootKey, float, 0, mRotationConstraint__Enabled);
			NEXTMEM2(rootk, mRotationConstraint_MaxBendAngularAcceleration, RootKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			NEXTMEM2(rootk, mRotationConstraint_MaxTwistAngularVelocity, RootKey, float, 0, mRotationConstraint_MaxBendAngularAcceleration);
			NEXTMEM2(rootk, mRotationConstraint_MaxTwistAngularAcceleration, RootKey, float, 0, mRotationConstraint_MaxTwistAngularVelocity);
			ADD(rootk);

			DEFINET2(pivot, PivotJointKey);
			FIRSTMEM2(pivot, mBoneLengthConstraint__Enabled, PivotJointKey, bool, 0);
			NEXTMEM2(pivot, mBoneLengthConstraint_RestRelative, PivotJointKey, bool, 0, mBoneLengthConstraint__Enabled);
			NEXTMEM2(pivot, mBoneLengthConstraint_NodeMobility, PivotJointKey, float, 0, mBoneLengthConstraint_RestRelative);
			NEXTMEM2(pivot, mBoneLengthConstraint_ParentMobility, PivotJointKey, float, 0, mBoneLengthConstraint_NodeMobility);
			NEXTMEM2(pivot, mAngleConstraint__Enabled, PivotJointKey, bool, 0, mBoneLengthConstraint_ParentMobility);
			NEXTMEM2(pivot, mAngleConstraint_MinHorizontalBendAngle, PivotJointKey, float, 0, mAngleConstraint__Enabled);
			NEXTMEM2(pivot, mAngleConstraint_MaxHorizontalBendAngle, PivotJointKey, float, 0, mAngleConstraint_MinHorizontalBendAngle);
			NEXTMEM2(pivot, mAngleConstraint_MinVerticalBendAngle, PivotJointKey, float, 0, mAngleConstraint_MaxHorizontalBendAngle);
			NEXTMEM2(pivot, mAngleConstraint_MaxVerticalBendAngle, PivotJointKey, float, 0, mAngleConstraint_MinVerticalBendAngle);
			NEXTMEM2(pivot, mTranslationConstraint__Enabled, PivotJointKey, bool, 0, mAngleConstraint_MaxVerticalBendAngle);
			NEXTMEM2(pivot, mTranslationConstraint_ReferenceFrame, PivotJointKey, int, MetaFlag_EnumIntType, mTranslationConstraint__Enabled);
			FIRSTENUM2(pivot, mTranslationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(pivot, mTranslationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(pivot, mTranslationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(pivot, mTranslationConstraint_MaxVelocity, PivotJointKey, float, 0, mTranslationConstraint_ReferenceFrame);
			NEXTMEM2(pivot, mTranslationConstraint_MaxAcceleration, PivotJointKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(pivot, mRotationConstraint__Enabled, PivotJointKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(pivot, mRotationConstraint_ReferenceFrame, PivotJointKey, int, MetaFlag_EnumIntType, mRotationConstraint__Enabled);
			FIRSTENUM2(pivot, mRotationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(pivot, mRotationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(pivot, mRotationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(pivot, mRotationConstraint_MaxBendAngularVelocity, PivotJointKey, float, 0, mRotationConstraint_ReferenceFrame);
			NEXTMEM2(pivot, mRotationConstraint_MaxBendAngularAcceleration, PivotJointKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			ADD(pivot);

			DEFINET2(ball, BallJointKey);
			FIRSTMEM2(ball, mBoneLengthConstraint__Enabled, BallJointKey, bool, 0);
			NEXTMEM2(ball, mBoneLengthConstraint_RestRelative, BallJointKey, bool, 0, mBoneLengthConstraint__Enabled);
			NEXTMEM2(ball, mBoneLengthConstraint_NodeMobility, BallJointKey, float, 0, mBoneLengthConstraint_RestRelative);
			NEXTMEM2(ball, mBoneLengthConstraint_ParentMobility, BallJointKey, float, 0, mBoneLengthConstraint_NodeMobility);
			NEXTMEM2(ball, mAngleConstraint__Enabled, BallJointKey, bool, 0, mBoneLengthConstraint_ParentMobility);
			NEXTMEM2(ball, mAngleConstraint_MaxBendAngle, BallJointKey, float, 0, mAngleConstraint__Enabled);
			NEXTMEM2(ball, mTranslationConstraint__Enabled, BallJointKey, bool, 0, mAngleConstraint_MaxBendAngle);
			NEXTMEM2(ball, mTranslationConstraint_ReferenceFrame, BallJointKey, int, MetaFlag_EnumIntType, mTranslationConstraint__Enabled);
			FIRSTENUM2(ball, mTranslationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(ball, mTranslationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(ball, mTranslationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(ball, mTranslationConstraint_MaxVelocity, BallJointKey, float, 0, mTranslationConstraint_ReferenceFrame);
			NEXTMEM2(ball, mTranslationConstraint_MaxAcceleration, BallJointKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(ball, mRotationConstraint__Enabled, BallJointKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(ball, mRotationConstraint_ReferenceFrame, BallJointKey, int, MetaFlag_EnumIntType, mRotationConstraint__Enabled);
			FIRSTENUM2(ball, mRotationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(ball, mRotationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(ball, mRotationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(ball, mRotationConstraint_MaxBendAngularVelocity, BallJointKey, float, 0, mRotationConstraint_ReferenceFrame);
			NEXTMEM2(ball, mRotationConstraint_MaxBendAngularAcceleration, BallJointKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			ADD(ball);

			DEFINET2(hinge, HingeJointKey);
			FIRSTMEM2(hinge, mBoneLengthConstraint__Enabled, HingeJointKey, bool, 0);
			NEXTMEM2(hinge, mBoneLengthConstraint_RestRelative, HingeJointKey, bool, 0, mBoneLengthConstraint__Enabled);
			NEXTMEM2(hinge, mBoneLengthConstraint_NodeMobility, HingeJointKey, float, 0, mBoneLengthConstraint_RestRelative);
			NEXTMEM2(hinge, mBoneLengthConstraint_ParentMobility, HingeJointKey, float, 0, mBoneLengthConstraint_NodeMobility);
			NEXTMEM2(hinge, mAngleConstraint__Enabled, HingeJointKey, bool, 0, mBoneLengthConstraint_ParentMobility);
			NEXTMEM2(hinge, mAngleConstraint_MinBendAngle, HingeJointKey, float, 0, mAngleConstraint__Enabled);
			NEXTMEM2(hinge, mAngleConstraint_MaxBendAngle, HingeJointKey, float, 0, mAngleConstraint_MinBendAngle);
			NEXTMEM2(hinge, mTranslationConstraint__Enabled, HingeJointKey, bool, 0, mAngleConstraint_MaxBendAngle);
			NEXTMEM2(hinge, mTranslationConstraint_ReferenceFrame, HingeJointKey, int, MetaFlag_EnumIntType, mTranslationConstraint__Enabled);
			FIRSTENUM2(hinge, mTranslationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(hinge, mTranslationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(hinge, mTranslationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(hinge, mTranslationConstraint_MaxVelocity, HingeJointKey, float, 0, mTranslationConstraint_ReferenceFrame);
			NEXTMEM2(hinge, mTranslationConstraint_MaxAcceleration, HingeJointKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(hinge, mRotationConstraint__Enabled, HingeJointKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(hinge, mRotationConstraint_ReferenceFrame, HingeJointKey, int, MetaFlag_EnumIntType, mRotationConstraint__Enabled);
			FIRSTENUM2(hinge, mRotationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(hinge, mRotationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(hinge, mRotationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(hinge, mRotationConstraint_MaxBendAngularVelocity, HingeJointKey, float, 0, mRotationConstraint_ReferenceFrame);
			NEXTMEM2(hinge, mRotationConstraint_MaxBendAngularAcceleration, HingeJointKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			ADD(hinge);

			DEFINET2(balltwist, BallTwistJointKey);
			FIRSTMEM2(balltwist, mBoneLengthConstraint__Enabled, BallTwistJointKey, bool, 0);
			NEXTMEM2(balltwist, mBoneLengthConstraint_RestRelative, BallTwistJointKey, bool, 0, mBoneLengthConstraint__Enabled);
			NEXTMEM2(balltwist, mBoneLengthConstraint_NodeMobility, BallTwistJointKey, float, 0, mBoneLengthConstraint_RestRelative);
			NEXTMEM2(balltwist, mBoneLengthConstraint_ParentMobility, BallTwistJointKey, float, 0, mBoneLengthConstraint_NodeMobility);
			NEXTMEM2(balltwist, mAngleConstraint__Enabled, BallTwistJointKey, bool, 0, mBoneLengthConstraint_ParentMobility);
			NEXTMEM2(balltwist, mAngleConstraint_MaxBendAngle, BallTwistJointKey, float, 0, mAngleConstraint__Enabled);
			NEXTMEM2(balltwist, mAngleConstraint_MinTwistAngle, BallTwistJointKey, float, 0, mAngleConstraint_MaxBendAngle);
			NEXTMEM2(balltwist, mAngleConstraint_MaxTwistAngle, BallTwistJointKey, float, 0, mAngleConstraint_MinTwistAngle);
			NEXTMEM2(balltwist, mTranslationConstraint__Enabled, BallTwistJointKey, bool, 0, mAngleConstraint_MaxTwistAngle);
			NEXTMEM2(balltwist, mTranslationConstraint_ReferenceFrame, BallTwistJointKey, int, MetaFlag_EnumIntType, mTranslationConstraint__Enabled);
			FIRSTENUM2(balltwist, mTranslationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(balltwist, mTranslationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(balltwist, mTranslationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(balltwist, mTranslationConstraint_MaxVelocity, BallTwistJointKey, float, 0, mTranslationConstraint_ReferenceFrame);
			NEXTMEM2(balltwist, mTranslationConstraint_MaxAcceleration, BallTwistJointKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(balltwist, mRotationConstraint__Enabled, BallTwistJointKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(balltwist, mRotationConstraint_ReferenceFrame, BallTwistJointKey, int, MetaFlag_EnumIntType, mRotationConstraint__Enabled);
			FIRSTENUM2(balltwist, mRotationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(balltwist, mRotationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(balltwist, mRotationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(balltwist, mRotationConstraint_MaxBendAngularVelocity, BallTwistJointKey, float, 0, mRotationConstraint_ReferenceFrame);
			NEXTMEM2(balltwist, mRotationConstraint_MaxBendAngularAcceleration, BallTwistJointKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			NEXTMEM2(balltwist, mRotationConstraint_MaxTwistAngularVelocity, BallTwistJointKey, float, 0, mRotationConstraint_MaxBendAngularAcceleration);
			NEXTMEM2(balltwist, mRotationConstraint_MaxTwistAngularAcceleration, BallTwistJointKey, float, 0, mRotationConstraint_MaxTwistAngularVelocity);
			ADD(balltwist);

			DEFINET2(pballtwist, PlaceableBallTwistJointKey);
			FIRSTMEM2(pballtwist, mBoneLengthConstraint__Enabled, PlaceableBallTwistJointKey, bool, 0);
			NEXTMEM2(pballtwist, mBoneLengthConstraint_RestRelative, PlaceableBallTwistJointKey, bool, 0, mBoneLengthConstraint__Enabled);
			NEXTMEM2(pballtwist, mBoneLengthConstraint_NodeMobility, PlaceableBallTwistJointKey, float, 0, mBoneLengthConstraint_RestRelative);
			NEXTMEM2(pballtwist, mBoneLengthConstraint_ParentMobility, PlaceableBallTwistJointKey, float, 0, mBoneLengthConstraint_NodeMobility);
			NEXTMEM2(pballtwist, mAngleConstraint__Enabled, PlaceableBallTwistJointKey, bool, 0, mBoneLengthConstraint_ParentMobility);
			NEXTMEM2(pballtwist, mAngleConstraint_MaxBendAngle, PlaceableBallTwistJointKey, float, 0, mAngleConstraint__Enabled);
			NEXTMEM2(pballtwist, mAngleConstraint_MinTwistAngle, PlaceableBallTwistJointKey, float, 0, mAngleConstraint_MaxBendAngle);
			NEXTMEM2(pballtwist, mAngleConstraint_MaxTwistAngle, PlaceableBallTwistJointKey, float, 0, mAngleConstraint_MinTwistAngle);
			NEXTMEM2(pballtwist, mPlacementLockToAnimConstraint__Enabled, PlaceableBallTwistJointKey, bool, 0, mAngleConstraint_MaxTwistAngle);
			NEXTMEM2(pballtwist, mPlacementLockToAnimConstraint_LockMode, PlaceableBallTwistJointKey, int, MetaFlag_EnumIntType, mPlacementLockToAnimConstraint__Enabled);
			FIRSTENUM2(pballtwist, mPlacementLockToAnimConstraint_LockMode, "pos", a, 1, 0);
			NEXTENUM2(pballtwist, mPlacementLockToAnimConstraint_LockMode, "rot", b, 2, 0, a);
			NEXTENUM2(pballtwist, mPlacementLockToAnimConstraint_LockMode, "pos & rot", c, 0, 0, b);
			NEXTMEM2(pballtwist, mPlacementLockToNodeConstraint__Enabled, PlaceableBallTwistJointKey, bool, 0, mPlacementLockToAnimConstraint_LockMode);
			NEXTMEM2(pballtwist, mPlacementLockToNodeConstraint_Node, PlaceableBallTwistJointKey, int, MetaFlag_EnumIntType, mPlacementLockToNodeConstraint__Enabled);
			FIRSTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "world", a, 0, 0);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "root", b, 1, 0, a);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "spine1", c, 2, 0, b);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "spine2", d, 3, 0, c);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "spine3", e, 4, 0, d);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "spine4", f, 5, 0, e);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "shoulder_L", g, 6, 0, f);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "arm_L", h, 7, 0, g);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "elbow_L", i, 8, 0, h);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "wrist_L", j, 9, 0, i);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "middleF1_L", k, 10, 0, j);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "middleF2_L", l, 11, 0, k);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "middleF3_L", m, 12, 0, l);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "indexF1_L", n, 13, 0, m);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "indexF2_L", o, 14, 0, n);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "indexF3_L", p, 15, 0, o);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "thumbF1_L", q, 16, 0, p);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "thumbF2_L", r, 17, 0, q);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "thumbF3_L", s, 18, 0, r);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ringF1_L", t, 19, 0, s);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ringF2_L", u, 20, 0, t);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ringF3_L", v, 21, 0, u);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pinkyF1_L", w, 22, 0, v);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pinkyF2_L", x, 23, 0, w);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pinkyF3_L", y, 24, 0, x);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "shoulder_R", z, 25, 0, y);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "arm_R", aa, 26, 0, z);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "elbow_R", bb, 27, 0, aa);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "wrist_R", bc, 28, 0, bb);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pinkyF1_R", bd, 29, 0, bc);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pinkyF2_R", be, 30, 0, bd);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pinkyF3_R", bf, 31, 0, be);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "middleF1_R", bg, 32, 0, bf);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "middleF2_R", bh, 33, 0, bg);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "middleF3_R", bi, 34, 0, bh);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "indexF1_R", bj, 35, 0, bi);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "indexF2_R", bk, 36, 0, bj);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "indexF3_R", bl, 37, 0, bk);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "thumbF1_R", bm, 38, 0, bl);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "thumbF2_R", bn, 39, 0, bm);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "thumbF3_R", bo, 40, 0, bn);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ringF1_R", bp, 41, 0, bo);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ringF2_R", bq, 42, 0, bp);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ringF3_R", br, 43, 0, bq);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "neck", bs, 44, 0, br);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "neck2", bt, 45, 0, bs);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "head", bu, 46, 0, bt);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "eye_L", bv, 47, 0, bu);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "eye_R", bw, 48, 0, bv);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "pelvis", bx, 49, 0, bw);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "leg_L", by, 50, 0, bx);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "knee_L", bz, 51, 0, by);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ankle_L", baa, 52, 0, bz);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "leg_R", bbb, 53, 0, baa);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "knee_R", bcc, 54, 0, bbb);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_Node, "ankle_R", bdd, 55, 0, bcc);
			NEXTMEM2(pballtwist, mPlacementLockToNodeConstraint_LockMode, PlaceableBallTwistJointKey, int, MetaFlag_EnumIntType, mPlacementLockToNodeConstraint_Node);
			FIRSTENUM2(pballtwist, mPlacementLockToNodeConstraint_LockMode, "pos", a, 1, 0);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_LockMode, "rot", b, 2, 0, a);
			NEXTENUM2(pballtwist, mPlacementLockToNodeConstraint_LockMode, "pos & rot", c, 0, 0, b);
			NEXTMEM2(pballtwist, mTranslationConstraint__Enabled, PlaceableBallTwistJointKey, bool, 0, mPlacementLockToNodeConstraint_LockMode);
			NEXTMEM2(pballtwist, mTranslationConstraint_ReferenceFrame, PlaceableBallTwistJointKey, int, MetaFlag_EnumIntType, mTranslationConstraint__Enabled);
			FIRSTENUM2(pballtwist, mTranslationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(pballtwist, mTranslationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(pballtwist, mTranslationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(pballtwist, mTranslationConstraint_MaxVelocity, PlaceableBallTwistJointKey, float, 0, mTranslationConstraint_ReferenceFrame);
			NEXTMEM2(pballtwist, mTranslationConstraint_MaxAcceleration, PlaceableBallTwistJointKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(pballtwist, mRotationConstraint__Enabled, PlaceableBallTwistJointKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(pballtwist, mRotationConstraint_ReferenceFrame, PlaceableBallTwistJointKey, int, MetaFlag_EnumIntType, mRotationConstraint__Enabled);
			FIRSTENUM2(pballtwist, mRotationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(pballtwist, mRotationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(pballtwist, mRotationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(pballtwist, mRotationConstraint_MaxBendAngularVelocity, PlaceableBallTwistJointKey, float, 0, mRotationConstraint_ReferenceFrame);
			NEXTMEM2(pballtwist, mRotationConstraint_MaxBendAngularAcceleration, PlaceableBallTwistJointKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			NEXTMEM2(pballtwist, mRotationConstraint_MaxTwistAngularVelocity, PlaceableBallTwistJointKey, float, 0, mRotationConstraint_MaxBendAngularAcceleration);
			NEXTMEM2(pballtwist, mRotationConstraint_MaxTwistAngularAcceleration, PlaceableBallTwistJointKey, float, 0, mRotationConstraint_MaxTwistAngularVelocity);
			ADD(pballtwist);

			DEFINET2(lballtwist, LinkedBallTwistJointKey);
			FIRSTMEM2(lballtwist, mBoneLengthConstraint__Enabled, LinkedBallTwistJointKey, bool, 0);
			NEXTMEM2(lballtwist, mBoneLengthConstraint_RestRelative, LinkedBallTwistJointKey, bool, 0, mBoneLengthConstraint__Enabled);
			NEXTMEM2(lballtwist, mBoneLengthConstraint_NodeMobility, LinkedBallTwistJointKey, float, 0, mBoneLengthConstraint_RestRelative);
			NEXTMEM2(lballtwist, mBoneLengthConstraint_ParentMobility, LinkedBallTwistJointKey, float, 0, mBoneLengthConstraint_NodeMobility);
			NEXTMEM2(lballtwist, mAngleConstraint__Enabled, LinkedBallTwistJointKey, bool, 0, mBoneLengthConstraint_ParentMobility);
			NEXTMEM2(lballtwist, mAngleConstraint_MaxBendAngle, LinkedBallTwistJointKey, float, 0, mAngleConstraint__Enabled);
			NEXTMEM2(lballtwist, mAngleConstraint_MinTwistAngle, LinkedBallTwistJointKey, float, 0, mAngleConstraint_MaxBendAngle);
			NEXTMEM2(lballtwist, mAngleConstraint_MaxTwistAngle, LinkedBallTwistJointKey, float, 0, mAngleConstraint_MinTwistAngle);
			NEXTMEM2(lballtwist, mTranslationConstraint__Enabled, LinkedBallTwistJointKey, bool, 0, mAngleConstraint_MaxTwistAngle);
			NEXTMEM2(lballtwist, mTranslationConstraint_ReferenceFrame, LinkedBallTwistJointKey, int, MetaFlag_EnumIntType, mTranslationConstraint__Enabled);
			FIRSTENUM2(lballtwist, mTranslationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(lballtwist, mTranslationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(lballtwist, mTranslationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(lballtwist, mTranslationConstraint_MaxVelocity, LinkedBallTwistJointKey, float, 0, mTranslationConstraint_ReferenceFrame);
			NEXTMEM2(lballtwist, mTranslationConstraint_MaxAcceleration, LinkedBallTwistJointKey, float, 0, mTranslationConstraint_MaxVelocity);
			NEXTMEM2(lballtwist, mRotationConstraint__Enabled, LinkedBallTwistJointKey, bool, 0, mTranslationConstraint_MaxAcceleration);
			NEXTMEM2(lballtwist, mRotationConstraint_ReferenceFrame, LinkedBallTwistJointKey, int, MetaFlag_EnumIntType, mRotationConstraint__Enabled);
			FIRSTENUM2(lballtwist, mRotationConstraint_ReferenceFrame, "World", a, 0, 0);
			NEXTENUM2(lballtwist, mRotationConstraint_ReferenceFrame, "Root", b, 1, 0, a);
			NEXTENUM2(lballtwist, mRotationConstraint_ReferenceFrame, "Parent", c, 2, 0, b);
			NEXTMEM2(lballtwist, mRotationConstraint_MaxBendAngularVelocity, LinkedBallTwistJointKey, float, 0, mRotationConstraint_ReferenceFrame);
			NEXTMEM2(lballtwist, mRotationConstraint_MaxBendAngularAcceleration, LinkedBallTwistJointKey, float, 0, mRotationConstraint_MaxBendAngularVelocity);
			NEXTMEM2(lballtwist, mRotationConstraint_MaxTwistAngularVelocity, LinkedBallTwistJointKey, float, 0, mRotationConstraint_MaxBendAngularAcceleration);
			NEXTMEM2(lballtwist, mRotationConstraint_MaxTwistAngularAcceleration, LinkedBallTwistJointKey, float, 0, mRotationConstraint_MaxTwistAngularVelocity);
			NEXTMEM2(lballtwist, mLinkBendConstraint__Enabled, LinkedBallTwistJointKey, bool, 0, mRotationConstraint_MaxTwistAngularAcceleration);
			NEXTMEM2(lballtwist, mLinkBendConstraint_Strength, LinkedBallTwistJointKey, float, 0, mLinkBendConstraint__Enabled);
			NEXTMEM2(lballtwist, mLinkBendConstraint_EchoDelay, LinkedBallTwistJointKey, float, 0, mLinkBendConstraint_Strength);
			NEXTMEM2(lballtwist, mLinkTwistConstraint__Enabled, LinkedBallTwistJointKey, bool, 0, mLinkBendConstraint_EchoDelay);
			NEXTMEM2(lballtwist, mLinkTwistConstraint_Strength, LinkedBallTwistJointKey, float, 0, mLinkTwistConstraint__Enabled);
			NEXTMEM2(lballtwist, mLinkTwistConstraint_EchoDelay, LinkedBallTwistJointKey, float, 0, mLinkTwistConstraint_Strength);
			ADD(lballtwist);

			DEFINET2(enlmcb, EnlightenModule::EnlightenCubemapSettings);
			FIRSTMEM2(enlmcb, mFaceWidth, EnlightenModule::EnlightenCubemapSettings, int, 0);
			NEXTMEM2(enlmcb, mBoxOrigin, EnlightenModule::EnlightenCubemapSettings, vec3, 0, mFaceWidth);
			ADD(enlmcb);

			DEFINET2(enlq, EnlightenModule::EnumeQuality);
			FIRSTMEM2(enlq, mVal, EnlightenModule::EnumeQuality, int, MetaFlag_EnumIntType);
			FIRSTENUM2(enlq, mVal, "Background", a, 0, 0);
			NEXTENUM2(enlq, mVal, "Low", b, 1, 0, a);
			NEXTENUM2(enlq, mVal, "Medium", C, 2, 0, b);
			NEXTENUM2(enlq, mVal, "High", d, 3, 0, C);
			NEXTENUM2(enlq, mVal, "Auto", e, 4, 0, d);
			NEXTMEM1(enlq, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeQuality, enumbase, MetaFlag_BaseClass, mVal);
			ADD(enlq);

			DEFINET2(qqq, EnlightenModule::EnlightenAdaptiveProbeVolumeSettings);
			FIRSTMEM2(qqq, mQuality, EnlightenModule::EnlightenAdaptiveProbeVolumeSettings, enlq, 0);
			ADD(qqq);

			DEFINET2(aaa, EnlightenModule::EnlightenAutoProbeVolumeSettings);
			FIRSTMEM2(aaa, mQuality, EnlightenModule::EnlightenAutoProbeVolumeSettings, enlq, 0);
			NEXTMEM2(aaa, mMinProbeSetSize, EnlightenModule::EnlightenAutoProbeVolumeSettings, int, 0, mQuality);
			NEXTMEM2(aaa, mMergingThreshold, EnlightenModule::EnlightenAutoProbeVolumeSettings, float, 0, mMinProbeSetSize);
			NEXTMEM2(aaa, mMinProbeSpacing, EnlightenModule::EnlightenAutoProbeVolumeSettings, int, 0, mMergingThreshold);
			NEXTMEM2(aaa, mMaxProbeSpacing, EnlightenModule::EnlightenAutoProbeVolumeSettings, int, 0, mMinProbeSpacing);
			ADD(aaa);

			DEFINET2(bbb, EnlightenModule::EnlightenProbeVolumeSettings);
			FIRSTMEM2(bbb, mQuality, EnlightenModule::EnlightenProbeVolumeSettings, enlq, 0);
			NEXTMEM2(bbb, mLightmapType, EnlightenModule::EnlightenProbeVolumeSettings, string, 0, mQuality);
			NEXTMEM2(bbb, mResolution, EnlightenModule::EnlightenProbeVolumeSettings, vec3, 0, mLightmapType);
			ADD(bbb);

			DEFINET2(ccc, T3LightEnvLOD);
			FIRSTMEM2(ccc, mFlags, T3LightEnvLOD, flags, MetaFlag_FlagType);
			FIRSTFLAG(ccc, mFlags, "LOD on High Quality", a, 0x10000);
			NEXTFLAG(ccc, mFlags, "LOD on Medium Quality", b, 0x20000, a);
			NEXTFLAG(ccc, mFlags, "LOD on Low Quality", c, 0x40000, b);
			ADD(ccc);

			DEFINET2(ddd, EnumHBAOParticipationType);
			FIRSTMEM2(ddd, mVal, EnumHBAOParticipationType, int, MetaFlag_EnumIntType);
			FIRSTENUM2(ddd, mVal, "eHBAOParticipationTypeAuto", a, 0, 0);
			NEXTENUM2(ddd, mVal, "eHBAOParticipationTypeForceOn", b, 1, 0, a);
			NEXTENUM2(ddd, mVal, "eHBAOParticipationTypeForceOff", c, 2, 0, b);
			NEXTMEM1(ddd, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAOParticipationType, enumbase, MetaFlag_BaseClass, mVal);
			ADD(ddd);

			DEFINET2(eee, EnumT3LightEnvMobility);
			FIRSTMEM2(eee, mVal, EnumT3LightEnvMobility, int, MetaFlag_EnumIntType);
			FIRSTENUM2(eee, mVal, "eLightEnvMobility_Static", a, 0, 0);
			NEXTENUM2(eee, mVal, "eLightEnvMobility_Stationary", b, 1, 0, a);
			NEXTENUM2(eee, mVal, "eLightEnvMobility_Moveable", c, 2, 0, b);
			NEXTMEM1(eee, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3LightEnvMobility, enumbase, MetaFlag_BaseClass, mVal);
			ADD(eee);

			DEFINET2(fff, EnumT3LightEnvShadowType);
			FIRSTMEM2(fff, mVal, EnumT3LightEnvShadowType, int, MetaFlag_EnumIntType);
			FIRSTENUM2(fff, mVal, "eLightEnvShadowType_None", a, 0, 0);
			NEXTENUM2(fff, mVal, "eLightEnvShadowType_PerLight", b, 2, 0, a);
			NEXTENUM2(fff, mVal, "eLightEnvShadowType_Modulated", c, 3, 0, b);
			NEXTMEM1(fff, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3LightEnvShadowType, enumbase, MetaFlag_BaseClass, mVal);
			ADD(fff);

			DEFINET2(ggg, EnumT3LightEnvShadowQuality);
			FIRSTMEM2(ggg, mVal, EnumT3LightEnvShadowQuality, int, MetaFlag_EnumIntType);
			FIRSTENUM2(ggg, mVal, "eLightEnvShadowQuality_Low", a, 0, 0);
			NEXTENUM2(ggg, mVal, "eLightEnvShadowQuality_Medium", b, 1, 0, a);
			NEXTENUM2(ggg, mVal, "eLightEnvShadowQuality_High", c, 2, 0, b);
			NEXTMEM1(ggg, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3LightEnvShadowQuality, enumbase, MetaFlag_BaseClass, mVal);
			ADD(ggg);

			DEFINET2(M_3soLBPh71Y, ParticleLODKey);
			FIRSTMEM2(M_3soLBPh71Y, mCountScale, ParticleLODKey, float, 0x0);
			NEXTMEM2(M_3soLBPh71Y, mStrideScale, ParticleLODKey, float, 0x0, mCountScale);
			NEXTMEM2(M_3soLBPh71Y, mDivisionScale, ParticleLODKey, float, 0x0, mStrideScale);
			NEXTMEM2(M_3soLBPh71Y, mLifeScale, ParticleLODKey, float, 0x0, mDivisionScale);
			ADD(M_3soLBPh71Y);

			DEFINET2(M_exuieaoEiI, EnumEmitterParticleCountType);
			ADDFLAGS(M_exuieaoEiI, 0x8008);
			FIRSTMEM2(M_exuieaoEiI, mVal, EnumEmitterParticleCountType, int, 0x40);
			FIRSTENUM2(M_exuieaoEiI, mVal, "eEmitterParticleCountType_SpawnVolParticleCount", M_gxIX4a2dRE, 2, 0);
			NEXTENUM2(M_exuieaoEiI, mVal, "eEmitterParticleCountType_Count", M_bbSqWy6yhK, 1, 0, M_gxIX4a2dRE);
			NEXTMEM1(M_exuieaoEiI, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterParticleCountType, enumbase, 0x10, mVal);
			ADD(M_exuieaoEiI);


			DEFINET2(M_IDCdJOyapn1, EnumEmitterConstraintType);
			ADDFLAGS(M_IDCdJOyapn1, 0x8008);
			FIRSTMEM2(M_IDCdJOyapn1, mVal, EnumEmitterConstraintType, int, 0x40);
			FIRSTENUM2(M_IDCdJOyapn1, mVal, "eEmitterConstraintType_2Point", M_xrpMCARCr4, 4, 0);
			NEXTENUM2(M_IDCdJOyapn1, mVal, "eEmitterConstraintType_1Point", M_zdGc81tBDK, 3, 0, M_xrpMCARCr4);
			NEXTENUM2(M_IDCdJOyapn1, mVal, "eEmitterConstraintType_0Point", M_sMlaZTXC1O, 2, 0, M_zdGc81tBDK);
			NEXTENUM2(M_IDCdJOyapn1, mVal, "eEmitterConstraintType_None", M_8YFOGKjxRr, 1, 0, M_sMlaZTXC1O);
			NEXTMEM1(M_IDCdJOyapn1, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterConstraintType, enumbase, 0x10, mVal);
			ADD(M_IDCdJOyapn1);


			DEFINET2(M_JBdT3hVOfo, EnumParticleSortMode);
			ADDFLAGS(M_JBdT3hVOfo, 0x8008);
			FIRSTMEM2(M_JBdT3hVOfo, mVal, EnumParticleSortMode, int, 0x40);
			FIRSTENUM2(M_JBdT3hVOfo, mVal, "eParticleSortMode_OldestFirst", M_aMeAjSWfch, 4, 0);
			NEXTENUM2(M_JBdT3hVOfo, mVal, "eParticleSortMode_YoungestFirst", M_oZYFYZ5B6k, 3, 0, M_aMeAjSWfch);
			NEXTENUM2(M_JBdT3hVOfo, mVal, "eParticleSortMode_ByDistance", M_zMCk8R6BEu, 2, 0, M_oZYFYZ5B6k);
			NEXTENUM2(M_JBdT3hVOfo, mVal, "eParticleSortMode_None", M_ZMrF6cI6NX, 1, 0, M_zMCk8R6BEu);
			NEXTMEM1(M_JBdT3hVOfo, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumParticleSortMode, enumbase, 0x10, mVal);
			ADD(M_JBdT3hVOfo);


			DEFINET2(M_8DYdD3ojxS, EnumEmitterSpawnShape);
			ADDFLAGS(M_8DYdD3ojxS, 0x8008);
			FIRSTMEM2(M_8DYdD3ojxS, mVal, EnumEmitterSpawnShape, int, 0x40);
			FIRSTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_BoneBoxes", M_nqPTGfRyil, 8, 0);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_Bones", M_OYGxlSXPtL, 7, 0, M_nqPTGfRyil);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_ParticleInterpolate", M_JboH8S4kwI, 6, 0, M_OYGxlSXPtL);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_Particle", M_gTxSl1C00G, 5, 0, M_JboH8S4kwI);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_ToTarget", M_OzOLMrbAyf, 4, 0, M_gTxSl1C00G);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_Cylinder", M_KUUTsGCoRb, 3, 0, M_OzOLMrbAyf);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_Sphere", M_lsaqv6Updv, 2, 0, M_KUUTsGCoRb);
			NEXTENUM2(M_8DYdD3ojxS, mVal, "eEmitterSpawn_Box", M_NIsNrmwUlN, 1, 0, M_lsaqv6Updv);
			NEXTMEM1(M_8DYdD3ojxS, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterSpawnShape, enumbase, 0x10, mVal);
			ADD(M_8DYdD3ojxS);


			DEFINET2(M_5u9t3tgj2t, EnumEmitterBoneSelection);
			ADDFLAGS(M_5u9t3tgj2t, 0x8008);
			FIRSTMEM2(M_5u9t3tgj2t, mVal, EnumEmitterBoneSelection, int, 0x40);
			FIRSTENUM2(M_5u9t3tgj2t, mVal, "eEmitterBoneSelection_Children", M_usZBauFkrF, 2, 0);
			NEXTENUM2(M_5u9t3tgj2t, mVal, "eEmitterBoneSelection_All", M_CJCmZFOv1Q, 1, 0, M_usZBauFkrF);
			NEXTMEM1(M_5u9t3tgj2t, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterBoneSelection, enumbase, 0x10, mVal);
			ADD(M_5u9t3tgj2t);


			DEFINET2(M_DIIXJNZI95, EnumEmitterColorType);
			ADDFLAGS(M_DIIXJNZI95, 0x8008);
			FIRSTMEM2(M_DIIXJNZI95, mVal, EnumEmitterColorType, int, 0x40);
			FIRSTENUM2(M_DIIXJNZI95, mVal, "eEmitterColorType_KeyControl01", M_hFQr77BIEl, 6, 0);
			NEXTENUM2(M_DIIXJNZI95, mVal, "eEmitterColorType_Index_Lerp", M_yct4Aj6PKn, 5, 0, M_hFQr77BIEl);
			NEXTENUM2(M_DIIXJNZI95, mVal, "eEmitterColorType_BurstLife_Lerp", M_ZpzRiKYpZg, 4, 0, M_yct4Aj6PKn);
			NEXTENUM2(M_DIIXJNZI95, mVal, "eEmitterColorType_Random_Discrete", M_nSOKlq8Azr, 3, 0, M_ZpzRiKYpZg);
			NEXTENUM2(M_DIIXJNZI95, mVal, "eEmitterColorType_Random_Lerp", M_HqDoGL4RHy, 2, 0, M_nSOKlq8Azr);
			NEXTENUM2(M_DIIXJNZI95, mVal, "eEmitterColorType_Constant", M_qyX3g5Ayy6, 1, 0, M_HqDoGL4RHy);
			NEXTMEM1(M_DIIXJNZI95, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterColorType, enumbase, 0x10, mVal);
			ADD(M_DIIXJNZI95);


			DEFINET2(M_1leDYT3CEW, EnumEmitterSpriteAnimationType);
			ADDFLAGS(M_1leDYT3CEW, 0x8008);
			FIRSTMEM2(M_1leDYT3CEW, mVal, EnumEmitterSpriteAnimationType, int, 0x40);
			FIRSTENUM2(M_1leDYT3CEW, mVal, "eEmitterSpriteAnimationType_Random", M_8K69rklPGt, 2, 0);
			NEXTENUM2(M_1leDYT3CEW, mVal, "eEmitterSpriteAnimationType_Linear", M_fUR2IZ2mB1, 1, 0, M_8K69rklPGt);
			NEXTMEM1(M_1leDYT3CEW, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterSpriteAnimationType, enumbase, 0x10, mVal);
			ADD(M_1leDYT3CEW);


			DEFINET2(M_S7NLt2nQfj, EnumEmitterSpriteAnimationSelection);
			ADDFLAGS(M_S7NLt2nQfj, 0x8008);
			FIRSTMEM2(M_S7NLt2nQfj, mVal, EnumEmitterSpriteAnimationSelection, int, 0x40);
			FIRSTENUM2(M_S7NLt2nQfj, mVal, "eEmitterSpriteAnimationSelection_KeyControl01", M_3SL3XByImc, 4, 0);
			NEXTENUM2(M_S7NLt2nQfj, mVal, "eEmitterSpriteAnimationSelection_LinearStretch", M_iQTqVaAUnJ, 3, 0, M_3SL3XByImc);
			NEXTENUM2(M_S7NLt2nQfj, mVal, "eEmitterSpriteAnimationSelection_LinearLoop", M_vUmHVQGsTm, 2, 0, M_iQTqVaAUnJ);
			NEXTENUM2(M_S7NLt2nQfj, mVal, "eEmitterSpriteAnimationSelection_Random", M_vHeuSEBMSP, 1, 0, M_vUmHVQGsTm);
			NEXTMEM1(M_S7NLt2nQfj, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumEmitterSpriteAnimationSelection, enumbase, 0x10, mVal);
			ADD(M_S7NLt2nQfj);

			DEFINET2(M_h2Q6dQAjy4, Polar);
			FIRSTMEM2(M_h2Q6dQAjy4, mR, Polar, float, 0x0);
			NEXTMEM2(M_h2Q6dQAjy4, mTheta, Polar, float, 0x0, mR);
			NEXTMEM2(M_h2Q6dQAjy4, mPhi, Polar, float, 0x0, mTheta);
			ADD(M_h2Q6dQAjy4);

			DEFINET2(M_8arF3SLLny, EnlightenModule::EnumeProbeResolution);
			ADDFLAGS(M_8arF3SLLny, 0x8008);
			FIRSTMEM2(M_8arF3SLLny, mVal, EnlightenModule::EnumeProbeResolution, int, 0x40);
			FIRSTENUM2(M_8arF3SLLny, mVal, "1/8", M_30KKziHtRb, 3, 0);
			NEXTENUM2(M_8arF3SLLny, mVal, "1/4", M_OE2u8qvJP3, 2, 0, M_30KKziHtRb);
			NEXTENUM2(M_8arF3SLLny, mVal, "1/2", M_B6Uiu1gJ03, 1, 0, M_OE2u8qvJP3);
			NEXTENUM2(M_8arF3SLLny, mVal, "Full", M_OhmWvn7MIr, 0, 0, M_B6Uiu1gJ03);
			NEXTMEM1(M_8arF3SLLny, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeProbeResolution, enumbase, 0x10, mVal);
			ADD(M_8arF3SLLny);

			DEFINET2(M_KjsfSXF4YP, EnlightenModule::EnlightenSystemSettings);
			FIRSTMEM2(M_KjsfSXF4YP, mDefaultQuality, EnlightenModule::EnlightenSystemSettings, enlq, 0x0);
			NEXTMEM2(M_KjsfSXF4YP, mAdaptiveProbeResolution, EnlightenModule::EnlightenSystemSettings, M_8arF3SLLny, 0x0, mDefaultQuality);
			NEXTMEM2(M_KjsfSXF4YP, mDisableEnlighten, EnlightenModule::EnlightenSystemSettings, bool, 0x0, mAdaptiveProbeResolution);
			ADD(M_KjsfSXF4YP);

			DEFINET2(M_PaoaKk5lQn, T3LightEnvInternalData::QualityEntry);
			FIRSTMEM2(M_PaoaKk5lQn, mShadowLayer, T3LightEnvInternalData::QualityEntry, int, 0x0);
			NEXTMEM2(M_PaoaKk5lQn, mFlags, T3LightEnvInternalData::QualityEntry, flags, 0x0, mShadowLayer);
			ADD(M_PaoaKk5lQn);

			DEFINET2(M_9oDUTOX7NY, T3LightEnvInternalData);
			ADDFLAGS(M_9oDUTOX7NY, 0x20);
			FIRSTMEM1(M_9oDUTOX7NY, "mEntryForQuality[0]", a,mStationaryLightIndex, T3LightEnvInternalData, M_PaoaKk5lQn, 0x0);
			NEXTMEM1(M_9oDUTOX7NY, "mEntryForQuality[1]", b,mStationaryLightIndex,T3LightEnvInternalData, M_PaoaKk5lQn, 0x0, a);
			NEXTMEM1(M_9oDUTOX7NY, "mEntryForQuality[2]", c,mStationaryLightIndex,T3LightEnvInternalData, M_PaoaKk5lQn, 0x0, b);
			NEXTMEM1(M_9oDUTOX7NY, "mEntryForQuality[3]", d,mStationaryLightIndex,T3LightEnvInternalData, M_PaoaKk5lQn, 0x0, c);
			NEXTMEM2(M_9oDUTOX7NY, mStationaryLightIndex, T3LightEnvInternalData, int, 0x0, d);
			ADD(M_9oDUTOX7NY);

			DEFINET2(M_EkZgzUny1s, T3LightCinematicRigLOD);
			FIRSTMEM2(M_EkZgzUny1s, mFlags, T3LightCinematicRigLOD, flags, 0x40000);
			FIRSTFLAG(M_EkZgzUny1s, mFlags, "LOD Intensity on Low Quality", M_wObdYSEvWR, 0x40000);
			FIRSTFLAG(M_EkZgzUny1s, mFlags, "LOD Intensity on Medium Quality", M_BLzZ02h67Y, 0x20000);
			FIRSTFLAG(M_EkZgzUny1s, mFlags, "LOD Intensity on High Quality", M_rlvBndn4cX, 0x10000);
			ADD(M_EkZgzUny1s);

			DEFINET2(M_81nP7VxEE4, EnumT3MaterialNormalSpaceType);
			ADDFLAGS(M_81nP7VxEE4, 0x8008);
			FIRSTMEM2(M_81nP7VxEE4, mVal, EnumT3MaterialNormalSpaceType, int, 0x40);
			FIRSTENUM2(M_81nP7VxEE4, mVal, "eMaterialNormalSpace_World", M_FdgcL9fOPy, 1, 0);
			NEXTENUM2(M_81nP7VxEE4, mVal, "eMaterialNormalSpace_Tangent", M_c98bI89qTP, 0, 0, M_FdgcL9fOPy);
			NEXTMEM1(M_81nP7VxEE4, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3MaterialNormalSpaceType, enumbase, 0x10, mVal);
			ADD(M_81nP7VxEE4);

			DEFINET2(M_FtysL1Q4GT, EnumT3MaterialLODFullyRough);
			ADDFLAGS(M_FtysL1Q4GT, 0x8008);
			FIRSTMEM2(M_FtysL1Q4GT, mVal, EnumT3MaterialLODFullyRough, int, 0x40);
			FIRSTENUM2(M_FtysL1Q4GT, mVal, "eMaterialFullyRough_Always", M_OPwD7kkqPq, 3, 0);
			NEXTENUM2(M_FtysL1Q4GT, mVal, "eMaterialFullyRough_OnLowQuality", M_Q5PNEEfuTl, 2, 0, M_OPwD7kkqPq);
			NEXTENUM2(M_FtysL1Q4GT, mVal, "eMaterialFullyRough_OnLowestQuality", M_CH9WpIYaPH, 1, 0, M_Q5PNEEfuTl);
			NEXTENUM2(M_FtysL1Q4GT, mVal, "eMaterialFullyRough_Never", M_WfUTjlTOSj, 0, 0, M_CH9WpIYaPH);
			NEXTMEM1(M_FtysL1Q4GT, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3MaterialLODFullyRough, enumbase, 0x10, mVal);
			ADD(M_FtysL1Q4GT);

			DEFINET2(M_qiwdo3Hrdt, PathTo);
			FIRSTMEM2(M_qiwdo3Hrdt, mEnablePathing, PathTo, bool, 0x0);
			NEXTMEM2(M_qiwdo3Hrdt, mDistanceTraveled, PathTo, float, 0x0, mEnablePathing);
			NEXTMEM2(M_qiwdo3Hrdt, mMaxPathLength, PathTo, float, 0x0, mDistanceTraveled);
			NEXTMEM2(M_qiwdo3Hrdt, mRadius, PathTo, float, 0x0, mMaxPathLength);
			NEXTMEM2(M_qiwdo3Hrdt, mAvoidanceRadius, PathTo, float, 0x0, mRadius);
			NEXTMEM1(M_qiwdo3Hrdt, "mRadius", mRadius_Dup, mRadius, PathTo, float, 0x0, mAvoidanceRadius);
			NEXTMEM2(M_qiwdo3Hrdt, mUseDir, PathTo, vec3, 0x0, mRadius_Dup);
			ADD(M_qiwdo3Hrdt);

			DEFINET2(M_RpRVcwo6HM, PlaybackController);
			FIRSTMEM2(M_RpRVcwo6HM, mName, PlaybackController, symbol, 0x0);
			NEXTMEM2(M_RpRVcwo6HM, mLength, PlaybackController, float, 0x0, mName);
			NEXTMEM2(M_RpRVcwo6HM, mTime, PlaybackController, float, 0x0, mLength);
			NEXTMEM2(M_RpRVcwo6HM, mTimeScale, PlaybackController, float, 0x0, mTime);
			NEXTMEM2(M_RpRVcwo6HM, mContribution, PlaybackController, float, 0x0, mTimeScale);
			NEXTMEM2(M_RpRVcwo6HM, mSoundVolume, PlaybackController, float, 0x0, mContribution);
			NEXTMEM2(M_RpRVcwo6HM, mAdditiveMix, PlaybackController, float, 0x0, mSoundVolume);
			NEXTMEM2(M_RpRVcwo6HM, mFlags, PlaybackController, flags, 0x0, mAdditiveMix);
			NEXTMEM2(M_RpRVcwo6HM, mLoops, PlaybackController, int, 0x0, mFlags);
			NEXTMEM2(M_RpRVcwo6HM, mPriority, PlaybackController, int, 0x0, mLoops);
			ADD(M_RpRVcwo6HM);

			DEFINET2(M_ubpCzI71MR, EnumLightCellBlendMode);
			ADDFLAGS(M_ubpCzI71MR, 0x8008);
			FIRSTMEM2(M_ubpCzI71MR, mVal, EnumLightCellBlendMode, int, 0x40);
			FIRSTENUM2(M_ubpCzI71MR, mVal, "Overlay", M_VOgDko8fnP, 4, 0);
			NEXTENUM2(M_ubpCzI71MR, mVal, "Screen", M_YlapdvxlUs, 3, 0, M_VOgDko8fnP);
			NEXTENUM2(M_ubpCzI71MR, mVal, "Multiply", M_BH29Cm3ORY, 2, 0, M_YlapdvxlUs);
			NEXTENUM2(M_ubpCzI71MR, mVal, "Dodge", M_kfrjRZDdw6, 1, 0, M_BH29Cm3ORY);
			NEXTENUM2(M_ubpCzI71MR, mVal, "Normal", M_LcpX33e1IO, 0, 0, M_kfrjRZDdw6);
			NEXTMEM1(M_ubpCzI71MR, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumLightCellBlendMode, enumbase, 0x10, mVal);
			ADD(M_ubpCzI71MR);

			DEFINET2(M_wWR2uaAtZV, EnlightenModule::EnlightenLightSettings);
			FIRSTMEM2(M_wWR2uaAtZV, mEnlightenSaturation, EnlightenModule::EnlightenLightSettings, float, 0x0);
			NEXTMEM2(M_wWR2uaAtZV, mEnlightenOnly, EnlightenModule::EnlightenLightSettings, bool, 0x0, mEnlightenSaturation);
			NEXTMEM2(M_wWR2uaAtZV, mCastDynamicEnlightenShadows, EnlightenModule::EnlightenLightSettings, bool, 0x0, mEnlightenOnly);
			ADD(M_wWR2uaAtZV);

			DEFINET2(M_BQjgnZSCe8, Selectable);
			FIRSTMEM2(M_BQjgnZSCe8, mbGameSelectable, Selectable, bool, 0x0);
			NEXTMEM2(M_BQjgnZSCe8, mSelectionOrder, Selectable, int, 0x0, mbGameSelectable);
			NEXTMEM2(M_BQjgnZSCe8, mBoundingBox, Selectable, bbox, 0x0, mSelectionOrder);
			ADD(M_BQjgnZSCe8);

			DEFINET2(M_M9T0ci76Ro, EnumTextOrientationType);
			ADDFLAGS(M_M9T0ci76Ro, 0x8008);
			FIRSTMEM2(M_M9T0ci76Ro, mVal, EnumTextOrientationType, int, 0x40);
			FIRSTENUM2(M_M9T0ci76Ro, mVal, "WorldXYZ", M_jHkpC99IR3, 2, 0);
			NEXTENUM2(M_M9T0ci76Ro, mVal, "WorldZ", M_WsIMd3jKuW, 1, 0, M_jHkpC99IR3);
			NEXTENUM2(M_M9T0ci76Ro, mVal, "Screen", M_wPswERoC0n, 0, 0, M_WsIMd3jKuW);
			NEXTMEM1(M_M9T0ci76Ro, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumTextOrientationType, enumbase, 0x10, mVal);
			ADD(M_M9T0ci76Ro);

			DEFINET2(M_pyZdABIGpA, EnlightenModule::EnumeQualityWithDefault);
			ADDFLAGS(M_pyZdABIGpA, 0x8008);
			FIRSTMEM2(M_pyZdABIGpA, mVal, EnlightenModule::EnumeQualityWithDefault, int, 0x40);
			FIRSTENUM2(M_pyZdABIGpA, mVal, "Use Default", M_SsMZ6uUeHJ, 5, 0);
			NEXTENUM2(M_pyZdABIGpA, mVal, "Auto", M_j8Ut6sSUOT, 4, 0, M_SsMZ6uUeHJ);
			NEXTENUM2(M_pyZdABIGpA, mVal, "High", M_0ouihnKJGj, 3, 0, M_j8Ut6sSUOT);
			NEXTENUM2(M_pyZdABIGpA, mVal, "Medium", M_kvaPWolzch, 2, 0, M_0ouihnKJGj);
			NEXTENUM2(M_pyZdABIGpA, mVal, "Low", M_USmJqhTmfA, 1, 0, M_kvaPWolzch);
			NEXTENUM2(M_pyZdABIGpA, mVal, "Background", M_UKYckdKB9P, 0, 0, M_USmJqhTmfA);
			NEXTMEM1(M_pyZdABIGpA, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeQualityWithDefault, enumbase, 0x10, mVal);
			ADD(M_pyZdABIGpA);


			DEFINET2(M_pfr6SkbeR8, EnlightenModule::EnumeUpdateMethod);
			ADDFLAGS(M_pfr6SkbeR8, 0x8008);
			FIRSTMEM2(M_pfr6SkbeR8, mVal, EnlightenModule::EnumeUpdateMethod, int, 0x40);
			FIRSTENUM2(M_pfr6SkbeR8, mVal, "Auto", M_PKPlKtGBly, 2, 0);
			NEXTENUM2(M_pfr6SkbeR8, mVal, "Static", M_LUlctAOGXl, 1, 0, M_PKPlKtGBly);
			NEXTENUM2(M_pfr6SkbeR8, mVal, "Dynamic", M_dP7MVJKo22, 0, 0, M_LUlctAOGXl);
			NEXTMEM1(M_pfr6SkbeR8, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeUpdateMethod, enumbase, 0x10, mVal);
			ADD(M_pfr6SkbeR8);


			DEFINET2(M_zHcL8AII08, EnlightenModule::EnumeInstanceType);
			ADDFLAGS(M_zHcL8AII08, 0x8008);
			FIRSTMEM2(M_zHcL8AII08, mVal, EnlightenModule::EnumeInstanceType, int, 0x40);
			FIRSTENUM2(M_zHcL8AII08, mVal, "Disabled", M_n8YXDzJFnl, 5, 0);
			NEXTENUM2(M_zHcL8AII08, mVal, "Auto", M_NLYG35SqTG, 4, 0, M_n8YXDzJFnl);
			NEXTENUM2(M_zHcL8AII08, mVal, "Probe Radiosity", M_tifPRHmoGr, 3, 0, M_NLYG35SqTG);
			NEXTENUM2(M_zHcL8AII08, mVal, "Fully Dynamic", M_gKq6MbYsSX, 2, 0, M_tifPRHmoGr);
			NEXTENUM2(M_zHcL8AII08, mVal, "Radiosity", M_vy60MjweSb, 0, 0, M_gKq6MbYsSX);
			NEXTMEM1(M_zHcL8AII08, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeInstanceType, enumbase, 0x10, mVal);
			ADD(M_zHcL8AII08);

			DEFINET2(M_E0pAli9Mk6, EnlightenModule::EnlightenPrimitiveSettings);
			FIRSTMEM2(M_E0pAli9Mk6, mSystemId, EnlightenModule::EnlightenPrimitiveSettings, string, 0x0);
			NEXTMEM2(M_E0pAli9Mk6, mEnlightenLightingMode, EnlightenModule::EnlightenPrimitiveSettings, M_zHcL8AII08, 0x0, mSystemId);
			NEXTMEM2(M_E0pAli9Mk6, mEnlightenUpdateMethod, EnlightenModule::EnlightenPrimitiveSettings, M_pfr6SkbeR8, 0x0, mEnlightenLightingMode);
			NEXTMEM2(M_E0pAli9Mk6, mEnlightenQuality, EnlightenModule::EnlightenPrimitiveSettings, M_pyZdABIGpA, 0x0, mEnlightenUpdateMethod);
			ADD(M_E0pAli9Mk6);

			DEFINET2(M_K31tz14EbA, EnumT3LightEnvBakeOnStatic);
			ADDFLAGS(M_K31tz14EbA, 0x8008);
			FIRSTMEM2(M_K31tz14EbA, mVal, EnumT3LightEnvBakeOnStatic, int, 0x40);
			FIRSTENUM2(M_K31tz14EbA, mVal, "eLightEnvBakeOnStatic_NeverAllow", M_1KG3aMSFgD, 2, 0);
			NEXTENUM2(M_K31tz14EbA, mVal, "eLightEnvBakeOnStatic_AlwaysAllow", M_mnfMwafStM, 1, 0, M_1KG3aMSFgD);
			NEXTENUM2(M_K31tz14EbA, mVal, "eLightEnvBakeOnStatic_Default", M_icLMPCqabz, 0, 0, M_mnfMwafStM);
			NEXTMEM1(M_K31tz14EbA, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3LightEnvBakeOnStatic, enumbase, 0x10, mVal);
			ADD(M_K31tz14EbA);


			DEFINET2(M_tw5K1eD28K, EnumT3LightEnvLODBehavior);
			ADDFLAGS(M_tw5K1eD28K, 0x8008);
			FIRSTMEM2(M_tw5K1eD28K, mVal, EnumT3LightEnvLODBehavior, int, 0x40);
			FIRSTENUM2(M_tw5K1eD28K, mVal, "eLightEnvLOD_BakeOnly", M_in4mMxkcNZ, 1, 0);
			NEXTENUM2(M_tw5K1eD28K, mVal, "eLightEnvLOD_Disable", M_nWrc6hIXQd, 0, 0, M_in4mMxkcNZ);
			NEXTMEM1(M_tw5K1eD28K, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3LightEnvLODBehavior, enumbase, 0x10, mVal);
			ADD(M_tw5K1eD28K);


			DEFINET2(M_czX4fr0RMt, EnumT3LightEnvEnlightenBakeBehavior);
			ADDFLAGS(M_czX4fr0RMt, 0x8008);
			FIRSTMEM2(M_czX4fr0RMt, mVal, EnumT3LightEnvEnlightenBakeBehavior, int, 0x40);
			FIRSTENUM2(M_czX4fr0RMt, mVal, "eLightEnvEnlightenBake_Disable", M_ivVlvkI3S1, 2, 0);
			NEXTENUM2(M_czX4fr0RMt, mVal, "eLightEnvEnlightenBake_Enable", M_WSxzzthFYq, 1, 0, M_ivVlvkI3S1);
			NEXTENUM2(M_czX4fr0RMt, mVal, "eLightEnvEnlightenBake_Auto", M_GEdkVpeKgH, 0, 0, M_WSxzzthFYq);
			NEXTMEM1(M_czX4fr0RMt, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3LightEnvEnlightenBakeBehavior, enumbase, 0x10, mVal);
			ADD(M_czX4fr0RMt);


			DEFINET2(M_aJ5P1fJXE0, EnumDepthOfFieldType);
			ADDFLAGS(M_aJ5P1fJXE0, 0x8008);
			FIRSTMEM2(M_aJ5P1fJXE0, mVal, EnumDepthOfFieldType, int, 0x40);
			FIRSTENUM2(M_aJ5P1fJXE0, mVal, "eDepthOfFieldType_Brush", M_YMq1we9QL2, 2, 0);
			NEXTENUM2(M_aJ5P1fJXE0, mVal, "eDepthOfFieldType_Default", M_rKEs8yKDAV, 1, 0, M_YMq1we9QL2);
			NEXTMEM1(M_aJ5P1fJXE0, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumDepthOfFieldType, enumbase, 0x10, mVal);
			ADD(M_aJ5P1fJXE0);

			DEFINET2(M_Lq080gXW8z, IdleTransitionSettings);
			FIRSTMEM2(M_Lq080gXW8z, mTransitionTime, IdleTransitionSettings, float, 0x0);
			NEXTMEM2(M_Lq080gXW8z, mTransitionStyleDummy, IdleTransitionSettings, int, 0x0, mTransitionTime);
			ADD(M_Lq080gXW8z);

			DEFINET2(M_NKb4LTfLMM, IdleSlotDefaults);
			FIRSTMEM2(M_NKb4LTfLMM, mTransitionInDefault, IdleSlotDefaults, M_Lq080gXW8z, 0x0);
			NEXTMEM2(M_NKb4LTfLMM, mTransitionDefault, IdleSlotDefaults, M_Lq080gXW8z, 0x0, mTransitionInDefault);
			NEXTMEM2(M_NKb4LTfLMM, mTransitionOutDefault, IdleSlotDefaults, M_Lq080gXW8z, 0x0, mTransitionDefault);
			NEXTMEM2(M_NKb4LTfLMM, mBasePriority, IdleSlotDefaults, int, 0x0, mTransitionOutDefault);
			NEXTMEM2(M_NKb4LTfLMM, mSlotFriendlyName, IdleSlotDefaults, symbol, 0x0, mBasePriority);
			ADD(M_NKb4LTfLMM);

			DEFINET2(M_W5v6qCQNwW, T3MeshPropertyEntry);
			FIRSTMEM2(M_W5v6qCQNwW, mIncludeFilter, T3MeshPropertyEntry,string, 0x0);
			NEXTMEM2(M_W5v6qCQNwW, mExcludeFilter, T3MeshPropertyEntry,string, 0x0, mIncludeFilter);
			NEXTMEM2(M_W5v6qCQNwW, mhProperties, T3MeshPropertyEntry, Handlepropset, 0x0, mExcludeFilter);
			NEXTMEM2(M_W5v6qCQNwW, mPriority, T3MeshPropertyEntry, int, 0x0, mhProperties);
			ADD(M_W5v6qCQNwW);

			DEFINET2(M_aErAnGePtx, EnumHBAOPreset);
			ADDFLAGS(M_aErAnGePtx, 0x8008);
			FIRSTMEM2(M_aErAnGePtx, mVal, EnumHBAOPreset, int, 0x40);
			FIRSTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetUltra", M_5v3gYKbBkp, 7, 0);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetHigh", M_7iE8TLmAZu, 6, 0, M_5v3gYKbBkp);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetMedium", M_sXVJpKqtgP, 5, 0, M_7iE8TLmAZu);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetLow", M_AOg1vriJN0, 4, 0, M_sXVJpKqtgP);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetDisabled", M_4X29XdBh8j, 3, 0, M_AOg1vriJN0);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetPS4", M_0SCA0zKFZd, 2, 0, M_4X29XdBh8j);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetXBone", M_SKQc5eTRuv, 1, 0, M_0SCA0zKFZd);
			NEXTENUM2(M_aErAnGePtx, mVal, "eHBAOPresetFromTool", M_ykKPltKud5, 0, 0, M_SKQc5eTRuv);
			NEXTMEM1(M_aErAnGePtx, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAOPreset, enumbase, 0x10, mVal);
			ADD(M_aErAnGePtx);


			DEFINET2(M_EUWDutAgMV, EnumHBAODeinterleaving);
			ADDFLAGS(M_EUWDutAgMV, 0x8008);
			FIRSTMEM2(M_EUWDutAgMV, mVal, EnumHBAODeinterleaving, int, 0x40);
			FIRSTENUM2(M_EUWDutAgMV, mVal, "eHBAODeinterleaving_4x", M_svmUM03Ymj, 2, 0);
			NEXTENUM2(M_EUWDutAgMV, mVal, "eHBAODeinterleaving_2x", M_IqtYFpEFlj, 1, 0, M_svmUM03Ymj);
			NEXTENUM2(M_EUWDutAgMV, mVal, "eHBAODeinterleaving_Disabled", M_gwsA1Anevj, 0, 0, M_IqtYFpEFlj);
			NEXTMEM1(M_EUWDutAgMV, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAODeinterleaving, enumbase, 0x10, mVal);
			ADD(M_EUWDutAgMV);


			DEFINET2(M_PJC0d543tU, EnumHBAOQualityLevel);
			ADDFLAGS(M_PJC0d543tU, 0x8008);
			FIRSTMEM2(M_PJC0d543tU, mVal, EnumHBAOQualityLevel, int, 0x40);
			FIRSTENUM2(M_PJC0d543tU, mVal, "eHBAOQualityLevelHighest", M_YLjC0XVLuq, 4, 0);
			NEXTENUM2(M_PJC0d543tU, mVal, "eHBAOQualityLevelHigh", M_9CaGJhxWkh, 3, 0, M_YLjC0XVLuq);
			NEXTENUM2(M_PJC0d543tU, mVal, "eHBAOQualityLevelMedium", M_VYO3dJfBuC, 2, 0, M_9CaGJhxWkh);
			NEXTENUM2(M_PJC0d543tU, mVal, "eHBAOQualityLevelLow", M_ov1eETvOmd, 1, 0, M_VYO3dJfBuC);
			NEXTENUM2(M_PJC0d543tU, mVal, "eHBAOQualityLevelLowest", M_RXXNsyE4NH, 0, 0, M_ov1eETvOmd);
			NEXTMEM1(M_PJC0d543tU, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAOQualityLevel, enumbase, 0x10, mVal);
			ADD(M_PJC0d543tU);


			DEFINET2(M_RkTZCBldIk, EnumHBAOResolution);
			ADDFLAGS(M_RkTZCBldIk, 0x8008);
			FIRSTMEM2(M_RkTZCBldIk, mVal, EnumHBAOResolution, int, 0x40);
			FIRSTENUM2(M_RkTZCBldIk, mVal, "eHBAOResolutionQuarter", M_bSUz2k4XdH, 2, 0);
			NEXTENUM2(M_RkTZCBldIk, mVal, "eHBAOResolutionHalf", M_9VxSffSaIT, 1, 0, M_bSUz2k4XdH);
			NEXTENUM2(M_RkTZCBldIk, mVal, "eHBAOResolutionFull", M_vgEsjieOpx, 0, 0, M_9VxSffSaIT);
			NEXTMEM1(M_RkTZCBldIk, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAOResolution, enumbase, 0x10, mVal);
			ADD(M_RkTZCBldIk);


			DEFINET2(M_cfBn4tzYAB, EnumRenderAntialiasType);
			ADDFLAGS(M_cfBn4tzYAB, 0x8008);
			FIRSTMEM2(M_cfBn4tzYAB, mVal, EnumRenderAntialiasType, int, 0x40);
			FIRSTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_TAA", M_ythRDUSLUc, 9, 0);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_TAA_MSAA_8x", M_bCkFeXVwVQ, 8, 0, M_ythRDUSLUc);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_TAA_MSAA_4x", M_ZaOA2jb1ND, 7, 0, M_bCkFeXVwVQ);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_TAA_MSAA_2x", M_CX3vP20n39, 6, 0, M_ZaOA2jb1ND);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_MSAA_8x", M_X6hhgnz4Mf, 5, 0, M_CX3vP20n39);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_MSAA_4x", M_1it18OLKdtt, 4, 0, M_X6hhgnz4Mf);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_MSAA_2x", M_m9WgMaF0mJ, 3, 0, M_1it18OLKdtt);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_SMAA", M_KATWd43Ofl, 2, 0, M_m9WgMaF0mJ);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_FXAA", M_AQsoC7WO95, 1, 0, M_KATWd43Ofl);
			NEXTENUM2(M_cfBn4tzYAB, mVal, "eRenderAntialias_None", M_efK2h3B98k, 0, 0, M_AQsoC7WO95);
			NEXTMEM1(M_cfBn4tzYAB, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumRenderAntialiasType, enumbase, 0x10, mVal);
			ADD(M_cfBn4tzYAB);


			DEFINET2(M_TDHqyzKXoJ, EnumRenderTAAJitterType);
			ADDFLAGS(M_TDHqyzKXoJ, 0x8008);
			FIRSTMEM2(M_TDHqyzKXoJ, mVal, EnumRenderTAAJitterType, int, 0x40);
			FIRSTENUM2(M_TDHqyzKXoJ, mVal, "eRenderTAAJitter_Hammersley8x", M_MFchjR1wTY, 4, 0);
			NEXTENUM2(M_TDHqyzKXoJ, mVal, "eRenderTAAJitter_Hammersley4x", M_I67ekrUydL, 3, 0, M_MFchjR1wTY);
			NEXTENUM2(M_TDHqyzKXoJ, mVal, "eRenderTAAJitter_Uniform2x", M_RJBUDZXXFy, 2, 0, M_I67ekrUydL);
			NEXTENUM2(M_TDHqyzKXoJ, mVal, "eRenderTAAJitter_None", M_xfRWIz9RwJ, 1, 0, M_RJBUDZXXFy);
			NEXTMEM1(M_TDHqyzKXoJ, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumRenderTAAJitterType, enumbase, 0x10, mVal);
			ADD(M_TDHqyzKXoJ);


			DEFINET2(M_akyvPmMoLB, EnumDOFQualityLevel);
			ADDFLAGS(M_akyvPmMoLB, 0x8008);
			FIRSTMEM2(M_akyvPmMoLB, mVal, EnumDOFQualityLevel, int, 0x40);
			FIRSTENUM2(M_akyvPmMoLB, mVal, "eDOFQualityLevelHigh", M_CRyp0qZbXH, 3, 0);
			NEXTENUM2(M_akyvPmMoLB, mVal, "eDOFQualityLevelMedium", M_jtanzWWpCm, 2, 0, M_CRyp0qZbXH);
			NEXTENUM2(M_akyvPmMoLB, mVal, "eDOFQualityLevelLow", M_fLf8hgMcVO, 1, 0, M_jtanzWWpCm);
			NEXTENUM2(M_akyvPmMoLB, mVal, "eDofQualityLevelDisabled", M_xm7E0c9GUa, 0, 0, M_fLf8hgMcVO);
			NEXTMEM1(M_akyvPmMoLB, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumDOFQualityLevel, enumbase, 0x10, mVal);
			ADD(M_akyvPmMoLB);


			DEFINET2(M_8m6xqqEZ1E, EnumBokehQualityLevel);
			ADDFLAGS(M_8m6xqqEZ1E, 0x8008);
			FIRSTMEM2(M_8m6xqqEZ1E, mVal, EnumBokehQualityLevel, int, 0x40);
			FIRSTENUM2(M_8m6xqqEZ1E, mVal, "eBokehQualityLevelHigh", M_NaQPFKnSJu, 3, 0);
			NEXTENUM2(M_8m6xqqEZ1E, mVal, "eBokehQualityLevelMedium", M_hqhH2Wlcvy, 2, 0, M_NaQPFKnSJu);
			NEXTENUM2(M_8m6xqqEZ1E, mVal, "eBokehQualityLevelLow", M_L4MAeQXggM, 1, 0, M_hqhH2Wlcvy);
			NEXTENUM2(M_8m6xqqEZ1E, mVal, "eBokehQualityLevelDisabled", M_nn79XAVJRr, 0, 0, M_L4MAeQXggM);
			NEXTMEM1(M_8m6xqqEZ1E, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumBokehQualityLevel, enumbase, 0x10, mVal);
			ADD(M_8m6xqqEZ1E);


			DEFINET2(M_z1uBdbraRv, EnumGlowQualityLevel);
			ADDFLAGS(M_z1uBdbraRv, 0x8008);
			FIRSTMEM2(M_z1uBdbraRv, mVal, EnumGlowQualityLevel, int, 0x40);
			FIRSTENUM2(M_z1uBdbraRv, mVal, "eGlowQualityLevelHigh", M_C4iU1b6GvS, 3, 0);
			NEXTENUM2(M_z1uBdbraRv, mVal, "eGlowQualityLevelMedium", M_1TiZMqvUrw, 2, 0, M_C4iU1b6GvS);
			NEXTENUM2(M_z1uBdbraRv, mVal, "eGlowQualityLevelLow", M_cY7RJCXTNI, 1, 0, M_1TiZMqvUrw);
			NEXTENUM2(M_z1uBdbraRv, mVal, "eGlowQualityLevelOld", M_fkulpeHgd1, 0, 0, M_cY7RJCXTNI);
			NEXTMEM1(M_z1uBdbraRv, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumGlowQualityLevel, enumbase, 0x10, mVal);
			ADD(M_z1uBdbraRv);


			DEFINET2(M_xYctR9RF3Y, EnumBokehOcclusionType);
			ADDFLAGS(M_xYctR9RF3Y, 0x8008);
			FIRSTMEM2(M_xYctR9RF3Y, mVal, EnumBokehOcclusionType, int, 0x40);
			FIRSTENUM2(M_xYctR9RF3Y, mVal, "eBokehOcclusionZTest", M_xVeBJKYQu7, 3, 0);
			NEXTENUM2(M_xYctR9RF3Y, mVal, "eBokehOcclusionZTestAndScaleOccluded", M_A0XTaehlb8, 1, 0, M_xVeBJKYQu7);
			NEXTENUM2(M_xYctR9RF3Y, mVal, "eBokehOcclusionScaleOccluded", M_tRbhXVz9Az, 2, 0, M_A0XTaehlb8);
			NEXTENUM2(M_xYctR9RF3Y, mVal, "eBokehOcclusionDisabled", M_1I6BDvjwRg, 0, 0, M_tRbhXVz9Az);
			NEXTMEM1(M_xYctR9RF3Y, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumBokehOcclusionType, enumbase, 0x10, mVal);
			ADD(M_xYctR9RF3Y);

			DEFINEMAP2(String, Map<String SEP String SEP std::less<String>>, str, mss, std::less<String>);

			DEFINET2(M_w6eX3aKWTS, Localization::Language);
			FIRSTMEM2(M_w6eX3aKWTS, mName, Localization::Language, string, 0x0);
			NEXTMEM2(M_w6eX3aKWTS, mDisplayText, Localization::Language, string, 0x0, mName);
			NEXTMEM2(M_w6eX3aKWTS, mPlatformToSyncFSLocation, Localization::Language, Map_String_String, 0x0, mDisplayText);
			NEXTMEM2(M_w6eX3aKWTS, mPlatformToSubgroupToAudioSyncFSLocation, Localization::Language, Map_str_mss, 0x0, mPlatformToSyncFSLocation);
			NEXTMEM2(M_w6eX3aKWTS, mSubgroupToResourceSetName, Localization::Language, Map_String_String, 0x0, mPlatformToSubgroupToAudioSyncFSLocation);
			NEXTMEM2(M_w6eX3aKWTS, mAudioResourceSetName, Localization::Language, string, 0x0, mSubgroupToResourceSetName);
			NEXTMEM2(M_w6eX3aKWTS, mIndex, Localization::Language, int, 0x0, mAudioResourceSetName);
			NEXTMEM2(M_w6eX3aKWTS, mVersionNumber, Localization::Language, int, 0x0, mIndex);
			NEXTMEM2(M_w6eX3aKWTS, mAudioVersionNumber, Localization::Language, int, 0x0, mVersionNumber);
			NEXTMEM2(M_w6eX3aKWTS, mFlags, Localization::Language, flags, 0x40000, mAudioVersionNumber);
			FIRSTFLAG(M_w6eX3aKWTS, mFlags, "Bidirectional", M_qZTqOpaZDA, 8);
			FIRSTFLAG(M_w6eX3aKWTS, mFlags, "Right To Left", M_qyRvJykTwF, 1);
			FIRSTFLAG(M_w6eX3aKWTS, mFlags, "NOT USED", M_n3VsamFkqU, 2);
			FIRSTFLAG(M_w6eX3aKWTS, mFlags, "Installed", M_1BcDuG07mj, 4);
			ADD(M_w6eX3aKWTS);
			
			DEFINESET_(String SEP StringCompareCaseInsensitive, stri);

			DEFINET2(M_jq873tLM70, ChorecorderParameters);
			FIRSTMEM2(M_jq873tLM70, mFlags, ChorecorderParameters, flags, 0x40000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Allow look-At animation", M_Rm3tBHYf2F, 8000000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Allow phoneme animation", M_pMSsvHgod6, 4000000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Resource Out Facial Animation", M_GX3LkY8vbd, 2000000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Add Anims to Both Chore and New Cut", M_uFDFDHnZw3, 1000000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Dynamics Hair Only", M_L1gm3H3VlG, 800000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Unity Maya Export", M_aUSvXonVZY, 40000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Unity Export As", M_jXaRgVNcJN, 200000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Unity Export", M_zs08I7m4ba, 20000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Auto Select Agents", M_YXjmxEHysO, 10000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Include All Except Dynamics", M_ozZhzLj2hX, 8000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Skip recording inverse kinematics", M_yhlbqHeXKI, 4000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Show all chore agents in selection list", M_YpOhzGZdGT, 2000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Treat agents as visible if they are only visible in a shadow view", M_zkPJe5AhAz, 1000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Only record visible agents (for each cut)", M_vRdS9AaopP, 800);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Extend time keys to the start and end of each block", M_bPCxcxwTJI, 400);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Set new cut as Master", M_K38Dwmj69I, 200);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Add Anims To Chore", M_K6KVxSN2yF, 100);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Add Camera Anims to Chore", M_0SCl9nTdej, 80);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Dynamics Only", M_MgZRlccTGX, 40);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Edit Jira Polish Task", M_8vuKrACuTi, 100000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Make Jira Polish Task", M_9DM5uXp9fE, 20);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Check in files", M_KZABTNEQDg, 10);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Split Anims At Guides", M_sYmP69ydQs, 80000);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Split Anims At Camera Cuts", M_KCjSE1FtVo, 8);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Record Camera", M_sqEsoZncwB, 4);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Create New Cut", M_ZWwjV3ZoDN, 2);
			FIRSTFLAG(M_jq873tLM70, mFlags, "Select Agents to Chorecord", M_Jq8VJQUEOL, 1);
			NEXTMEM2(M_jq873tLM70, mhDialogChore, ChorecorderParameters, Handlehchore, 0x20, mFlags);
			NEXTMEM2(M_jq873tLM70, mhCutChore, ChorecorderParameters, Handlehchore, 0x20, mhDialogChore);
			NEXTMEM2(M_jq873tLM70, mNewCutName, ChorecorderParameters, string, 0x0, mhCutChore);
			NEXTMEM2(M_jq873tLM70, mCameraAgentToUse, ChorecorderParameters, string, 0x0, mNewCutName);
			NEXTMEM2(M_jq873tLM70, mAgentsToChorecord, ChorecorderParameters, Set_stri, 0x20, mCameraAgentToUse);
			NEXTMEM2(M_jq873tLM70, mChorecordingName, ChorecorderParameters, string, 0x0, mAgentsToChorecord);
			ADD(M_jq873tLM70);

			DEFINEDCARRAY2(int, darray_int);
			MKNAME(meta_DCArray_darray_int, "DArray<int>");

			DEFINEDCARRAY2(bool, darray_bool);
			MKNAME(meta_DCArray_darray_bool, "DArray<bool>");

			DEFINET2(M_R0FvWCDz06, VoiceData);
			EXT(M_R0FvWCDz06, vox);
			FIRSTMEM2(M_R0FvWCDz06, mbEncrypted, VoiceData, bool, 0x0);
			NEXTMEM2(M_R0FvWCDz06, mLength, VoiceData, float, 0x0, mbEncrypted);
			NEXTMEM2(M_R0FvWCDz06, mAllPacketsSize, VoiceData, int, 0x0, mLength);
			NEXTMEM2(M_R0FvWCDz06, mPacketSamples, VoiceData, int, 0x0, mAllPacketsSize);
			NEXTMEM2(M_R0FvWCDz06, mSampleRate, VoiceData, int, 0x0, mPacketSamples);
			NEXTMEM2(M_R0FvWCDz06, mMode, VoiceData,int, 0x0, mSampleRate);
			NEXTMEM2(M_R0FvWCDz06, mPacketPositions, VoiceData, DCArray_darray_int, 0x0, mMode);
			ADD(M_R0FvWCDz06);

			DEFINET2(M_yo4opxOK84, StyleGuideRef);
			FIRSTMEM2(M_yo4opxOK84, mhStyleGuide, StyleGuideRef, Handlestyle, 0x0);
			NEXTMEM2(M_yo4opxOK84, mPaletteClassUID, StyleGuideRef, int, 0x0, mhStyleGuide);
			NEXTMEM2(M_yo4opxOK84, mPaletteClassGroupUID, StyleGuideRef, int, 0x0, mPaletteClassUID);
			NEXTMEM2(M_yo4opxOK84, mOverridden, StyleGuideRef, bool, 0x20, mPaletteClassGroupUID);
			NEXTMEM2(M_yo4opxOK84, mPaletteClassIndex, StyleGuideRef, int, 0x20, mOverridden);
			NEXTMEM2(M_yo4opxOK84, mPalettesUsed, StyleGuideRef, DCArray_darray_bool, 0x20, mPaletteClassIndex);
			ADD(M_yo4opxOK84);

			DEFINET2(M_fta7oiKuQl, SoundEventBankDummy);
			ADDFLAGS(M_fta7oiKuQl, 0x6000000);/*is not meta file, dont async load*/
			EXT(M_fta7oiKuQl, bank);
			ADD(M_fta7oiKuQl);

			DEFINET2(agi, SaveGame::AgentInfo);
			FIRSTMEM2(agi, mAgentName, SaveGame::AgentInfo, symbol, 0);
			NEXTMEM2(agi, mSceneName, SaveGame::AgentInfo, symbol, 0, mAgentName);
			NEXTMEM2(agi, mPosition, SaveGame::AgentInfo, vec3, 0, mSceneName);
			NEXTMEM2(agi, mQuaternion, SaveGame::AgentInfo, quat, 0, mPosition);
			NEXTMEM2(agi, mbAttached, SaveGame::AgentInfo, bool, 0, mQuaternion);
			NEXTMEM2(agi, mAttachedToAgent, SaveGame::AgentInfo, symbol, 0, mbAttached);
			NEXTMEM2(agi, mAttachedToNode, SaveGame::AgentInfo, symbol, 0, mAttachedToAgent);
			ADD(agi);

			DEFINEDCARRAY2(SaveGame::AgentInfo, savegameagi);

			DEFINET2(M_KNVH7Wfi1z, SaveGame);
			EXT(M_KNVH7Wfi1z, save);
			FIRSTMEM2(M_KNVH7Wfi1z, mLuaDoFile, SaveGame, string, 0x0);
			NEXTMEM2(M_KNVH7Wfi1z, mAgentInfo, SaveGame, DCArray_savegameagi, 0x0, mLuaDoFile);
			NEXTMEM2(M_KNVH7Wfi1z, mRuntimePropNames, SaveGame, Set_symbol, 0x0, mAgentInfo);
			NEXTMEM2(M_KNVH7Wfi1z, mEnabledDynamicSets, SaveGame, Set_symbol, 0x0, mRuntimePropNames);
			ADD(M_KNVH7Wfi1z);

			DEFINET2(M_oV5Y4fYPkS, PointOfInterestBlocking);
			ADDFLAGS(M_oV5Y4fYPkS, 0x100000);//memberless
			EXT(M_oV5Y4fYPkS, poiblocking);
			ADD(M_oV5Y4fYPkS);

			DEFINET2(M_4jF7ZYNRVd, BlendCameraResource);
			ADDFLAGS(M_4jF7ZYNRVd, 0x100000);//memberless
			EXT(M_4jF7ZYNRVd, blendcameraresource);
			ADD(M_4jF7ZYNRVd);

			DEFINET2(M_crqCQMsVUX, CameraSelect);
			EXT(M_crqCQMsVUX, cameraselectresource);
			FIRSTMEM2(M_crqCQMsVUX, mCameraNames, CameraSelect, DCArray_Symbol, 0x0);
			ADD(M_crqCQMsVUX);

			DEFINET2(M_eGjxoXjAGx, EnumHBAOPerPixelNormals);
			ADDFLAGS(M_eGjxoXjAGx, 0x8008);
			FIRSTMEM2(M_eGjxoXjAGx, mVal, EnumHBAOPerPixelNormals, int, 0x40);
			FIRSTENUM2(M_eGjxoXjAGx, mVal, "eHBAOPerPixelNormalsReconstructed", M_tI8m19vmCZ, 1, 0);
			NEXTENUM2(M_eGjxoXjAGx, mVal, "eHBAOPerPixelNormalsGBuffer", M_I9YZU23lIc, 0, 0, M_tI8m19vmCZ);
			NEXTMEM1(M_eGjxoXjAGx, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAOPerPixelNormals, enumbase, 0x10, mVal);
			ADD(M_eGjxoXjAGx);


			DEFINET2(M_1H1JyHaoYL, EnumHBAOBlurQuality);
			ADDFLAGS(M_1H1JyHaoYL, 0x8008);
			FIRSTMEM2(M_1H1JyHaoYL, mVal, EnumHBAOBlurQuality, int, 0x40);
			FIRSTENUM2(M_1H1JyHaoYL, mVal, "eHBAOBlurQualityExtraWide", M_HGNfzs4hiV, 4, 0);
			NEXTENUM2(M_1H1JyHaoYL, mVal, "eHBAOBlurQualityWide", M_6eYBQFaJ81, 3, 0, M_HGNfzs4hiV);
			NEXTENUM2(M_1H1JyHaoYL, mVal, "eHBAOBlurQualityMedium", M_eyTn3iSAM5, 2, 0, M_6eYBQFaJ81);
			NEXTENUM2(M_1H1JyHaoYL, mVal, "eHBAOBlurQualityNarrow", M_r4R3MBFj8v, 1, 0, M_eyTn3iSAM5);
			NEXTENUM2(M_1H1JyHaoYL, mVal, "eHBAOBlurQualityNone", M_RrqVxLAOy9, 0, 0, M_r4R3MBFj8v);
			NEXTMEM1(M_1H1JyHaoYL, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumHBAOBlurQuality, enumbase, 0x10, mVal);
			ADD(M_1H1JyHaoYL);

			DEFINET2(M_9exXT5bbbv, TextAlignmentType);
			FIRSTMEM2(M_9exXT5bbbv, mAlignmentType, TextAlignmentType, int, 0x40);
			FIRSTENUM2(M_9exXT5bbbv, mAlignmentType, "Bottom", M_HYEMyEWq1n, 32, 0);
			NEXTENUM2(M_9exXT5bbbv, mAlignmentType, "Middle", M_8pIK3y8Peb, 16, 0, M_HYEMyEWq1n);
			NEXTENUM2(M_9exXT5bbbv, mAlignmentType, "Top", M_TkARVZR0Ds, 8, 0, M_8pIK3y8Peb);
			NEXTENUM2(M_9exXT5bbbv, mAlignmentType, "Right Justified", M_hPvYlRIDNg, 4, 0, M_TkARVZR0Ds);
			NEXTENUM2(M_9exXT5bbbv, mAlignmentType, "Centered", M_IaW8hqeueI, 2, 0, M_hPvYlRIDNg);
			NEXTENUM2(M_9exXT5bbbv, mAlignmentType, "Left Justified", M_FFc8Ah5Qrf, 1, 0, M_IaW8hqeueI);
			NEXTENUM2(M_9exXT5bbbv, mAlignmentType, "None", M_APnUt1fm1j, 0, 0, M_FFc8Ah5Qrf);
			ADD(M_9exXT5bbbv);

			DEFINET2(M_omYqLMlfqg, T3RenderStateBlock);
			FIRSTMEM2(M_omYqLMlfqg, mData, T3RenderStateBlock, sarray_u32_3, 0x0);
			ADD(M_omYqLMlfqg);

			DEFINET2(M_zwzAENDGJV, EnumT3MaterialSwizzleType);
			ADDFLAGS(M_zwzAENDGJV, 0x8008);
			FIRSTMEM2(M_zwzAENDGJV, mVal, EnumT3MaterialSwizzleType, int, 0x40);
			FIRSTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_One", M_m9kFydOVVq, 6, 0);
			NEXTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_Zero", M_0pUZ63bMA4, 5, 0, M_m9kFydOVVq);
			NEXTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_W", M_rciLwRDfos, 4, 0, M_0pUZ63bMA4);
			NEXTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_Z", M_pupEkLJQ65, 3, 0, M_rciLwRDfos);
			NEXTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_Y", M_rN8p5O7p6U, 2, 0, M_pupEkLJQ65);
			NEXTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_X", M_MlRxwQ0gpL, 1, 0, M_rN8p5O7p6U);
			NEXTENUM2(M_zwzAENDGJV, mVal, "eMaterialSwizzle_None", M_RWaNEKvGkm, 0, 0, M_MlRxwQ0gpL);
			NEXTMEM1(M_zwzAENDGJV, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3MaterialSwizzleType,enumbase, 0x10, mVal);
			ADD(M_zwzAENDGJV);

			DEFINET2(M_oTPRr0e6FA, T3MaterialSwizzleParams);
			FIRSTMEM1(M_oTPRr0e6FA, "mSwizzle[0]", mSwizzle0, __reserve, T3MaterialSwizzleParams, M_zwzAENDGJV, 0x0);
			NEXTMEM1(M_oTPRr0e6FA, "mSwizzle[1]", mSwizzle1, __reserve, T3MaterialSwizzleParams, M_zwzAENDGJV, 0x0, mSwizzle0);
			NEXTMEM1(M_oTPRr0e6FA, "mSwizzle[2]", mSwizzle2, __reserve, T3MaterialSwizzleParams, M_zwzAENDGJV, 0x0, mSwizzle1);
			NEXTMEM1(M_oTPRr0e6FA, "mSwizzle[3]", mSwizzle3, __reserve, T3MaterialSwizzleParams, M_zwzAENDGJV, 0x0, mSwizzle2);
			ADD(M_oTPRr0e6FA);

			DEFINET2(M_FHZl1F6yKW, StyleIdleTransitionsRes);
			EXT(M_FHZl1F6yKW, trans);
			FIRSTMEM2(M_FHZl1F6yKW, mOwningAgent, StyleIdleTransitionsRes, string, 0x1);
			NEXTMEM2(M_FHZl1F6yKW, mGuideName, StyleIdleTransitionsRes, string, 0x0, mOwningAgent);
			ADD(M_FHZl1F6yKW);

			DEFINET2(M_N9wGTC2s6a, Scene::AgentQualitySettings);
			FIRSTMEM2(M_N9wGTC2s6a, mFlags, Scene::AgentQualitySettings, flags, 0x40000);
			FIRSTFLAG(M_N9wGTC2s6a, mFlags, "Exclude from Low Quality Builds", M_atIYHgVdPe, 1);
			ADD(M_N9wGTC2s6a);

			DEFINET2(M_dIoA7DDHLh, StringFilter);
			FIRSTMEM2(M_dIoA7DDHLh, mFilterStrings, StringFilter, Set_String, 0x0);
			ADD(M_dIoA7DDHLh);

			DEFINET2(sbe, SoundBankWaveMapEntry);
			FIRSTMEM2(sbe, fLengthSeconds, SoundBankWaveMapEntry, float, 0);
			NEXTMEM2(sbe, strFileName, SoundBankWaveMapEntry, string, 0, fLengthSeconds);
			ADD(sbe);

			DEFINEMAP(Symbol, SoundBankWaveMapEntry, std::less<Symbol>);

			DEFINET2(M_S46TFudBBK, SoundBankWaveMap);
			EXT(M_S46TFudBBK, bankwavemap);
			FIRSTMEM2(M_S46TFudBBK, mWaveMap, SoundBankWaveMap, Map_Symbol_SoundBankWaveMapEntry, 0x0);
			ADD(M_S46TFudBBK);

			DEFINET2(M_zHOTJcJwef, SoundBusSnapshot::Snapshot);
			EXT(M_zHOTJcJwef, bussnapshot);
			FIRSTMEM2(M_zHOTJcJwef, mfFadeTime, SoundBusSnapshot::Snapshot, float, 0x0);
			NEXTMEM2(M_zHOTJcJwef, mBusVolumes, SoundBusSnapshot::Snapshot, Map_Symbol_float, 0x0, mfFadeTime);
			ADD(M_zHOTJcJwef);

			DEFINEMAP2(Symbol, Handle<SoundBusSnapshot::Snapshot>, Symbol, Hsnap, std::less<Symbol>);

			DEFINET2(M_a5jiZPnCED, SoundBusSnapshot::SnapshotSuite);
			EXT(M_a5jiZPnCED, snapshotsuite);
			FIRSTMEM2(M_a5jiZPnCED, mSnapshotMap, SoundBusSnapshot::SnapshotSuite, Map_Symbol_Hsnap, 0x0);
			ADD(M_a5jiZPnCED);

			DEFINEHANDLE(snapss, SoundBusSnapshot::Snapshot);

			DEFINET2(M_PBkwzFeaKQ, ProceduralEyes);
			EXT(M_PBkwzFeaKQ, eyes);
			FIRSTMEM1(M_PBkwzFeaKQ, "Baseclass_Animation", Baseclass_Animation, __reserve, ProceduralEyes, anm, 0x10);
			ADD(M_PBkwzFeaKQ);

			DEFINET2(M_MEwGusP3Sj, PhysicsObject);
			FIRSTMEM2(M_MEwGusP3Sj, mbEnabledPropertyOn, PhysicsObject, bool, 0x0);
			NEXTMEM2(M_MEwGusP3Sj, mCollisionType, PhysicsObject, int, 0x40, mbEnabledPropertyOn);
			FIRSTENUM2(M_MEwGusP3Sj, mCollisionType, "Raycast", M_wgyDrgY9Tc, 1, 0);
			NEXTENUM2(M_MEwGusP3Sj, mCollisionType, "InterAgent", M_X1dY3RIctv, 0, 0, M_wgyDrgY9Tc);
			NEXTMEM2(M_MEwGusP3Sj, mBoundingVolumeType, PhysicsObject, int, 0x40, mCollisionType);
			FIRSTENUM2(M_MEwGusP3Sj, mBoundingVolumeType, "Sphere", M_UY16XSOiiZ, 2, 0);
			NEXTENUM2(M_MEwGusP3Sj, mBoundingVolumeType, "Box", M_qTo9VxlSDa, 1, 0, M_UY16XSOiiZ);
			NEXTENUM2(M_MEwGusP3Sj, mBoundingVolumeType, "Cylinder", M_EFPgTs79ax, 0, 0, M_qTo9VxlSDa);
			NEXTMEM2(M_MEwGusP3Sj, mfBoundingVolumeScalingFactor, PhysicsObject, float, 0x0, mBoundingVolumeType);
			ADD(M_MEwGusP3Sj);

			DEFINET2(M_7uR5QwN3gc, PhysicsData);
			EXT(M_7uR5QwN3gc, t3bullet);
			FIRSTMEM2(M_7uR5QwN3gc, mDataBuffer, PhysicsData, bb, 0x0);
			ADD(M_7uR5QwN3gc);

			DEFINET2(crf, ClipResourceFilter);
			FIRSTMEM2(crf, mResources, ClipResourceFilter, Set_symbol, 0);
			NEXTMEM2(crf, mbExclusiveMode, ClipResourceFilter, bool, 0, mResources);
			ADD(crf);
	
			DEFINEMAP(String, ClipResourceFilter, StringCompareCaseInsensitive);
			
			DEFINET2(M_ifOpKg0RM3, PerAgentClipResourceFilter);
			FIRSTMEM2(M_ifOpKg0RM3, mIncludedAgents, PerAgentClipResourceFilter, Map_String_ClipResourceFilter, 0x0);
			NEXTMEM2(M_ifOpKg0RM3, mExcludedAgents, PerAgentClipResourceFilter, Set_stri, 0x0, mIncludedAgents);
			NEXTMEM2(M_ifOpKg0RM3, mbExclusiveMode, PerAgentClipResourceFilter, bool, 0x0, mExcludedAgents);
			ADD(M_ifOpKg0RM3);

			DEFINET2(M_RXuN3aKtr6, ParticlePropConnect);
			FIRSTMEM2(M_RXuN3aKtr6, mDriveType, ParticlePropConnect, int, 0x40);
			FIRSTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_DistanceToCamera", M_0OvEXdaEYS, 9, 0);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_KeyControl04", M_8fsa4aLEli, 8, 0, M_0OvEXdaEYS);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_KeyControl03", M_2Yl4L4vSa6, 7, 0, M_8fsa4aLEli);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_KeyControl02", M_xKTm1QJJ0A, 6, 0, M_2Yl4L4vSa6);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_KeyControl01", M_iMpIzXoasw, 5, 0, M_xKTm1QJJ0A);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_CameraDot", M_tGzU6wy9iU, 4, 0, M_iMpIzXoasw);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_BurstTime", M_xz2Y2fKO8c, 3, 0, M_tGzU6wy9iU);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_DistanceToTarget", M_7WYFWJhnUq, 2, 0, M_xz2Y2fKO8c);
			NEXTENUM2(M_RXuN3aKtr6, mDriveType, "ePartPropDriver_EmitterSpeed", M_Un6TDQvVpD, 1, 0, M_7WYFWJhnUq);
			NEXTMEM2(M_RXuN3aKtr6, mDriveMin, ParticlePropConnect, float, 0x0, mDriveType);
			NEXTMEM2(M_RXuN3aKtr6, mDriveMax, ParticlePropConnect, float, 0x0, mDriveMin);
			NEXTMEM2(M_RXuN3aKtr6, mModType, ParticlePropConnect, int, 0x40, mDriveMax);
			FIRSTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Enable", M_lfyXC9nuPU, 28, 0);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Geometry_Scale", M_ABJkOmfySd, 27, 0, M_lfyXC9nuPU);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_Alpha", M_sZHXrZ3S2G, 26, 0, M_ABJkOmfySd);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_KeyControl04", M_U59vVJVXO6, 25, 0, M_sZHXrZ3S2G);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_KeyControl03", M_aMx3QRb0xN, 24, 0, M_U59vVJVXO6);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_KeyControl02", M_2pI1y7pEgB, 23, 0, M_aMx3QRb0xN);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_KeyControl01", M_pN6u9bEKwO, 22, 0, M_2pI1y7pEgB);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Velocity_Timescale_Modifier", M_76uAIrnHfc, 21, 0, M_pN6u9bEKwO);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Velocity_Turbulence_Speed", M_FYWeSgMJM0, 20, 0, M_76uAIrnHfc);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Velocity_Turbulence_Force", M_8iYfRYDLB9, 19, 0, M_FYWeSgMJM0);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Target_Render_Lerp", M_RxG5WLsE9f, 18, 0, M_8iYfRYDLB9);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Spawn_Volume_Sweep_Offset", M_6ebsOTWbp8, 17, 0, M_RxG5WLsE9f);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Spawn_Volume_Sweep", M_XIDgvWpgXG, 16, 0, M_6ebsOTWbp8);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Spawn_Angle", M_ELxdEQOmio, 15, 0, M_XIDgvWpgXG);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Time_Scale", M_gcVQq7sZe2, 12, 0, M_ELxdEQOmio);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_Intensity", M_nsaw4Ee2fg, 11, 0, M_gcVQq7sZe2);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_Speed", M_Wq21JU2Idl, 10, 0, M_nsaw4Ee2fg);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Sprite_Animation_Cycles", M_6E5F6wvy5J, 14, 0, M_Wq21JU2Idl);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Sprite_Animation_Rate", M_vKJa2djXrv, 13, 0, M_6E5F6wvy5J);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_RotationSpeed", M_vFDy6HLhk9, 9, 0, M_vKJa2djXrv);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_Rotation", M_1Rggxp9WW1, 8, 0, M_vFDy6HLhk9);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_Lifespan", M_wYsVx5W0yG, 7, 0, M_1Rggxp9WW1);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_PP_Scale", M_83ALuOHiI6, 6, 0, M_wYsVx5W0yG);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Max_Particles", M_yefMSqsKTl, 5, 0, M_83ALuOHiI6);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Global_Acceleration", M_XhBhUCvxQX, 4, 0, M_yefMSqsKTl);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Global_Alpha", M_Sd1tY9cmDz, 3, 0, M_XhBhUCvxQX);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Geometry_Turbulence", M_PdSIiwEtLH, 2, 0, M_Sd1tY9cmDz);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Effect_Scale", M_XVZsObjhSX, 1, 0, M_PdSIiwEtLH);
			NEXTENUM2(M_RXuN3aKtr6, mModType, "ePartPropModifier_Constraint_Length", M_pcfJ2aHgWe, 0, 0, M_XVZsObjhSX);
			NEXTMEM2(M_RXuN3aKtr6, mModMin, ParticlePropConnect, float, 0x0, mModType);
			NEXTMEM2(M_RXuN3aKtr6, mModMax, ParticlePropConnect, float, 0x0, mModMin);
			NEXTMEM2(M_RXuN3aKtr6, mInvert, ParticlePropConnect, bool, 0x0, mModMax);
			ADD(M_RXuN3aKtr6);

			DEFINET2(M_PRW0M25Q8A, MovieCaptureInfo::EnumCompressorType);
			ADDFLAGS(M_PRW0M25Q8A, 0x8008);
			FIRSTMEM2(M_PRW0M25Q8A, mVal, MovieCaptureInfo::EnumCompressorType, int, 0x40);
			FIRSTENUM2(M_PRW0M25Q8A, mVal, "IV50", M_WyfdiFwB4s, 5, 0);
			NEXTENUM2(M_PRW0M25Q8A, mVal, "MSVC", M_AfmWR9Y4CP, 4, 0, M_WyfdiFwB4s);
			NEXTENUM2(M_PRW0M25Q8A, mVal, "IV32", M_51DYzsDa6K, 3, 0, M_AfmWR9Y4CP);
			NEXTENUM2(M_PRW0M25Q8A, mVal, "CVID", M_VaDXAq6SHA, 2, 0, M_51DYzsDa6K);
			NEXTENUM2(M_PRW0M25Q8A, mVal, "Uncompressed", M_LXYpYf1bCi, 1, 0, M_VaDXAq6SHA);
			NEXTMEM1(M_PRW0M25Q8A, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, MovieCaptureInfo::EnumCompressorType, enumbase, 0x10, mVal);
			ADD(M_PRW0M25Q8A);

			DEFINET2(M_pNpLPnVhxS, MovieCaptureInfo);
			FIRSTMEM2(M_pNpLPnVhxS, mFPS, MovieCaptureInfo, int, 0x0);
			NEXTMEM2(M_pNpLPnVhxS, mCType, MovieCaptureInfo, M_PRW0M25Q8A, 0x0, mFPS);
			ADD(M_pNpLPnVhxS);

			DEFINET2(M_WozXys0QpZ, InverseKinematicsBase);
			ADDFLAGS(M_WozXys0QpZ, 0x1);
			ADD(M_WozXys0QpZ);

			DEFINET2(M_3rnPGGKZPb, InverseKinematicsDerived);
			ADDFLAGS(M_3rnPGGKZPb, 0x1);
			ADD(M_3rnPGGKZPb);

			DEFINET2(M_G9Uv54oGDD, InverseKinematicsAttach);
			FIRSTMEM1(M_G9Uv54oGDD, "Baseclass_InverseKinematicsDerived", Baseclass_InverseKinematicsDerived, 
				__reserve, InverseKinematicsAttach, M_3rnPGGKZPb, 0x10);
			ADD(M_G9Uv54oGDD);

			DEFINET2(M_ujLwzo8X96, ParticleInverseKinematics);
			EXT(M_ujLwzo8X96, ik);
			FIRSTMEM1(M_ujLwzo8X96, "Baseclass_InverseKinematicsBase", Baseclass_InverseKinematicsBase,
				__reserve, ParticleInverseKinematics, M_WozXys0QpZ, 0x10);
			ADD(M_ujLwzo8X96);

			DEFINET2(M_U5Qe8dZhX6, InverseKinematics);
			EXT(M_U5Qe8dZhX6, ik);
			FIRSTMEM1(M_U5Qe8dZhX6, "Baseclass_InverseKinematicsDerived", Baseclass_InverseKinematicsDerived,
				mhTargetAnimation, InverseKinematics, M_3rnPGGKZPb, 0x10);
			NEXTMEM2(M_U5Qe8dZhX6, mhTargetAnimation, InverseKinematics, HandleLockhanm, 0x0, Baseclass_InverseKinematicsDerived);
			ADD(M_U5Qe8dZhX6);

			DEFINEDCARRAY2(Handle<SoundData>, HSoundData);
			DEFINEMAP2(SoundFootsteps::EnumMaterial, DCArray<Handle<SoundData>>, enummat, arrayhsound, std::less<SoundFootsteps::EnumMaterial>);

			DEFINET2(M_8jpgBfQtNR, FootSteps::FootstepBank);
			FIRSTMEM2(M_8jpgBfQtNR, mSounds, FootSteps::FootstepBank, DCArray_HSoundData, 0x0);
			NEXTMEM2(M_8jpgBfQtNR, mMaterialFootsteps, FootSteps::FootstepBank, Map_enummat_arrayhsound, 0x0, mSounds);
			ADD(M_8jpgBfQtNR);

			DEFINET2(M_RxJ7Gc2dwP, FilterArea);
			FIRSTMEM2(M_RxJ7Gc2dwP, mText, FilterArea, string, 0x0);
			ADD(M_RxJ7Gc2dwP);

			DEFINET2(M_0at2jVEr6i, FileNameBase);
			FIRSTMEM2(M_0at2jVEr6i, mFileName, FileNameBase, symbol, 0x0);
			ADD(M_0at2jVEr6i);

			DEFINET2(M_MY20H08Xwi, FileName<SoundEventBankDummy>);
			FIRSTMEM1(M_MY20H08Xwi, "Baseclass_FileNameBase", Baseclass_FileNameBase, 
				mFileName, FileName<SoundEventBankDummy>, M_0at2jVEr6i, 0x10);
			ADD(M_MY20H08Xwi);

			DEFINET2(M_hUPEYYFiiH, EnumRenderTextureResolution);
			ADDFLAGS(M_hUPEYYFiiH, 0x8008);
			FIRSTMEM2(M_hUPEYYFiiH, mVal, EnumRenderTextureResolution, int, 0x40);
			FIRSTENUM2(M_hUPEYYFiiH, mVal, "eRenderTextureResolution_Ultra", M_S6HDN1kvhy, 4, 0);
			NEXTENUM2(M_hUPEYYFiiH, mVal, "eRenderTextureResolution_Default", M_s8yv2BTixW, 3, 0, M_S6HDN1kvhy);
			NEXTENUM2(M_hUPEYYFiiH, mVal, "eRenderTextureResolution_Medium", M_go5eeCVjy4, 2, 0, M_s8yv2BTixW);
			NEXTENUM2(M_hUPEYYFiiH, mVal, "eRenderTextureResolution_Small", M_wOCWaqQ12l, 1, 0, M_go5eeCVjy4);
			NEXTMEM1(M_hUPEYYFiiH, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumRenderTextureResolution, enumbase, 0x10, mVal);
			ADD(M_hUPEYYFiiH);

			DEFINET2(lmp, T3LightSceneInternalData::LightmapPage);
			FIRSTMEM2(lmp, mhTextureAtlas, T3LightSceneInternalData::LightmapPage, Handletex, 0);
			NEXTMEM2(lmp, mFlags, T3LightSceneInternalData::LightmapPage, flags, 0, mhTextureAtlas);
			ADD(lmp);

			DEFINEDCARRAY2(T3LightSceneInternalData::LightmapPage, lmp);

			DEFINET2(M_4BG5xMLLGy, T3LightSceneInternalData::QualityEntry);
			FIRSTMEM2(M_4BG5xMLLGy, mLightmapPages, T3LightSceneInternalData::QualityEntry, DCArray_lmp, 0x0);
			NEXTMEM2(M_4BG5xMLLGy, mhStaticShadowVolumeTexture, T3LightSceneInternalData::QualityEntry, Handletex, 0x0, mLightmapPages);
			ADD(M_4BG5xMLLGy);

			DEFINET2(M_rf58Tp9dlF, T3LightSceneInternalData);
			ADDFLAGS(M_rf58Tp9dlF, 0x20);
			FIRSTMEM1(M_rf58Tp9dlF, "mEntryForQuality[0]", e0, mBakeVersion, T3LightSceneInternalData, M_4BG5xMLLGy, 0x0);
			NEXTMEM1(M_rf58Tp9dlF, "mEntryForQuality[1]", e1, mBakeVersion, T3LightSceneInternalData, M_4BG5xMLLGy, 0x0, e0);
			NEXTMEM1(M_rf58Tp9dlF, "mEntryForQuality[2]",e2, mBakeVersion, T3LightSceneInternalData, M_4BG5xMLLGy, 0x0, e1);
			NEXTMEM1(M_rf58Tp9dlF, "mEntryForQuality[3]", e3, mBakeVersion, T3LightSceneInternalData, M_4BG5xMLLGy, 0x0, e2);
			NEXTMEM2(M_rf58Tp9dlF, mStationaryLightCount, T3LightSceneInternalData, int, 0x0, e3);
			NEXTMEM2(M_rf58Tp9dlF, mBakeVersion, T3LightSceneInternalData, int, 0x0, mStationaryLightCount);
			ADD(M_rf58Tp9dlF);

			DEFINET2(M_bR4bVrNFVC, EnumT3NPRSpecularType);
			ADDFLAGS(M_bR4bVrNFVC, 0x8008);
			FIRSTMEM2(M_bR4bVrNFVC, mVal, EnumT3NPRSpecularType, int, 0x40);
			FIRSTENUM2(M_bR4bVrNFVC, mVal, "T3NPRSpecularType_Anisotropic", M_YsA4K3Z6qe, 2, 0);
			NEXTENUM2(M_bR4bVrNFVC, mVal, "T3NPRSpecularType_Isotropic", M_Y0dLL7FNP3, 1, 0, M_YsA4K3Z6qe);
			NEXTENUM2(M_bR4bVrNFVC, mVal, "T3NPRSpecularType_None", M_EehjTFOB7B, 0, 0, M_Y0dLL7FNP3);
			NEXTMEM1(M_bR4bVrNFVC, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3NPRSpecularType, enumbase, 0x10, mVal);
			ADD(M_bR4bVrNFVC);


			DEFINET2(M_oSptqXhGNF, EnumT3DetailShadingType);
			ADDFLAGS(M_oSptqXhGNF, 0x8008);
			FIRSTMEM2(M_oSptqXhGNF, mVal, EnumT3DetailShadingType, int, 0x40);
			FIRSTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_Animated_Detail", M_ohRDz6vZVD, 6, 0);
			NEXTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_Single_Channel_Detail", M_ReB6mxB0ZR, 5, 0, M_ohRDz6vZVD);
			NEXTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_Packed_Detail", M_axbIRBtHa9, 4, 0, M_ReB6mxB0ZR);
			NEXTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_Packed_Detail_And_Tiled_Packed_Detail", M_qrMZEYnTQm, 3, 0, M_axbIRBtHa9);
			NEXTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_Sharp_Detail", M_K7lIhDLqGD, 2, 0, M_qrMZEYnTQm);
			NEXTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_Old_Toon", M_yxsvVr0wSS, 1, 0, M_K7lIhDLqGD);
			NEXTENUM2(M_oSptqXhGNF, mVal, "T3DetailShadingType_No_Detail_Map", M_m5SgMHz26Y, 0, 0, M_yxsvVr0wSS);
			NEXTMEM1(M_oSptqXhGNF, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumT3DetailShadingType, enumbase, 0x10, mVal);
			ADD(M_oSptqXhGNF);


			DEFINET2(M_jkS8taguJg, EnumParticlePropModifier);
			ADDFLAGS(M_jkS8taguJg, 0x8008);
			FIRSTMEM2(M_jkS8taguJg, mVal, EnumParticlePropModifier, int, 0x40);
			FIRSTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Enable", M_NakyOO9TJX, 28, 0);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Geometry_Scale", M_QXpS68k8Rb, 27, 0, M_NakyOO9TJX);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_Alpha", M_kxga7ZYwr4, 26, 0, M_QXpS68k8Rb);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_KeyControl04", M_aTG2C7Qia3, 25, 0, M_kxga7ZYwr4);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_KeyControl03", M_eeMydkfj2i, 24, 0, M_aTG2C7Qia3);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_KeyControl02", M_13Pl32CTfF, 23, 0, M_eeMydkfj2i);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_KeyControl01", M_J8zmglOu2G, 22, 0, M_13Pl32CTfF);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Velocity_Timescale_Modifier", M_ohPm2YQox9, 21, 0, M_J8zmglOu2G);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Velocity_Turbulence_Speed", M_Kos6Ydc0ah, 20, 0, M_ohPm2YQox9);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Velocity_Turbulence_Force", M_MkQYpMbiZj, 19, 0, M_Kos6Ydc0ah);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Target_Render_Lerp", M_9SKb47Qxlf, 18, 0, M_MkQYpMbiZj);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Spawn_Volume_Sweep_Offset", M_FjnWpMXD22, 17, 0, M_9SKb47Qxlf);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Spawn_Volume_Sweep", M_rqy3wHKdP8, 16, 0, M_FjnWpMXD22);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Spawn_Angle", M_es092dAcDH, 15, 0, M_rqy3wHKdP8);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Sprite_Animation_Cycles", M_XjkOCjQJ0c, 14, 0, M_es092dAcDH);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Sprite_Animation_Rate", M_yIXmWiNHDa, 13, 0, M_XjkOCjQJ0c);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Time_Scale", M_hXnZfIpnL4, 12, 0, M_yIXmWiNHDa);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_Intensity", M_IzwW8GrZPp, 11, 0, M_hXnZfIpnL4);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_Speed", M_wYWcp6k8ca, 10, 0, M_IzwW8GrZPp);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_RotationSpeed", M_ctZ8rqYI0j, 9, 0, M_wYWcp6k8ca);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_Rotation", M_HSGvY134iP, 8, 0, M_ctZ8rqYI0j);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_Lifespan", M_lCeClSxjb7, 7, 0, M_HSGvY134iP);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_PP_Scale", M_OdAsExNBNc, 6, 0, M_lCeClSxjb7);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Global_Acceleration", M_NDG5OqjGag, 4, 0, M_OdAsExNBNc);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Global_Alpha", M_Eu75ImAuFo, 3, 0, M_NDG5OqjGag);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Geometry_Turbulence", M_iMD1uTyJ7x, 2, 0, M_Eu75ImAuFo);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Effect_Scale", M_gEfAATAO8k, 1, 0, M_iMD1uTyJ7x);
			NEXTENUM2(M_jkS8taguJg, mVal, "ePartPropModifier_Constraint_Length", M_k8zTp0KVDi, 0, 0, M_gEfAATAO8k);
			NEXTMEM1(M_jkS8taguJg, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumParticlePropModifier, enumbase, 0x10, mVal);
			ADD(M_jkS8taguJg);


			DEFINET2(M_bIRxxEqm7G, EnumParticlePropDriver);
			ADDFLAGS(M_bIRxxEqm7G, 0x8008);
			FIRSTMEM2(M_bIRxxEqm7G, mVal, EnumParticlePropDriver, int, 0x40);
			FIRSTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_DistanceToCamera", M_YOpb2ewRo8, 9, 0);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_KeyControl04", M_LXY4dLywY6, 8, 0, M_YOpb2ewRo8);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_KeyControl03", M_pHUqINoKRX, 7, 0, M_LXY4dLywY6);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_KeyControl02", M_8bIc6hBsZX, 6, 0, M_pHUqINoKRX);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_KeyControl01", M_DwghPpNQni, 5, 0, M_8bIc6hBsZX);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_CameraDot", M_4Lq5Tx60lu, 4, 0, M_DwghPpNQni);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_BurstTime", M_da1xIhy5fB, 3, 0, M_4Lq5Tx60lu);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_DistanceToTarget", M_efyAMnFn1x, 2, 0, M_da1xIhy5fB);
			NEXTENUM2(M_bIRxxEqm7G, mVal, "ePartPropDriver_EmitterSpeed", M_H2rTdqgIka, 1, 0, M_efyAMnFn1x);
			NEXTMEM1(M_bIRxxEqm7G, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnumParticlePropDriver, enumbase, 0x10, mVal);
			ADD(M_bIRxxEqm7G);

			DEFINET2(M_E2HxsdX1Kl, EnlightenModule::EnumeDistributedBuildSystem);
			ADDFLAGS(M_E2HxsdX1Kl, 0x8008);
			FIRSTMEM2(M_E2HxsdX1Kl, mVal, EnlightenModule::EnumeDistributedBuildSystem, int, 0x40);
			FIRSTENUM2(M_E2HxsdX1Kl, mVal, "Incredibuild", M_WXtqtyKwsZ, 2, 0);
			NEXTENUM2(M_E2HxsdX1Kl, mVal, "SN-DBS", M_Sva2AIvKLu, 1, 0, M_WXtqtyKwsZ);
			NEXTENUM2(M_E2HxsdX1Kl, mVal, "None", M_vHJniJMmpQ, 0, 0, M_Sva2AIvKLu);
			NEXTMEM1(M_E2HxsdX1Kl, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeDistributedBuildSystem, enumbase, 0x10, mVal);
			ADD(M_E2HxsdX1Kl);


			DEFINET2(M_NGl6WXVW0i, EnlightenModule::EnumeSceneOptimisationMode);
			ADDFLAGS(M_NGl6WXVW0i, 0x8008);
			FIRSTMEM2(M_NGl6WXVW0i, mVal, EnlightenModule::EnumeSceneOptimisationMode, int, 0x40);
			FIRSTENUM2(M_NGl6WXVW0i, mVal, "Voxelisation", M_dt8Zaj95Sz, 2, 0);
			NEXTENUM2(M_NGl6WXVW0i, mVal, "Equal Pixel Area", M_kXT8ZpDQjl, 1, 0, M_dt8Zaj95Sz);
			NEXTENUM2(M_NGl6WXVW0i, mVal, "None", M_947fr7MoUF, 0, 0, M_kXT8ZpDQjl);
			NEXTMEM1(M_NGl6WXVW0i, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeSceneOptimisationMode, enumbase, 0x10, mVal);
			ADD(M_NGl6WXVW0i);


			DEFINET2(M_0FlcbtqyiC, EnlightenModule::EnumeBackfaceType);
			ADDFLAGS(M_0FlcbtqyiC, 0x8008);
			FIRSTMEM2(M_0FlcbtqyiC, mVal, EnlightenModule::EnumeBackfaceType, int, 0x40);
			FIRSTENUM2(M_0FlcbtqyiC, mVal, "doubleSided", M_vQEAVXHGUE, 4, 0);
			NEXTENUM2(M_0FlcbtqyiC, mVal, "transparent", M_nG96tWCTpb, 3, 0, M_vQEAVXHGUE);
			NEXTENUM2(M_0FlcbtqyiC, mVal, "black", M_JXpgSkSjBb, 2, 0, M_nG96tWCTpb);
			NEXTENUM2(M_0FlcbtqyiC, mVal, "invalid", M_8QRKGcEr9E, 1, 0, M_JXpgSkSjBb);
			NEXTENUM2(M_0FlcbtqyiC, mVal, "auto", M_jJT8GSvwdk, 0, 0, M_8QRKGcEr9E);
			NEXTMEM1(M_0FlcbtqyiC, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeBackfaceType, enumbase, 0x10, mVal);
			ADD(M_0FlcbtqyiC);


			DEFINET2(M_PZXdphPRP5, EnlightenModule::EnumeAutoUVSimplificationMode);
			ADDFLAGS(M_PZXdphPRP5, 0x8008);
			FIRSTMEM2(M_PZXdphPRP5, mVal, EnlightenModule::EnumeAutoUVSimplificationMode, int, 0x40);
			FIRSTENUM2(M_PZXdphPRP5, mVal, "EAUM Disable", M_Ylz86bsQem, 2, 0);
			NEXTENUM2(M_PZXdphPRP5, mVal, "EAUM Override", M_dPcNj711hz, 1, 0, M_Ylz86bsQem);
			NEXTENUM2(M_PZXdphPRP5, mVal, "EAUM Automatic", M_V7fdWFcRBR, 0, 0, M_dPcNj711hz);
			NEXTMEM1(M_PZXdphPRP5, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeAutoUVSimplificationMode, enumbase, 0x10, mVal);
			ADD(M_PZXdphPRP5);


			DEFINET2(M_EldWDBwkev, EnlightenModule::EnumeProbeSampleMethod);
			ADDFLAGS(M_EldWDBwkev, 0x8008);
			FIRSTMEM2(M_EldWDBwkev, mVal, EnlightenModule::EnumeProbeSampleMethod, int, 0x40);
			FIRSTENUM2(M_EldWDBwkev, mVal, "Force Multiple", M_QR4yfm8dE4, 2, 0);
			NEXTENUM2(M_EldWDBwkev, mVal, "Force Single", M_WK2uFXHuib, 1, 0, M_QR4yfm8dE4);
			NEXTENUM2(M_EldWDBwkev, mVal, "Use Bounds", M_siEsoM3wSt, 0, 0, M_WK2uFXHuib);
			NEXTMEM1(M_EldWDBwkev, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeProbeSampleMethod, enumbase, 0x10, mVal);
			ADD(M_EldWDBwkev);


			DEFINET2(M_IAQI66y2qt, EnlightenModule::EnumeDisplayQuality);
			ADDFLAGS(M_IAQI66y2qt, 0x8008);
			FIRSTMEM2(M_IAQI66y2qt, mVal, EnlightenModule::EnumeDisplayQuality, int, 0x40);
			FIRSTENUM2(M_IAQI66y2qt, mVal, "High", M_uF65y5qC32, 2, 0);
			NEXTENUM2(M_IAQI66y2qt, mVal, "Medium", M_OM1JOkib5I, 1, 0, M_uF65y5qC32);
			NEXTENUM2(M_IAQI66y2qt, mVal, "Low", M_5qY2lFhXWG, 0, 0, M_OM1JOkib5I);
			NEXTMEM1(M_IAQI66y2qt, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeDisplayQuality, enumbase, 0x10, mVal);
			ADD(M_IAQI66y2qt);


			DEFINET2(M_XsC6Pt0GIs, EnlightenModule::EnumeRadiositySampleRate);
			ADDFLAGS(M_XsC6Pt0GIs, 0x8008);
			FIRSTMEM2(M_XsC6Pt0GIs, mVal, EnlightenModule::EnumeRadiositySampleRate, int, 0x40);
			FIRSTENUM2(M_XsC6Pt0GIs, mVal, "VeryHigh", M_FTnTZSg06V, 3, 0);
			NEXTENUM2(M_XsC6Pt0GIs, mVal, "High", M_DgG1FdC5dy, 2, 0, M_FTnTZSg06V);
			NEXTENUM2(M_XsC6Pt0GIs, mVal, "Medium", M_GE2qW4HvjB, 1, 0, M_DgG1FdC5dy);
			NEXTENUM2(M_XsC6Pt0GIs, mVal, "Low", M_RZVStdwwCU, 0, 0, M_GE2qW4HvjB);
			NEXTMEM1(M_XsC6Pt0GIs, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeRadiositySampleRate, enumbase, 0x10, mVal);
			ADD(M_XsC6Pt0GIs);


			DEFINET2(M_zCgWm0oojR, EnlightenModule::EnumeUpdateMethodWithDefault);
			ADDFLAGS(M_zCgWm0oojR, 0x8008);
			FIRSTMEM2(M_zCgWm0oojR, mVal, EnlightenModule::EnumeUpdateMethodWithDefault, int, 0x40);
			FIRSTENUM2(M_zCgWm0oojR, mVal, "Use Level Default", M_Zf5LDFzwJs, 2, 0);
			NEXTENUM2(M_zCgWm0oojR, mVal, "Static", M_EkG1BZUdWp, 1, 0, M_Zf5LDFzwJs);
			NEXTENUM2(M_zCgWm0oojR, mVal, "Dynamic", M_6sah2jzzjF, 0, 0, M_EkG1BZUdWp);
			NEXTMEM1(M_zCgWm0oojR, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeUpdateMethodWithDefault, enumbase, 0x10, mVal);
			ADD(M_zCgWm0oojR);


			DEFINET2(M_WtxsrDzQOU, EnlightenModule::EnumeProbeResolutionWithDefault);
			ADDFLAGS(M_WtxsrDzQOU, 0x8008);
			FIRSTMEM2(M_WtxsrDzQOU, mVal, EnlightenModule::EnumeProbeResolutionWithDefault, int, 0x40);
			FIRSTENUM2(M_WtxsrDzQOU, mVal, "Use Level Default", M_2vYTHHoJCD, 4, 0);
			NEXTENUM2(M_WtxsrDzQOU, mVal, "1/8", M_eC83luem7r, 3, 0, M_2vYTHHoJCD);
			NEXTENUM2(M_WtxsrDzQOU, mVal, "1/4", M_aLUO3NPVQN, 2, 0, M_eC83luem7r);
			NEXTENUM2(M_WtxsrDzQOU, mVal, "1/2", M_jnEo925b76, 1, 0, M_aLUO3NPVQN);
			NEXTENUM2(M_WtxsrDzQOU, mVal, "Full", M_we7p5ieeI8, 0, 0, M_jnEo925b76);
			NEXTMEM1(M_WtxsrDzQOU, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeProbeResolutionWithDefault, enumbase, 0x10, mVal);
			ADD(M_WtxsrDzQOU);


			DEFINET2(M_tYWmFGzuR3, EnlightenModule::EnumeAgentUsage);
			ADDFLAGS(M_tYWmFGzuR3, 0x8008);
			FIRSTMEM2(M_tYWmFGzuR3, mVal, EnlightenModule::EnumeAgentUsage, int, 0x40);
			FIRSTENUM2(M_tYWmFGzuR3, mVal, "Conservative", M_cRjrXkzmQa, 2, 0);
			NEXTENUM2(M_tYWmFGzuR3, mVal, "Aggressive", M_4DXFe9OH46, 1, 0, M_cRjrXkzmQa);
			NEXTENUM2(M_tYWmFGzuR3, mVal, "Default", M_yIUFl0XjDq, 0, 0, M_4DXFe9OH46);
			NEXTMEM1(M_tYWmFGzuR3, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeAgentUsage, enumbase, 0x10, mVal);
			ADD(M_tYWmFGzuR3);


			DEFINET2(M_0qZFUD7BQv, EnlightenModule::EnumeSimplifyMode);
			ADDFLAGS(M_0qZFUD7BQv, 0x8008);
			FIRSTMEM2(M_0qZFUD7BQv, mVal, EnlightenModule::EnumeSimplifyMode, int, 0x40);
			FIRSTENUM2(M_0qZFUD7BQv, mVal, "simplifyUsingUvs", M_0TPW4sVCdF, 2, 0);
			NEXTENUM2(M_0qZFUD7BQv, mVal, "simplifyNoUvs", M_sdgaixwB9d, 1, 0, M_0TPW4sVCdF);
			NEXTENUM2(M_0qZFUD7BQv, mVal, "none", M_yCCsn00KTM, 0, 0, M_sdgaixwB9d);
			NEXTMEM1(M_0qZFUD7BQv, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, EnlightenModule::EnumeSimplifyMode, enumbase, 0x10, mVal);
			ADD(M_0qZFUD7BQv);

			DEFINET2(M_0YKI4JSgVS, EnlightenModule::EnlightenMeshSettings::AutoUVSettings);
			FIRSTMEM2(M_0YKI4JSgVS, mSimplificationMode, EnlightenModule::EnlightenMeshSettings::AutoUVSettings, M_0qZFUD7BQv, 0x0);
			NEXTMEM2(M_0YKI4JSgVS, mMaxDistance, EnlightenModule::EnlightenMeshSettings::AutoUVSettings, float, 0x0, mSimplificationMode);
			NEXTMEM2(M_0YKI4JSgVS, mMaxInitialNormalDeviation, EnlightenModule::EnlightenMeshSettings::AutoUVSettings, float, 0x0, mMaxDistance);
			NEXTMEM2(M_0YKI4JSgVS, mMaxGeneralNormalDeviation, EnlightenModule::EnlightenMeshSettings::AutoUVSettings, float, 0x0, mMaxInitialNormalDeviation);
			NEXTMEM2(M_0YKI4JSgVS, mExpansionFactor, EnlightenModule::EnlightenMeshSettings::AutoUVSettings, float, 0x0, mMaxGeneralNormalDeviation);
			NEXTMEM2(M_0YKI4JSgVS, mSignificantAreaRatio, EnlightenModule::EnlightenMeshSettings::AutoUVSettings, float, 0x0, mExpansionFactor);
			ADD(M_0YKI4JSgVS);

			DEFINET2(M_DsE9G4Pxx5, EnlightenModule::EnlightenMeshSettings);
			FIRSTMEM2(M_DsE9G4Pxx5, mEnlightenLightingMode, EnlightenModule::EnlightenMeshSettings, M_zHcL8AII08, 0x0);
			NEXTMEM2(M_DsE9G4Pxx5, mAutoUVSettings, EnlightenModule::EnlightenMeshSettings, M_0YKI4JSgVS, 0x0, mEnlightenLightingMode);
			NEXTMEM2(M_DsE9G4Pxx5, mEnlightenQuality, EnlightenModule::EnlightenMeshSettings, enlq, 0x0, mAutoUVSettings);
			ADD(M_DsE9G4Pxx5);

			DEFINET2(M_HDPQPeQ8Hz, DialogItem::EnumPlaybackMode);
			ADDFLAGS(M_HDPQPeQ8Hz, 0x8008);
			FIRSTMEM2(M_HDPQPeQ8Hz, mVal, DialogItem::EnumPlaybackMode, int, 0x40);
			FIRSTENUM2(M_HDPQPeQ8Hz, mVal, "first_then_shuffle_repeat_remaining", M_ynx6mMA0N7, 6, 0);
			NEXTENUM2(M_HDPQPeQ8Hz, mVal, "shuffle_die_off", M_j95k7Ai4Z9, 5, 0, M_ynx6mMA0N7);
			NEXTENUM2(M_HDPQPeQ8Hz, mVal, "shuffle_repeat_final", M_3xaG0PEPp1, 4, 0, M_j95k7Ai4Z9);
			NEXTENUM2(M_HDPQPeQ8Hz, mVal, "shuffle_repeat_all", M_ZNxGFFhR1Q, 3, 0, M_3xaG0PEPp1);
			NEXTENUM2(M_HDPQPeQ8Hz, mVal, "sequential_die_off", M_MD6Wuzlezf, 2, 0, M_ZNxGFFhR1Q);
			NEXTENUM2(M_HDPQPeQ8Hz, mVal, "sequential_repeat_final", M_98VVWnEzSJ, 1, 0, M_MD6Wuzlezf);
			NEXTENUM2(M_HDPQPeQ8Hz, mVal, "sequential_looping", M_sfumcgLBzV, 0, 0, M_98VVWnEzSJ);
			NEXTMEM1(M_HDPQPeQ8Hz, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, DialogItem::EnumPlaybackMode, enumbase, 0x10, mVal);
			ADD(M_HDPQPeQ8Hz);

			DEFINET2(M_rOHYmmgZ0T, ColorHDR);
			ADDFLAGS(M_rOHYmmgZ0T, 0x2);
			FIRSTMEM2(M_rOHYmmgZ0T, r, ColorHDR, float, 0x0);
			NEXTMEM2(M_rOHYmmgZ0T, g, ColorHDR, float, 0x0, r);
			NEXTMEM2(M_rOHYmmgZ0T, b, ColorHDR, float, 0x0, g);
			NEXTMEM2(M_rOHYmmgZ0T, intensity, ColorHDR, float, 0x0, b);
			ADD(M_rOHYmmgZ0T);

			DEFINET2(M_of9VjztUg5, Chore::EnumExtentsMode);
			ADDFLAGS(M_of9VjztUg5, 0x8008);
			FIRSTMEM2(M_of9VjztUg5, mVal, Chore::EnumExtentsMode, int, 0x40);
			FIRSTENUM2(M_of9VjztUg5, mVal, "eSpillout", M_Wq0L8hVnUs, 2, 0);
			NEXTENUM2(M_of9VjztUg5, mVal, "eLangRes", M_ngdq6ON7GZ, 1, 0, M_Wq0L8hVnUs);
			NEXTMEM1(M_of9VjztUg5, "Baseclass_EnumBase", Baseclass_EnumBase, mVal, Chore::EnumExtentsMode, enumbase, 0x10, mVal);
			ADD(M_of9VjztUg5);

			DEFINET2(M_SQmAsorh9h, AnimationConstraintParameters);
			FIRSTMEM2(M_SQmAsorh9h, mUseAnimationConstraints, AnimationConstraintParameters, float, 0x0);
			NEXTMEM2(M_SQmAsorh9h, mUseSpineEcho, AnimationConstraintParameters, float, 0x0, mUseAnimationConstraints);
			NEXTMEM2(M_SQmAsorh9h, mLockToAnimLeftWrist, AnimationConstraintParameters, float, 0x0, mUseSpineEcho);
			NEXTMEM2(M_SQmAsorh9h, mLockToAnimRightWrist, AnimationConstraintParameters, float, 0x0, mLockToAnimLeftWrist);
			NEXTMEM2(M_SQmAsorh9h, mLockToNodeLeftWrist, AnimationConstraintParameters, float, 0x0, mLockToAnimRightWrist);
			NEXTMEM2(M_SQmAsorh9h, mLockToNodeRightWrist, AnimationConstraintParameters, float, 0x0, mLockToNodeLeftWrist);
			NEXTMEM2(M_SQmAsorh9h, mLockToAnimLeftWristOffset, AnimationConstraintParameters, transform, 0x0, mLockToNodeRightWrist);
			NEXTMEM2(M_SQmAsorh9h, mLockToAnimRightWristOffset, AnimationConstraintParameters, transform, 0x0, mLockToAnimLeftWristOffset);
			NEXTMEM2(M_SQmAsorh9h, mLockToNodeLeftWristOffset, AnimationConstraintParameters, transform, 0x0, mLockToAnimRightWristOffset);
			NEXTMEM2(M_SQmAsorh9h, mLockToNodeRightWristOffset, AnimationConstraintParameters, transform, 0x0, mLockToNodeLeftWristOffset);
			NEXTMEM2(M_SQmAsorh9h, mUseLimitConstraints, AnimationConstraintParameters, float, 0x0, mLockToNodeRightWristOffset);
			NEXTMEM2(M_SQmAsorh9h, mUseBoneLengthConstraints, AnimationConstraintParameters, float, 0x0, mUseLimitConstraints);
			NEXTMEM2(M_SQmAsorh9h, mpAgent, AnimationConstraintParameters, ptrbase, 0x0, mUseBoneLengthConstraints);
			ADD(M_SQmAsorh9h);

			DEFINET2(M_wp5rVitT7q, DebugString);
			SERIALIZER(M_wp5rVitT7q, DebugString);
			ADD(M_wp5rVitT7q);

			DEFINEKEYFRAMEDVALUE(hmsh, Handle<D3DMesh>, mesh);
			DEFINEANMVALUEI(hmshs, Handle<D3DMesh>);
			DEFINESET_(FileName<SoundEventBankDummy>, fnsebd);
			DEFINEHANDLE(hpart, ParticleProperties);
			DEFINESET(Color);
			DEFINEDCARRAY(ParticlePropConnect);
			DEFINEMAP2(Symbol, Footsteps2::FootstepBank, sym, fb2, std::less<Symbol>);
			DEFINEHANDLE(hdlgr, DialogResource);
			DEFINEDCARRAY(Vector2);
			DEFINEHANDLE(snap, SoundBusSnapshot::Snapshot);
			DEFINEMAP(String, bool, std::less<String>);
			DEFINEMAP2(Symbol, FootSteps::FootstepBank, sym, fb, std::less<Symbol>);
			DEFINEDCARRAY2(FileName<SoundEventBankDummy>, fnseb);
			DEFINEHANDLE(PhysicsData, PhysicsData);
			DEFINEHANDLE(sbss, SoundBusSnapshot::SnapshotSuite);
			DEFINEHANDLE(bggG, BlendGraphManager);
			DEFINEDCARRAY2(Map<String SEP String SEP std::less<String>>, Map_Str_Str);
			DEFINEDCARRAY(T3MeshPropertyEntry);
			DEFINEMAP2(Symbol, Localization::Language, Symbol, loclang, std::less<Symbol>);
			DEFINEMAP(String, ChorecorderParameters, std::less<String>);
			DEFINEMAP2(String, Set<Symbol SEP std::less<Symbol>>, str, setstr, StringCompareCaseInsensitive);
			DEFINEMAP2(Symbol, Map < Symbol SEP Set<Symbol SEP std::less<Symbol>> SEP std::less<Symbol>>, sym, Map_Sym_Setstr, std::less<Symbol>);
			DEFINEMAP2(Symbol, Map<Symbol SEP int SEP std::less<Symbol>>, sym, Map_Sym_Int, std::less<Symbol>);
			DEFINEMAP2(Symbol, PreloadPackage::ResourceSeenTimes, sym, pprst, std::less<Symbol>);
			DEFINEMAP2(PreloadPackage::ResourceKey, PreloadPackage::ResourceSeenTimes, pprk, pprst, std::less<PreloadPackage::ResourceKey>);

			DEFINEMAP2(unsigned int, Set<Symbol SEP std::less<Symbol>>, uint, Set_Sym, std::less<unsigned int>);
			DEFINEMAP2(unsigned long, Font::GlyphInfo, uLong, fontglyinf, std::less<unsigned long>);
			DEFINEMAP2(unsigned long, LanguageResource, uLong, langres, std::less<unsigned long>);
			MKNAME(meta_Map_uint_Set_Sym, "Map<unsignedint,Set<Symbol,less<Symbol>>,less<unsignedint>>");
			MKNAME(meta_Map_uLong_fontglyinf, "Map<unsignedlong,Font::GlyphInfo,less<unsignedlong>>");
			MKNAME(meta_Map_uLong_langres, "Map<unsignedlong,LanguageRes,less<unsignedlong>>");

			DEFINEMAP2(String, Handle<PropertySet>, str, hprop, std::less < String>);
			DEFINEMAP(String, Vector3, std::less<String>);
			DEFINEMAP2(Symbol, DCArray<LanguageResLocal>, sym, array_langresloc, std::less<Symbol>);
			DEFINEMAP(String, StyleGuideRef, std::less<String>);
			DEFINEMAP2(String, Set<Symbol SEP std::less<Symbol>>, str, Set_Sym, std::less<String>);
			DEFINEMAP2(String, Set<String SEP std::less<String>>, str, Set_Str, std::less<String>);
			DEFINEMAP2(String, PhonemeTable::PhonemeEntry, str, ptableentry, std::less<String>);
			DEFINEMAP2(String, Map<String SEP DCArray<String> SEP std::less<String>>, str, Map_Str_Array_String, std::less<String>);
			DEFINEMAP(String, float, std::less<String>);
			DEFINEMAP(String, AnimOrChore, std::less<String>);
			DEFINEMAP(int, PropertySet, std::less<int>);
			DEFINEMAP(DlgObjID, int, DlgObjIDLess);

			DEFINEMAP2(String, DCArray<unsigned char>, str, array_uchar, std::less<String>);
			MKNAME(meta_Map_str_array_uchar, "Map<String,DCArray<unsignedchar>,less<String>>");

			DEFINELIST(Vector3);
			DEFINELIST_(unsigned int, uint);
			MKNAME(meta_List_uint, "List<unsignedint>");

			DEFINELIST(T3ToonGradientRegion);
			DEFINELIST_(Map<String SEP String SEP std::less<String>>, Map_Str_Str);
			DEFINELIST_(List<PropertySet>, List_Prop);
			DEFINELIST_(HandleLock<Scene>, hlockscene);
			DEFINELIST_(Handle<T3Texture>, htex);
			DEFINELIST_(Handle<Scene>, hscene);
			DEFINELIST_(Handle<Rules>, hrules);
			DEFINELIST_(Handle<PropertySet>, hprop);
			DEFINELIST_(Handle<Chore>, hchore);
			DEFINELIST_(Handle<D3DMesh>, hmesh);
			DEFINELIST_(Handle<SoundData>, hsound);
			DEFINELIST_(Handle<AudioData>, haud);
			DEFINELIST_(Handle<AnimOrChore> ,hanorch);
			DEFINELIST(float);
			DEFINELIST_(List < Symbol>, List_Sym);
			DEFINELIST_(DCArray<String>, Array_String);
			DEFINELIST(Color);
			DEFINELIST(bool);

			DEFINEDCARRAY(PropertySet);
			DEFINEDCARRAY2(Handle<AudioData>, haud);
			DEFINEDCARRAY2(Handle<AnimOrChore>, hanorch);
			DEFINEDCARRAY2(DCArray<String>, array_str);
			DEFINEDCARRAY2(DCArray<PropertySet>, array_prop);
			DEFINEDCARRAY(Color);
			
			DEFINEKEYFRAMEDVALUE(Polar, Polar, M_h2Q6dQAjy4);
			DEFINEKEYFRAMEDVALUE(Vector4, Vector4, vec4);
			DEFINEKEYFRAMEDVALUE(Vector2, Vector2, vec2);
			DEFINEKEYFRAMEDVALUE(Symbol, Symbol, symbol);
			DEFINEKEYFRAMEDVALUE(ksen0, SoundEventName<SoundEventNameBase::NT_DEFAULT>, sen0);
			DEFINEKEYFRAMEDVALUE(ksen2, SoundEventName<SoundEventNameBase::NT_DIALOG>, sen2);
			DEFINEKEYFRAMEDVALUE(ksen1, SoundEventName<SoundEventNameBase::NT_SNAPSHOT>, sen1);

			MKNAME(meta_kfv_ksen0, "KeyframedValue<SoundEventName<0>>");
			MKNAME(meta_kfv_ksen1, "KeyframedValue<SoundEventName<1>>");
			MKNAME(meta_kfv_ksen2, "KeyframedValue<SoundEventName<2>>");
			MKNAME(meta_sen0, "SoundEventName<0>");
			MKNAME(meta_sen1, "SoundEventName<1>");
			MKNAME(meta_sen2, "SoundEventName<2>");
			MKNAME(meta_anmi_ksen0, "AnimatedValueInterface<SoundEventName<0>>");
			MKNAME(meta_anmi_ksen1, "AnimatedValueInterface<SoundEventName<1>>");
			MKNAME(meta_anmi_ksen2, "AnimatedValueInterface<SoundEventName<2>>");

			DEFINET2(M_nmJIgWyT7Y, ScriptEnum);
			ADDFLAGS(M_nmJIgWyT7Y, 0x200);
			FIRSTMEM2(M_nmJIgWyT7Y, mCurValue, ScriptEnum, string, 0x0);
			ADD(M_nmJIgWyT7Y);

			DEFINEKEYFRAMEDVALUE(ScriptEnum, ScriptEnum, M_nmJIgWyT7Y);
			DEFINEKEYFRAMEDVALUE(Quaternion, Quaternion, quat);
			DEFINEKEYFRAMEDVALUE(HWALK, Handle<WalkBoxes>, Handlewbox);
			DEFINEKEYFRAMEDVALUE(HAMB, Handle<SoundAmbience::AmbienceDefinition>, Handlehambb);
			DEFINEKEYFRAMEDVALUE(HSUITE, Handle<SoundBusSnapshot::SnapshotSuite>, Handlesbss);
			DEFINEKEYFRAMEDVALUE(HSNAP, Handle<SoundBusSnapshot::Snapshot>, Handlesnapss);
			DEFINEKEYFRAMEDVALUE(HREVERB, Handle<SoundReverbDefinition>, Handlereverb);
			DEFINEKEYFRAMEDVALUE(HESD, Handle<SoundEventSnapshotData>, Handlesedd1h);
			DEFINEKEYFRAMEDVALUE(HSNDD, Handle<SoundEventData>, Handleseddh);
			DEFINEKEYFRAMEDVALUE(HSCENE, Handle<Scene>, Handlescene);
			DEFINEKEYFRAMEDVALUE(HFONT, Handle<Font>, Handlefont);
			DEFINEKEYFRAMEDVALUE(HDLG, Handle<Dlg>, Handledlg);
			DEFINEKEYFRAMEDVALUE(HMESH, Handle<D3DMesh>, Handlemesh);

			DEFINEHANDLE(VoiceData, VoiceData);
			DEFINEHANDLE(StyleIdleTransitionsRes, StyleIdleTransitionsRes);
			DEFINEHANDLE(StyleGuideRef, StyleGuideRef);
			//darray<bool> already done in case rep same as int
			DEFINEHANDLE(sebd, SoundEventBankDummy);
			DEFINEHANDLE(save, SaveGame);
			DEFINEHANDLE(Rule, Rule);
			DEFINEHANDLE(ResourceGroupInfo, ResourceGroupInfo);
			DEFINEHANDLE(ResourceBundle, ResourceBundle);
			DEFINEHANDLE(PhysicsObject, PhysicsObject);
			
			DEFINEDCARRAY2(ParticleProperties::Animation, propanm);
			DEFINEMAP2(String, LocomotionDB::AnimationInfo, str, locdbanmi, std::less<String>);

			DEFINEHANDLE(LanguageResource, LanguageResource);
			DEFINEHANDLE(EventStorage, EventStorage);

			DEFINEMAP2(unsigned int, Font::GlyphInfo, uint, fontglyinf, std::less<unsigned int>);
			MKNAME(meta_Map_uLong_fontglyinf, "Map<unsignedint,Font::GlyphInfo,less<unsignedint>>");

			DEFINEHANDLE(BlendMode, BlendMode);
			DEFINEHANDLE(AudioData, AudioData);
			DEFINEHANDLE(AnimOrChore, AnimOrChore);
			DEFINEHANDLE(BlendGraph, BlendGraph);

			DEFINET2(Guide, Guide);
			FIRSTMEM2(Guide, m_Time, Guide, float, 0);
			NEXTMEM2(Guide, m_Bitfield, Guide, int, 0, m_Time);
			NEXTMEM2(Guide, m_AutoActRole, Guide, int, 0, m_Bitfield);
			ADD(Guide);

			MKNAME(meta_Map_Symbol_WalkPath, "Map<Symbol,WalkPath,less<Symbol>>");
			DEFINEMAP2(Symbol, Handle<SoundBusSnapshot::Snapshot>, sym, hsnap, std::less<Symbol>);
			DEFINEDCARRAY(Guide);
			DEFINEMAP2(String, Map<String SEP String SEP std::less<String>>, str, Map_Str_Str, std::less<String>);
			
			DEFINET2(aud, AudioData);
			EXT(aud, aud);
			FIRSTMEM2(aud, mFilename, AudioData, string, 0);
			NEXTMEM2(aud, mLength, AudioData, float, 0, mFilename);
			ADD(aud);

			DEFINET2(pps, ParticlePropertySamples);
			FIRSTMEM2(pps, mSampleCount, ParticlePropertySamples, int, 0);
			SERIALIZER(pps, ParticlePropertySamples);
			ADD(pps);

			DEFINET2(nd, Node);
			FIRSTMEM2(nd, mName, Node, symbol, 0);
			NEXTMEM2(nd, mFlags, Node, flags, 0, mName);
			NEXTMEM2(nd, mLocalXform, Node, transform, 0, mFlags);
			NEXTMEM2(nd, mGlobalXform, Node, transform, 0, mLocalXform);
			ADD(nd);

			DEFINET2(snode, SklNodeData);
			FIRSTMEM2(snode, mNode, SklNodeData, nd, 0);
			ADD(snode);

			DEFINEDCARRAY(SklNodeData, SklNodeData);

			// LEGACY D3DMESH 

			DEFINET2(M_frAQBc8Wsa, LegacyD3DMesh::T3VertexComponent);
			FIRSTMEM2(M_frAQBc8Wsa, mOffset, LegacyD3DMesh::T3VertexComponent, __uint32, 0x0);
			NEXTMEM2(M_frAQBc8Wsa, mCount, LegacyD3DMesh::T3VertexComponent, __uint32, 0x0, mOffset);
			NEXTMEM2(M_frAQBc8Wsa, mType, LegacyD3DMesh::T3VertexComponent, __uint32, 0x40, mCount);
			FIRSTENUM2(M_frAQBc8Wsa, mType, "eVTypeU16N", M_1xVPfvJcrg, 5, 0);
			NEXTENUM2(M_frAQBc8Wsa, mType, "eVTypeS16N", M_RYwTiizs2t, 4, 0, M_1xVPfvJcrg);
			NEXTENUM2(M_frAQBc8Wsa, mType, "eVTypeU8N", M_rQF69AzBla, 3, 0, M_RYwTiizs2t);
			NEXTENUM2(M_frAQBc8Wsa, mType, "eVTypeS8N", M_x3CF3EDNhm, 2, 0, M_rQF69AzBla);
			NEXTENUM2(M_frAQBc8Wsa, mType, "eVTypeFloat", M_3soLBPh71Y, 1, 0, M_x3CF3EDNhm);
			NEXTENUM2(M_frAQBc8Wsa, mType, "eVTypeNone", M_exuieaoEiI, 0, 0, M_3soLBPh71Y);
			ADD(M_frAQBc8Wsa);

			DEFINESARRAY_(LegacyD3DMesh::T3VertexComponent, meshcmpleg, 13);

			DEFINET2(M_gxIX4a2dRE, LegacyD3DMesh::T3VertexBuffer);
			FIRSTMEM2(M_gxIX4a2dRE, mNumVerts, LegacyD3DMesh::T3VertexBuffer, int, 0x0);
			NEXTMEM2(M_gxIX4a2dRE, mVertSize, LegacyD3DMesh::T3VertexBuffer, int, 0x0, mNumVerts);
			NEXTMEM2(M_gxIX4a2dRE, mFlags, LegacyD3DMesh::T3VertexBuffer, flags, 0x0, mVertSize);
			NEXTMEM2(M_gxIX4a2dRE, mUsage, LegacyD3DMesh::T3VertexBuffer, int, 0x0, mFlags);
			NEXTMEM2(M_gxIX4a2dRE, mVertexComponents, LegacyD3DMesh::T3VertexBuffer,sarray_meshcmpleg_13, 0x0, mUsage);
			ADD(M_gxIX4a2dRE);

			DEFINET2(M_bbSqWy6yhK, LegacyD3DMesh::T3IndexBuffer);
			FIRSTMEM2(M_bbSqWy6yhK, mFormat, LegacyD3DMesh::T3IndexBuffer, int, 0x0);
			NEXTMEM2(M_bbSqWy6yhK, mNumIndicies, LegacyD3DMesh::T3IndexBuffer, int, 0x0, mFormat);
			NEXTMEM2(M_bbSqWy6yhK, mFlags, LegacyD3DMesh::T3IndexBuffer, flags, 0x0, mNumIndicies);
			NEXTMEM2(M_bbSqWy6yhK, mUsage, LegacyD3DMesh::T3IndexBuffer, int, 0x0, mFlags);
			ADD(M_bbSqWy6yhK);

			DEFINET2(M_IDCdJOyapn, LegacyD3DMesh::Texture);
			FIRSTMEM2(M_IDCdJOyapn, mName, LegacyD3DMesh::Texture, Handletex, 0x0);
			NEXTMEM2(M_IDCdJOyapn, mFlags, LegacyD3DMesh::Texture, flags, 0x40000, mName);
			FIRSTFLAG(M_IDCdJOyapn, mFlags, "eFlagHasSpecular", M_xrpMCARCr4, 0x4);
			NEXTFLAG(M_IDCdJOyapn, mFlags, "eFlagHasNonLightmap", M_zdGc81tBDK, 0x2, M_xrpMCARCr4);
			NEXTFLAG(M_IDCdJOyapn, mFlags, "eFlagHasLightmap", M_sMlaZTXC1O, 0x1, M_zdGc81tBDK);
			NEXTMEM2(M_IDCdJOyapn, mNameSymbol, LegacyD3DMesh::Texture, symbol, 0x0, mFlags);
			NEXTMEM2(M_IDCdJOyapn, mBoundingBox, LegacyD3DMesh::Texture, bbox, 0x0, mNameSymbol);
			NEXTMEM2(M_IDCdJOyapn, mBoundingSphere, LegacyD3DMesh::Texture, sphere, 0x0, mBoundingBox);
			NEXTMEM2(M_IDCdJOyapn, mMaxObjAreaPerUVArea, LegacyD3DMesh::Texture, float, 0x0, mBoundingSphere);
			NEXTMEM2(M_IDCdJOyapn, mAverageObjAreaPerUVArea, LegacyD3DMesh::Texture, float, 0x0, mMaxObjAreaPerUVArea);
			ADD(M_IDCdJOyapn);

			DEFINESARRAY(int, 14);

			DEFINET2(M_8YFOGKjxRr, LegacyD3DMesh::TriangleSet);
			FIRSTMEM2(M_8YFOGKjxRr, mFlags, LegacyD3DMesh::TriangleSet, flags, 0x40000);
			FIRSTFLAG(M_8YFOGKjxRr, mFlags, "eFlagSoftDepthTest", M_JBdT3hVOfo, 0x10000000);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagVertexColorSubtract", M_aMeAjSWfch, 0x4000000, M_JBdT3hVOfo);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagSpecularGloss", M_oZYFYZ5B6k, 0x2000000, M_aMeAjSWfch);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagSingleMatrixSkinning", M_zMCk8R6BEu, 0x1000000, M_oZYFYZ5B6k);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagSharpDetailMap", M_ZMrF6cI6NX, 0x800000, M_zMCk8R6BEu);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagViewDependentDetailMap", M_8DYdD3ojxS, 0x400000, M_ZMrF6cI6NX);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagHasToonOutline", M_nqPTGfRyil, 0x200000, M_8DYdD3ojxS);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagNewToonOutline", M_OYGxlSXPtL, 0x100000, M_nqPTGfRyil);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagShadowInvertDetailmap", M_JboH8S4kwI, 0x80000, M_OYGxlSXPtL);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagDistanceFieldLightmap", M_gTxSl1C00G, 0x40000, M_JboH8S4kwI);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagVertexColor", M_OzOLMrbAyf, 0x20000, M_gTxSl1C00G);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagTriangleStrips", M_KUUTsGCoRb, 0x10000, M_OzOLMrbAyf);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagUVScreenSpaceZoom", M_lsaqv6Updv, 0x4000, M_KUUTsGCoRb);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagOverride3DAlpha", M_NIsNrmwUlN, 0x2000, M_lsaqv6Updv);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagVertexAlpha", M_5u9t3tgj2t, 0x400, M_NIsNrmwUlN);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagSelfIlluminated", M_usZBauFkrF, 0x200, M_5u9t3tgj2t);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagCastShadows", M_CJCmZFOv1Q, 0x100, M_usZBauFkrF);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagReceiveShadows", M_DIIXJNZI95, 0x80, M_CJCmZFOv1Q);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagDetailBumpAsNormalMap", M_hFQr77BIEl, 0x40, M_DIIXJNZI95);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagBumpAsNormalMap", M_yct4Aj6PKn, 0x20, M_hFQr77BIEl);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagDoubleSided", M_ZpzRiKYpZg, 0x10, M_yct4Aj6PKn);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagVertexAnimation", M_nSOKlq8Azr, 0x8, M_ZpzRiKYpZg);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagToonEnvLighting", M_HqDoGL4RHy, 0x4, M_nSOKlq8Azr);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagToonRendering", M_qyX3g5Ayy6, 0x2, M_HqDoGL4RHy);
			NEXTFLAG(M_8YFOGKjxRr, mFlags, "eFlagSelected", M_1leDYT3CEW, 0x1, M_qyX3g5Ayy6);
			NEXTMEM2(M_8YFOGKjxRr, mBonePaletteIndex, LegacyD3DMesh::TriangleSet, int, 0x0, mFlags);
			NEXTMEM2(M_8YFOGKjxRr, mLocalTransformPaletteIndex, LegacyD3DMesh::TriangleSet, int, 0x0, mBonePaletteIndex);
			NEXTMEM2(M_8YFOGKjxRr, mLocalTransformIndex, LegacyD3DMesh::TriangleSet, int, 0x0, mLocalTransformPaletteIndex);
			NEXTMEM2(M_8YFOGKjxRr, mMinVertIndex, LegacyD3DMesh::TriangleSet, int, 0x0, mLocalTransformIndex);
			NEXTMEM2(M_8YFOGKjxRr, mMaxVertIndex, LegacyD3DMesh::TriangleSet, int, 0x0, mMinVertIndex);
			NEXTMEM2(M_8YFOGKjxRr, mStartIndex, LegacyD3DMesh::TriangleSet, int, 0x0, mMaxVertIndex);
			NEXTMEM2(M_8YFOGKjxRr, mNumPrimitives, LegacyD3DMesh::TriangleSet, int, 0x0, mStartIndex);
			NEXTMEM2(M_8YFOGKjxRr, mBoundingBox, LegacyD3DMesh::TriangleSet, bbox, 0x0, mNumPrimitives);
			NEXTMEM2(M_8YFOGKjxRr, mBoundingSphere, LegacyD3DMesh::TriangleSet, sphere, 0x0, mBoundingBox);
			NEXTMEM2(M_8YFOGKjxRr, mTxIndex, LegacyD3DMesh::TriangleSet, sarray_int_14, 0x0, mBoundingSphere);
			NEXTMEM2(M_8YFOGKjxRr, mTriStrips, LegacyD3DMesh::TriangleSet, DCArray_ushort, 0x0, mTxIndex);
			NEXTMEM2(M_8YFOGKjxRr, mNumTotalIndices, LegacyD3DMesh::TriangleSet, int, 0x0, mTriStrips);
			NEXTMEM2(M_8YFOGKjxRr, mfBumpHeight, LegacyD3DMesh::TriangleSet, float, 0x0, mNumTotalIndices);
			NEXTMEM2(M_8YFOGKjxRr, mfDetailBumpHeight, LegacyD3DMesh::TriangleSet, float, 0x0, mfBumpHeight);
			NEXTMEM2(M_8YFOGKjxRr, mSpecularPower, LegacyD3DMesh::TriangleSet, float, 0x0, mfDetailBumpHeight);
			NEXTMEM2(M_8YFOGKjxRr, mSpecularColor, LegacyD3DMesh::TriangleSet, color, 0x0, mSpecularPower);
			NEXTMEM2(M_8YFOGKjxRr, mAmbientColor, LegacyD3DMesh::TriangleSet, color, 0x0, mSpecularColor);
			NEXTMEM2(M_8YFOGKjxRr, mToonOutlineColor, LegacyD3DMesh::TriangleSet, color, 0x0, mAmbientColor);
			NEXTMEM2(M_8YFOGKjxRr, mToonOutlineInvertColor, LegacyD3DMesh::TriangleSet, color, 0x0, mToonOutlineColor);
			NEXTMEM2(M_8YFOGKjxRr, mToonOutlineSize, LegacyD3DMesh::TriangleSet, float, 0x0, mToonOutlineInvertColor);
			NEXTMEM2(M_8YFOGKjxRr, mToonMaxZConstOutlineSize, LegacyD3DMesh::TriangleSet, float, 0x0, mToonOutlineSize);
			NEXTMEM2(M_8YFOGKjxRr, mToonMinZConstOutlineSize, LegacyD3DMesh::TriangleSet, float, 0x0, mToonMaxZConstOutlineSize);
			NEXTMEM2(M_8YFOGKjxRr, mGlowIntensity, LegacyD3DMesh::TriangleSet, float, 0x0, mToonMinZConstOutlineSize);
			NEXTMEM2(M_8YFOGKjxRr, mReceiveShadowIntensity, LegacyD3DMesh::TriangleSet, float, 0x0, mGlowIntensity);
			NEXTMEM2(M_8YFOGKjxRr, mAlphaMode, LegacyD3DMesh::TriangleSet, int, 0x40, mReceiveShadowIntensity);
			FIRSTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAlphaInvAlphaTest", M_8K69rklPGt, 10, 0);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAlphaSubtract", M_fUR2IZ2mB1, 9, 0, M_8K69rklPGt);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAlphaAdd", M_S7NLt2nQfj, 8, 0, M_fUR2IZ2mB1);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeInvMultiply", M_3SL3XByImc, 7, 0, M_S7NLt2nQfj);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeMultiply", M_iQTqVaAUnJ, 6, 0, M_3SL3XByImc);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAdd", M_vUmHVQGsTm, 5, 0, M_iQTqVaAUnJ);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeInvAlphaTest", M_vHeuSEBMSP, 4, 0, M_vUmHVQGsTm);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAlphaTest", M_h2Q6dQAjy4, 3, 0, M_vHeuSEBMSP);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAlphaAlphaTest", M_KjsfSXF4YP, 2, 0, M_h2Q6dQAjy4);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeAlpha", M_8arF3SLLny, 1, 0, M_KjsfSXF4YP);
			NEXTENUM2(M_8YFOGKjxRr, mAlphaMode, "eBlendModeNormal", M_30KKziHtRb, 0, 0, M_8arF3SLLny);
			NEXTMEM2(M_8YFOGKjxRr, mfReflectivity, LegacyD3DMesh::TriangleSet, float, 0x0, mAlphaMode);
			NEXTMEM2(M_8YFOGKjxRr, mToonShades, LegacyD3DMesh::TriangleSet, int, 0x0, mfReflectivity);
			NEXTMEM2(M_8YFOGKjxRr, mUVGenMode, LegacyD3DMesh::TriangleSet, int, 0x40, mToonShades);
			FIRSTENUM2(M_8YFOGKjxRr, mUVGenMode, "TriangleSet::eUVGENPositionCameraSpace", M_OE2u8qvJP3, 1, 0);
			NEXTENUM2(M_8YFOGKjxRr, mUVGenMode, "TriangleSet::eUVGENDefault", M_B6Uiu1gJ03, 0, 0, M_OE2u8qvJP3);
			NEXTMEM2(M_8YFOGKjxRr, mUVScreenSpaceScaling, LegacyD3DMesh::TriangleSet, float, 0x0, mUVGenMode);
			NEXTMEM2(M_8YFOGKjxRr, mSpecularOnAlpha, LegacyD3DMesh::TriangleSet, float, 0x0, mUVScreenSpaceScaling);
			NEXTMEM2(M_8YFOGKjxRr, mSubsurfaceScateringRadius, LegacyD3DMesh::TriangleSet, float, 0x0, mSpecularOnAlpha);
			NEXTMEM2(M_8YFOGKjxRr, mDiffuseColor, LegacyD3DMesh::TriangleSet, color, 0x0, mSubsurfaceScateringRadius);
			ADD(M_8YFOGKjxRr);

			DEFINET2(M_OhmWvn7MIr, LegacyD3DMesh::PaletteEntry);
			FIRSTMEM2(M_OhmWvn7MIr, mBoneName, LegacyD3DMesh::PaletteEntry, symbol, 0x0);
			NEXTMEM2(M_OhmWvn7MIr, mBoundingBox, LegacyD3DMesh::PaletteEntry, bbox, 0x0, mBoneName);
			NEXTMEM2(M_OhmWvn7MIr, mBoundingSphere, LegacyD3DMesh::PaletteEntry, sphere, 0x0, mBoundingBox);
			NEXTMEM2(M_OhmWvn7MIr, mNumVerts, LegacyD3DMesh::PaletteEntry, int, 0x0, mBoundingSphere);
			ADD(M_OhmWvn7MIr);

			DEFINET2(M_0YKI4JSgVS1, LegacyD3DMesh::LocalTransformEntry);
			FIRSTMEM2(M_0YKI4JSgVS1, mTransform, LegacyD3DMesh::LocalTransformEntry, transform, 0x0);
			NEXTMEM2(M_0YKI4JSgVS1, mCameraFacingType, LegacyD3DMesh::LocalTransformEntry, int, 0x0, mTransform);
			ADD(M_0YKI4JSgVS1);

			DEFINET2(legmeshanim, LegacyD3DMesh::VertexAnimation);
			meta_legmeshanim.mbTypeUnfinished = true;
			ADD(legmeshanim);//TODO

			DEFINET2(legmeshskin, LegacyD3DMesh::SkinningEntry);
			meta_legmeshskin.mbTypeUnfinished = true;
			ADD(legmeshskin);//TODO

			DEFINET2(legmeshbone, LegacyD3DMesh::BoneEntry);
			meta_legmeshbone.mbTypeUnfinished = true;
			ADD(legmeshbone);//TODO

			DEFINET2(legmeshanmvert, LegacyD3DMesh::AnimatedVertexEntry);
			meta_legmeshanmvert.mbTypeUnfinished = true;
			ADD(legmeshanmvert);//TODO

			DEFINET2(legmeshanmvertgroup, LegacyD3DMesh::AnimatedVertexGroupEntry);
			meta_legmeshanmvertgroup.mbTypeUnfinished = true;
			ADD(legmeshanmvertgroup);//TODO

			DEFINEDCARRAY2(LegacyD3DMesh::TriangleSet, legtriset);
			DEFINEDCARRAY2(LegacyD3DMesh::VertexAnimation, legvertanm);
			DEFINEDCARRAY2(LegacyD3DMesh::PaletteEntry, legpalentry);
			DEFINEDCARRAY2(LegacyD3DMesh::SkinningEntry, legskinentry);
			DEFINEDCARRAY2(LegacyD3DMesh::BoneEntry, legboneentry);
			DEFINEDCARRAY2(LegacyD3DMesh::LocalTransformEntry, legloctrans);
			DEFINEDCARRAY2(DCArray<LegacyD3DMesh::PaletteEntry>, legpalentryarray);
			DEFINEDCARRAY2(DCArray<LegacyD3DMesh::LocalTransformEntry>, legloctransarray);
			DEFINEDCARRAY2(LegacyD3DMesh::AnimatedVertexEntry, leganmentry);
			DEFINEDCARRAY2(LegacyD3DMesh::Texture, legmeshtex_arr);
			DEFINESARRAY_(DCArray<LegacyD3DMesh::Texture>, legmeshtex, 14);

			DEFINEMAP2(Symbol, LegacyD3DMesh::AnimatedVertexGroupEntry, Symbol, leganmvertgroupentry, std::less<Symbol>);
			MKNAME(meta_Map_Symbol_leganmvertgroupentry, "Map<Symbol,LegacyD3DMesh::AnimatedVertexGroupEntry,less<Symbol>>");

			DEFINET2(M_1it18OLKdt, LegacyD3DMesh);
			SERIALIZER(M_1it18OLKdt, LegacyD3DMesh);
			ADDFLAGS(M_1it18OLKdt, 0x200000);
			FIRSTMEM2(M_1it18OLKdt, mName, LegacyD3DMesh, string, 0x0);
			NEXTMEM2(M_1it18OLKdt, mVersion, LegacyD3DMesh, int, 0x0, mName);
			NEXTMEM2(M_1it18OLKdt, mFlags, LegacyD3DMesh, flags, 0x40000, mVersion);
			FIRSTFLAG(M_1it18OLKdt, mFlags, "eDeformable", M_m9WgMaF0mJ, 0x800000);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasSoftwareSkinningStream", M_KATWd43Ofl, 0x400000, M_m9WgMaF0mJ);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasInterleavedStream", M_AQsoC7WO95, 0x200000, M_KATWd43Ofl);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eIsManualSort", M_efK2h3B98k, 0x100000, M_AQsoC7WO95);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasZeroVertexAlpha", M_TDHqyzKXoJ, 0x80000, M_efK2h3B98k);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eTriangleSetsFixedUp", M_MFchjR1wTY, 0x40000, M_TDHqyzKXoJ);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasVertexAnimation", M_I67ekrUydL, 0x10000, M_MFchjR1wTY);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasColorStream", M_RJBUDZXXFy, 0x800, M_I67ekrUydL);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasTangentStream", M_xfRWIz9RwJ, 0x400, M_RJBUDZXXFy);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasUV4Stream", M_akyvPmMoLB, 0x200, M_xfRWIz9RwJ);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasUV3Stream", M_CRyp0qZbXH, 0x100, M_akyvPmMoLB);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasUV2Stream", M_jtanzWWpCm, 0x80, M_CRyp0qZbXH);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasUV1Stream", M_fLf8hgMcVO, 0x40, M_jtanzWWpCm);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasBlendIdxStream", M_xm7E0c9GUa, 0x20, M_fLf8hgMcVO);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasBlendWeightStream", M_8m6xqqEZ1E, 0x10, M_xm7E0c9GUa);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasSmoothNormStream", M_NaQPFKnSJu, 0x8, M_8m6xqqEZ1E);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasNormStream", M_hqhH2Wlcvy, 0x4, M_NaQPFKnSJu);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasPosStream", M_L4MAeQXggM, 0x2, M_hqhH2Wlcvy);
			NEXTFLAG(M_1it18OLKdt, mFlags, "eHasIndexBuffer", M_nn79XAVJRr, 0x1, M_L4MAeQXggM);
			NEXTMEM2(M_1it18OLKdt, mBoundingBox, LegacyD3DMesh, bbox, 0x0, mFlags);
			NEXTMEM2(M_1it18OLKdt, mBoundingSphere, LegacyD3DMesh, sphere, 0x0, mBoundingBox);
			NEXTMEM2(M_1it18OLKdt, mTriangleSets, LegacyD3DMesh, DCArray_legtriset, 0x0, mBoundingSphere);
			NEXTMEM2(M_1it18OLKdt, mVertexAnimations, LegacyD3DMesh, DCArray_legvertanm, 0x0, mTriangleSets);
			NEXTMEM2(M_1it18OLKdt, mBonePalettes, LegacyD3DMesh, DCArray_legpalentryarray, 0x0, mVertexAnimations);
			NEXTMEM2(M_1it18OLKdt, mSkinningData, LegacyD3DMesh, DCArray_legskinentry, 0x0, mBonePalettes);
			NEXTMEM2(M_1it18OLKdt, mBoneData, LegacyD3DMesh, DCArray_legboneentry, 0x0, mSkinningData);
			NEXTMEM2(M_1it18OLKdt, mLocalTransformPalettes, LegacyD3DMesh, DCArray_legloctransarray, 0x0, mBoneData);
			NEXTMEM2(M_1it18OLKdt, mTriangleStripState, LegacyD3DMesh, int, 0x0, mLocalTransformPalettes);
			NEXTMEM2(M_1it18OLKdt, mAnimatedVertexCount, LegacyD3DMesh, int, 0x0, mTriangleStripState);
			NEXTMEM2(M_1it18OLKdt, mTextures, LegacyD3DMesh, sarray_legmeshtex_14, 0x0, mAnimatedVertexCount);
			NEXTMEM2(M_1it18OLKdt, mDiffuseScale, LegacyD3DMesh, vec2, 0x0, mTextures);
			NEXTMEM2(M_1it18OLKdt, mLightMapScale, LegacyD3DMesh, vec2, 0x0, mDiffuseScale);
			NEXTMEM2(M_1it18OLKdt, mShadowMapScale, LegacyD3DMesh, vec2, 0x0, mLightMapScale);
			NEXTMEM2(M_1it18OLKdt, mDetailScale, LegacyD3DMesh, vec2, 0x0, mShadowMapScale);
			NEXTMEM2(M_1it18OLKdt, mScaledUVScale, LegacyD3DMesh, vec2, 0x0, mDetailScale);
			NEXTMEM2(M_1it18OLKdt, mToolAnimatedVertexEntries, LegacyD3DMesh, DCArray_leganmentry, 0x0, mScaledUVScale);
			NEXTMEM2(M_1it18OLKdt, mToolAnimatedVertexGroupEntries, LegacyD3DMesh, Map_Symbol_leganmvertgroupentry, 0x0, mToolAnimatedVertexEntries);
			NEXTMEM2(M_1it18OLKdt, mToolProps, LegacyD3DMesh, tp, 0x20, mToolAnimatedVertexGroupEntries);
			ADD(M_1it18OLKdt);

			DEFINET2(dlgr, DialogResource);
			meta_dlgr.mbTypeUnfinished = true;
			EXT(dlgr, "dlg");
			ADD(dlgr);

			//TODO BUNDLE FILES
			// OLD DIALOG, DIALOGRESOURCE (OLDER GAMES)
			// PARTICLE PROPERTIES (sEE THE HEADER) .particle
			// CLASS AUDIODATA (.aud), legacy, has mFilename str followed by mLength  float thats it, check older game
			//TODO ADD ALL KEYS FROM SEARCH IN IDA ::CreateModuleProps AND ::CreatePrefs ::CreateToolPrefs
			//TODO ALL SingleValue<T> (make the templaet class) and also CompressedKeys... and CompressedKeys<T>

			//TODO keyframedvalue<compressedpathblockingvalue::compressedpathinfokey>
			// keyframedvalue<handle<chore>>
			//todo animatedvalueinterface<handle<t3texture>>
			//hingejointkey pivotjointkey
			//enumemittertriggerenable
			//enumhbaoblurquality
			//footsteps::footstepbank
			//footsteps2::footstepbank
			//animatedvalueinterface<handle<font>>
			//keyframedvalue<scriptenum>
			//enumparticlegeometrytype
			//enlightenmodule::enlightencubemapsettings
			//enlightenmodule::enlightenprobevolumesettings
			//keyframedvalue<quaternion>
			//keyframedvalue<handle<sounddata>>
			//enumt3lightenvshadowtype
			//keyframedvalue<symbol> , ensure polar too idk
			//idleslotdefaults
			//enlightenmodule::enumesimplifymode, TODO check all enlighten meta classes in ida
			//filenamebase
			//particleinversekinematics
			//keyframedvalue<handle<walkboxes>>
			//animatedvalueinterface<handle<d3dmesh>>
			//animatedvalueinterface<compressedpathblockingvalue::compressedpathinfokey>
			//chorecorderparameters
			//enumemittersenabletype
			//enumemitterspriteanimationselection
			//keyframedvalue<soundeventname<1>> and 0, ensure compressedkeys soundeventname 0 and 1 too
			//enumhbaodeinterleaving
			//enlightenmodule::enlightenadaptiveprobevolumesettings
			//enlightenmodule::enumeupdatemethod
			//animatedvalueinterface<handle<dlg>>
			// //keyframedvalue<handle<dlg>>
			//t3lightprobeinternaldata::qualityentry
			//enlightenmodule::enumequality
			//enumhbaoresolution
			//enumt3lightenvshadowquality
			//enlightenmodule::enlightensystemsettings
			//procedural_lookat::constraint
			//keyframedvalue<vector2> ensure others etoo
			//compressedpathblockingvalue::compressedpathinfokey
			//keyframedvalue<handle<d3dmesh>>
			//soundreverbpreset
			//animatedvalueinterface<handle<soundreverbdefinition>>
			//enumemitterparticlecounttype
			//keyframedvalue<soundeventname<0>> done above
			//enumparticleaffectortype
			//animatedvalueinterface<vector2> check otther vecs
			//enumemitterconstrainttype
			//filename<soundeventbankdummy>
			// //keyframedvalue<handle<font>>
			// enlightenmodule::enlightenmeshsettings
			// enumemitterspriteanimationtype
			// //enumemittercolortype
			// animatedvalueinterface<symbol>
			// enumparticlesortmode
			// animatedvalueinterface<scriptenum>
			// t3lightsceneinternaldata::qualityentry
			// enumhbaoqualitylevel
			//enumemitterspawnshape
			//enlightenmodule::enumeinstancetype
			//animatedvalueinterface<quaternion>
			// linkedballtwistjointkey
			//animatedvalueinterface<soundeventname<0>>
			//enlightenmodule::enumeproberesolution
			//enumbokehqualitylevel
			//enumt3lightenvmobility
			//font::fontcreateparam
			//styleidletransitionsres
			// enlightenmodule::enumequalitywithdefault
			//animatedvalueinterface<soundeventname<1>>
			//peragentclipresourcefilter
			//enumglowqualitylevel
			//animatedvalueinterface<handle<phonemetable>>
			//particlepropconnect
			//enumtextorientationtype
			//enlightenmodule::enlightenlightsettings
			//animatedvalueinterface<color>
			//keyframedvalue<color>
			// //t3lightenvlod
			//animatedvalueinterface<handle<walkboxes>>
			//enumt3materialnormalspacetype
			//enumhbaoparticipationtype
			// //keyframedvalue<animorchore>
			//t3lightprobeinternaldata
			// t3lightenvinternaldata
			// scene::agentqualitysettings
			// animatedvalueinterface<handle<sounddata>>
			//compressedpathblockingvalue
			//enlightenmodule::enlightenprimitivesettings
			//enumbokehocclusiontype
			//scriptenum ? eg WDC_pc_WalkingDead101_anichore.ttarch2/env_motorinnmission_timer_upstairsZombies1_1.chore
			//keyframedvalue<handle<soundreverbdefinition>>
			//keyframedvalue<handle<phonemetable>>
			//animatedvalueinterface<handle<chore>>
			// keyframedvalue<handle<t3texture>>
			// enumdepthoffieldtype
			// enumhbaopreset
			// t3lightenvinternaldata::qualityentry
			// clipresourcefilter
			// placeableballtwistjointkey
			// idletransitionsettings
			//polar ??!
			// enumhbaoperpixelnormals
			// enlightenmodule::enlightenautoprobevolumesettings
			//enumlightcellblendmode
			//particlelodkey

			//ffecc5b70ad06be6 lots of props eg  WDC_pc_ProjectSeason4_data.ttarch2/player.prop
			//f048fe3ecb28e1e1 - WDC_pc_Project_data.ttarch2/project_fonts.prop
			//ee65a936c01e9c9a lots of chores eg WDC_pc_Menu_anichore.ttarch2/dorian_face_default.chore
			//edaf4fa786b3cd1d some chores WDC_pc_WalkingDead402_anichore.ttarch2/env_greenhouse_act2_cs_opening_8.chore
			//e2b3077ab1c2dce7 lots of chores eg WDC_pc_WalkingDead201_anichore.ttarch2/env_cabinExterior_CS_enter_8.chore
			//d98575c31ca63753 - WDC_pc_ProjectSeason1_data.ttarch2/statsInfo_text_italian.prop
			//d940a60d3de56488 WDC_pc_Project_data.ttarch2/module_animation_constraints.prop
			//d06ada3711d9cba0 WDC_pc_Project_data.ttarch2/project_fonts.prop
			//cdfc7236db3b636b WDC_pc_WalkingDead401_data.ttarch2/adv_trainStation.scene
			// cc644d410f58e65c lots of chores eg WDC_pc_MenuSeason2_anichore.ttarch2/ui_menu_pausemusic.chore likely a music related thing
			//cb9b7b7c2d2a90e7 lots of scene so prop eg WDC_pc_Project_data.ttarch2/module_env_light.prop
			//bdcd794531efec9d lots of chores WDC_pc_Boot_anichore.ttarch2/ui_boot_legal.chore
			//bb7e5208092caa9f  WDC_pc_Project_data.ttarch2/module_animation_constraints.prop
			//ab929fc5155636ef see - WDC_pc_Project_data.ttarch2/module_animation_constraints.prop
			//a96424e7fccbf8eb lighting something eg WDC_pc_Project_data.ttarch2/module_env_light.prop
			//a86bac01e869bc29 lots of chores WDC_pc_MenuSeason2_anichore.ttarch2/ui_menu_backButton_out.chore
			//a062bdc063405146 lots of chores 	 - WDC_pc_WalkingDead401_anichore.ttarch2/adv_forestShack_aJsitOnBed.chore
			//9b20c35ca1e12bd5 see  WDC_pc_Project_data.ttarch2/project_render_system.prop
			//995fc5c3cb87e5f8 some prop dlog scene , physics? DC_pc_Project_data.ttarch2/module_physicsobject.prop
			//909b73dd05e14be3 lots of chores eh WDC_pc_Menu_anichore.ttarch2/ui_statsBar_idle.chore
			//74a5f683afac29ed lots of scenes eg WDC_pc_WalkingDead101_data.ttarch2/ui_keyArt.scene
			//69cc70c207e837f3 lots of scenes and a prop likely lighing  WDC_pc_Project_data.ttarch2/module_env_light.prop
			//65813b7d04ec2309 lots of chores  WDC_pc_MenuSeason3_anichore.ttarch2/ui_keyArt_idle.chore
			//6113dd4685b2760c lots of chors  - WDC_pc_MenuSeason1_anichore.ttarch2/ui_menu_rewind_right.chore
			//580897c10167b2e0 lots of chores  eg WDC_pc_WalkingDead401_anichore.ttarch2/env_trainStation_act1_cs_firstZombie_1.chore
			//5697b0200859d9b7 check  WDC_pc_ProjectSeason1_data.ttarch2/statsInfo_text_italian.prop
			//50244e8c35100e64 lots LOTS eg WDC_pc_Menu_anichore.ttarch2/omar_face_default.chore
			//487d0696c99d7761 WDC_pc_Project_data.ttarch2/module_base_particleemitter.prop
			//390898756fe17e4a some chores WDC_pc_WalkingDead205_anichore.ttarch2/ui_endDays.chore
			//33a1f0cb9e5bbc70 some chores WDC_pc_WalkingDead402_anichore.ttarch2/zombat_clemKillZombieKnife03.chore
			//2f887206c5aef2e0 lots of chores WDC_pc_WalkingDead401_anichore.ttarch2/env_trainStation_act1_cs_backSeatZombie_1.chore
			//2889e75b877634e9 lots of chores eg  WDC_pc_Menu_anichore.ttarch2/zoo_season4_animMitch_05.chore
			//23f45a685c64f85f lots of chores eg WDC_pc_MenuSeasonM_anichore.ttarch2/ui_diorama_episode3Show.chore
			//1ad7882b6eb612a6 lots of chores eg wdc menu ani, ui episodes show
			//19f654ed32a2c708 WDC_pc_Project_data.ttarch2/module_material.prop
			//18525353197ddc4e WDC_pc_Project_data.ttarch2/project_render_system.prop
			//f8213b836fac58c lots o f chores in eg  WDC_pc_WalkingDead403_anichore.ttarch2/zombat_clemKillZombieKnife03.chore
			//e683ed960d93e60 WDC_pc_WalkingDead303_anichore.ttarch2/env_garciaDominguezHouse_cs_dustSettles_11.chore
			//dcb910d0f1db2c7 WDC_pc_WalkingDead202_data.ttarch2/sk55_zombie200_torsoLongSleeveA.prop
			//a5030be2c2b62c7 in chores mostly menus3 (wdc) anichore, eg ui menu new icon
			// 633339b0ebd064b scenes. look in wd301 keyart ui, junkyard for eg LIKELY AN ENUM IN A AGENT PROP
			//421a6b6c808e28f ? in chores, likely a keyframe

		}
		Initialize2();
		Initialize3();
		Initialize4();
	}

	//The rest of the initialize functions would be for that they say but it would take long to search each member up so its in initialize()

	void Initialize2() {



		//HANDLES
	}

	void Initialize3() {



		//ANIM VALUE INTERFACES
	}

	void Initialize4() {



		//CONTAINERS 
	}

}
