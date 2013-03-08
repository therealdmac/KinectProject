#pragma once

#include "GestureEnumTypes.h"
#using <mscorlib.dll>
using namespace system;

//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System;


/// <summary>
/// The gesture event arguments
/// </summary>
public __gc class GestureEventArgs : public EventArgs
{
	/// <summary>
	/// Initializes a new instance of the <see cref="GestureEventArgs"/> class.
	/// </summary>
	/// <param name="type">The gesture type.</param>
	/// <param name="trackingID">The tracking ID.</param>
	/// <param name="userID">The user ID.</param>
public:
	GestureEventArgs(GestureType type, int trackingId);

	/// <summary>
	/// Gets or sets the type of the gesture.
	/// </summary>
	/// <value>
	/// The type of the gesture.
	/// </value>
private:
	GestureType privateGestureType;
public:
	const GestureType &getGestureType() const;
	void setGestureType(const GestureType &value);

	/// <summary>
	/// Gets or sets the tracking ID.
	/// </summary>
	/// <value>
	/// The tracking ID.
	/// </value>
private:
	int privateTrackingId;
public:
	const int &getTrackingId() const;
	void setTrackingId(const int &value);
};

