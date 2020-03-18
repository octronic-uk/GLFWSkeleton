/*
 * Widget.h
 * 	octronic
*/

#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../Common/GLHeader.h"

using std::string;
using glm::vec3;
using glm::mat4;
using std::vector;
using std::string;

namespace octronic
{
    class AppState;
    class Widget
    {
    public:
        Widget(AppState* state,  bool visible = true);
        virtual ~Widget();

        virtual bool Init() = 0;
        virtual void Update() = 0;
        virtual void Draw(const mat4& view, const mat4& projection) = 0;

        void SetPosition(const vec3&);
        vec3 GetPosition();

        bool GetVisible() const;
        void SetVisible(bool);

    protected: // Member Functions

        virtual bool InitShader() = 0;

    protected: // Variables
        AppState* mAppState;
        bool mVisible;
        mat4 mModelMatrix;
        GLuint mShaderProgram;
    };
}
