#pragma once

#include "../skeleton.h"
#include <vector>

//waving left with your left hand consists of 2 gesture parts

class WaveLeftSegments{
public:
	int fail;
	int pausing;
	int success;
	int numParts;
	bool part1Success;
	bool part2Success;
	//std::vector<bool*> partStatus; 
	
	//constructors & destructors
	WaveLeftSegments();
	//~WaveLeftSegment(void);

	//methods
	int waveleftsegment1(Skeleton *skel);
	int waveleftsegment2(Skeleton *skel);

	bool IsSegment1Detected();
	bool IsSegment2Detected();
};

