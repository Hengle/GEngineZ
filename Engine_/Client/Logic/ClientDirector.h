#pragma once

#include <vector>
#include <Core/Logic/Director.h>
#include <Client/Scene/Scene.h>

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

