#include "SwipeDownSegments.h"
//#include <NuiApi.h>
#include "../stdafx.h"
#include "stdafx.h"

using namespace std;

SwipeDownSegments::SwipeDownSegments() {
	fail = 0;
	pausing = 1;
	success = 2;
	numParts = 3;
	part1Success = false;
	part2Success = false;
	part3Success = false;
	frameCapturedPart1 = 0;
	frameCapturedPart2 = 0;
	frameCapturedPart3 = 0;
}

int SwipeDownSegments::swipeDownSegment1(Skeleton *skel) {
	//if right hand is above head and in front of right shoulder
	if (skel->handR.y > skel->head.y) {
		//hand in front of right shoulder
		if (skel->handR.z < skel->shoulderR.z) {
			if (skel->handR.x > skel->shoulderR.x) {
				part1Success = true;
				return 2;
			}
		}
		return 1;
	}
	return 0;
}

int SwipeDownSegments::swipeDownSegment2(Skeleton *skel) {
	//if hand is lower than shoulder
	if (skel->handR.y < skel->shoulderR.y) {
		//if hand is on the right side of shoulder
		if (skel->handR.x > skel->shoulderR.y) {
			//if hand is higher than hip
			if (skel->handR.y > skel->hipR.y) {
				part2Success = true;
				return 2;
			}
		}
		return 1;
	}
	return 0;
}

int SwipeDownSegments::swipeDownSegment3(Skeleton *skel) {
	if (skel->handR.y < skel->hipR.y) {
		//if hand is behind hip
		if (skel->handR.z > skel->hipR.z) {
			part3Success = true;
			return 2;
		}
		return 1;
	}
	return 0;
}

bool SwipeDownSegments::IsSegment1Detected() {
	return part1Success;
}

bool SwipeDownSegments::IsSegment2Detected() {
	return part2Success;
}

bool SwipeDownSegments::IsSegment3Detected() {
	return part3Success;
}

void SwipeDownSegments::setDetectedCapturedFrame1(int dFrame) {
	frameCapturedPart1 = dFrame;
}

void SwipeDownSegments::setDetectedCapturedFrame2(int dFrame) {
	frameCapturedPart2 = dFrame;
}

void SwipeDownSegments::setDetectedCapturedFrame3(int dFrame) {
	frameCapturedPart3 = dFrame;
}
