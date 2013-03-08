#include "stdafx.h"
#include "PostProcess.h"

PostProcessThread_data ppt_data[MAX_NUM_THREADS+1]; //add one for handbag -> always the last ID
pthread_t pp_thread[MAX_NUM_THREADS];
pthread_t bag_thread;	//Just one for handbag
pthread_attr_t attr;
bool bag_thread_launched;
bool clothes_thread_launched;
bool clothes_lock;
bool bag_lock;
bool bIsWorking[MAX_NUM_THREADS + 1]; //add one for handbag -> always ID=7
bool bDoJob[MAX_NUM_THREADS+1];


pthread_mutex_t doJob_mutex[MAX_NUM_THREADS+1];
pthread_mutex_t wait_mutex[MAX_NUM_THREADS+1];
pthread_cond_t doJob_cond[MAX_NUM_THREADS+1];

double NOISE_AMPLIFY_LEVEL=8.0;
//box muller
//Z_0 = R \cos(\Theta) =\sqrt{-2 \ln U_1} \cos(2 \pi U_2)\, 

int RGBrandn( int *anum1, int *anum2)
//int RGBrandn( )
{
	double u1 =  2.0*((double) rand())/(double)RAND_MAX-1.0;		//uniform distributed from -1 to 1
	double u2 =  2.0*((double) rand())/(double)RAND_MAX-1.0; 
	double r = u1*u1+u2*u2;
	r=r>1.0?1.0:r;

	double m = sqrt(-2.0*log(r)/r);
	double z0 = m*u1;
	double z1 = m*u2;
	

				*anum1= (int)(z0*NOISE_AMPLIFY_LEVEL);		//use this
				*anum2 = (int)(z1*NOISE_AMPLIFY_LEVEL);
				//int anum=(int)(z0*15.0);		//use this to check - more obvious
				//fprintf(stdout, "anum = %d\n", anum);

			//unsigned char rgbnum= anum&0xff;

	return *anum1;
	//return anum;
}

