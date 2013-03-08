#include "stdafx.h"
#include "skeleton.h"

//constructor
Skeleton::Skeleton()
{
	indexForBM = 0;
	shoulder_width = 0.335;
	shoulder_hip_dist = 0.37;
	height = 1.3;
	neck_height = 1.0;
	neck_offsetL = neck_offsetR = 0.1f;
	for(int i=0; i<MAX_DATA_LENGTH; i++) 
	{
		shoulder_width_array[i] = shoulder_width;
		shoulder_hip_dist_array[i] = shoulder_hip_dist;
		height_array[i] = height;
	}
	bUpdateMeasurements = true;
	bInStandardPose=false;
	cameraTiltAngle = -11;
		
	floorPlane.x = 0.085814685;
	floorPlane.y = 0.96914744;
	floorPlane.z = -0.23106116;
	floorPlane.w = 1.52;
}

void Skeleton::Update(bool bUserChanged)
{
	CheckPose();
	//GetBodyMeasurements(bUserChanged);
//	if(bUserChanged) {
	GetShoulderWidth();
	//height = 1.3f;
	if(bInStandardPose) {
		height = (floorPlane.x*(shoulderL.x + shoulderR.x)*0.5f + 
				 floorPlane.y*(shoulderL.y + shoulderR.y)*0.5f+	
				 floorPlane.z*(shoulderL.z + shoulderR.z)*0.5f + 
				 floorPlane.w );
		height = floor(100.0*height+0.5f)/100; //round to 2 decimal places
	}
//	}
}


void Skeleton::CheckPose() {
	
	//shoulder rotation;
	double dy = shoulderR.z - shoulderL.z;
	double dx = shoulderR.x - shoulderL.x;
	shoulder_rotation = atan2(dy, dx) * 180 / CV_PI;

	
	//bInStandardPose = COM.y>handL.y && COM.y>handR.y && fabs(shoulder_rotation)<10 && 
	//	              fabs(handL.x - shoulderL.x)<0.20 && fabs(handR.x - shoulderR.x)<0.20;

	bInStandardPose = shoulderL.y>handL.y && shoulderR.y>handR.y && fabs(shoulder_rotation)<10 && 
		              fabs(handL.x - shoulderL.x)<0.50 && fabs(handR.x - shoulderR.x)<0.50;
}

void Skeleton::GetShoulderWidth() {
	int oldShWidth = 0;
	if(bInStandardPose) {
		oldShWidth = shoulder_width;
		shoulder_width = 0.8*shoulder_width + 0.2*sqrt	((	shoulderL.x-shoulderR.x	) * ( shoulderL.x-shoulderR.x )+
		(	shoulderL.y-shoulderR.y	) * ( shoulderL.y-shoulderR.y )+
		(	shoulderL.z-shoulderR.z	) *	( shoulderL.z-shoulderR.z ));
	}	
//	if(shoulder_width<oldShWidth*0.6)
	//	shoulder_width = oldShWidth;
}


void Skeleton::GetBodyMeasurements(bool bUserChanged)
{
	if(bUserChanged) 
	{
		bUpdateMeasurements = true;
		indexForBM = 0;
	}

	if(status ==0) return;
	if(abs(shoulderC.x) > 0.1) return; //ensure that user is in camera line 

	//	if(bUpdateMeasurements)
	//	{
	//shoulder width
	double dist = sqrt	((	shoulderL.x-shoulderR.x	) * ( shoulderL.x-shoulderR.x )+
		(	shoulderL.y-shoulderR.y	) * ( shoulderL.y-shoulderR.y )+
		(	shoulderL.z-shoulderR.z	) *	( shoulderL.z-shoulderR.z ));
	shoulder_width = shoulder_width * MAX_DATA_LENGTH - shoulder_width_array[indexForBM];
	shoulder_width_array[indexForBM] = dist;
	shoulder_width += shoulder_width_array[indexForBM];
	shoulder_width /= MAX_DATA_LENGTH;

	//shoulder to hip distance
	dist =	shoulderC.y - hipC.y;
	shoulder_hip_dist = shoulder_hip_dist * MAX_DATA_LENGTH - shoulder_hip_dist_array[indexForBM];				
	shoulder_hip_dist_array[indexForBM] = dist;
	shoulder_hip_dist += shoulder_hip_dist_array[indexForBM];
	shoulder_hip_dist /= MAX_DATA_LENGTH;

	//height
	dist =	1.72+head.y - COM.z * sin(12*CV_PI/180);
	/*
	height = height * MAX_DATA_LENGTH - height_array[indexForBM];				
	height_array[indexForBM] = dist;
	height += height_array[indexForBM];
	height /= MAX_DATA_LENGTH;
	*/
	indexForBM = (indexForBM+1) % MAX_DATA_LENGTH;
	if(indexForBM == MAX_DATA_LENGTH) bUpdateMeasurements = false;
	//	}

	//shoulder rotation;
	double dy = shoulderR.z - shoulderL.z;
	double dx = shoulderR.x - shoulderL.x;
	shoulder_rotation = atan2(dy, dx) * 180 / CV_PI;



}
