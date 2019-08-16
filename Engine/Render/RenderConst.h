#pragma once
#include <string>

namespace z {


enum ERenderSet {
	RSET_EDITOR,
	RSET_OPAQUE,
};

enum ERenderTech {
	RTECH_NORMAL = 0,

};

// shader params
enum EShaderParams {
	SP_SUN_COLOR = 0,	// {r,g,b}
	SP_SUN_DIRECTION,	// {x,y,z}
	SP_AMBIENT_COLOR,	// {r,g,b,density}
	SP_MAX
};


extern math::Vector4F gShaderParams[SP_MAX];

// default material
const std::string EMPTY_MATERIAL = "Empty";

}