//Non-Threaded version
/*
void PostProcess_nt( unsigned char* inImg, unsigned char* outImg, int nwidth, int nheight )
{
	clock_t start, end;
	start = clock();
	srand(start);
	unsigned char* rgbdata = inImg;
	unsigned char* newdata = outImg;
	int anum;
	for (int i=0; i<nheight; i++)
	{
		for (int j=0; j<nwidth; j++)
		{
			int idx = i*nwidth*4 + j*4;
			//int randnum = RGBrandn();
			//double r = 2.0*((double) rand())/(double)RAND_MAX-1.0;	//-1 to 1
			//int randnum = (int) (r*100);
			if (rgbdata[idx+3]>=10)	//only add noise to the non-transparent areas
			{

				anum = rgbdata[idx]+RGBrandn();
				//anum = rgbdata[idx]+randnum;
				anum=anum>255?255:anum;
				anum=anum<0?0:anum;
				newdata[idx] = anum&0xff;

				anum = rgbdata[idx+1]+RGBrandn();
				//anum = rgbdata[idx+1]+randnum;
				anum=anum>255?255:anum;
				anum=anum<0?0:anum;
				newdata[idx+1] = anum&0xff;

				anum = rgbdata[idx+2]+RGBrandn();
				//anum = rgbdata[idx+2]+randnum;
				anum=anum>255?255:anum;
				anum=anum<0?0:anum;
				newdata[idx+2] = anum&0xff;
			}
			else
			{
				newdata[idx] = rgbdata[idx];
				newdata[idx+1] = rgbdata[idx+1];
				newdata[idx+2] = rgbdata[idx+2];
			}

			newdata[idx+3] = rgbdata[idx+3];

		}
	}
	end=clock();
	double dif =((double)(end-start))/CLOCKS_PER_SEC;
	fprintf(stdout, "thread takes %f sec\n", dif);
	fflush(stdout);

}
*/
//Threaded version
void* __cdecl PostProcess( void* inArg )
{
	struct PostProcessThread_data* inData = (struct PostProcessThread_data*) inArg;
	int randnum;
	int count=0;
	int n[2];
	RGBrandn( &n[0], &n[1] );
	int threadId = inData->id;
	//BOOL bdoJob = true;
	bDoJob[threadId] = true;
	bool firsttime = true;
	//fprintf(stdout, "threadid %d created!\n", threadId );
	//fflush(stdout);
	//while (!bIsWorking[threadId])
	while (1)
	{

		if (!firsttime)	//don;t have to wait for first round
		{
				//let's wait until signalled
				pthread_mutex_lock( &wait_mutex[threadId]);
				pthread_cond_wait( &doJob_cond[threadId], &wait_mutex[threadId]);
				pthread_mutex_unlock(&wait_mutex[threadId]);
		}
		else
		{
			firsttime = false;
		}
		bIsWorking[threadId] = true;
		clock_t start, end;
		start = clock();

    	srand(start);		//do not use time(NULL) as it changes much slower than clock, and so seeding is the same

		unsigned char* rgbdata = inData->inImg;
		unsigned char* newdata = inData->outImg;
		int nwidth = inData->nwidth;
		int nheight = inData->nheight;
		int anum;
		for (int i=0; i<nheight; i++)
		{
			for (int j=0; j<nwidth; j++)
			{
				int idx = i*nwidth*4 + j*4;

				if (rgbdata[idx+3]>=10)
				{
					if (count<2)
					{
						randnum=n[count]; count++;
						if (count==2)
						{
							RGBrandn( &n[0], &n[1] );
							count = 0;
						}
					}
					anum = rgbdata[idx]+randnum;
					//anum = rgbdata[idx]+RGBrandn();

					anum=anum>255?255:anum;
					anum=anum<0?0:anum;
					newdata[idx] = anum&0xff;


					if (count<2)
					{
						randnum=n[count]; count++;
						if (count==2)
						{
							RGBrandn( &n[0], &n[1] );
							count = 0;
						}
					}
					anum = rgbdata[idx+1]+randnum;
					//anum = rgbdata[idx+1]+RGBrandn();

					anum=anum>255?255:anum;
					anum=anum<0?0:anum;
					newdata[idx+1] = anum&0xff;

					if (count<2)
					{
						randnum=n[count]; count++;
						if (count==2)
						{
							RGBrandn( &n[0], &n[1] );
							count = 0;
						}
					}
					anum = rgbdata[idx+2]+randnum;										
					//anum = rgbdata[idx+2]+RGBrandn();

					anum=anum>255?255:anum;
					anum=anum<0?0:anum;
					newdata[idx+2] = anum&0xff;
				}
				else	//no need to do for alphas that are low
				{
					newdata[idx] = rgbdata[idx];
					newdata[idx+1] = rgbdata[idx+1];
					newdata[idx+2] = rgbdata[idx+2];
				}

				newdata[idx+3] = rgbdata[idx+3];

			}
		}


				//check timing
				end=clock();
				double dif =((double)(end-start))/CLOCKS_PER_SEC;
				//if (threadId == MAX_NUM_THREADS)	
				//{
					//fprintf(stdout, "thread %d with %d takes %f sec\n", inData->id, inData->nheight, dif);
					//fflush(stdout);
				//}

		pthread_mutex_lock( &doJob_mutex[threadId] );		
				bIsWorking[threadId] = false;
		


		if (!bDoJob[threadId])
		{  pthread_mutex_unlock( &doJob_mutex[threadId] );	//end of processing
			break;
		}
		pthread_mutex_unlock( &doJob_mutex[threadId] );	//end of processing

	}	//while
	//else
	//{
	//	fprintf(stdout, "thread %d busy\n", inData->id);
	//	fflush(stdout);
	//}
	//fprintf(stdout, "%d end here\n", threadId );
	//fflush(stdout);
	
	pthread_exit(NULL);
	return NULL;
}

