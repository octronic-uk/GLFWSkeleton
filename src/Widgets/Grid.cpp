/*
 * Grid.cpp
 *
 * Created: 07 2017 by Ashley
 *
 * Copyright 2017 octronic. All rights reserved.
 *
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

#include "Grid.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../AppState.h"
#include "../Common/Logger.h"

namespace octronic
{

    Grid::Grid(AppState* state, float majorSpacing, float minorSpacing,
		vec3 majorColour, vec3 minorColour)
        : Widget3D(state),
          mGridArea(300,300),
          mMajorSpacing(majorSpacing), mMinorSpacing(minorSpacing),
          mMajorColour(majorColour), mMinorColour(minorColour)
    {
        debug("Grid: Constructor");
    }

    Grid::~Grid()
    {
        debug("Grid: Destructor");
    }

    bool Grid::Init()
    {
        debug("Grid: {}",__FUNCTION__);
        if (!Widget3D::Init()) return false;
        RecalculateGridLines();
        return true;
    }

    void Grid::Update()
    {

    }

    void Grid::InitMajorGridData()
    {
        debug("Grid: {}",__FUNCTION__);
        WidgetVertex lineStart, lineEnd;
		lineStart.Color = mMajorColour;
		lineEnd.Color = mMajorColour;

		// X
        for (float step = 0; step <= mGridArea.x; step += mMajorSpacing)
        {
			lineStart.Position = vec3(step, 0.0f, 0);
			lineEnd.Position   = vec3(step, mGridArea.x, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }

        // Y
        for (float step = 0; step <= mGridArea.y; step += mMajorSpacing)
        {
			lineStart.Position = vec3(0, step, 0.0f);
			lineEnd.Position   = vec3(mGridArea.y, step, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }
    }

    void Grid::InitMinorGridData()
    {
        debug("Grid: {}",__FUNCTION__);

        WidgetVertex lineStart, lineEnd;
		lineStart.Color = mMinorColour;
		lineEnd.Color = mMinorColour;

		// X
        for (float step = 0; step <= mGridArea.x; step += mMinorSpacing)
        {
            if (fmod(step,mMajorSpacing) == 0.0f) continue;
			lineStart.Position = vec3(step, 0.0f, 0);
			lineEnd.Position   = vec3(step, mGridArea.x, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }

        // Y
        for (float step = 0; step <= mGridArea.y; step += mMinorSpacing)
        {
            if (fmod(step,mMajorSpacing) == 0.0f) continue;
			lineStart.Position = vec3(0, step, 0.0f);
			lineEnd.Position   = vec3(mGridArea.y, step, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }
	}

    vec3 Grid::GetMinorColour() const
    {
        return mMinorColour;
    }

    void Grid::SetMinorColour(vec3 minorColour)
    {
        mMinorColour = minorColour;
    }

    vec3 Grid::GetMajorColour() const
    {
        return mMajorColour;
    }

    void Grid::SetMajorColour(vec3 majorColour)
    {
        mMajorColour = majorColour;
    }

    void Grid::SetTranslation(vec3 translation)
    {
        mModelMatrix = glm::translate(mat4(1.0f),translation);
    }

    float Grid::GetMajorSpacing()
    {
        return mMajorSpacing;
    }

    void Grid::SetMajorSpacing(float ms)
    {
        mMajorSpacing = ms < 1.0f ? 1.0f : ms;
    }

    float Grid::GetMinorSpacing()
    {
        return mMinorSpacing;
    }

    void Grid::SetMinorSpacing(float ms)
    {
        mMinorSpacing = ms < 0.1f ? 0.1f : ms;
    }

    void Grid::RecalculateGridLines()
    {
        debug("Grid: {}",__FUNCTION__);
        mLineVertexBuffer.clear();
        ClearLineVertexBuffer();
        InitMajorGridData();
        InitMinorGridData();
        SubmitLineVertexBuffer();
    }
}
