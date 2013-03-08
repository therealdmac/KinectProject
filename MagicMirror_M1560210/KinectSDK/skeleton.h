#ifndef STRUCT_SKELETON
#define STRUCT_SKELETON
#include <NuiApi.h>
 
struct vector6
{
	float x, y, z; //3d positions wrt camera
	int status;		//0: not tracked; 1: inferred; 2: tracked
	int u, v;		//screen coordinates 
};
 
class Skeleton
{
public:
	vector6 shoulderL, shoulderR, shoulderC;
	vector6 hipL, hipR, hipC;
	vector6 elbowL, elbowR;//
	vector6 footL, footR;//
	vector6 wristL, wristR;//
	vector6 handL, handR;
	vector6 kneeL, kneeR;//
	vector6 ankleL, ankleR;//
	vector6 spine;//
	vector6 head;
	vector6 COM;
	Vector4 floorPlane;

	long cameraTiltAngle;
	 
	bool status, bInStandardPose;

	double shoulder_width;
	double shoulder_rotation;
	double shoulder_hip_dist;
	double height;
	double neck_height;
	double neck_offsetL, neck_offsetR;

	Skeleton();
	void Update(bool bUserChanged);

	void GetShoulderWidth();
private:

	static const int MAX_DATA_LENGTH = 20;
	int indexForBM;
	double shoulder_width_array[MAX_DATA_LENGTH], shoulder_hip_dist_array[MAX_DATA_LENGTH], height_array[MAX_DATA_LENGTH];
	bool bUpdateMeasurements;
	void GetBodyMeasurements(bool bUserChanged);
	void CheckPose();
};

#endif