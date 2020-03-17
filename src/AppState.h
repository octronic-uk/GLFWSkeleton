#pragma once

#include "Window.h"
#include "Widgets/Grid.h"

namespace octronic
{
	class AppState
	{
	public:
		AppState(int argc, char** argv);
        bool Init();
        bool Run();

        bool GetLooping() const;
        void SetLooping(bool looping);

        Window& GetWindow();

    protected:
        bool CreateWidgets();

    private:
        bool mLooping;
        int mArgc;
        char** mArgv;
        Window mWindow;
        Grid mGridDrawer;
	};
}
