#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderItem.h>
#include <Render/Mesh.h>
#include <Render/Material.h>

#include "RenderStep.h"

namespace z {

class BaseScreenStep : public RenderStep {
public:
	
	void DrawScreen(RenderTarget* dst, MaterialInstance *material) {
		RenderStage::CurStage()->SetRenderTarget(dst);
		// blend state
		RenderStage::CurStage()->SetBlendState({
			false,
			BLEND_FACTOR_ONE, BLEND_FACTOR_ZERO, BLEND_OP_ADD,
			BLEND_FACTOR_ONE, BLEND_FACTOR_ZERO, BLEND_OP_ADD
			}
		);
		// render rect & scissor rect

		// Render Item
		RenderItem item;
		item.Mesh = GetScreenMesh();
		item.SetMeshIndexGroup(0);
		item.SetMeshVertexGroup(0);
		item.Material = material;
		item.Material->SetCullMode(RS_CULL_BACK);
		item.Material->SetEnableDepthTest(false);
		item.Material->SetEnableDepthWrite(false);
		item.Draw();
	}

private:
	RenderMesh* GetScreenMesh() {
		static RefCountPtr<RenderMesh> mesh = nullptr;
		if (mesh) {
			return mesh;
		}

		mesh = new RenderMesh();
		mesh->SetVertexSemantics({ SEMANTIC_POSITION, SEMANTIC_UV0 });
		mesh->SetIndexStride(2);
		float data[] = {
			0.f, 0.f, 0.f, 0.f, 1.f,
			0.f, 1.f, 0.f, 0.f, 0.f,
			1.f, 1.f, 0.f, 1.f, 0.f,
			1.f, 0.f, 0.f, 1.f, 1.f
		};
		uint16_t index[] = {
			0, 1, 2, 0, 2, 3
		};

		mesh->CopyVertex(0, 80, data, 0);
		mesh->CopyIndex(0, 12, index, 0);
		mesh->Complete(1, 1);

		return mesh;
	}
};


}