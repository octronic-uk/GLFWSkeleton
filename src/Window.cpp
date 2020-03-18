/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "Common/GLHeader.h"

#include "Window.h"


#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "AppState.h"
#include "Widgets/Widget.h"
#include "Common/Logger.h"

using std::cout;
using std::endl;
using glm::ortho;
using glm::perspective;

static bool WindowSizeChanged = false;
static bool WindowShouldClose = false;

void FramebufferSizeCallback(GLFWwindow*, int width, int height)
{
    debug("Window: {} {}x{}",__FUNCTION__, width, height);
    WindowSizeChanged = true;
}

void WindowShouldCloseCallback(GLFWwindow* window)
{
    WindowShouldClose = true;
}

void GlfwErrorCallback(int _errno, const char* errmsg)
{
    error("Window: GLFW Error Number {}\nMessage:\n{}", _errno ,errmsg);
}

namespace octronic
{
    Window::Window(AppState* state) :
        mWindowWidth(DEFAULT_WINDOW_WIDTH),
        mWindowHeight(DEFAULT_WINDOW_HEIGHT),
        mName("PiDash"),
        mAppState(state),
        mClearColor(0.5f,0.5f,0.5f),
        mCameraPosition(0.f,10.f,10.f),
        mCameraPitch(glm::radians(0.f)),
        mCameraYaw(glm::radians(0.f)),
        mUpVector(0.f,0.f,1.f),
        mViewMatrix(mat4(1.0f)),
        mProjectionMatrix(mat4(1.0f)),
        mNearClip(.1f),
        mFarClip(1000.f),
        mProjectionType(Perspective)
    {
        debug("Window: Constructor");
    }

    Window::~Window
    ()
    {
        debug("Window: Destructor");
        if (mWindow)
        {
            glfwTerminate();
            mWindow = nullptr;
        }
    }

    void Window::SetCameraPosition(const vec3& v)
    {
        mCameraPosition = v;
    }

    bool Window::Update()
    {
        debug("Window: {}",__FUNCTION__);

        glfwPollEvents();

        if(glfwWindowShouldClose(mWindow) || WindowShouldClose)
        {
            mAppState->SetLooping(false);
        }

        if (WindowSizeChanged)
        {
            debug("Window: Size Changed to {}x{}",mWindowWidth,mWindowHeight);
            glfwGetFramebufferSize(mWindow, &mWindowWidth, &mWindowHeight);
            glViewport(0,0,mWindowWidth,mWindowHeight);
            WindowSizeChanged = false;
        }

        glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, 0.0f);
        GLCheckError();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLCheckError();

        DrawWidgets();

        SwapBuffers();
        GLCheckError();
        return true;
    }

    bool Window::Init()
    {
        debug("Window: {}", __FUNCTION__);
        if (!InitGLFW()) return false;
        if (!InitGL())   return false;
        InitViewMatrix();
        InitProjectionMatrix();
        return true;
    }

    bool Window::InitGLFW()
    {
        debug("Window: {}", __FUNCTION__);
        /* Initialize the library */
        if (!glfwInit())
        {
            return false;
        }
        debug("Window: {} passed glfwInit()", __FUNCTION__);
        /* Create a windowed mode window and its OpenGL context */
        //glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef WIN32
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif
#ifdef __linux__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#endif

        debug("Window: {} passed set hints", __FUNCTION__);

        mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mName.c_str(), nullptr,nullptr);

        if (mWindow == nullptr)
        {
            glfwTerminate();
            return false;
        }

        debug("Window: {} created window", __FUNCTION__);

        glfwMakeContextCurrent(mWindow);

        debug("Window: {} got ctx", __FUNCTION__);

        glfwSetErrorCallback(GlfwErrorCallback);
        glfwSetFramebufferSizeCallback(mWindow, FramebufferSizeCallback);
        glfwSwapInterval(1);

#ifdef __APPLE__
        glfwGetMonitorContentScale(glfwGetPrimaryMonitor(),&mDPIScaleX,&mDPIScaleY); //Requires GLFW >=3.3
#endif

        glfwGetFramebufferSize(mWindow, &mWindowWidth, &mWindowHeight);
        glfwSetWindowCloseCallback(mWindow, WindowShouldCloseCallback);

        return true;
    }


    bool Window::InitGL()
    {
        debug("Window: {}", __FUNCTION__);
        if(!gladLoadGL())
        {
            error("Window: Error initialising GLAD!\n");
            return false;
        }

        glViewport(0,0,mWindowWidth,mWindowHeight);

        debug("Window: OpenGL Version {}, Shader Version {}",
              glGetString(GL_VERSION),
              glGetString(GL_SHADING_LANGUAGE_VERSION));

        GLCheckError();

        return true;
    }

    void Window::SwapBuffers()
    {
        if (mWindow != nullptr)
        {
            glfwSwapBuffers(mWindow);
            GLCheckError();
        }
    }

    void
    Window::DrawWidgets
    ()
    {
        debug("Window: {}, {}", __FUNCTION__, mWidgets.size());

        for (Widget* widget : mWidgets)
        {
            if(widget->GetVisible())
            {
                widget->Update();
                widget->Draw(mViewMatrix, mProjectionMatrix);
            }
        }
    }

    void Window::AddWidget (Widget* widget)
    {
        debug("Window: {}",__FUNCTION__);
        auto end = mWidgets.end();
        auto itr = find(mWidgets.begin(), end, widget);

        if (itr == end)
        {
            mWidgets.push_back(widget);
        }
    }

    void Window::RemoveWidget(Widget* widget)
    {
        debug("Window: {}",__FUNCTION__);
        auto end = mWidgets.end();
        auto itr = find(mWidgets.begin(), end, widget);
        if (itr != end)
        {
            mWidgets.erase(itr);
        }
    }

    void Window::InitViewMatrix()
    {
        mViewMatrix = glm::lookAt(
			mCameraPosition,   // the position of your camera, in world space
			vec3(0.f,0.f,0.f), // Centre
			mUpVector          // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
		);
    }

    void Window::InitProjectionMatrix()
    {
        switch (mProjectionType)
        {
            case Ortho:
                // left, right, top, bottom, near, far
                mProjectionMatrix = ortho(
                            0.f, (float)mWindowWidth,
                            0.f, (float)mWindowHeight,
                            mNearClip, mFarClip);
                break;
            case Perspective:
                // FOV, Aspect, Near, Far
                mProjectionMatrix = perspective(
                            glm::radians(45.0f),
                            (float)mWindowWidth / (float)mWindowHeight,
                            mNearClip, mFarClip);
                break;
        }
    }

    mat4 Window::GetViewMatrix()
    {
        return mViewMatrix;
    }

    mat4 Window::GetProjectionMatrix()
    {
        return mProjectionMatrix;
    }
}
