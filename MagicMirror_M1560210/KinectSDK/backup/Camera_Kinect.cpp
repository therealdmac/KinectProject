#include "StdAfx.h"
  
//-----------------------------------------------------------------------------------------------
CameraKinect::CameraKinect(BYTE *color_buf, 
	BYTE *depth_buf, BYTE *user_buf, Skeleton* skeleton)
	//------------------------------------------------------------------------------------------------
{
	//Structures are declared in main code. Here, just pointers are copied.
	cBuf = color_buf;
	dBuf = depth_buf;
	uBuf = user_buf;
	skel = skeleton;

	//automatically change the values of depth image resolution based on config.h 
	dImageResX = DEPTH_RES_X;
	dImageResY = DEPTH_RES_Y;
	if(dImageResX == 320 && dImageResY == 240) dImageRes = NUI_IMAGE_RESOLUTION_320x240;
	else if(dImageResX == 640 && dImageResY == 480) dImageRes = NUI_IMAGE_RESOLUTION_640x480;

	realDepth = new USHORT[dImageResX * dImageResY];
	uIdx = new BYTE[dImageResX * dImageResY];

	//automatically change the values of color image resolution based on config.h 
	cImageResX = COLOR_RES_X;
	cImageResY = COLOR_RES_Y;
	if(cImageResX == 320 && cImageResY == 240)  cImageRes = NUI_IMAGE_RESOLUTION_320x240;
	else if(cImageResX == 640 && cImageResY == 480)	cImageRes = NUI_IMAGE_RESOLUTION_640x480;
	else if(cImageResX == 1280&& cImageResY == 960) cImageRes = NUI_IMAGE_RESOLUTION_1280x960;

	bFoundSkeleton = false;			//status of skeleton
	currentUser = prevUser = -1;	// -1 means no user	
	bUserChanged = false;

	t_start = GetTickCount();	//for frame rate calculation
	nFrames = 0;				//for frame rate calculation
	
	floorPlane.x = 0.085814685;
	floorPlane.y = 0.96914744;
	floorPlane.z = -0.23106116;
	floorPlane.w = 1.52;

	//initialize the camera
	Init(); 
}

//------------------------------------------------------------------------
CameraKinect::~CameraKinect(void)
//-------------------------------------------------------------------------
{
	UnInit();
}

//--------------------------------------------------------------------------
void CameraKinect::UnInit( )
//--------------------------------------------------------------------------
{
	if ( m_pNuiSensor )
	{
		m_pNuiSensor->NuiShutdown( );
		m_pNuiSensor->Release();
		m_pNuiSensor = NULL;
	}

	m_hNextSkeletonEvent = NULL;
	m_hNextDepthFrameEvent = NULL;
	m_hNextColorFrameEvent = NULL;

	delete[] uIdx;
	delete[] realDepth;
}

//-------------------------------------------------------------------
bool CameraKinect::Init( )
	//--------------------------------------------------------------------------
{
	//create the events. Note that these are optional and NULL pointers can be passed.
	m_hNextDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hNextColorFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hNextSkeletonEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

	HRESULT hr = NuiCreateSensorByIndex(0, &m_pNuiSensor);
	if ( FAILED( hr ) ) return false;

	m_instanceId = m_pNuiSensor->NuiDeviceConnectionId();  

	// initialise the camera
	DWORD nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON |  NUI_INITIALIZE_FLAG_USES_COLOR;
	hr = m_pNuiSensor->NuiInitialize( nuiFlags );
	if ( E_NUI_SKELETAL_ENGINE_BUSY == hr )
	{
		nuiFlags = NUI_INITIALIZE_FLAG_USES_DEPTH |  NUI_INITIALIZE_FLAG_USES_COLOR;
		hr = m_pNuiSensor->NuiInitialize( nuiFlags) ;
	}
	if ( FAILED( hr ) ) return false;

	GetTiltAngle();

	//Enable Skeleton Tracking
	//parametes are a handle (set when data is ready) and flags (to control tracking). 
	//Both can be zero if not needed.ameters:
	if ( HasSkeletalEngine( m_pNuiSensor ) )
	{
		hr = m_pNuiSensor->NuiSkeletonTrackingEnable( m_hNextSkeletonEvent, 0 );
		if( FAILED( hr ) ) return false;
	}

	//Create a color and a depth stream
	/*NuiImageStreamOpen has the following parameters:
	The type of image, which is a value of the NUI_IMAGE_TYPE enumeration.
	The requested image resolution, which is a value of the NUI_IMAGE_RESOLUTION enumeration.
	A set of flags that control image preprocessing. If unused, set this to 0.
	The maximum number of lookahead buffers that the application can hold concurrently. This application uses two buffers so the application can draw with one buffer while capturing data to the other buffer.
	A handle to an event that the runtime sets when the next frame is available.
	A location to receive a handle to the open stream.*/

	hr = m_pNuiSensor->NuiImageStreamOpen(
		NUI_IMAGE_TYPE_COLOR,
		cImageRes, //NUI_IMAGE_RESOLUTION_640x480,
		0,
		2,
		m_hNextColorFrameEvent,
		&m_pVideoStreamHandle );

	if( FAILED( hr ) ) return false;

	hr = m_pNuiSensor->NuiImageStreamOpen(
		HasSkeletalEngine(m_pNuiSensor) ? NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX : NUI_IMAGE_TYPE_DEPTH,
		dImageRes, 0, 2, m_hNextDepthFrameEvent, &m_pDepthStreamHandle );

	if( FAILED( hr ) ) return false;

	return true;
}

