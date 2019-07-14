#include "ClientDirector.h"
#include <Core/Common/Common.h>

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