void StartPostProcessThread( unsigned char* inImg, unsigned char* outImg, int nwidth, int nheight )
{
	clothes_lock =true;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE);
	//PostProcessInit();

	int subheight=(int)(nheight/(MAX_NUM_THREADS-1));
	int remheight=nheight-(MAX_NUM_THREADS-1)*subheight;
	for (int i=0; i<MAX_NUM_THREADS;i++)
	{	
		bIsWorking[i] = false;
		bDoJob[i] = false;
		//if (!bIsWorking[i])
		//{
			ppt_data[i].inImg=inImg + i*subheight*nwidth*4;
			ppt_data[i].outImg = outImg+ i*subheight*nwidth*4;
			ppt_data[i].nwidth = nwidth;
			ppt_data[i].id = i;
			if (i==MAX_NUM_THREADS-1)
				ppt_data[i].nheight = remheight;
			else
				ppt_data[i].nheight =  subheight;

			//initialise condition and mutex
			pthread_mutex_init(&doJob_mutex[i], NULL);
			pthread_mutex_init(&wait_mutex[i], NULL);
			pthread_cond_init (&doJob_cond[i], NULL);
			int rc = pthread_create(&pp_thread[i], &attr,  PostProcess, (void *) &ppt_data[i]);

			if (rc){
				printf("ERROR; return code from pthread_create() is %d\n", rc);
				exit(-1);
			}

	}
	clothes_thread_launched = true;
	clothes_lock = false;

}
void DoPostProcess()
{

	if (!clothes_thread_launched)	//don't do anything if no threads are launched
		return;
	if (clothes_lock)	//don't do anything if ending or starting
		return;
		//Wake up all threads
	for (int i=0; i<MAX_NUM_THREADS; i++)
	{
		pthread_mutex_lock( &wait_mutex[i]);
		pthread_cond_signal( &doJob_cond[i]);
		pthread_mutex_unlock( &wait_mutex[i]);
	}
}

bool WaitPostProcessing()
{
	if (!clothes_thread_launched) //don't do anything if no threads are launched
		return true;
	bool ok=false;
	int count=0;
	while (count < 100000 )	//don't wait forever
	{
		ok=true;
		//fprintf(stdout, "%d ", count);
		for (int i=0; i<MAX_NUM_THREADS; i++)
		{
			pthread_mutex_lock( &doJob_mutex[i]);
			ok = ok&&!bIsWorking[i];
			//fprintf(stdout, "%d:%d ", i, bIsWorking[i]);

			pthread_mutex_unlock(&doJob_mutex[i]);
		}
		//fprintf(stdout, "\n");
		//fflush(stdout);
		if (ok)
			break;
		count++;
	}
	if (count==100000)
	{
		fprintf(stdout, "wait over, exit wait. threads not done. \n");
		fflush(stdout);
		for (int i=0; i<MAX_NUM_THREADS; i++)
		{
			fprintf(stdout, "%d:%d ", i, bIsWorking[i]);
		}
		fprintf(stdout, "\n");
		fflush(stdout);
		return false;
	}
	//fprintf(stdout, "all threads done\n");
	//fflush(stdout);
	return true;
}

bool EndPostProcessThread()
{
	
	if (!clothes_thread_launched)	//no need to end threads if they are not launched
		return true;
	clothes_lock = true;
	int rc;
	void* status;
/*	fprintf(stdout, "ending process\n");
	fflush(stdout)*/;
	//WaitPostProcessing();	//let them finish their job

	//and then wake to kill
	for (int i=0; i<MAX_NUM_THREADS; i++)
	{
		pthread_mutex_lock( &doJob_mutex[i] );		
		bDoJob[i]=false;
		pthread_mutex_unlock( &doJob_mutex[i] );	//end of processing

		//if the thread is in the middle of the loop, bDoJob will exit the thread, before reaching the signalling
		//if the thread is waiting, the following will do one more loop and exit.
		pthread_mutex_lock( &wait_mutex[i]);
		pthread_cond_signal( &doJob_cond[i]);	
		pthread_mutex_unlock( &wait_mutex[i]);
	}
	pthread_attr_destroy(&attr);
	for (int i=0; i<MAX_NUM_THREADS; i++)
	{
		//fprintf(stdout, "waiting for thread %d to end\n", i);
		//fflush(stdout);
		rc = pthread_join(pp_thread[i], &status);

		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			return false;
		}
		//printf("Main: completed join with thread %ld having a status of %ld\n",i,(long)status);
		pthread_mutex_destroy(&doJob_mutex[i]);
		pthread_mutex_destroy(&wait_mutex[i]);
		pthread_cond_destroy(&doJob_cond[i]);

	}
	clothes_thread_launched = false;
	clothes_lock = false;
	return true;
}

