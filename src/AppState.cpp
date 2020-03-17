#include "AppState.h"
#include "Logger.h"
#include <thread>

using std::this_thread::yield;

namespace octronic
{
	AppState::AppState(int argc, char** argv) :
        mLooping(true),
        mArgc(argc),
    	mArgv(argv),
      	mWindow(this),
        mGridDrawer(this)
	{
		debug("AppState: Constructor");
    }

    bool AppState::Init()
    {
		debug("AppState: Init");
        if (!mWindow.Init())       return false;
        if (!CreateWidgets())    return false;
        return true;
    }

    bool AppState::CreateWidgets()
    {
		debug("AppState: CreateGLWidgets");
        if (!mGridDrawer.Init()) return false;
        mWindow.AddWidget(&mGridDrawer);
        return true;
    }

    bool AppState::GetLooping() const
    {
        return mLooping;
    }

    void AppState::SetLooping(bool looping)
    {
        mLooping = looping;
    }

    bool AppState::Run()
    {
		debug("AppState: Run");
        while (mLooping)
        {
            mWindow.Update();
            yield();
        }
        return true;
    }

    Window& AppState::GetWindow()
    {
        return mWindow;
    }
}
