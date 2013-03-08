#pragma once

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

const int DEPTH_RES_X = 320;
const int DEPTH_RES_Y = 240;

const int COLOR_RES_X = 1280;
const int COLOR_RES_Y = 960;



struct Stats {
	float totalTimeSpent;
	int nPictures,
		 nUploads,
		 counts;	

	struct Stats() {
		totalTimeSpent = 0;
		nUploads = 0;
		nPictures = 0;
		counts = 0;
	}
	  
};

//FOR USE IN LAB
#define LAB_MACHINE
