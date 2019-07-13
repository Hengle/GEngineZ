#pragma once

#include <core/sched/scheduler.h>
#include <core/foundation/frame.h>
namespace z {

class Director {
public:
	Director();
	virtual ~Director();
	
	void Update();


	virtual bool FrameTick();

	void SetFrameLimit(int limit) {
		frame_interval_ = int(1000.0f / limit);
	}


protected:
	Frame cur_frame_;
	sched::Worker* worker_;

	int frame_interval_ = 0;
};

extern Director* GDirector;


}
