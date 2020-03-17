/*
 * Widget.h
 * 	octronic
*/

#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../GLHeader.h"

using std::string;
using glm::vec3;
using glm::mat4;
using std::vector;
using std::string;

namespace octronic
{
    struct WidgetVertex
    {
        vec3 Position;
        vec3 Color;
    };

    class AppState;
    class Widget
    {
    public:
        Widget(AppState* state, string name, bool visible = true);
        virtual ~Widget();

        bool GetVisible() const;
        void SetVisible(bool);
        bool* VisiblePointer();

        string GetName() const;
        void setName(string name);

        virtual bool Init();
        virtual void Update() = 0;
        virtual void Draw(const mat4& view, const mat4& projection);

        void SetPosition(const vec3&);
        vec3 GetPosition();


    protected: // Member Functions
        void AddLineVertex(const WidgetVertex& v);
        void AddLineVertices(const vector<WidgetVertex>& v);

        void AddTriangleVertex(const WidgetVertex& v);
        void AddTriangleVertices(const vector<WidgetVertex>& v);

        void AddPointVertex(const WidgetVertex& v);
        void AddPointVertices(const vector<WidgetVertex>& v);

        void DefaultShader();
        bool InitShader();
        bool InitLineBuffers();
        bool InitTriangleBuffers();
        bool InitPointBuffers();

        void ClearLineVertexBuffer();
        void ClearTriangleVertexBuffer();
        void ClearPointVertexBuffer();

        void SubmitLineVertexBuffer();
        void SubmitTriangleVertexBuffer();
        void SubmitPointVertexBuffer();

    protected: // Variables
        AppState* mAppState;
        string mName;
        bool mVisible;

        mat4 mModelMatrix;

        GLuint mLineVao;
        GLuint mLineVbo;
        vector<WidgetVertex> mLineVertexBuffer;

        GLuint mTriangleVao;
        GLuint mTriangleVbo;
        vector<WidgetVertex> mTriangleVertexBuffer;

        GLuint mPointVao;
        GLuint mPointVbo;
        vector<WidgetVertex> mPointVertexBuffer;

        GLuint mShaderProgram;
        string mVertexShaderSource;
        string mFragmentShaderSource;
        GLint mModelUniform;
        GLint mViewUniform;
        GLint mProjectionUniform;
    };
}
