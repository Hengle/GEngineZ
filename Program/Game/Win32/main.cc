#include <Platform/Win32/win32App.h>




int main(int argc, char *argv) {
    z::Win32App app;
    if (app.Init()) app.Run();
    return 0;
}