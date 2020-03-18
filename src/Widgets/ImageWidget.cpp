#include "ImageWidget.h"
#include <glm/gtc/type_ptr.hpp>

namespace octronic
{
	ImageWidget::ImageWidget
    (AppState* state, string image_path, bool visible)
        : Widget(state, visible),
          mImageFilePath(image_path),
          mModelUniform(0),
          mViewUniform(0),
          mProjectionUniform(0),
          mImageWidth(0),
          mImageHeight(0),
          mVao(0),
          mVbo(0),
          mTextureID(0)
	{
        debug("ImageWidget: Constructor");
    }

    ImageWidget::~ImageWidget()
    {
        debug("ImageWidget: Destructor");
        if (mImageData != nullptr) SOIL_free_image_data(mImageData);
        if (mTextureID > 0) glDeleteTextures(1,&mTextureID);
        if (mVao > 0)       glDeleteVertexArrays(1,&mVao);
        if (mVbo > 0)       glDeleteBuffers(1,&mVbo);
    }

    bool ImageWidget::Init()
    {
        debug("ImageWidget: Init");
        if (!InitShader())    return false;
        if (!LoadImageData()) return false;
        if (!LoadIntoGL())    return false;
        if (!InitGeometry())  return false;
        if(!InitGLBuffers())  return false;
        SubmitVertexBuffer();
        return true;
    }

    bool ImageWidget::InitGLBuffers()
    {
      info("ImageWidget: {}", __FUNCTION__);

        // VAO
        glGenVertexArrays(1,&mVao);
        if (mVao < 0)
        {
            error("ImageWidget: Error creating Triangle VAO");
            return false;
        }
        glBindVertexArray(mVao);

        // VBO
        glGenBuffers(1,&mVbo);
        if (mVbo < 0)
        {
            error("ImageWidget: Error creating Triangle VBO");
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER,mVbo);

        // Vertex Position Attributes
        glVertexAttribPointer(0,
            2, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(ImageWidgetVertex)),
            static_cast<GLvoid*>(0)
        );
        glEnableVertexAttribArray(0);

        // Vertex UV Attributes
        glVertexAttribPointer(1,
            2, GL_FLOAT, GL_FALSE,
            static_cast<GLint>(sizeof(ImageWidgetVertex)),
            (GLvoid*)(sizeof(float)*2)
        );
        glEnableVertexAttribArray(1);

        GLCheckError();

        glBindVertexArray(0);

