#pragma once
#include <imgui/imgui.h>

namespace z {


struct UIData {
	// infomation window
	bool showInfoWin;
	float frameRate;
	int winWidth;
	int winHeight;
	int drawcalls;
	int drawfaces;

	// camera controller
	float camMoveSensity;
	float camRotateSensity;

	// environment
	float sunRotate[3];
	float sunColor[3];
	float ambientColor[3];

	// render options
	bool hdr;


	// other
	bool reloadAllShader;
};

class UIManager {
public:
	UIManager();
	void NewFrame();

private:
	void SetUIStyle();

	void InitData();
	void LoadData();
	void StoreData();

	UIData t;
	UIData ot;
	
};

}