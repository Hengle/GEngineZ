#pragma once

#include <vector>

namespace z {

class FrameSched {
public:
	TickFrame();

private:

	std::vector<View*> views_;
};

}