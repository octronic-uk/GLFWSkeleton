/*
 * Widget.h
 * 	octronic
*/

#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Widget.h"
#include "../Common/GLHeader.h"

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
    class Widget3D : public Widget
    {
    public:
        Widget3D(AppState* state,  bool visible = true);
        virtual ~Widget3D();

        virtual bool Init();
        virtual void Update() = 0;
        virtual void Draw(const mat4& view, const mat4& projection);

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
        GLuint mLineVao;
        GLuint mLineVbo;
        vector<WidgetVertex> mLineVertexBuffer;

        GLuint mTriangleVao;
        GLuint mTriangleVbo;
        vector<WidgetVertex> mTriangleVertexBuffer;

        GLuint mPointVao;
        GLuint mPointVbo;
        vector<WidgetVertex> mPointVertexBuffer;

        GLint mModelUniform;
        GLint mViewUniform;
        GLint mProjectionUniform;
    };
}
