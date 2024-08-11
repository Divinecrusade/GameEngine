#pragma once

#include <dwrite.h>


namespace GameEngine
{
	enum class DWriteTextVerticalAlignment
	{
		TOP = DWRITE_PARAGRAPH_ALIGNMENT_NEAR,
		BOTTOM = DWRITE_PARAGRAPH_ALIGNMENT_FAR,
		CENTER = DWRITE_PARAGRAPH_ALIGNMENT_CENTER
	};
}