#pragma once

#include "../skeleton.h"
#include <vector>

class SwipeDownSegments
{
public:
	int fail;
	int pausing;
	int success;
	int numParts;
	bool part1Success;
	bool part2Success;
	bool part3Success;
	int frameCapturedPart1;
	int frameCapturedPart2;
	int frameCapturedPart3;

	//constructors & destructors
	SwipeDownSegments();
	//~(void);

	//methods
	int swipeDownSegment1(Skeleton *skel);
	int swipeDownSegment2(Skeleton *skel);
	int swipeDownSegment3(Skeleton *skel);

	bool IsSegment1Detected();
	bool IsSegment2Detected();
	bool IsSegment3Detected();

	//these are the frames where the gesture parts are detected
	void setDetectedCapturedFrame1(int dFrame);
	void setDetectedCapturedFrame2(int dFrame);
	void setDetectedCapturedFrame3(int dFrame);

};