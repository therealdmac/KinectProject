#pragma once

#include "../skeleton.h"
#include <vector>

//waving right with your right hand consists of 2 gesture parts

class WaveRightSegments{
public:
	int fail;
	int pausing;
	int success;
	int numParts;
	bool part1Success;
	bool part2Success;
	
	//constructors & destructors
	WaveRightSegments();
	//~WaveLeftSegment(void);

	//methods
	int waveRightSegment1(Skeleton *skel);
	int waveRightSegment2(Skeleton *skel);

	bool IsSegment1Detected();
	bool IsSegment2Detected();
};