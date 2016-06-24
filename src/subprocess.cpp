#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OS_WIN
#include "include/cef_app.h"

/*void main(int argc, char** argv) {
	std::ofstream a("C:/Users/Mad-P/Desktop/log1.txt");

	a << "Hello from the CEF subprocess!\nWhy does this have to be so fucking complicated, god damn it?\nIt's nowhere near as bad as gm_v8 or gm_voxelate yet, though >:)\nArguments:" << std::endl;

	for (int i = 0; i < argc; i++) {
		a << argv[i] << std::endl;
	}

	for (;;);
}*/

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	cef_log(0, 0, 0, "proc start");
	CefMainArgs args(hInstance);
	int res = CefExecuteProcess(args, nullptr, nullptr);
	cef_log(0, 0, 0, "proc end");
	return res;
}