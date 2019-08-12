#include "PrimitiveComp.h"
#include <Client/Entity/IEntity.h>
#include <Client/Main/App.h>
#include <Util/Luaconf/Luaconf.h>
#include <Util/Mesh/ZMeshLoader.h>
#include <Util/Image/Image.h>
#include <Render/RenderItem.h>
#include <Render/RenderScene.h>
#include <RHI/RHIDevice.h>

namespace z {

namespace lc = luaconf;

PrimitiveComp::PrimitiveComp() {}

bool PrimitiveComp::LoadFromFile(std::string file) {
		lc::Value model;
		if (!lc::ParseFile(file, model)) {
			return false;
		}

		// load material info
		std::unordered_map<int, std::string> materials;
		size_t materialNum = model["material"].Size();
		for (size_t materialIdx = 0; materialIdx < materialNum; materialIdx++) {
			lc::Value material = model["material"][materialIdx];
			int idx = material["idx"].Get<lc::int_t>();
			std::string file = material["file"].Get<lc::string_t>();

			materials[idx] = file;
		}

		// load mesh
		std::string meshname = model["mesh"].Get<lc::string_t>();
		RefCountPtr<Mesh> mesh = ZMeshLoader::Load(GApp->GetContentPath() / meshname);
		if (mesh == nullptr) {
			return false;
		}

		// create render items
		for (size_t meshIdx = 0; meshIdx < mesh->Indices.size(); meshIdx++) {
			RefCountPtr<RenderItem> item = new RenderItem();
			item->mesh = mesh;
			item->meshIdx = meshIdx;

			// Get Material
			if (materials.count(meshIdx) == 0) {
				item->material = MaterialManager::GetMaterialInstance(EMPTY_MATERIAL);
			} else {
				std::string materialPath = GApp->GetContentPath() / materials[meshIdx];
				item->material = LoadMaterialFile(materialPath);
				if (item->material == nullptr) {
					item->material = MaterialManager::GetMaterialInstance(EMPTY_MATERIAL);
				}
			}
			// save render item
			mRenderItems.push_back(item);
		}
		return true;
	}

MaterialInstance* PrimitiveComp::LoadMaterialFile(std::string file) {
		lc::Value material;
		if (!lc::ParseFile(file, material)) {
			return nullptr;
		}

		std::string shader = material["shader"].Get<lc::string_t>();

		MaterialInstance* materialInst = MaterialManager::GetMaterialInstance(shader);
		if (materialInst == nullptr) {
			return nullptr;
		}

		size_t paramNum = material["params"].Size();
		for (size_t paramIdx = 0; paramIdx < paramNum; paramIdx++) {
			lc::Value param = material["params"][paramIdx];
			std::string name = param["name"].Get<lc::string_t>();
			std::string type = param["type"].Get<lc::string_t>();
			if (type == "texture") {
				std::string imgFile = param["value"].Get<lc::string_t>();
				RHITexture* tex = LoadTextureFile(GApp->GetRootPath() / "Content" / imgFile);
				if (tex) {
					materialInst->SetParameter(name, tex);
				} else {
					CHECK(0);
				}
			} else if (type == "float4") {
				std::vector<float> vals;
				if (param["value"].GetValSafety(vals)) {
					materialInst->SetParameter(name, vals.data(), 16);
				} else {
					CHECK(0);
				}
			}
		}
		return materialInst;
	}

RHITexture* PrimitiveComp::LoadTextureFile(std::string file) {
		Image* img = Image::Load(file);
		if (img == nullptr) {
			return nullptr;
		}
		RHITextureDesc desc;
		desc.sizeX = img->GetWidth();
		desc.sizeY = img->GetHeight();
		desc.sizeZ = 1;
		desc.flags = 0;
		desc.format = img->GetFormat();
		desc.dimension = TEX_DIMENSION_2D;
		desc.numMips = 1;
		return GDevice->CreateTexture(desc, img->GetData());
	}

void PrimitiveComp::CollectRenderItems(RenderScene* scn) {
		for (RenderItem* item : mRenderItems) {
			item->WorldMatrix = mOwner->GetWorldTransform();
			scn->RenderItems.push_back(item);
		}
	}

}