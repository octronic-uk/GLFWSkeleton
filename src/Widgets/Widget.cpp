#include "Widget.h"

#include "../Common/Logger.h"
#include "../AppState.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace octronic
{
    Widget::Widget
    (AppState* project, bool visible) :
		mModelMatrix(mat4(1.0f)),
		mVisible(visible),
		mShaderProgram(0)
    {
        debug("Widget: Constructor");
    }

    Widget::~Widget()
    {
        debug("Widget: Destructor");
		// Shader
        if (mShaderProgram > 0) glDeleteProgram(mShaderProgram);
    }

    bool Widget::GetVisible() const
    {
        return mVisible;
    }

    void Widget::SetVisible (bool v)
    {
        mVisible = v;
    }

    void Widget::SetPosition(const vec3& pos)
    {
        mModelMatrix = glm::translate(mat4(1.0f),pos);
    }

    vec3 Widget::GetPosition()
    {
       return vec3(mModelMatrix[3]);
    }

}
