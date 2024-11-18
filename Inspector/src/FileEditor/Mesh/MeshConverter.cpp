#include "../../TelltaleInspector.h"

void write_face(std::ofstream& obj, unsigned short* currentFace, int off) {
	obj << "f "
		<< (currentFace[0] + 1 + off) << "/" << (currentFace[0] + 1 + off) << "/" << (currentFace[0] + 1 + off) << " ";
	obj << (currentFace[1] + 1 + off) << "/" << (currentFace[1] + 1 + off) << "/" << (currentFace[1] + 1 + off) << " ";
	obj << (currentFace[2] + 1 + off) << "/" << (currentFace[2] + 1 + off) << "/" << (currentFace[2] + 1 + off) << "\n\n";
}

//CONVERT NEW D3DMESH => OBJ (GAMES NEWER AND INCLUDING THE WALKING DEAD: MICHONNE)
bool convert_obj_newer(D3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t) {
	std::ofstream obj{ objfile };
	TelltaleToolLib_SetBlowfishKey(gid);
	std::ofstream mtl{ mtlfile };
	obj << "mtllib " << (std::filesystem::path{ mtlfile }.filename().string()) << "\n\n";
	obj << "# Converted to OBJ by The Telltale Inspector\n";
	obj << "# https://github.com/LucasSaragosa\n\n\n";
	mtl << "# Converted to MTL by The Telltale Inspector\n";
	mtl << "# https://github.com/LucasSaragosa\n";

	if (mesh.mMeshData.mLODs.GetSize() == 0) {
		TTL_Log("WARNING: Mesh contains no LODs, assuming its empty..\n");
		return true;
	}

	std::vector<void*> full_index_buffer{};
	for (int i = 0; i < mesh.mMeshData.mVertexStates.mSize; i++) {
		u32 size = 0;
		for (int j = 0; j < mesh.mMeshData.mVertexStates[i]->mIndexBufferCount; j++) {
			T3GFXBuffer* buf = mesh.mMeshData.mVertexStates[i]->mpIndexBuffer[j].get();
			if (buf->mBufferFormat != eGFXPlatformFormat_U16) {
				err = "Index buffer format is not U16";
				return false;
			}
			size += buf->mCount * buf->mStride;
		}
		void* p = malloc(size);
		u32 offset = 0;
		for (int j = 0; j < mesh.mMeshData.mVertexStates[i]->mIndexBufferCount; j++) {
			T3GFXBuffer* buf = mesh.mMeshData.mVertexStates[i]->mpIndexBuffer[j].get();
			memcpy(((char*)p) + offset, buf->mpCPUBuffer, buf->mCount * buf->mStride);
			offset += buf->mCount * buf->mStride;
		}
		full_index_buffer.push_back(p);
	}

	T3MeshLOD& lod = mesh.mMeshData.mLODs[0];//highest lod will be exported.
	//TODO if michonne look into mBatchesM

	T3GFXVertexState& vertexState = *mesh.mMeshData.mVertexStates[lod.mVertexStateIndex];
	T3GFXBuffer& positionsBuffer = *vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_Position)->mBufferIndex];//must have positions
	void* faces = full_index_buffer[lod.mVertexStateIndex];

	//WRITE POSITIONS
	for (int i = 0; i < positionsBuffer.mCount; i++) {
		Vector3 vert{};
		if (get_params(vertexState, eGFXPlatformAttribute_Position)->mFormat == eGFXPlatformFormat_UN16x4) {
			u16* raw = (u16*)((char*)positionsBuffer.mpCPUBuffer + (positionsBuffer.mStride * i) + get_params(vertexState, eGFXPlatformAttribute_Position)->mBufferOffset);
			Vector3 posScale = mesh.mMeshData.mPositionScale;
			Vector3 posOffset = mesh.mMeshData.mPositionOffset;//position W offset is for the last one but we dont need it
			vert.x = ((((float)raw[0]) / 65536.f) * posScale.x) + posOffset.x;
			vert.y = ((((float)raw[1]) / 65536.f) * posScale.y) + posOffset.y;
			vert.z = ((((float)raw[2]) / 65536.f) * posScale.z) + posOffset.z;
		}//might another fucking weird one idk ill do later if i see it
		else {
			if (get_params(vertexState, eGFXPlatformAttribute_Position)->mFormat != eGFXPlatformFormat_F32x3) {
				err = "Positions format is not F32x3!";
				return false;
			}
			int vertind = i;
			float* currentVert = (float*)((char*)positionsBuffer.mpCPUBuffer + (positionsBuffer.mStride * vertind) + get_params(vertexState, eGFXPlatformAttribute_Position)->mBufferOffset);
			vert = { currentVert[0], currentVert[1], currentVert[2] };
			if (vert.x > 0.f && vert.x < 0.00001f)
				vert.x = 0.f;
			if (vert.y > 0.f && vert.y < 0.00001f)
				vert.y = 0.f;
			if (vert.z > 0.f && vert.z < 0.00001f)
				vert.z = 0.f;
		}
		obj << "v " << vert.x << " " << vert.y << " " << vert.z << "\n";
	}

	//WRITE UVS
	if (get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mFormat != eGFXPlatformFormat_None) {
		T3GFXBuffer& uvBuffer = *vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mBufferIndex];
		GFXPlatformFormat fmt = get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mFormat;
		T3MeshTexCoordTransform uvTransform = mesh.mMeshData.mTexCoordTransform[0];//first UVs
		for (int i = 0; i < uvBuffer.mCount; i++) {
			void* currentVert = (float*)((char*)uvBuffer.mpCPUBuffer + (uvBuffer.mStride * i) + get_params(vertexState, eGFXPlatformAttribute_TexCoord)->mBufferOffset);
			obj << "vt " << decompose_new_UV(currentVert, fmt, 0, uvTransform) << " " << decompose_new_UV(currentVert, fmt, 1, uvTransform) << "\n";
		}
	}

	//WRITE NORMALS
	if (get_params(vertexState, eGFXPlatformAttribute_Normal)->mFormat != eGFXPlatformFormat_None) {
		T3GFXBuffer& nmBuffer = *vertexState.mpVertexBuffer[get_params(vertexState, eGFXPlatformAttribute_Normal)->mBufferIndex];
		GFXPlatformFormat fmt = get_params(vertexState, eGFXPlatformAttribute_Normal)->mFormat;
		for (int i = 0; i < nmBuffer.mCount; i++) {
			void* currentVert = (float*)((char*)nmBuffer.mpCPUBuffer + (nmBuffer.mStride * i) + get_params(vertexState, eGFXPlatformAttribute_Normal)->mBufferOffset);
			obj << "vn " << decompose_to_float(currentVert, fmt, 0) << " " << decompose_to_float(currentVert, fmt, 1) << " " << decompose_to_float(currentVert, fmt, 2) << "\n";
		}
	}

	bool bexit = false;
	for (int j = 0; j < 1; j++) {//only do one batch array? no idea what second one is - FIGURED OUT ITS  SHADOWS, FUCK THEM.
		for (int k = 0; k < lod.mBatches[j].GetSize(); k++) {
			bexit = false;
			T3MeshBatch& batch = lod.mBatches[j][k];
			T3MeshMaterial& material = mesh.mMeshData.mMaterials[batch.mMaterialIndex];
			PropertySet* pMaterialProps = nullptr;
			for (int i = 0; i < mesh.mInternalResources.mSize; i++) {
				if (mesh.mInternalResources[i].mHandleObjectInfo.mObjectName == material.mhMaterial.mHandleObjectInfo.mObjectName) {
					pMaterialProps = (PropertySet*)mesh.mInternalResources[i].GetHandleObjectPointer();
					if (pMaterialProps == nullptr) {
						TTL_Log("ERROR: Material internal resource properties could not be found\n");
						err = "Material internal resource properties could not be found";
						return false;
					}
					if (mesh.mInternalResources[i].GetTypeDesc() != GetMetaClassDescription<PropertySet>()) {
						TTL_Log("ERROR: Ignoring material inside mesh, because its resource is not a property set!!\n");
						bexit = true;
						break;
					}
					break;
				}
			}
			if (bexit)
				continue;
			Handle<T3Texture>* pTexHandle = nullptr;
			if (pMaterialProps == nullptr) {
				TTL_Log("WARNING: Mesh contains no internal resources, skipping batch...\n");
				continue;
			}
			if ((pTexHandle = (Handle<T3Texture>*) pMaterialProps->GetProperty("Material - Diffuse Texture")) == nullptr) {
				TTL_Log("WARNING: Skipping mesh batch for material inside mesh because it does not have an associated diffuse texture!\n");
				continue;
			}
			std::string texFile = "";//with D3DTX extension
			for (auto& texName : texnames)
				if (Symbol{ texName.c_str() }.GetCRC() == pTexHandle->mHandleObjectInfo.mObjectName.GetCRC()) {
					texFile = texName;
					break;
				}
			if (texFile.length() == 0) {
				TTL_Log("WARNING: Skipping mesh batch for material inside mesh because its texture file could not be located from the material!\n");
				continue;
			}

			texFile = texFile.substr(0, texFile.find_last_of('.'));

			/*WRITE MATERIAL INFO*/
			mtl << "\n\nnewmtl " << texFile << "\n";
			//anything else?

			obj << "\n\no " << texFile << "\n";
			obj << "g " << texFile << "\n";
			obj << "usemtl " << texFile << "\ns 1\n\n";

			mtlCB(t, texFile);
			mtl << "\tmap_Kd " << texFile << "\n";

			//WRITE FACES
			for (int i = 0; i < batch.mNumPrimitives; i++) {
				int faceIndex = (batch.mStartIndex / 3) + i;
				unsigned short* currentFace = (unsigned short*)((char*)faces + sizeof(unsigned short) * faceIndex * 3);

				write_face(obj, currentFace, batch.mBaseIndex);
			}
		}
	}

	for (auto& ptr : full_index_buffer)
		free(ptr);
	return true;
}

