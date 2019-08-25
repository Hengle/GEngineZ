#pragma once
#include <Core/CoreHeader.h>

namespace z {


struct RenderOptions {
	bool HDR{ true };

};

extern struct RenderOptions GRenderOptions;

enum ERenderOption {

};

union RenderOptionValue {
	bool Enabled;
};

extern std::unordered_map<ERenderOption, RenderOptionValue> NewRenderOptions;
}