//-----------------------------------------------------------------------------------------------
void CameraKinect::Update(DWORD* pColorBuffer)
	//-----------------------------------------------------------------------------------------------
{
	// There can be two approaches.
	// First: to capture data when one or all events are triggered.
	// Second: not to use events and keep getting data in sequence in a loop. This will have high frame rate, 
	// but we cannot be sure if a color and depth image and skeleton are synchronised or are from different view.

	// this is the first approach.
	//const int numEvents = 3;
	//HANDLE hEvents[numEvents] = { m_hNextDepthFrameEvent, m_hNextColorFrameEvent, m_hNextSkeletonEvent };
	//int nEventIdx = WaitForMultipleObjects( numEvents, hEvents, TRUE, 100 );
	//if(nEventIdx == WAIT_TIMEOUT) return;

	//once all the events are triggered, we get data sequentially.
	//if we do not wait for events, frame rate gets doubled.
	//if we wait for just one trigger, frame rate is 1.5 times.
	//some tricks might be played here to increase frame rate without missing a frame.

	bool s;
	if(pColorBuffer == NULL)
		s = GetColorFrame();
	else 
		s = GetColorFrame(pColorBuffer);

	if(s) s = GetDepthFrame(false);
	if(s) s = GetSkeletonFrame();
	if(!s) skel->status = false;

	SegmentUser();

	if(currentUser != prevUser) bUserChanged = true;
	else bUserChanged = false;
	prevUser = currentUser;
}


//------------------------------------------------------------------
bool CameraKinect::GetColorFrame(DWORD* pBuffer)
//-------------------------------------------------------------------
{
	int nEventIdx = WaitForSingleObject(m_hNextColorFrameEvent, 100 );
	if(nEventIdx == WAIT_TIMEOUT) return false;

	HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame( m_pVideoStreamHandle, 0, &cImageFrame );
	if ( FAILED( hr ) )	return false;

	INuiFrameTexture * pTexture = cImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );


	if ( LockedRect.Pitch != 0 ) //pitch is no of bytes in a row
	{
		//data needs to copied here. pointer cannot be passed
		//because when NuiStream is released, the pointer is no more there
		//other ways besides the code below could be:
		if(pBuffer == NULL)
			memcpy(cBuf, LockedRect.pBits, cImageResX * cImageResY * 4);
		else
		memcpy(pBuffer, LockedRect.pBits, cImageResX * cImageResY * 4);
	}

	pTexture->UnlockRect( 0 );

	//releasing the stream is a must, otherwise it gives an error
	m_pNuiSensor->NuiImageStreamReleaseFrame( m_pVideoStreamHandle, &cImageFrame );
	return true;
}

//----------------------------------------------------------------------------
bool CameraKinect::GetDepthFrame( bool bWait )
	//depth image of whole scene and user only are obtained here.
	//also we get user buffer uBuf aligned to color image cBuf
