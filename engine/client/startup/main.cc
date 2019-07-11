
#include <stdio.h>

#include <core/common/common.h>
#include <core/sched/scheduler.h>

#include <client/startup/win32/win32app.h>

#include <chrono>
#include <memory>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

using namespace z;

int main(int argc, char* argv[]) {
	Win32App app;
	if (app.Init()) {
		app.Run();
	}
	return 0;
}


/*


class MainView {
	class Scene;
	class RenderView
}


*/



  
