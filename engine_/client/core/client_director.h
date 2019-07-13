#pragma once

#include <vector>
#include <core/foundation/director.h>
#include <client/scene/scene.h>

namespace z {

class ClientDirector : public Director {
public:
	ClientDirector();
	~ClientDirector();
	
	bool FrameTick() override;

private:
	std::vector<Scene*> scenes_;
	

};

extern ClientDirector* GClientDirector;


}

