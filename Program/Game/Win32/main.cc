#include <Startup/Win32/win32App.h>
#include <Util/Image/Image.h>

using namespace z;

int main(int argc, char *argv) {

	z::Win32App app;
	app.SetEditorUIEnable(true);
    if (app.Init()) app.Run();
    return 0;
}