//-----------------------------------------------------------------------------
{
	if(bWait)
	{
		int nEventIdx = WaitForSingleObject(m_hNextDepthFrameEvent, 100 );
		if(nEventIdx == WAIT_TIMEOUT) return false;
	}

	HRESULT hr = m_pNuiSensor->NuiImageStreamGetNextFrame(m_pDepthStreamHandle, 0, &dImageFrame );
	if ( FAILED( hr ) )  return false;

	INuiFrameTexture * pTexture = dImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );

	if ( 0 != LockedRect.Pitch ) //pitch is no of bytes in a row
	{
		USHORT* pBuff = (USHORT*) LockedRect.pBits;

		memset(uBuf, 0, dImageResY * dImageResX);

		for(long j=0; j<dImageResY; j++)
			for(long i=0; i<dImageResX; i++)
			{
				long k = j * dImageResX + i;

				//first 3 bits have user index in 1-6 range
				uIdx[k] = pBuff[k] & 0x07;			

				//shift right to get next 13 bits which have depth
				//USHORT realDepth = (pBuff[k] & 0xFFF8) >> 3;
				realDepth[k] = (pBuff[k] & 0xFFF8) >> 3;

				//divide by max and invert color, and set depth buffer in 0-255 range
				dBuf[k] = 255 - (BYTE)(256*realDepth[k]/0x0fff);
			}
	}
	pTexture->UnlockRect( 0 );
	m_pNuiSensor->NuiImageStreamReleaseFrame( m_pDepthStreamHandle, &dImageFrame );
	return true;
}


//------------------------------------------------------
void CameraKinect::SegmentUser()
//segments the user pixels. As a result uBuf will have
//depth values of user pixels. Pixel indices are
//transformed to align with color image.
//------------------------------------------------------
{
	memset(uBuf, 0, dImageResX * dImageResY);
	for(long j=0; j<dImageResY; j++)
	for(long i=0; i<dImageResX; i++)
	{
	
		long k = j * dImageResX + i;
		if(uIdx[k] != currentUser+1) continue;
		uBuf[k] = dBuf[k];
#if 0
		//below is if we want to make uBuf aligned to cImg
		//Next we set user buffer. The user pixels get depth value while rest are zero. 
		//The pixels are transformed to align the color image
		//till SDK1.0, transformation works only for 640x480 color and 320x240 depth images
		//div by 2 in kk is to handle different resolutions of color and depth images
		long ii, jj;
		NuiImageGetColorPixelCoordinatesFromDepthPixel(NUI_IMAGE_RESOLUTION_640x480, 0, i, j, realDepth[k]<<3, &ii, &jj);
		if(ii<0||ii>=640||jj<0||jj>=480) continue;
		long kk = jj/2 * dImageResX + ii/2; 
		uBuf[kk] = dBuf[k];
#endif
	}
}

