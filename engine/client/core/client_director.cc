#include "client_director.h"
#include <core/common/common.h>

namespace z {
ClientDirector* GClientDirector = nullptr;

ClientDirector::ClientDirector() {
	InitializeSingleton<ClientDirector>(GClientDirector, this);
}

ClientDirector::~ClientDirector() {
	FinalizeSingleton<ClientDirector>(GClientDirector, this);
}

bool ClientDirector::FrameTick() {
	if (!Director::FrameTick()) {
		return false;
	}
	return true;
}



}