bool convert_obj_legacy(LegacyD3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t) {
	std::ofstream obj{ objfile };
	std::ofstream mtl{ mtlfile };
	obj << "mtllib " << (std::filesystem::path{ mtlfile }.filename().string()) << "\n\n";
	obj << "# Converted to OBJ by The Telltale Inspector\n";
	obj << "# https://github.com/LucasSaragosa\n\n\n";
	mtl << "# Converted to MTL by The Telltale Inspector\n";
	mtl << "# https://github.com/LucasSaragosa\n";

	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mType != 1) {
		TTL_Log("Cannot convert mesh: unknown vert pos type!\n");
		return false;
	}
	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mCount != 3) {
		TTL_Log("Cannot convert mesh: unknown vert pos type (not 3 components)!\n");
		return false;
	}

	if (mesh.mIndexBuffer.mFormat != 0x65 || mesh.mIndexBuffer.mFlags.mFlags != 0) {
		TTL_Log("Cannot convert mesh: untested index buffer flags / format\n");
		return false;
	}

	int posOffset = mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mOffset;

	/*Write actual mesh data*/

	LegacyD3DMesh::T3VertexBuffer* buf = mesh.GetVertexBuffer(LegacyD3DMesh::COMPONENT_TYPE_POSITION);

	if (buf == nullptr) {
		TTL_Log("Cannot convert mesh: contains no position buffer");
		return false;
	}

	//verts
	for (int vert = 0; vert < buf->mNumVerts; vert++) {
		float* currentVert = (float*)((char*)buf->pData + (buf->mVertSize * vert) + posOffset);
		if (currentVert[0] > 0.f && currentVert[0] < 0.00001f)
			currentVert[0] = 0.f;
		if (currentVert[1] > 0.f && currentVert[1] < 0.00001f)
			currentVert[1] = 0.f;
		if (currentVert[2] > 0.f && currentVert[2] < 0.00001f)
			currentVert[2] = 0.f;
		obj << "v " << currentVert[0] << " " << currentVert[1] << " " << currentVert[2] << "\n";//warn: dont use decompose normal, write own function for half since it does some magic
	}

	obj << "\n\n";

	buf = mesh.GetVertexBuffer(LegacyD3DMesh::COMPONENT_TYPE_UV);

	if (buf) {
		if (buf->mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount != 2) {
			TTL_Log("unknown vert UV type (not 2 components)!\n");
			return false;
		}
		int type = buf->mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mType;
		int off = buf->mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mOffset;
		for (int vert = 0; vert < buf->mNumVerts; vert++) {
			void* currentVert = (void*)((char*)buf->pData + (buf->mVertSize * vert) + off);
			obj << "vt " << decompose_normal(currentVert, type, 0, 0, true) << " " << decompose_normal(currentVert, type, 1, 0, true) << "\n";
		}
	}

	obj << "\n\n";

	buf = mesh.GetVertexBuffer(LegacyD3DMesh::COMPONENT_TYPE_NORMAL);

	//normals
	if (buf) {
		if (buf->mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount != 3) {
			TTL_Log("unknown vert normal type (not 3 components)!\n");
		}
		int type = buf->mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mType;
		int off = buf->mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mOffset;
		for (int vert = 0; vert < buf->mNumVerts; vert++) {
			void* currentVert = (void*)((char*)buf->pData + (buf->mVertSize * vert) + off);
			obj << "vn " << decompose_normal(currentVert, type, 0, 0) << " " << decompose_normal(currentVert, type, 1, 0) << " " << decompose_normal(currentVert, type, 2, 0) << "\n";
		}
	}

	// ...

	int faceStart = 0;
	for (int i = 0; i < mesh.mTriangleSets.GetSize(); i++) {
		LegacyD3DMesh::TriangleSet& set = mesh.mTriangleSets[i];
		if (set.mTxIndex.mData[0] == -1) {
			TTL_Log("Mesh triangle set has no diffuse texture !!");//no diffuse tex? wtf
			return false;
		}
		LegacyD3DMesh::Texture& diffuse = mesh.mTextures.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE][set.mTxIndex.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE]];

		std::string textureF = tex(diffuse.mName.mHandleObjectInfo.mObjectName.GetCRC(), texnames, true);

		/*Write the texture material information*/
		mtl << "\n\nnewmtl " << textureF << "\n";
		//anything else in the mtl here?
		mtl << "\tKs " << set.mSpecularColor.r << " " << set.mSpecularColor.g << " " << set.mSpecularColor.b << "\n";
		mtl << "\tNs " << set.mSpecularPower << "\n";
		mtl << "\tKd " << set.mDiffuseColor.r << " " << set.mDiffuseColor.g << " " << set.mDiffuseColor.b << "\n";
		mtl << "\tKa " << set.mAmbientColor.r << " " << set.mAmbientColor.g << " " << set.mAmbientColor.b << "\n";


		obj << "\n\no " << textureF << "\n";
		obj << "g " << textureF << "\n";
		obj << "usemtl " << textureF << "\ns 1\n\n";

		//faces
		for (int i = 0; i < set.mNumTotalIndices / 3; i++) {
			int faceIndex = faceStart / 3 + i;
			unsigned short* currentFace = (unsigned short*)((char*)mesh.mIndexBuffer.pData + sizeof(unsigned short) * faceIndex * 3);
			obj << "f " << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << " ";
			obj << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << " ";
			obj << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "\n\n";
		}

		mtlCB(t, textureF);
		mtl << "\tmap_Kd " << textureF << "\n";

		faceStart += set.mNumTotalIndices;
	}

	return true;
}

