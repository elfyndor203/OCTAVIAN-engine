#pragma once
#include "vectors/vec2.h"

typedef struct {
	OCT_vec2 origin;
	float rotation;
	float width;
	float height;
} OCT_rect2;

/// <summary>
/// Returns the bottom left vertex position and the rest optionally.
/// </summary>
/// <param name="rect"></param>
/// <param name="botR"></param>
/// <param name="topR"></param>
/// <param name="topL"></param>
/// <returns></returns>
OCT_vec2 OCT_rect2_getVerts(OCT_rect2 rect, OCT_vec2* botR, OCT_vec2* topR, OCT_vec2* topL);

/// <summary> Returns the width normal and the height normal optionally. </summary>/// <param name="rect"></param>/// <param name="heightAxis"></param>/// <returns></returns>
OCT_vec2 OCT_rect2_getNormals(OCT_rect2 rect, OCT_vec2* heightAxis);