//-----------------------------------------------------------------
bool CameraKinect::GetSkeletonFrame(  )
	//-----------------------------------------------------------------
{

	HRESULT hr;
	hr= m_pNuiSensor->NuiSkeletonGetNextFrame( 0, &SkeletonFrame );
	
	if ( FAILED( hr ) )  return false;

	// update current user to one nearest to the camera center
	float minX = 1000000.0;
	currentUser = -1;
	for( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
	{
		if( SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ||
			SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY)
		{
			//bFoundSkeleton = true;

			if(fabs(SkeletonFrame.SkeletonData[i].Position.x)<=minX)
			{
				minX=fabs(SkeletonFrame.SkeletonData[i].Position.x);
				currentUser = i;
			}
		}

	}

	if(currentUser > -1)
	{
		// smooth out the skeleton data
		hr = m_pNuiSensor->NuiTransformSmooth(&SkeletonFrame,NULL);
		if(FAILED(hr))  return false;

		//ProcessSkeleton();
//		skel->SkeletonFrame = &SkeletonFrame;

		skel->status = true;
		UpdateSkeleton();

		//get the bone orientations
		NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
		NuiSkeletonCalculateBoneOrientations(&SkeletonFrame.SkeletonData[currentUser], boneOrientations);
		for (int j=0; j<NUI_SKELETON_POSITION_COUNT; j++) 
			{
				NUI_SKELETON_POSITION_TRACKING_STATE trackingState = SkeletonFrame.SkeletonData[currentUser].eSkeletonPositionTrackingState[j];
				if (NUI_SKELETON_POSITION_TRACKED == trackingState)
				{ 
					//only update the points/orientations which are currently tracked
					const Vector4 p = SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[j];
					jointPositions[j] = glm::vec3(p.x, p.y, -p.z);

					NUI_SKELETON_BONE_ORIENTATION & orientation = boneOrientations[j];				
					//Matrix4& m =  orientation.absoluteRotation.rotationMatrix;
					Matrix4& m =  orientation.hierarchicalRotation.rotationMatrix;
					memcpy(glm::value_ptr(jointOrientations[j]),&m.M11, sizeof(float)*16);
					 

					glm::quat&  tmp = jointQuaternions[orientation.endJoint]; 
					const Vector4 pQuat = orientation.hierarchicalRotation.rotationQuaternion;
					tmp.x = pQuat.x;
					tmp.y = pQuat.y;
					tmp.z = pQuat.z;
					tmp.w = pQuat.w; 
				}  
			}

		//return true;
	}
	else skel->status = false;

	if(SkeletonFrame.vFloorClipPlane.w > 0)
	{
		double alpha = 0.9;
		floorPlane.x = alpha * floorPlane.x + (1-alpha) * SkeletonFrame.vFloorClipPlane.x;
		floorPlane.y = alpha * floorPlane.y + (1-alpha) * SkeletonFrame.vFloorClipPlane.y;
		floorPlane.z = alpha * floorPlane.z + (1-alpha) * SkeletonFrame.vFloorClipPlane.z;
		//floorPlane.w = alpha * floorPlane.w + (1-alpha) * SkeletonFrame.vFloorClipPlane.w;
		skel->floorPlane = floorPlane;
	}

	return skel->status;
}

//----------------------------------------------------------------------------
void CameraKinect::UpdateSkeleton()
	//----------------------------------------------------------------------------
{
	//Any required mappings can be made here from Kinect's native skeleton to user defined skeleton.

	CopySkel(&skel->hipC,		NUI_SKELETON_POSITION_HIP_CENTER);
	CopySkel(&skel->hipL,		NUI_SKELETON_POSITION_HIP_LEFT);
	CopySkel(&skel->hipR,		NUI_SKELETON_POSITION_HIP_RIGHT);

	CopySkel(&skel->shoulderC,	NUI_SKELETON_POSITION_SHOULDER_CENTER);
	CopySkel(&skel->shoulderL,	NUI_SKELETON_POSITION_SHOULDER_LEFT);
	CopySkel(&skel->shoulderR,	NUI_SKELETON_POSITION_SHOULDER_RIGHT);

	//	CopySkel(&skel->elbowL,		NUI_SKELETON_POSITION_ELBOW_LEFT);
	//	CopySkel(&skel->elbowR,		NUI_SKELETON_POSITION_ELBOW_RIGHT);
	CopySkel(&skel->handL,		NUI_SKELETON_POSITION_HAND_LEFT);
	CopySkel(&skel->handR,		NUI_SKELETON_POSITION_HAND_RIGHT);
	//	CopySkel(&skel->footL,		NUI_SKELETON_POSITION_FOOT_LEFT);
	//	CopySkel(&skel->footR,		NUI_SKELETON_POSITION_FOOT_RIGHT);
	//	CopySkel(&skel->ankleL,		NUI_SKELETON_POSITION_ANKLE_LEFT);
	//	CopySkel(&skel->ankleR,		NUI_SKELETON_POSITION_ANKLE_RIGHT);
	//	CopySkel(&skel->kneeL,		NUI_SKELETON_POSITION_KNEE_LEFT);
	//	CopySkel(&skel->kneeR,		NUI_SKELETON_POSITION_KNEE_RIGHT);
	//	CopySkel(&skel->wristL,		NUI_SKELETON_POSITION_WRIST_LEFT);
	//	CopySkel(&skel->wristR,		NUI_SKELETON_POSITION_WRIST_RIGHT);
	CopySkel(&skel->head,		NUI_SKELETON_POSITION_HEAD);
	//	CopySkel(&skel->spine,		NUI_SKELETON_POSITION_SPINE);

	skel->COM.x = SkeletonFrame.SkeletonData[currentUser].Position.x;
	skel->COM.y = SkeletonFrame.SkeletonData[currentUser].Position.y;
	skel->COM.z = SkeletonFrame.SkeletonData[currentUser].Position.z;
	CameraToImageCoord(&skel->COM, &(SkeletonFrame.SkeletonData[currentUser].Position));
}

//----------------------------------------------------------------------------------------------------------
void CameraKinect::CopySkel(vector6 *dst, NUI_SKELETON_POSITION_INDEX idx)
//----------------------------------------------------------------------------------------------------------
{
	//Copy 3D coordinates, 2D coordinates and status (2:tracked 1:inferred 0:not tracked).
	dst->x = SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[idx].x;
	dst->y = SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[idx].y;
	dst->z = SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[idx].z;
	dst->status = SkeletonFrame.SkeletonData[currentUser].eSkeletonPositionTrackingState[idx];

	CameraToImageCoord(dst, &(SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[idx]));
}

//----------------------------------------------------------------------------------------------------------
void CameraKinect::CameraToImageCoord(vector6 *dst, Vector4 *src)
//----------------------------------------------------------------------------------------------------------
{
	float x, y;
	NuiTransformSkeletonToDepthImage(*src, &x, &y);

	dst->u = (int) (x+0.5);
	dst->v = (int) (y+0.5);

	if(dst->u < 0 ||  dst->u > 320 || dst->v < 0 || dst->v > 240) return;

	long ii, jj;
	long k = dst->v * dImageResX + dst->u;
	//dst->z = realDepth[k];

	//note that if we use dst->z instead of realDepth[k], the result is not correct in SDK1.0. 
	//This needs to be reported to Microsoft. 
	//For now we resolve it by using realDepth calculated in GetDepthFrame().
	NuiImageGetColorPixelCoordinatesFromDepthPixel(NUI_IMAGE_RESOLUTION_640x480, 0, dst->u, dst->v, realDepth[k]<<3, &ii, &jj);
	//printf("%d\t%d\t%d\t%d\n", dst->u, dst->v, ii/2, jj/2);
	if(!(ii<0||ii>640||jj<0||jj>480)) {dst->u = ii/2; dst->v = jj/2;}

	// adjustment of skeleton for higher resolution depth image
	if(dImageResX == 640 && dImageResY == 480){ dst->u *= 2; dst->v *= 2;}
}

//---------------------------------------------------------------------------------------------
void CameraKinect::ProcessSkeleton()
//--------------------------------------------------------------------------------------------
{
	//any smoothing of skeleton will be done here

	//for example ankle should always be below the knee etc. this can be ensured here.
	/*Vector4 *ankleL = &(SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT]);
	Vector4 *kneeL = &(SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT]);
	Vector4 *ankleR = &(SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT]);
	Vector4 *kneeR = &(SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT]);*/
}

//---------------------------------------------------------------------------------------------
double CameraKinect::GetFrameRate()
//----------------------------------------------------------------------------------------------
{
	//frame rate can be based on fixed time span or fixed number of frames.
	//the following code prints once per second
	nFrames++;
	DWORD t_end = GetTickCount();
	double t_elapsed = t_end - t_start;

	if(t_elapsed >= 5000)
	{		
		double fps = (double) nFrames / t_elapsed * 1000;
		t_start = t_end;
		nFrames = 0;
		printf("frame rate is %lg\n", fps);
		return fps;
	}
	return -1;	
}

//Added by Mobeen
const long CameraKinect::GetTiltAngle() const {
	NuiCameraElevationGetAngle(&skel->cameraTiltAngle);
	return skel->cameraTiltAngle;
}

//---------------------------------------------------------------------------------------------
void CameraKinect::GetNeckOffset()
//----------------------------------------------------------------------------------------------
{
	if(!skel->status) return;
	if(!skel->bInStandardPose) return;
	
	long u=0, v=0, v_neck=0;
	USHORT d;
	int THRESH = 10;

	//get shoulder posaition in screen coord
	NuiTransformSkeletonToDepthImage((SkeletonFrame.SkeletonData[currentUser].SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT]), 
		&u, &v, &d, dImageRes);
	
	long k = v * dImageResX + u;
	BYTE depth_Value = uBuf[k];

	//find a point on shoulder line in screen coord
	//for(v_neck=v; v_neck>skel->head.v; v_neck--)
	for(v_neck=v; v-20; v_neck--)
	{
		long k = v_neck * dImageResX + u;
		if(abs(uBuf[k]-depth_Value) > THRESH) break;
		//if(uBuf[k] == 0) break;
	}
	/*
	long cX, cY;
	NuiImageGetColorPixelCoordinatesFromDepthPixel(cImageRes, NULL, u, v_neck, 
										realDepth[(v_neck+2)*dImageResX+u]<<3, &cX,&cY);
	for(int jj=cY-3;jj<cY+3;jj++){
	for(int ii=cX-3;ii<cX+3;ii++){
	cBuf[4*(jj*cImageResX+ii)] = 255;
	cBuf[4*(jj*cImageResX+ii)+1] = 0;
	cBuf[4*(jj*cImageResX+ii)+2] = 0;}}
	*/
	//get 3D positions for the point 
	Vector4 V = NuiTransformDepthImageToSkeleton(u, v_neck, realDepth[(v_neck+2)*dImageResX+u]<<3);
	
	//get offset from shoulder
	double offset = fabs(skel->shoulderL.y-V.y);

	//filter the offset
	if(fabs(offset-skel->neck_offsetL ) > 0.01f)
		skel->neck_offsetL = 0.9*skel->neck_offsetL + 0.1*offset;

	//for right
	NuiTransformSkeletonToDepthImage((SkeletonFrame.SkeletonData[currentUser].
		SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT]), &u, &v, &d, dImageRes);	
	k = v * dImageResX + u;
	depth_Value = uBuf[k];
	//for(v_neck=v; v_neck>skel->head.v; v_neck--)
	for(v_neck=v; v-20; v_neck--)
	{
		long k = v_neck * dImageResX + u;
		if(abs(uBuf[k]-depth_Value) > THRESH) break;
		//if(uBuf[k] == 0) break;
	}
	/*
	NuiImageGetColorPixelCoordinatesFromDepthPixel(cImageRes, NULL, u, v_neck, 
										realDepth[(v_neck+2)*dImageResX+u]<<3, &cX,&cY);
	for(int jj=cY-3;jj<cY+3;jj++){
	for(int ii=cX-3;ii<cX+3;ii++){
	cBuf[4*(jj*cImageResX+ii)] = 255;
	cBuf[4*(jj*cImageResX+ii)+1] = 0;
	cBuf[4*(jj*cImageResX+ii)+2] = 0;}}
	*/
	V = NuiTransformDepthImageToSkeleton(u, v_neck, realDepth[(v_neck+2)*dImageResX+u]<<3);	
	offset = fabs(skel->shoulderR.y-V.y);
	if(fabs(offset-skel->neck_offsetR ) > 0.01f)
		skel->neck_offsetR = 0.9*skel->neck_offsetR + 0.1*offset;
}

