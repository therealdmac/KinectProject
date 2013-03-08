#pragma once
#include <Windows.h>
#include <NuiApi.h>
#include <cv.h>
#include "stdafx.h"
#include "skeleton.h"
#include "GestureController.h"

#ifndef dlsfjskjfsjfskfj
#define dlsfjskjfsjfskfj
struct vector3d
{
	double x, y, z;
};
#endif

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class CameraKinect
{
private:

	INuiSensor*	m_pNuiSensor;
	BSTR m_instanceId;

	//following two handles are for depth and video stream
	HANDLE	m_pDepthStreamHandle;
	HANDLE	m_pVideoStreamHandle;

	//these three handles are for events, which are optional. NULL can be passed instead, and frames can be collected
	//sequentially without waiting for event to occur. This can increase the frame rate but without feedback there
	//is no surity if color & depth images and skeleton are synchronised, i.e, belong to same view, or not.
	HANDLE	m_hNextDepthFrameEvent;
	HANDLE	m_hNextColorFrameEvent;
	HANDLE	m_hNextSkeletonEvent;

	//color, depth and skeleton frames
	NUI_IMAGE_FRAME		cImageFrame;	
	NUI_IMAGE_FRAME		dImageFrame;	
	NUI_SKELETON_FRAME	SkeletonFrame;	

	// image resolution stings and values
	NUI_IMAGE_RESOLUTION	cImageRes;	
	NUI_IMAGE_RESOLUTION	dImageRes;
	int	dImageResX, dImageResY;
	int cImageResX, cImageResY;

	//pointers to color and depth buffers passed in constructor.
	BYTE *cBuf, *dBuf, *uBuf;		
	USHORT *realDepth;	//real depth at current user
	BYTE *uIdx;			//indices of current user

	//skeleton of the user
	Skeleton* skel;

	//high if skeleton in found
	bool bFoundSkeleton;
	
	//used to calculate frame rate
	DWORD t_start;
	int nFrames;
	
	//initialise & release camera
	bool Init();

	//copy from NUI structures to our skeleton structure
	void CopySkel(vector6 *dst, NUI_SKELETON_POSITION_INDEX idx);
	void CameraToImageCoord(vector6 *dst, Vector4 *src);
	void UpdateSkeleton();

	//we can add our ideas in this function to process the raw skeleton.
	void ProcessSkeleton();

	int prevUser;

	

public:
	glm::vec3 jointPositions[NUI_SKELETON_POSITION_COUNT];
	glm::mat4 jointOrientations[NUI_SKELETON_POSITION_COUNT];
	glm::quat jointQuaternions[NUI_SKELETON_POSITION_COUNT];
	
	//gesture controller obj
	GestureController *gesCtrl;

	//gesture event handlers-source and receivers
	GestureReceiver gesRcv;
	GestureSource gesSrc;

	//non-negative if user detected
	int	currentUser;	
	bool bUserChanged;
	Vector4 floorPlane;

	CameraKinect(BYTE *color_buf, BYTE *depth_buf, BYTE *user_buf, Skeleton* skeleton);
	~CameraKinect(void);

	//API for user applications
	void Update(DWORD* pBuffer = NULL);
	double GetFrameRate();
	void UnInit();
	bool GetColorFrame(DWORD* pBuffer = NULL);
	bool GetDepthFrame(bool bWait = true);
	bool GetSkeletonFrame();
	void SegmentUser();
	//void SegmentUserClean();
	double GetNeckLine();

	//Added by Mobeen
	//Returns the tilt angle of the Kinect
	const long GetTiltAngle() const;
	void GetPointCloud(IplImage *uImg, vector3d *v);
	void GetNeckOffset();
};
