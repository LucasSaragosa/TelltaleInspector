#pragma once

#ifdef WL_PLATFORM_WINDOWS

extern Walnut::Application* Walnut::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

namespace Walnut {

	int Main(int argc, char** argv)
	{
		while (g_ApplicationRunning)
		{
			Walnut::Application* app = Walnut::CreateApplication(argc, argv);
			app->Run();
			delete app;
		}
		//_CrtDumpMemoryLeaks();
		return 0;
	}

}

int main(int argc, char** argv)
{
	return Walnut::Main(argc, argv);
}


#ifdef WL_RELEASE

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Walnut::Main(__argc, __argv);
}

#endif // WL_DIST

#endif // WL_PLATFORM_WINDOWS