#if 0
//---------------------------------------------------------------------------------------------
void CameraKinect::Calibrate()
//----------------------------------------------------------------------------------------------
{
	int code;
	int image_count = 10;
	int point_count = 6;
	CvMat* image_points = cvCreateMat( image_count*point_count, 2, CV_32FC1 );
	CvMat* object_points = cvCreateMat(image_count*point_count, 3, CV_32FC1 );
	CvMat* point_counts = cvCreateMat( image_count, 1, CV_32SC1 );
	CvSize size = cvSize(640, 480);
	CvMat *camera_matrix = cvCreateMat(3, 3, CV_32F);
	CvMat *dist_coeffs = cvCreateMat(5, 1, CV_32F);

	for(int i=0; i<image_count*point_count; i+=point_count)
	{
		while(!skel->status)
		Update();
		
		cvSetReal2D(object_points, i, 0, skel->shoulderL.x);
		cvSetReal2D(object_points, i, 1, skel->shoulderL.y);
		cvSetReal2D(object_points, i, 2, 0);
		cvSetReal2D(image_points, i, 0, 2*skel->shoulderL.u);
		cvSetReal2D(image_points, i, 1, 2*skel->shoulderL.v);

		cvSetReal2D(object_points, i+1, 0, skel->shoulderR.x);
		cvSetReal2D(object_points, i+1, 1, skel->shoulderR.y);
		cvSetReal2D(object_points, i+1, 2, 0);
		cvSetReal2D(image_points, i+1, 0, 2*skel->shoulderR.u);
		cvSetReal2D(image_points, i+1, 1, 2*skel->shoulderR.v);

		cvSetReal2D(object_points, i+2, 0, skel->hipL.x);
		cvSetReal2D(object_points, i+2, 1, skel->hipL.y);
		cvSetReal2D(object_points, i+2, 2, 0);
		cvSetReal2D(image_points, i+2, 0, 2*skel->hipL.u);
		cvSetReal2D(image_points, i+2, 1, 2*skel->hipL.v);

		cvSetReal2D(object_points, i+3, 0, skel->hipR.x);
		cvSetReal2D(object_points, i+3, 1, skel->hipR.y);
		cvSetReal2D(object_points, i+3, 2, 0);
		cvSetReal2D(image_points, i+3, 0, 2*skel->hipR.u);
		cvSetReal2D(image_points, i+3, 1, 2*skel->hipR.v);		

		cvSetReal2D(object_points, i+4, 0, skel->hipC.x);
		cvSetReal2D(object_points, i+4, 1, skel->hipC.y);
		cvSetReal2D(object_points, i+4, 2, 0);
		cvSetReal2D(image_points, i+4, 0, 2*skel->hipC.u);
		cvSetReal2D(image_points, i+4, 1, 2*skel->hipC.v);

		cvSetReal2D(object_points, i+5, 0, skel->head.x);
		cvSetReal2D(object_points, i+5, 1, skel->head.y);
		cvSetReal2D(object_points, i+5, 2, 0);
		cvSetReal2D(image_points, i+5, 0, 2*skel->head.u);
		cvSetReal2D(image_points, i+5, 1, 2*skel->head.v);
	}

 	cvSet( point_counts, cvScalar(point_count) );
 	
	cvZero( camera_matrix );
	cvZero( dist_coeffs );

	cvSetReal2D(camera_matrix, 0, 0, 500);
	cvSetReal2D(camera_matrix, 1, 1, 500);
	cvSetReal2D(camera_matrix, 0, 2, 320);
	cvSetReal2D(camera_matrix, 1, 2, 240);
	cvSetReal2D(camera_matrix, 2, 2, 1);

	 
	try {
		cvCalibrateCamera2(object_points, image_points, point_counts, size, camera_matrix, dist_coeffs, 
	0, 0, 0);
	}
	catch(cv::Exception e) {
		printf("Error: %s\n", e.what());
	}

	printf("camera_matrix %g\t%g\n", camera_matrix->data.db[0], camera_matrix->data.db[4]);
}

