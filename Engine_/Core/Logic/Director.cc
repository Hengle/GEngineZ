#pragma once

#include "Director.h"

namespace z {

Director* GDirector = nullptr;

Director::Director() {
	InitializeSingleton<Director>(GDirector, this);

	worker_ = new sched::ThreadWorker(8);


}

Director::~Director() {
	FinalizeSingleton<Director>(GDirector, this);
}

void Director::Update() {
	static uint64_t last_update_ms = ZTime::Now().TotalMs();
	uint64_t now = ZTime::Now().TotalMs();

	if (last_update_ms + frame_interval_ < now) {
		last_update_ms = now;
		FrameTick();
	}
}

bool Director::FrameTick() {
	Log<LINFO>("tick..");
	return true;
}


}

