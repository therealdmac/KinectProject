#include "GestureController.h"
//#include "Segments/JoinedHandsSegment.h"
//#include "Segments/Menu/MenuSegments.h"
//#include "Segments/SwipeLeft/SwipeLeftSegment1.h"
//#include "Segments/SwipeLeft/SwipeLeftSegment2.h"
//#include "Segments/SwipeLeft/SwipeLeftSegment3.h"
//#include "Segments/SwipeRight/SwipeRightSegment1.h"
//#include "Segments/SwipeRight/SwipeRightSegment2.h"
//#include "Segments/SwipeRight/SwipeRightSegment3.h"
#include "Segments/WaveRightSegments.h"
#include "Segments/WaveLeftSegments.h"
//#include "Segments/ZoomSegments.h"
#include "stdafx.h"

using namespace std;

GestureController::GestureController()
{
	// Define the gestures

	InitializeInstanceFields();

	//add the gestures here. More gestures to be added later
	AddGesture(1);
	AddGesture(2);
	AddGesture(3);
	
}
//is called in some internal Kinect camera/skeleton class where frame runs are involved
//required to be called as it updates frames per sec
void GestureController::UpdateAllGestures(Skeleton *data)
{
	for (std::vector<Gesture*>::const_iterator gesture = this->gestures.begin(); gesture != this->gestures.end(); ++gesture)
	{
		(*gesture)->UpdateGesture(data);
	}
}

//this is used instead of the earlier method
void GestureController::UpdateAllGestures2(Skeleton *data, int frames)
{
	for (std::vector<Gesture*>::const_iterator gesture = this->gestures.begin(); gesture != this->gestures.end(); ++gesture)
	{
		(*gesture)->UpdateGesture(data, frames);
	}
}

//is called when gesture controller object is created
void GestureController::AddGesture(int gesType)
{
	//create a new gesture obj based on its type
	Gesture *gesture = new Gesture(gesType);
	//add a new eventhandler to the gesture
	//same as the hookback to GestureSource
	//GestureSource src;
	//GestureReceiver rcv; // = new GestureReceiver();
	//gesRcv.hookEvent(&gesSrc); //if pointer type, will be rcv->hookEvent(&src);
	//push it back to the list of gestures
	gestures.push_back(gesture);
}

Gesture* GestureController::GetGesture(int gesType)
{
	for (std::vector<Gesture*>::const_iterator g = this->gestures.begin(); g != this->gestures.end(); ++g)
	{
		if ((*g)->getGesType() == gesType) {
			return (*g);
		}
	}
}

std::vector<Gesture*> GestureController::getGestureList() {
	return gestures;
}

//may have to call this method outside wen gesturecontroller object is created
void GestureController::GestureIsRecognized(int gesType)
{
	//raise the event 
	//__raise gesSrc.GestureRecognised(gesType);
	//reset all the gestures
	for (std::vector<Gesture*>::const_iterator g = this->gestures.begin(); g != this->gestures.end(); ++g)
	{
		(*g)->Reset();
	}
}

void GestureController::InitializeInstanceFields()
{
	//create the list of gestures for storage
	gestures = std::vector<Gesture*>();
}