//---------------------------------------------------------------------------------------------
void CameraKinect::GetPointCloud(IplImage *uImg, vector3d *PointCloud)
//----------------------------------------------------------------------------------------------
{
	
	IplImage *temp = cvCreateImage(cvSize(dImageResX, dImageResY), 8, 1);

	for(long j=0; j<dImageResY; j++)
	for(long i=0; i<dImageResX; i++)
	{
		long k = j * dImageResX + i;
		temp->imageData[k] = 255;

		int d = 1;
		for(int m=j-d; m<j+d; m++)
		for(int n=i-d; n<i+d; n++)
		{
			if((m<0||m>=dImageResY||n<0||n>dImageResX)) continue;
			int k1 = m * dImageResX + n;
			if(uBuf[k1]==0) {temp->imageData[k]=0; continue;}
		}
	}

	cvDilate(temp, temp, 0, 10);
	cvErode (temp, temp, 0, 10);

	for(long j=0; j<dImageResY; j++)
	for(long i=0; i<dImageResX; i++)
	{
		long k = j * dImageResX + i;
		if(temp->imageData[k]==0)uBuf[k]=0;
	}
//	cvShowImage("temp", temp);
	cvReleaseImage(&temp);

	for(long j=0; j<dImageResY; j++)
	for(long i=0; i<dImageResX; i++)
	{
		long k = j * dImageResX + i;
		PointCloud[k].x = PointCloud[k].y = PointCloud[k].z = 0;
	}

	for(long j=0; j<dImageResY; j++)
		for(long i=0; i<dImageResX; i++)
		{
			long k = j * dImageResX + i;

			long ii, jj;
			NuiImageGetColorPixelCoordinatesFromDepthPixel(NUI_IMAGE_RESOLUTION_640x480, NULL, i, j, realDepth[k]<<3, &ii,&jj);
			if((ii<0||ii>640||jj<0||jj>480)) continue;
			ii = ii/2; jj = jj/2;
			long kk = jj * dImageResX + ii;
			if(uBuf[kk] == 0)continue;

			Vector4 v = NuiTransformDepthImageToSkeleton(i, j, realDepth[k]<<3);
			PointCloud[kk].x = v.x;
			PointCloud[kk].y = v.y;
			PointCloud[kk].z = v.z;
		}
}
 
