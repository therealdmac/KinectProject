#include "WaveRightSegments.h"
//#include <NuiApi.h>
#include "../stdafx.h"
#include "stdafx.h"

using namespace std;

WaveRightSegments::WaveRightSegments() {
	fail = 0;
	pausing = 1;
	success = 2;
	numParts = 2;
	part1Success = false;
	part2Success = false;
}

int WaveRightSegments::waveRightSegment1(Skeleton *skel){
	// hand above elbow
	if (skel->handR.y > skel->elbowR.y)//y
	{
		// hand right of elbow
		if (skel->handR.x > skel->elbowR.x)//x
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

int WaveRightSegments::waveRightSegment2(Skeleton *skel){
	// hand above elbow
	if (skel->handR.y > skel->elbowR.y)//y
	{
	// hand right of elbow
		if (skel->handR.x < skel->elbowR.x)//x
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

bool WaveRightSegments::IsSegment1Detected()
{
	return part1Success;
}

bool WaveRightSegments::IsSegment2Detected()
{
	return part2Success;
}