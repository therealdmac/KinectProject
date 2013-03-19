#pragma once

#include "Gesture.h"
//#include "GestureEnumTypes.h"
//#include "IRelativeGestureSegment.h"
//#include "GestureEventArgs.h"
#include <vector>


class GestureController
{
	/// <summary>
	/// The list of all gestures we are currently looking for
	/// We will use vectors to store the list of gestures
	/// </summary>
public:
	std::vector<Gesture*> gestures;

	/// <summary>
	/// Initializes a new instance of the <see cref="GestureController"/> class.
	/// </summary>
public:
	GestureController();

	/// <summary>
	/// Occurs when [gesture recognised].
	/// </summary>
	///	public event EventHandler<GestureEventArgs> GestureRecognized;
	/// above is unusable in native C++. we will be using these handlers instead
	//GestureSource gesSrc;
	//GestureReceiver gesRcv;not needed already

	/// <summary>
	/// Updates all gestures.
	/// </summary>
	/// <param name="data">The skeleton data.</param>
	void UpdateAllGestures(Skeleton *data);
	void UpdateAllGestures2(Skeleton *data, int frames);

	/// <summary>
	/// Adds the gesture.
	/// </summary>
	/// <param name="type">The gesture type.</param>
	/// <param name="gestureDefinition">The gesture definition.</param>
	void AddGesture(int intGesType);

	/// <summary>
	/// Gets the gesture.
	/// </summary>
	/// <param name="type">The gesture type.</param>
	/// <param name="gestureDefinition">The gesture definition.</param>
	Gesture* GetGesture(int intGesType);


	/// <summary>
	/// Get the gesture list.
	/// </summary>
	/// <param name=""></param>
	std::vector<Gesture*> getGestureList();

	/// <summary>
	/// Handles the GestureRecognized event of the g control.
	/// </summary>
	/// <param name="sender">The source of the event.</param>
	/// <param name="e">The <see cref="KinectSkeltonTracker.GestureEventArgs"/> instance containing the event data.</param>
	/// private void Gesture_GestureRecognized(object sender, GestureEventArgs e)
	/// for this handler, this is equivalent to the handlers in the GestureReceiver class in native C++
private:
	//void OnGestureRecognized(void *sender, GestureEventArgs *e);

	void GestureIsRecognized(int gesType);

private:
	void InitializeInstanceFields();
};

