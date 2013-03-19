#include "WaveLeftSegments.h"
//#include <NuiApi.h>
#include "../stdafx.h"
#include "stdafx.h"

using namespace std;

WaveLeftSegments::WaveLeftSegments() {
	fail = 0;
	pausing = 1;
	success = 2;
	numParts = 2;
	part1Success = false;
	part2Success = false;
	frameCapturedPart1 = 0;
	frameCapturedPart2 = 0;
}

int WaveLeftSegments::waveleftsegment1(Skeleton *skel){
	// hand above elbow
	if (skel->handL.y > skel->shoulderL.y)//y
	{
		// hand right of elbow
		if (skel->handL.x > skel->shoulderL.x)//x
		{
			part1Success = true;
			return 2;//success
		}

		// hand has not dropped but is not quite where we expect it to be, pausing till next frame
		return 1;//pausing
	}

	// hand dropped - no gesture fails
	return 0;//fail
}

//left palm is at the right side of elbow

int WaveLeftSegments::waveleftsegment2(Skeleton *skel){
	// hand above elbow
	if (skel->handL.y > skel->shoulderL.y)//y
	{
	// hand right of elbow
		if (skel->handL.x < skel->shoulderL.x)//x
		{
			part2Success = true;
			return 2;
		}

		// hand has not dropped but is not quite where we expect it to be, pausing till next frame
		return 1;
	}

	// hand dropped - no gesture fails
	return 0;
}

bool WaveLeftSegments::IsSegment1Detected()
{
	return part1Success;
}

bool WaveLeftSegments::IsSegment2Detected()
{
	return part2Success;
}

void WaveLeftSegments::setDetectedCapturedFrame1(int dFrame)
{
	frameCapturedPart1 = dFrame;
}

void WaveLeftSegments::setDetectedCapturedFrame2(int dFrame)
{
	frameCapturedPart2 = dFrame;
}




