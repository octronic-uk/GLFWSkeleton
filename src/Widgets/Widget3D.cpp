#include "Widget3D.h"

#include "../Common/Logger.h"
#include "../AppState.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace octronic
{
    Widget3D::Widget3D
    (AppState* state, bool visible) :
        Widget(state,visible),
		mLineVao(0),
		mLineVbo(0),
		mTriangleVao(0),
		mTriangleVbo(0),
		mPointVao(0),
		mPointVbo(0)
    {
        debug("Widget3D: Constructor");
    }

    Widget3D::~Widget3D()
    {
        debug("Widget3D: Destructor");

        // Line
        if (mLineVao > 0) glDeleteVertexArrays(1,&mLineVao);
        if (mLineVbo > 0) glDeleteBuffers(1,&mLineVbo);

        // Triangle
        if (mTriangleVao > 0) glDeleteVertexArrays(1,&mTriangleVao);
        if (mTriangleVbo > 0) glDeleteBuffers(1,&mTriangleVbo);

        // Point
        if (mPointVao > 0) glDeleteVertexArrays(1,&mPointVao);
        if (mPointVbo > 0) glDeleteBuffers(1,&mPointVbo);

        GLCheckError();
    }

    bool Widget3D::Init()
    {
        debug("Widget3D3D: {}",__FUNCTION__);
        if (!InitShader())          return false;
        if (!InitLineBuffers())     return false;
        if (!InitTriangleBuffers()) return false;
        if (!InitPointBuffers())    return false;

        return true;
    }

    bool Widget3D::InitTriangleBuffers()
    {
        info("Widget3D: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mTriangleVao);
        if (mTriangleVao < 0)
        {
            error("Widget3D: Error creating Triangle VAO");
            return false;
        }
        glBindVertexArray(mTriangleVao);

        // VBO
        glGenBuffers(1,&mTriangleVbo);
        if (mTriangleVbo < 0)
        {
            error("Widget3D: Error creating Triangle VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mTriangleVbo);

        // Vertex Position Attributes
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(WidgetVertex)),
            static_cast<GLvoid*>(0)
        );
        glEnableVertexAttribArray(0);

        // Vertex Color Attributes
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(WidgetVertex)),
            (GLvoid*)(sizeof(float)*3)
        );
        glEnableVertexAttribArray(1);

        GLCheckError();

        glBindVertexArray(0);

        //  Final Check
        if (mTriangleVao != -1 && mTriangleVbo != -1)
        {
            info("Widget3D: Triangle VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("Widget3D: Triangle VAO/VBO Error VAO:{} VBO:{}",mTriangleVao,mTriangleVbo);
        }
        return false;
    }

    bool Widget3D::InitLineBuffers()
    {
        debug("Widget3D: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mLineVao);
        if (mLineVao < 0)
        {
            error("Widget3D: Error creating Line VAO");
            return false;
        }
        glBindVertexArray(mLineVao);

        // VBO
        glGenBuffers(1,&mLineVbo);
        if (mLineVbo < 0)
        {
            error("Widget3D: Error creating Line VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mLineVbo);

        // Vertex Position Attributes
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(WidgetVertex)),
            static_cast<GLvoid*>(0)
        );
        glEnableVertexAttribArray(0);

        // Vertex Color Attributes
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(WidgetVertex)),
            (GLvoid*)(sizeof(float)*3)
        );
        glEnableVertexAttribArray(1);

        GLCheckError();

        glBindVertexArray(0);

        //  Final Check
        if (mLineVao != -1 && mLineVbo != -1)
        {
            info("Widget3D: Line VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("Widget3D: Line VAO/VBO Error VAO:{} VBO:{}",mLineVao,mLineVbo);
        }
        return false;
    }

    bool Widget3D::InitPointBuffers()
    {
        info("Widget3D: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mPointVao);
        if (mPointVao < 0)
        {
            error("Widget3D: Error creating Point VAO");
            return false;
        }
        glBindVertexArray(mPointVao);

        // VBO
        glGenBuffers(1,&mPointVbo);
        if (mLineVbo < 0)
        {
            error("Widget3D: Error creating Point VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mPointVbo);

        // Vertex Position Attributes
        glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(WidgetVertex)),
            static_cast<GLvoid*>(0)
        );
        glEnableVertexAttribArray(0);

        // Vertex Color Attributes
        glVertexAttribPointer(
            1, 3, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(WidgetVertex)),
            (GLvoid*)(sizeof(float)*3)
        );
        glEnableVertexAttribArray(1);

        GLCheckError();

        glBindVertexArray(0);

        //  Final Check
        if (mPointVao != -1 && mPointVbo != -1)
        {
            info("Widget3D: Point VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("Widget3D: Point VAO/VBO Error VAO:{} VBO:{}",mPointVao,mPointVbo);
        }
        return false;
    }

    void Widget3D::Draw(const mat4& view, const mat4& projection)
    {
        debug("Widget3D: {}", __FUNCTION__);

        // Enable shader program
		glUseProgram(mShaderProgram);
		GLCheckError();

        // Set the projection matrix
		if (mModelUniform == -1)
		{
			error("Widget3D: ModelUniform not found in ShaderProgram");
			return;
		}
		else
		{
			glUniformMatrix4fv(mModelUniform, 1, GL_FALSE, glm::value_ptr(mModelMatrix));
			GLCheckError();
		}

		// Set the view matrix
		if (mViewUniform == -1)
		{
			error("Widget3D: ViewUniform not found in ShaderProgram");
			return;
		}
		else
		{
			glUniformMatrix4fv(mViewUniform, 1, GL_FALSE, glm::value_ptr(view));
			GLCheckError();
		}

		// Set the projection matrix
		if (mProjectionUniform == -1)
		{
			error("Widget3D: ProjectionUniform not found in ShaderProgram");
			return;
		}
		else
		{
			glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
			GLCheckError();
		}

        if (!mLineVertexBuffer.empty())
        {
            // Vertex Array
            glBindVertexArray(mLineVao);
        	GLCheckError();

            // Draw
            GLuint sz = mLineVertexBuffer.size();
            debug("Widget3D: Drawing {} lines", sz/2);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(sz));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }

        if (!mTriangleVertexBuffer.empty())
        {
            // Vertex Array
            glBindVertexArray(mTriangleVao);
        	GLCheckError();

            // Draw
            GLuint sz = mTriangleVertexBuffer.size();
            debug("Widget3D: Drawing {} Triangles", sz/3);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sz));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }

        if (!mPointVertexBuffer.empty())
        {
            // Vertex Array
            glBindVertexArray(mPointVao);
        	GLCheckError();

            // Draw
            GLuint sz = mPointVertexBuffer.size();
            debug("Widget3D: Drawing {} Points", sz);
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(sz));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }

    }

    bool Widget3D::InitShader()
    {
        info("Widget3D: {}", __FUNCTION__);

        static string vertexShaderSource =
            "#version 330 core\n"
            "layout (location = 0) in vec3 in_position;\n"
            "layout (location = 1) in vec3 in_color;\n"
            "out vec3 Color;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main () { "
            "    gl_Position = projection * view * model *  vec4(in_position.x, in_position.y, in_position.z, 1.0);\n"
            "    Color = in_color;\n"
            "}";

        static string fragmentShaderSource =
            "#version 330 core\n"
            "in vec3  Color;\n"
            "out vec4 FragColor;\n"
            "void main() { FragColor = vec4(Color,1.0); }";

        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        // Compile shaders
        GLint success;
        GLchar infoLog[512];

        // Vertex Shader
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vSource = vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vSource, nullptr);
        glCompileShader(vertexShader);

        // Print compile errors if any
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            error("Widget3D: Vertex Shader Error {}", infoLog);
            return false;
        }

        // Fragment Shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fSource = fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fSource, nullptr);
        glCompileShader(fragmentShader);

        // Print compile errors if any
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            error("Widget3D: Fragment Shader Error {}", infoLog);
            return false;

        }

        // Shader Program
        mShaderProgram = glCreateProgram();
        glAttachShader(mShaderProgram, vertexShader);
        glAttachShader(mShaderProgram, fragmentShader);
        glLinkProgram(mShaderProgram);

        // Print linking errors if any
        glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mShaderProgram, 512, nullptr, infoLog);
            error("Widget3D: Shader Linking Error {}", infoLog);
            return false;

        }

        // Delete the shaders 'source objects' as they're linked into our program
        // now and no longer necessery
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Get Uniform Locations
        mModelUniform = glGetUniformLocation(mShaderProgram, "model");
        mViewUniform = glGetUniformLocation(mShaderProgram,"view");
        mProjectionUniform = glGetUniformLocation(mShaderProgram, "projection");

        GLCheckError();

        if (mModelUniform != -1 && mViewUniform != -1 && mProjectionUniform != -1)
        {
        	return true;
        }
        else
        {
       		error("Widget3D: Uniform Error M:{} V:{} P:{}",
                  mModelUniform, mViewUniform, mProjectionUniform);
            return false;
        }
    }

    void Widget3D::SubmitLineVertexBuffer()
    {
        if (!mLineVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mLineVao);
			glBindBuffer(GL_ARRAY_BUFFER, mLineVbo);
			glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLint>(mLineVertexBuffer.size() * sizeof(WidgetVertex)),
				&mLineVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }

	void Widget3D::SubmitTriangleVertexBuffer()
    {
        if (!mTriangleVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mTriangleVao);
			glBindBuffer(GL_ARRAY_BUFFER, mTriangleVbo);
			glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLint>(mTriangleVertexBuffer.size() * sizeof(WidgetVertex)),
				&mTriangleVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }

    void Widget3D::SubmitPointVertexBuffer()
    {
        if (!mPointVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mPointVao);
			glBindBuffer(GL_ARRAY_BUFFER, mPointVbo);
			glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLint>(mPointVertexBuffer.size() * sizeof(WidgetVertex)),
				&mPointVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }

    void Widget3D::AddLineVertex(const WidgetVertex& lv)
    {
        mLineVertexBuffer.push_back(lv);
    }

    void Widget3D::AddLineVertices(const vector<WidgetVertex>& lv)
    {
        mLineVertexBuffer.insert(mLineVertexBuffer.end(),lv.begin(),lv.end());
    }

    void Widget3D::AddTriangleVertex(const WidgetVertex& lv)
    {
        mTriangleVertexBuffer.push_back(lv);
    }

    void Widget3D::AddTriangleVertices(const vector<WidgetVertex>& lv)
    {
        mTriangleVertexBuffer.insert(mTriangleVertexBuffer.end(), lv.begin(), lv.end());
    }

    void Widget3D::AddPointVertex(const WidgetVertex& lv)
    {
        mPointVertexBuffer.push_back(lv);
    }

    void Widget3D::AddPointVertices(const vector<WidgetVertex>& lv)
    {
        mPointVertexBuffer.insert(mPointVertexBuffer.end(), lv.begin(), lv.end());
    }

    void Widget3D::ClearLineVertexBuffer()
    {
       mLineVertexBuffer.clear();
    }

	void Widget3D::ClearTriangleVertexBuffer()
    {
       mTriangleVertexBuffer.clear();
    }

    void Widget3D::ClearPointVertexBuffer()
    {
       mPointVertexBuffer.clear();
    }
}
