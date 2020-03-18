#pragma once

#include "../Common/GLHeader.h"
#include "Widget.h"
#include "SOIL.h"

using glm::vec2;

namespace octronic
{
    struct ImageWidgetVertex
    {
        vec2 position;
        vec2 uv;
    };

	class ImageWidget : public Widget
	{
	public:
		ImageWidget(AppState* state, string image_path, bool visible = true);
        ~ImageWidget();

        bool Init() override;
        void Update() override;
        void Draw(const mat4& view, const mat4& projection) override;

        string GetImageFilePath() const;
        void   SetImageFilePath(const string& imageFilePath);

    protected:
        bool InitShader() override;
        bool LoadImageData();
        bool LoadIntoGL();
        bool InitGeometry();
        bool InitGLBuffers();
		void SubmitVertexBuffer();

    private:
        string mImageFilePath;
        uint8_t* mImageData;
        GLuint mTextureID;
        GLuint mModelUniform;
        GLuint mViewUniform;
        GLuint mProjectionUniform;
        GLuint mTextureUniform;
        GLuint mVao;
        GLuint mVbo;
        vector<ImageWidgetVertex> mVertexBuffer;
        int mImageWidth;
        int mImageHeight;
        int mImageChannels;
    };
}

