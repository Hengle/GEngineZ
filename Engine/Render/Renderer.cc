#include "Renderer.h"
#include <Client/Scene/Scene.h>
#include <Client/Main/App.h>
#include <Client/Main/Director.h>
#include "RenderScene.h"

#include <imgui/imgui.h>

namespace z {

math::Vector4F gShaderParams[SP_MAX];

const char* ShaderParamKey[] = {
	"SunColor",
	"SunDirection",
	"AmbientColor"
};

Renderer::Renderer() :
	mViewportWidth(0),
	mViewportHeight(0) {
	MaterialManager::LoadShaders(GApp->GetRootPath() / "Content/Engine/Shader/hlsl");
	mRenderScene = new RenderScene();


}

Renderer::~Renderer() {

}

void Renderer::Resize(uint32_t width, uint32_t height) {
	mViewportWidth = width;
	mViewportHeight = height;
	if (!mRHIViewport) {
		mRHIViewport = GDevice->CreateViewport(width, height, PF_R8G8B8A8);
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->AddFontDefault();
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics system
		{
			RHITextureDesc desc;
			desc.sizeX = width;
			desc.sizeY = height;
			desc.sizeZ = 1;
			desc.format = PF_R8G8B8A8;
			desc.dimension = TEX_DIMENSION_2D;
			desc.numMips = 1;
			RHITexture* tex = GDevice->CreateTexture(desc, pixels);

			io.Fonts->TexID = (ImTextureID)tex;

		}

	} else {
		mRHIViewport->Resize(width, height);
	}

	mDepthStencil = GDevice->CreateDepthStencil(width, height, PF_D24S8);

}

void Renderer::Tick() {
	if (!mRHIViewport) {
		return;
	}

	// reset render scene
	mRenderScene->Reset();

	Scene* scn = GDirector->GetCurScene();
	if (scn) {
		scn->ColloectEnv(mRenderScene);
		scn->CollectItems(mRenderScene);
	}

}

void Renderer::Render() {
	if (!mRHIViewport) {
		return;
	}

	mRHIViewport->BeginDraw(RHIClearValue(1.0, 1.0, 1.0, 1.0));



	// === Render Scene ===
	mDepthStencil->Clear(RHIClearValue(1.0, 0));

	//GDevice->CreateRenderTarget()


	GDevice->SetOutputs({ mRHIViewport->GetBackBuffer() }, mDepthStencil);

	// render oqaque
	// render sky

	// just simple render everything now	
	for (auto item : mRenderScene->RenderItems) {
		CollectMaterialParametes(item);
		item->material->DrawIndexed(item->mesh->mVBuffer, item->mesh->mIBuffer[item->meshIdx]);
	}

	// === Post Process ===


	// === UI ===

	bool open = true;
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(&open);
	ImGui::Render();
	ImDrawData* drawData = ImGui::GetDrawData();
	Log<LINFO>(drawData->CmdListsCount);
	
	struct VERTEX_CONSTANT_BUFFER
	{
		float   mvp[4][4];
	};
	VERTEX_CONSTANT_BUFFER vertex_constant_buffer;
	{
		float L = drawData->DisplayPos.x;
		float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
		float T = drawData->DisplayPos.y;
		float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
		float mvp[4][4] =
		{
			{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
			{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
			{ 0.0f,         0.0f,           0.5f,       0.0f },
			{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
		};
		memcpy(&vertex_constant_buffer.mvp, mvp, sizeof(mvp));
	}
	ScreenRenderRect rect;
	rect.width = drawData->DisplaySize.x;
	rect.height = drawData->DisplaySize.y;
	rect.topLeftX = rect.topLeftY = 0.0f;
	mRHIViewport->SetRenderRect(rect);

	RHITexture *rt = mRHIViewport->GetBackBuffer();
	RHIBlendState state;
	state.Enable = true;
	state.SrcBlend = BLEND_FACTOR_SRC_ALPHA;
	state.DestBlend = BLEND_FACTOR_INV_SRC_ALPHA;
	state.BlendOp = BLEND_OP_ADD;
	state.SrcBlendAlpha = BLEND_FACTOR_INV_SRC_ALPHA;
	state.DestBlendAlpha = BLEND_FACTOR_ZERO;
	state.BlendOpAlpha = BLEND_OP_ADD;

	rt->SetBlendState(state);

	for (int n = 0; n < drawData->CmdListsCount; n++) {
		const ImDrawList* cmd_list = drawData->CmdLists[n];

		Mesh* mesh = new Mesh;
		mesh->VertCount = cmd_list->VtxBuffer.Size;
		mesh->Stride = sizeof(ImDrawVert);
		mesh->Semantics = { SEMANTIC_POSITION2D, SEMANTIC_UV0, SEMANTIC_COLOR };

		std::vector<float> vdata;
		std::vector<uint32_t> idata;
		vdata.resize(sizeof(ImDrawVert) / 4 * cmd_list->VtxBuffer.Size);
		idata.resize(cmd_list->IdxBuffer.Size);
		memcpy(vdata.data(), cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
		memcpy(idata.data(), cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

		mesh->Indices.push_back(idata);
		mesh->Vertexes = vdata;

		mesh->CreateBuffer();

		RefCountPtr<RenderItem> uiItem = new RenderItem();
		uiItem->mesh = mesh;
		uiItem->meshIdx = 0;
		uiItem->material = MaterialManager::GetMaterialInstance("IMGui");

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			uiItem->material->SetParameter("texture0", (RHITexture*)pcmd->TextureId);

			uiItem->material->SetParameter("ProjectionMatrix", (const float*)& vertex_constant_buffer, 16);
			uiItem->material->SetCullMode(RS_CULL_NONE);
			uiItem->material->SetEnableDepthTest(false);
			uiItem->material->SetEnableDepthWrite(true);

			uiItem->material->DrawIndexed(uiItem->mesh->mVBuffer, uiItem->mesh->mIBuffer[0]);
		}
	}


	mRHIViewport->EndDraw();

	// free resoruce and so on..
	GDevice->EndDraw();
}


void Renderer::CollectMaterialParametes(RenderItem* item) {
	math::Matrix4 world = math::Matrix4::Identity;
	// per frame
	item->material->SetParameter("ViewProj", (const float*)& mRenderScene->ViewProjMatrix, 16);
	math::Vector4F cameraPos = { mRenderScene->CameraPos, 0.0f };
	item->material->SetParameter("CameraPos", (const float*)cameraPos.value, 4);

	// per object
	item->material->SetParameter("World", (const float*)& item->WorldMatrix, 16);


	for (size_t i = 0; i < SP_MAX ; i++) {
		item->material->SetParameter(ShaderParamKey[i], gShaderParams[i].value, 4);
	}
}

}