#include "win32app.h"

#include <client/core/client_director.h>

namespace z {

bool Win32App::Init() {
	InitWindow();

	new ClientDirector();
	GClientDirector->SetFrameLimit(30);
	return true;
}

void Win32App::Run() {
	while (UpdateWindow()) {
		GClientDirector->Update();
	
	
	}
}


}