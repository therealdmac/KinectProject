#pragma once

//#include "IRelativeGestureSegment.h"
//#include "GestureEnumTypes.h"
//#include "GestureSource.h"
//#include "GestureReceiver.h"
#include "Segments\WaveLeftSegments.h"
#include "Segments\WaveRightSegments.h"
#include "Segments\SwipeDownSegments.h"
#include <vector>
#include <queue>

using namespace std;
class Gesture
{
	/// <summary>
	/// The parts that make up this gesture
	/// </summary>
private:
//Since the array size is not known in this declaration, this array is changed to a pointer.  
//You will need to call 'delete[]' where appropriate:
//private IRelativeGestureSegment[] gestureParts;
	//probably not using anymore
	//IRelativeGestureSegment *gestureParts;
	//IRelativeGestureSegment[] gestureParts;

	WaveLeftSegments *wavLeft;
	WaveRightSegments *wavRight;
	SwipeDownSegments *swipeDown;
	//include other specific gesture classes here

	/// <summary>
	/// The current gesture part that we are matching against
	/// </summary>
	int currentGesturePart;

	/// <summary>
	/// the number of frames to pause for when a pause is initiated
	/// </summary>
	int pausedFrameCount;

	/// <summary>
	/// The current frame that we are on
	/// </summary>
	int frameCount;

	/// <summary>
	/// Are we paused?
	/// </summary>
	bool paused;

	/// <summary>
	/// The type of gesture that this is
	/// Type of gestures classified into numbers. default is 0, no gesture.
	/// </summary>
	int theGestureType;

	/// <summary>
	/// Queue list of all sequential tracked gesture parts
	/// not needed
	/// </summary>
	/// std::vector<bool*> allPartStatus;
	std::queue<int> gesturePartNum;

	/// <summary>
	/// Is the gesture detected?
	/// </summary>
	bool detected;

	int detectpartnum;
	int detectpartnum2;

	/// <summary>
	/// Initializes a new instance of the <see cref="Gesture"/> class.
	/// </summary>
	/// <param name="type">The type of gesture.</param>
	/// <param name="gestureParts">The gesture parts.</param>
public:
	
	//constructor
	Gesture(int intGesType);
	//type 0: no gesture
	//type 1: wave left
	//type 2: wave right
	//type 3: swipe left
	//type 4: swipe right
	//include other gesture types for other recognisable gestures. Code them first and include them in this header file.

	/// <summary>
	/// Occurs when [gesture recognised].
	/// </summary>
	//public event EventHandler<GestureEventArgs> GestureRecognized;
	//GestureSource gesSrc;
	//GestureReceiver gesRcv;

	/// <summary>
	/// Updates the gesture.
	/// </summary>
	/// <param name="data">The skeleton data.</param>
	void UpdateGesture(Skeleton *data);
	void UpdateGesture(Skeleton *data, int frames);

	/// <summary>
	/// Resets this instance.
	/// </summary>
	void Reset();

	/// <summary>
	/// Update the frame rate member based on the Kinect Camera Frame rate
	/// Will be using 2nd function preferably
	/// </summary>
	void updateFrameRate();

	void updateFrameRate(int frames);

	void detectGestureParts(Skeleton *data);

	void printFrameRate();

	int getGesType();

	bool isDetected();

	void alterDetectedVal(bool newVal);

	int getpartnum();

	int getpartnum2();

private:
	void InitializeInstanceFields();
};
