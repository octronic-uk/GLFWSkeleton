#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#endif

#include "AppState.h"
#include "Window.h"
#include "Logger.h"

using namespace octronic;

int main(int argc,char** argv)
{
    spdlog::set_level(spdlog::level::debug);
   	debug("Starting main");
    AppState s(argc, argv);
    if (s.Init())
    {
	    return s.Run();
    }
    exit(1);
}