//--------------------------------------------------------
double CameraKinect::GetNeckLine()
//---------------------------------------------------------
{
	IplImage *temp = cvCreateImage(cvSize(dImageResX, dImageResY), 8, 1);
	cvSet(temp, cvScalar(0));

	//get a few depth frames to get a cleaner segmentation
	int nFrames = 10;
	for(int itr=0; itr<nFrames; itr++)
	{
		if(!GetDepthFrame()) continue;
	//	memcpy(temp->imageData, dBuf, dImageResX * dImageResY);
		for(long j=0; j<dImageResY; j++)
		{
			for(long i=0; i<dImageResX; i++)
			{
				long k = j * dImageResX + i;
				if(uIdx[k] == currentUser+1) temp->imageData[k]++;
			}
		}
	}

	//Remove the pixels which don't appear in most of the frames
	cvThreshold(temp, temp, nFrames-1, 255, CV_THRESH_BINARY);
	//cvSaveImage("test.jpg", temp);

	//get the skeleton, to be used to estimate vertical boundaries of shoulder-neck line
	GetSkeletonFrame();

	//following is the main algorithm to estimate shoulder-neck boundary
	int idx[320];		//keeps indices of non-zero pixels along a row
	double sd[240];		//standard deviation of non-zero pixels along a row

	int j_start = skel->shoulderC.v;	//lower boundary
	int j_end = skel->head.v;			//upper boundary
	
	for(int j = j_start; j > j_end; j--)
	{
		double sum = 0;
		int count = 0;
		sd[j_start-j] = 0;

		//get indices of non-zero pixels along jth row and their count
		for(int i=0; i<dImageResX; i++)
		{
			if(temp->imageData[j * dImageResX + i] !=0) 
			{
				idx[count] = i;
				sum += i;
				count ++;
			}
		}	

		//average of non-zero pixels along jth row
		double avg = 0;
		if(count != 0) avg = sum / count;  

		// standard deviation (actually variance) of indices of non-zero pixels along jth row
		for(int i=0; i<count; i++)
		{
			sd[j_start-j] += (avg-idx[i])*(avg-idx[i]);
		}
		if(count != 0) sd[j_start-j] /= count;
	}


	//estimate row of neck-shoulder joint, where variance drops significantly
	int j_neck = j_start;
	for(int j = j_start; j > j_end; j--)
	{
		if(sd[j_start-j]/sd[0] < 0.5)
		{
			j_neck = j;
			break;
		}
	}

	

	//cvLine(temp, cvPoint(0, j_neck), cvPoint(dImageResX-1, j_neck), cvScalar(255,255,255));
	//cvLine(temp, cvPoint(0, j_end), cvPoint(dImageResX-1, j_end), cvScalar(255,255,255));
	//cvLine(temp, cvPoint(0, j_start), cvPoint(dImageResX-1, j_start), cvScalar(255,255,255));
	//cvShowImage("temp", temp);
	//cvWaitKey(1);
	cvReleaseImage(&temp);

	if(skel->status == 0) return -1;

	//get the depth at center of estimated shoulder-neck line
	int i_neck = skel->head.u;
	int k = j_neck*dImageResX + i_neck;
	USHORT dpth = (realDepth[k]+realDepth[k+1]+realDepth[k-1])/3;

	//transform to skeleton space
	Vector4 v = NuiTransformDepthImageToSkeleton(i_neck, j_neck, dpth<<3);

	//get height from floor
	skel->neck_height = floorPlane.x*v.x + floorPlane.y*v.y +	floorPlane.z*v.z + floorPlane.w;
	float shoulder_height = floorPlane.x*skel->shoulderL.x + floorPlane.y*skel->shoulderL.y +	floorPlane.z*skel->shoulderL.z + floorPlane.w;
	//delta = skel->neck_height-shoulder_height+fOffset;
	//delta = delta * cos(CV_PI/180*11);
	/*
	double r = double(j_neck-j_start)/(j_end-j_start);
	skel->neck_height = skel->shoulderC.y + (skel->head.y-skel->shoulderC.y) * r;
	*/
	//skel->neck_height = floorPlane.w ;//+ v.y * cos(CV_PI/180.0*GetTiltAngle()) + v.z * sin(CV_PI/180.0*GetTiltAngle());
	printf("neck height is = %g\n", skel->neck_height);
	return skel->neck_height;
}
#endif
