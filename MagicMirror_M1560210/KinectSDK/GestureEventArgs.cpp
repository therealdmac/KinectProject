#include "GestureEventArgs.h"
#include "stdafx.h"

//C# TO C++ CONVERTER TODO TASK: The .NET System namespace is not available from native C++:
//using namespace System;

using namespace System;
	GestureEventArgs::GestureEventArgs(GestureType type, int trackingId)
	{
		this->setTrackingId(trackingId);
		this->setGestureType(type);
	}

	const GestureType &GestureEventArgs::getGestureType() const
	{
		return privateGestureType;
	}

	void GestureEventArgs::setGestureType(const GestureType &value)
	{
		privateGestureType = value;
	}

	const int &GestureEventArgs::getTrackingId() const
	{
		return privateTrackingId;
	}

	void GestureEventArgs::setTrackingId(const int &value)
	{
		privateTrackingId = value;
	}

