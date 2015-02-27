#include <Windows.h> // include Windows.h only if using WinMain
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>

// Use standard main to have console background:
// int main()

// Use WinMain if you don't want the console in the background:
int __stdcall WinMain(
	__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd
	)
{
	Fl_Window window(200, 200, "My window title");
	Fl_Box box(0, 50, 200, 20, "Hello");
	window.show();
	return Fl::run();
}