#include "EditorUI.h"
#include "CameraController.h"
#include <Client/Main/Director.h>
#include <Client/Main/App.h>
#include <Client/Scene/Scene.h>
#include <Client/Component/EnvComp.h>

#include <Render/Material.h>
#include <Render/RenderOption.h>

namespace z {

namespace ui = ImGui;

EditorUI::EditorUI() {
	InitData();
}

void EditorUI::NewFrame() {
	LoadData();
	SetUIStyle();


	ImGuiIO& io = ui::GetIO();

	// Infomation window
	ImVec2 windowPos = ImVec2(io.DisplaySize.x - 10.f, 10.0f);
	ImVec2 windowPosPivot = ImVec2(1.0f, 0.0f);
	ui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPosPivot);
	ui::SetNextWindowBgAlpha(0.35f);
	if (ui::Begin("Infomation", nullptr, 
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | 
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
		// FPS
		ui::Text("FPS: %.2f", t.frameRate);
		// Screen Size
		ui::Text("Screen: (%d x %d)", t.winWidth, t.winHeight);

		ui::Text("Draw Call: %d", t.drawcalls);
		ui::Text("Faces: %d", t.drawfaces);
	}
	ui::End();

	// Main Menu
	ui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0.0f, 0.0f));
	ui::SetNextWindowSize(ImVec2(300, 680), ImGuiCond_FirstUseEver);
	ui::SetNextWindowBgAlpha(0.8f);

	if (!ui::Begin("Main Settings", nullptr, 0)) {
		ui::End();
		return;
	}

	ui::PushItemWidth(ui::GetFontSize() * -12);
	
	if (ui::CollapsingHeader("Camera")) {
		// camera settings
		ui::SliderFloat("Move Sensitivity", &t.camMoveSensity, 0.f, 10.0f, "%.2f");
		ui::SliderFloat("Rotate Sensitivity", &t.camRotateSensity, 0.f, 3.0f, "%.2f");

	}

	if (ui::CollapsingHeader("Environment", ImGuiTreeNodeFlags_DefaultOpen)) {
		ui::Text("Sun Light:");
		static bool hdr = true;
		ImGui::Checkbox("With HDR", &hdr);
		ImGuiColorEditFlags miscFlags = (hdr ? ImGuiColorEditFlags_HDR : 0);

		ui::DragFloat3("Sun Rotater", t.sunRotate, 0.05, 0, math::K_2PI);
		ui::ColorEdit3("Sun Color", t.sunColor, miscFlags);
		ui::ColorEdit3("Ambient Color", t.ambientColor, miscFlags);
	}


	if (ui::CollapsingHeader("Render Options", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("HDR", &t.hdr);
	}

	if (ui::CollapsingHeader("Other", ImGuiTreeNodeFlags_DefaultOpen)) {
		t.reloadAllShader = ImGui::Button("Reload All Shader");
	}



	ui::End();

	StoreData();
}


void EditorUI::SetUIStyle() {
	// set ui style
	ImGuiStyle& style = ui::GetStyle();
	// style.WindowPadding = ImVec2(0.f, 0.0f);
}

void EditorUI::InitData() {
	t.showInfoWin = true;
}

void EditorUI::LoadData() {
	// Info
	ot.frameRate = GDirector->GetCurFps();
	ot.winWidth = GApp->GetWinSize().x;
	ot.winHeight = GApp->GetWinSize().y;
	ot.drawcalls = GDirector->GetDrawCalls();
	ot.drawfaces = GDirector->GetDrawFaces();

	// Camera
	ot.camMoveSensity = GDirector->GetCameraController()->GetSensityMove();
	ot.camRotateSensity = GDirector->GetCameraController()->GetSensityRotate();

	// Env
	Scene* scn = GDirector->GetCurScene();
	EnvComp *env = scn->GetComponent<EnvComp>();
	if (env) {
		const math::Vector3F sunColor = env->GetSunColor();
		const math::Vector3F sunDir = env->GetSunRotater();
		const math::Vector3F ambColor = env->GetAmbientColor();
		memcpy(ot.sunColor, sunColor.value, 12);
		memcpy(ot.sunRotate, sunDir.value, 12);
		memcpy(ot.ambientColor, ambColor.value, 12);
	}

	// Render Option
	ot.hdr = GRenderOptions.HDR;

	// Option
	

	t = ot;

}

void EditorUI::StoreData() {
	// Camera
	GDirector->GetCameraController()->SetSensityMove(t.camMoveSensity);
	GDirector->GetCameraController()->SetSensityRotate(t.camRotateSensity);

	// Env
	Scene* scn = GDirector->GetCurScene();
	EnvComp* env = scn->GetComponent<EnvComp>();
	if (env) {
		env->SetSunColor({ t.sunColor[0], t.sunColor[1], t.sunColor[2] });
		env->SetSunRotater({ t.sunRotate[0], t.sunRotate[1], t.sunRotate[2] });
		env->SetAmbientColor({ t.ambientColor[0], t.ambientColor[1], t.ambientColor[2] });
	}

	// Render Option
	if (t.hdr != ot.hdr) {
		Log<LINFO>("hdr changed....");
		GRenderOptions.HDR = t.hdr;
	}


	// Other
	if (t.reloadAllShader) {
		MaterialManager::ReloadAllShaders();
	}
}

}