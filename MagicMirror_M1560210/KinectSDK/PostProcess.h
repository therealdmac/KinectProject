#pragma once

#include <pthread.h>
#include <time.h>

#define MAX_NUM_THREADS 8
struct PostProcessThread_data{
   unsigned char* outImg;
   unsigned char* inImg;
   int nwidth;
   int nheight;
   int id;		//ID of thread. Not ID of job
};

void PostProcessInit();

//void* __cdecl PostProcess( void* inArg );
void  StartPostProcessThread( unsigned char* inImg, unsigned char* outImg, int nwidth, int nheight );
bool WaitPostProcessing();
bool EndPostProcessThread();
void DoPostProcess();

//will launch a single and separate thread for bag
void StartPostProcessThread_forBag( unsigned char* inImg, unsigned char* outImg, int nwidth, int nheight );
bool WaitPostProcessing_forBag();
bool EndPostProcessThread_forBag();
void DoPostProcess_forBag();