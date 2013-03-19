#include "Gesture.h"
//#include "GestureEventArgs.h"
#include "skeleton.h"
#include "stdafx.h"

using namespace std;

//we will be using this constructor
Gesture::Gesture(int type)
{
	//initialisation of all basic data member values
	InitializeInstanceFields();
	
	//integer of the type of gesture input by constructor parameter field
	theGestureType = type;
	//depending on the int type, create the child gesture

	//create new gesture segments if any.
	if (theGestureType == 1) {
		wavLeft = new WaveLeftSegments();
	} else if (theGestureType == 2) {
		wavRight = new WaveRightSegments();
	} else if (theGestureType == 3) {
		swipeDown = new SwipeDownSegments();
	}

	/*
	switch (theGestureType) {
		case 1:
			wavLeft = new WaveLeftSegments();
		//break;
		case 2:
			wavRight = new WaveRightSegments();
		//break;
		default:
		break;
	}		
	*/
}

void Gesture::detectGestureParts(Skeleton *data) 
{
	
	switch (theGestureType) {
		case 1:
			wavLeft->waveleftsegment1(data);
			wavLeft->waveleftsegment2(data);
		break;
		case 2:
			wavRight->waveRightSegment1(data);
			wavRight->waveRightSegment2(data);
		break;
		case 3:
			swipeDown->swipeDownSegment1(data);
			swipeDown->swipeDownSegment2(data);
			swipeDown->swipeDownSegment3(data);
		default:
		break;
	}	
}
//stub. Won't be using anymore
void Gesture::UpdateGesture(Skeleton *data) 
{
	
	if (paused) {
		if (frameCount == pausedFrameCount)
		{
			paused = false;
		}
		frameCount++;
		//updateFrameRate(int frames);
	}
	//instantiate gesture parts results first. 0-fail, 1-pausing, 2-success
	int intGestureResult = 0;
	int intGestureResult2 = 0;
	int intGestureResult3 = 0;

	//get the result, either fail-0, pausing-1 or success-2
	//get it by accessing the children of the gesture class and access their method return
	//if the gesturetype is 1(wave left)
	//if (this->theGestureType = 1) {
		intGestureResult = wavLeft->waveleftsegment1(data);
		intGestureResult2 = wavLeft->waveleftsegment2(data);
	//}
	//add more gesture parts if child gesture has more methods

	//if both gesture parts return success
	if (intGestureResult == 2 && intGestureResult2 ==2) {
		//gesRcv.hookEvent(&gesSrc);
		//call the event handler here
		//__raise gesSrc.GestureRecognised(theGestureType);
		//gesRcv.unhookEvent(&gesSrc);
		//reset the gesture
		Reset();
	}
	else if (intGestureResult == 0 || intGestureResult2 == 0 || this->frameCount > 50)
	{
		//reset the gesture
		currentGesturePart = 0;
		frameCount = 0;
		pausedFrameCount = 5;
		paused = true;
		//set any detected gesture parts to false
		if (theGestureType == 1) {
			wavLeft->part1Success = false;
			wavLeft->part2Success = false;
		}
	}
	else
	{
		//frameCount++;
		pausedFrameCount = 5;
		paused = true;
	}
}

//will be using this method, as it has got the frame counter argument as reference
void Gesture::UpdateGesture(Skeleton *data, int frames) 
{
	//instantiate gesture parts results first. 0-fail, 1-pausing, 2-success
	detectGestureParts(data);

	if (paused) {
		if (frameCount == pausedFrameCount)
		{
			paused = false;
		}
	}
	
	//add more gesture parts if child gesture has more methods

	switch (theGestureType) {
	case 1:

		if (wavLeft->part1Success) {
			gesturePartNum.push(1);
			if (wavLeft->part2Success) {
				gesturePartNum.push(2);
				if (gesturePartNum.back() == 2) {
					detected = true;
					detectpartnum = 999;
					printf_s("Waving Left gesture downwards detected at frame %d. \n", frames);
					printf_s("test %d. \n", detectpartnum);
					for (size_t i = 0; i < gesturePartNum.size(); ++i) {
						gesturePartNum.pop();
					}
					Reset();
					printf_s("test %d. \n", detectpartnum);
				} 
				else if (gesturePartNum.back() == 1) {
					detected = true;
					printf_s("Waving Left gesture upwards detected at frame %d. \n", frames);
					for (size_t i = 0; i < gesturePartNum.size(); ++i) {
						gesturePartNum.pop();
					}
					Reset();
				}
			}
		}
		break;
		
		case 2:

		if (wavRight->part1Success) {
			if (wavRight->part2Success) {
				detected = true;
				detectpartnum2 = 998;
				printf_s("Waving Right gesture detected at frame %d. \n", frames);
				printf_s("test %d. \n", detectpartnum2);
				Reset();
			}
		}
		break;

		case 3:

		if (swipeDown->part1Success) {
			if (swipeDown->part2Success) {
				if (swipeDown->part3Success) {
					detected = true;
					printf_s("Swiping Down gesture detected at frame %d. \n", frames);
					Reset();
				}
			}
		}
		break;

		default:
		break;
	}
	
}

void Gesture::Reset()
{
	currentGesturePart = 0;
	frameCount = 0;
	pausedFrameCount = 5;
	paused = true;
	//set all detected gesture parts to false
	//use switch later
	if (theGestureType == 1) {
		wavLeft->part1Success = false;
		wavLeft->part2Success = false;
	} else if (theGestureType == 2) {
		wavRight->part1Success = false;
		wavRight->part2Success = false;
	} else if (theGestureType == 3) {
		swipeDown->part1Success = false;
		swipeDown->part2Success = false;
		swipeDown->part3Success = false;
	}
	//for testing - comment off for testing
	//detected = false;
	detectpartnum = 0;
	detectpartnum2 = 0;
	
}

void Gesture::updateFrameRate() 
{
	this->frameCount++;
}

void Gesture::updateFrameRate(int frames) 
{
	//assign the frame value to the data framecount member
	frameCount = frames;
}

void Gesture::printFrameRate(){
	printf_s("This is the framerate from Gesture class %d. \n", frameCount);
}

int Gesture::getGesType() {
	return theGestureType;
}

bool Gesture::isDetected() {
	return detected;
}

int Gesture::getpartnum() {
	return detectpartnum;
}

int Gesture::getpartnum2() {
	return detectpartnum2;
}

//alters the detected value of the gesture with a new boolean value after it is being detected
void Gesture::alterDetectedVal(bool newVal) {
	detected = newVal;
}

void Gesture::InitializeInstanceFields()
{
	currentGesturePart = 0;
	pausedFrameCount = 10;
	frameCount = 0;
	paused = false;
	detected = false;
	detectpartnum = 0;
	detectpartnum2 = 0;
}
