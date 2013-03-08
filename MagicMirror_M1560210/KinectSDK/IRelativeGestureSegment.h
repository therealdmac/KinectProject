#pragma once

#include "GestureEnumTypes.h"
#include "skeleton.h"
#include <NuiApi.h>
#include <NuiSkeleton.h>

//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System::Collections::Generic;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System::Linq;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System::Text;


/// <summary>
/// Defines a single gesture segment which uses relative positioning 
/// of body parts to detect a gesture
/// </summary>
class IRelativeGestureSegment
{
	/// <summary>
	/// Checks the gesture.
	/// </summary>
	/// <param name="skeleton">The skeleton.</param>
	/// <returns>GesturePartResult based on if the gesture part has been completed</returns>
public:
	virtual GesturePartResult CheckGesture(Skeleton *skeleton) = 0;
};