bool convert_obj_legacy_OLD(LegacyD3DMesh& mesh, const char* gid, std::string objfile, std::string mtlfile, std::string& err, std::vector<std::string>& texnames, mtl_delta_cb mtlCB, MeshTask* t) {
	std::ofstream obj{ objfile };
	std::ofstream mtl{ mtlfile };
	obj << "mtllib " << (std::filesystem::path{ mtlfile }.filename().string()) << "\n\n";
	obj << "# Converted to OBJ by The Telltale Inspector\n";
	obj << "# https://github.com/LucasSaragosa\n\n\n";
	mtl << "# Converted to MTL by The Telltale Inspector\n";
	mtl << "# https://github.com/LucasSaragosa\n";

	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mType != 1) {
		TTL_Log("Cannot convert mesh: unknown vert pos type!\n");
		return false;
	}
	if (mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mCount != 3) {
		TTL_Log("Cannot convert mesh: unknown vert pos type (not 3 components)!\n");
		return false;
	}

	if (mesh.mIndexBuffer.mFormat != 0x65 || mesh.mIndexBuffer.mFlags.mFlags != 0) {
		TTL_Log("Cannot convert mesh: untested index buffer flags / format\n");
		return false;
	}

	int faceStart = 0;
	for (int i = 0; i < mesh.mTriangleSets.GetSize(); i++) {
		LegacyD3DMesh::TriangleSet& set = mesh.mTriangleSets[i];
		if (set.mTxIndex.mData[0] == -1) {
			TTL_Log("Mesh triangle set has no diffuse texture !!");//no diffuse tex? wtf
			return false;
		}
		LegacyD3DMesh::Texture& diffuse = mesh.mTextures.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE][set.mTxIndex.mData[LegacyD3DMesh::TEXTURE_INDEX_DIFFUSE]];

		std::string textureF = tex(diffuse.mName.mHandleObjectInfo.mObjectName.GetCRC(), texnames, true);

		/*Write the texture material information*/
		mtl << "\n\nnewmtl " << textureF << "\n";
		//anything else in the mtl here?
		mtl << "\tKs " << set.mSpecularColor.r << " " << set.mSpecularColor.g << " " << set.mSpecularColor.b << "\n";
		mtl << "\tNs " << set.mSpecularPower << "\n";
		mtl << "\tKd " << set.mDiffuseColor.r << " " << set.mDiffuseColor.g << " " << set.mDiffuseColor.b << "\n";
		mtl << "\tKa " << set.mAmbientColor.r << " " << set.mAmbientColor.g << " " << set.mAmbientColor.b << "\n";

		int posOffset = mesh.mVertexBuffer[0].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_POSITION].mOffset;

		/*Write actual mesh data*/

		for (int j = 0; j < 2; j++) {

			//verts
			for (int vert = set.mMinVertIndex; vert <= set.mMaxVertIndex; vert++) {
				float* currentVert = (float*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + posOffset);
				if (currentVert[0] > 0.f && currentVert[0] < 0.00001f)
					currentVert[0] = 0.f;
				if (currentVert[1] > 0.f && currentVert[1] < 0.00001f)
					currentVert[1] = 0.f;
				if (currentVert[2] > 0.f && currentVert[2] < 0.00001f)
					currentVert[2] = 0.f;
				obj << "v " << currentVert[0] << " " << currentVert[1] << " " << currentVert[2] << "\n";//warn: dont use decompose normal, write own function for half since it does some magic
			}

			obj << "\n\n";

			//uvs, todo are they in second vert buffer???
			if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount > 0) {
				if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mCount != 2) {
					TTL_Log("unknown vert UV type (not 2 components)!\n");
				}
				int type = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mType;
				int off = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_UV].mOffset;
				for (int vert = set.mMinVertIndex; vert <= set.mMaxVertIndex; vert++) {
					void* currentVert = (void*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + off);
					obj << "vt " << decompose_normal(currentVert, type, 0, 0, true) << " " << decompose_normal(currentVert, type, 1, 0, true) << "\n";
				}
			}

			obj << "\n\n";

			//normals
			if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount > 0) {
				if (mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mCount != 3) {
					TTL_Log("unknown vert normal type (not 3 components)!\n");
				}
				int type = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mType;
				int off = mesh.mVertexBuffer[j].mVertexComponents.mData[LegacyD3DMesh::COMPONENT_TYPE_NORMAL].mOffset;
				for (int vert = set.mMinVertIndex; vert <= set.mMaxVertIndex; vert++) {
					void* currentVert = (void*)((char*)mesh.mVertexBuffer[j].pData + (mesh.mVertexBuffer[j].mVertSize * vert) + off);
					obj << "vn " << decompose_normal(currentVert, type, 0, 0) << " " << decompose_normal(currentVert, type, 1, 0) << " " << decompose_normal(currentVert, type, 2, 0) << "\n";
				}
			}

			// ...

		}

		obj << "\n\no " << textureF << "\n";
		obj << "g " << textureF << "\n";
		obj << "usemtl " << textureF << "\ns 1\n\n";

		//faces
		for (int i = 0; i < set.mNumTotalIndices / 3; i++) {
			int faceIndex = faceStart / 3 + i;
			unsigned short* currentFace = (unsigned short*)((char*)mesh.mIndexBuffer.pData + sizeof(unsigned short) * faceIndex * 3);
			obj << "f " << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << "/" << (currentFace[0] + 1) << " ";
			obj << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << "/" << (currentFace[1] + 1) << " ";
			obj << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "/" << (currentFace[2] + 1) << "\n\n";
		}

		mtlCB(t, textureF);
		mtl << "\tmap_Kd " << textureF << "\n";

		faceStart += set.mNumTotalIndices;
	}

	return true;
}