        //  Final Check
        if (mVao != -1 && mVbo != -1)
        {
            info("ImageWidget: Triangle VAO/VBO Init Successful");
            return true;
        }
        else
        {
           error("ImageWidget: Triangle VAO/VBO Error VAO:{} VBO:{}",mVao,mVbo);
        }
        return false;
    }

    bool ImageWidget::InitGeometry()
    {
        mVertexBuffer.clear();
        ImageWidgetVertex topLeft, topRight, bottomLeft, bottomRight;
        topLeft.position     = vec2(-1.0f,  1.0f);
        topRight.position    = vec2( 1.0f,  1.0f);
        bottomLeft.position  = vec2(-1.0f, -1.0f);
        bottomRight.position = vec2( 1.0f, -1.0f);

        topLeft.uv     = vec2(0.0f, 1.0f);
        topRight.uv    = vec2(1.0f, 1.0f);
        bottomLeft.uv  = vec2(0.0f, 0.0f);
        bottomRight.uv = vec2(1.0f, 0.0f);

        // Clockwise

        // Triangle 1
        mVertexBuffer.push_back(topLeft);
        mVertexBuffer.push_back(bottomRight);
        mVertexBuffer.push_back(bottomLeft);

        // Triangle 2
        mVertexBuffer.push_back(topRight);
        mVertexBuffer.push_back(bottomRight);
        mVertexBuffer.push_back(topLeft);

        return true;
    }

    bool ImageWidget::LoadImageData()
    {
        debug("ImageWidget: LoadImageData");
        if (mImageFilePath.empty()) return false;
        mImageData = SOIL_load_image(mImageFilePath.c_str(),
    		&mImageWidth, &mImageHeight, &mImageChannels,
    		SOIL_LOAD_RGBA);

        return mImageData != nullptr;
    }

    bool ImageWidget::LoadIntoGL()
    {
        debug("ImageWidget: LoadIntoGL");
        // Assign texture to ID
        glGenTextures(1, &mTextureID);
        glBindTexture(GL_TEXTURE_2D, mTextureID);
        debug("Bound to texture id {}",mTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImageWidth, mImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Set Parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        SOIL_free_image_data(mImageData);
        mImageData = nullptr;
        return mTextureID != 0;
    }

    string ImageWidget::GetImageFilePath() const
    {
        return mImageFilePath;
    }

    void ImageWidget::SetImageFilePath(const string& imageFilePath)
    {
        mImageFilePath = imageFilePath;
    }

    void ImageWidget::Update()
    {

    }

	void ImageWidget::Draw(const glm::mat4& view, const glm::mat4& projection)
	{
        debug("ImageWidget: {}", __FUNCTION__);

        // Enable shader program
        debug("ImageWidget: Using shader {}",mShaderProgram);
		glUseProgram(mShaderProgram);
		GLCheckError();

        // Set the projection matrix
		if (mModelUniform == -1)
		{
			error("ImageWidget: ModelUniform not found in ShaderProgram");
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
			error("ImageWidget: ViewUniform not found in ShaderProgram");
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
			error("ImageWidget: ProjectionUniform not found in ShaderProgram");
			return;
		}
		else
		{
			glUniformMatrix4fv(mProjectionUniform, 1, GL_FALSE, glm::value_ptr(projection));
			GLCheckError();
		}

        /*
        // Set the texture
		if (mTextureUniform == -1)
		{
			error("ImageWidget: Texture Uniform not found in ShaderProgram");
			return;
		}
		else
		{
			glUniform1f(mTextureUniform, mTextureID);
			GLCheckError();
		}
        */

        if (!mVertexBuffer.empty())
        {
            // Bind Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mTextureID);

            // Vertex Array
            glBindVertexArray(mVao);
        	GLCheckError();

            // Draw
            GLuint sz = mVertexBuffer.size();
            debug("ImageWidget: Drawing {} Triangles", sz/3);
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sz));
        	GLCheckError();

            glBindVertexArray(0);
        	GLCheckError();
        }
	}

	bool ImageWidget::InitShader()
	{
        info("ImageWidget: {}", __FUNCTION__);

        static string vertexShaderSource =
            "#version 330 core\n"
            "layout (location = 0) in vec2 in_position;\n"
            "layout (location = 1) in vec2 in_uv;\n"
            "\n"
            "out vec2 out_texCoord;\n"
            "\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "\n"
            "void main () { "
            "    gl_Position = projection * view * model * vec4(in_position.x, in_position.y, 0.0, 1.0);\n"
            "	 out_texCoord = in_uv;\n"
            "}";

        static string fragmentShaderSource =
            "#version 330 core\n"
            "in  vec2 out_texCoord;\n"
            "out vec4 FragColor;\n"
            "uniform sampler2D ImgTexture;"
            "void main() { FragColor = texture(ImgTexture, out_texCoord); }";

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
            error("ImageWidget: Vertex Shader Error {}", infoLog);
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
            error("ImageWidget: Fragment Shader Error {}", infoLog);
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
            error("ImageWidget: Shader Linking Error {}", infoLog);
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
        mTextureUniform = glGetUniformLocation(mShaderProgram, "ImgTexture");

        GLCheckError();

        if (mModelUniform != -1 && mViewUniform != -1 && mProjectionUniform != -1 && mTextureUniform != -1)
        {
            debug("ImageWidget: Uniforms found Model:{} View:{} Projection:{} Texture:{}",
                  mModelUniform, mViewUniform, mProjectionUniform,mTextureUniform);
        	return true;
        }
        else
        {
       		error("ImageWidget: Uniform Error M:{} V:{} P:{} T:{}",
                  mModelUniform, mViewUniform, mProjectionUniform,mTextureUniform);
            return false;
        }
	}


	void ImageWidget::SubmitVertexBuffer()
    {
        if (!mVertexBuffer.empty())
        {
			// Vertex Array
			glBindVertexArray(mVao);
			glBindBuffer(GL_ARRAY_BUFFER, mVbo);
			glBufferData(GL_ARRAY_BUFFER, static_cast<GLint>(mVertexBuffer.size() * sizeof(ImageWidgetVertex)), &mVertexBuffer[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
        }
    }
}



