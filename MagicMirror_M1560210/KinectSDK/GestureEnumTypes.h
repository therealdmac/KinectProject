#pragma once

//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System::Collections::Generic;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System::Linq;
//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System::Text;


/// <summary>
/// the gesture part result
/// </summary>
enum GesturePartResult
{
	/// <summary>
	/// Gesture part fail
	/// </summary>
	Fail,

	/// <summary>
	/// Gesture part suceed
	/// </summary>
	Suceed,

	/// <summary>
	/// Gesture part result undetermined
	/// </summary>
	Pausing
	};

	/// <summary>
	/// The gesture type
	/// </summary>
	enum GestureType
	{
	None,
	JoinedHands,
	WaveRight,
	WaveLeft,
	Menu,
	SwipeLeft,
	SwipeRight,
	ZoomIn,
	ZoomOut
};