void StartPostProcessThread_forBag( unsigned char* inImg, unsigned char* outImg, int nwidth, int nheight )
{
	bag_lock = true;
	int idx = MAX_NUM_THREADS;

	bIsWorking[idx] = false;
	bDoJob[idx] = false;

	//if (!bIsWorking[idx])
	//{
		ppt_data[idx].inImg = inImg;
		ppt_data[idx].outImg = outImg;
		ppt_data[idx].nwidth = nwidth;
		ppt_data[idx].nheight = nheight;
		ppt_data[idx].id = idx;
			pthread_mutex_init(&doJob_mutex[idx], NULL);
			pthread_mutex_init(&wait_mutex[idx], NULL);
			pthread_cond_init (&doJob_cond[idx], NULL);
		int rc = pthread_create(&bag_thread, NULL,  PostProcess, (void *) &ppt_data[idx]);

		if (rc){
			printf("ERROR; return code from pthread_create() (#6) is %d\n", rc);
			exit(-1);
		}
		bag_thread_launched = true;
		bag_lock = false;
		//fprintf(stdout, "bag thread started\n");
		//fflush(stdout);
		
	//}
	//else
	//{
	//	fprintf(stdout, "Not creating thread #6 because it is busy\n");
	//	fflush(stdout);
	//}
}
bool EndPostProcessThread_forBag()
{	
	bag_lock = true;
	if (bag_thread_launched)
	{
		int idx = MAX_NUM_THREADS;
			bDoJob[idx]=false;
			pthread_mutex_lock( &wait_mutex[idx]);
			pthread_cond_signal( &doJob_cond[idx]);
			pthread_mutex_unlock( &wait_mutex[idx]);

		int rc=pthread_join(bag_thread, NULL);
		bag_thread_launched=false;
		if (rc)
		{
			bag_lock = false;
			printf("ERROR; return code from pthread_join() bags is %d\n", rc);
			return false;
		}
				pthread_mutex_destroy(&doJob_mutex[idx]);
			pthread_mutex_destroy(&wait_mutex[idx]);
			pthread_cond_destroy(&doJob_cond[idx]);
			fprintf(stdout, "bag_thread terminated\n");
			fflush(stdout);
	}
	bag_lock = false;
	return true;
}

void DoPostProcess_forBag()
{
	if (bag_thread_launched && !bag_lock)	//if ending or starting, don't do this.
	{
	pthread_mutex_lock( &wait_mutex[MAX_NUM_THREADS]);
	pthread_cond_signal( &doJob_cond[MAX_NUM_THREADS]);
	pthread_mutex_unlock( &wait_mutex[MAX_NUM_THREADS]);
	}
}

bool WaitPostProcessing_forBag()
{

	bool waitover = false;
	if (bag_thread_launched)
	{
		int count = 0;
		while (bIsWorking[MAX_NUM_THREADS])
		{
			count++;
			if (count>100000)
			{
				fprintf(stdout, "Wait thread for bag - time out. exit wait. thread not done\n");
				waitover = true;
				break;

			}
		}
	}
	if (waitover)
		return false;
	return true;
	
}
void PostProcessInit()
{
	for (int i=0; i<MAX_NUM_THREADS+1; i++)
	{
		bIsWorking[i] = false;
		bDoJob[i] = false;
	}
	bag_thread_launched = false;
	clothes_thread_launched = false;
	clothes_lock = false;
	bag_lock = false;
}