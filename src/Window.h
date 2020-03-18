#pragma once

#include "Common/GLHeader.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4;
using std::string;
using std::vector;

// Maintain include order for GL
#include <GLFW/glfw3.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 480

namespace octronic
{
    enum ProjectionType
    {
        Ortho,
        Perspective
    };

	class Widget;
    class AppState;

	class Window
	{

	public:
		Window(AppState* state);
        ~Window();

		bool Update();

        void AddWidget(Widget* widget);
        void RemoveWidget(Widget* widget);

        bool Init();

        void SetCameraPosition(const vec3&);

        mat4 GetViewMatrix();
        mat4 GetProjectionMatrix();

    protected:
        bool InitGLFW();
        bool InitGL();
        void InitViewMatrix();
        void InitProjectionMatrix();
        void SwapBuffers();
		void DrawWidgets();

    private:
        AppState* mAppState;
        GLFWwindow* mWindow;
        vector<Widget*> mWidgets;
        int mWindowWidth;
        int mWindowHeight;
        float mDPIScaleX, mDPIScaleY;
        string mName;
        vec3 mClearColor;
        float mCameraPitch;
        float mCameraYaw;
        vec3 mCameraPosition;
        vec3 mUpVector;
        mat4 mViewMatrix;
        mat4 mProjectionMatrix;
        ProjectionType mProjectionType;
        float mNearClip;
        float mFarClip;
	};
}
