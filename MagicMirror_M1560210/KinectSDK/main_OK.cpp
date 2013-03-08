 
#include "stdafx.h" 
#include "SOIL.h"
#include "gui.h"
#include <gl/glew.h> 
#include <gl/wglew.h> 
#include <gl/freeglut.h>
#include <windows.h>

//video player ffmpeg
//extern "C" {
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//}

#include <mmsystem.h> //for playing wav file 
#include <time.h> 
#pragma comment(lib, "glew32")

#include <cassert>
#define CHECK_GL_ERROR assert(glGetError() == GL_NO_ERROR);

char* path = ::getenv("MAGIC_MIRROR_DATA_ROOT");
char newpath[MAX_PATH]={'\0'};

//play video 
//CvCapture* g_Capture;// = "C:/Users/Magic Mirror/Desktop/Data/Pictures/Screensaver.avi";

//color, depth and user images. User image shows depth of main user pixels only 
IplImage* cImg = cvCreateImage(cvSize(COLOR_RES_X, COLOR_RES_Y), IPL_DEPTH_8U, 4); 
IplImage* dImg = cvCreateImage(cvSize(DEPTH_RES_X, DEPTH_RES_Y), IPL_DEPTH_8U, 1);
IplImage* uImg  = cvCreateImage(cvSize(DEPTH_RES_X, DEPTH_RES_Y), IPL_DEPTH_8U, 1);

//skeleton
Skeleton* skel = new Skeleton();

//the camera class; pointers to the buffers and skeleton are passed in the constructor
CameraKinect *camera = new CameraKinect((uchar*)cImg->imageData, 
										(uchar*) dImg->imageData, 
										(uchar*) uImg->imageData, skel);

//MClothing class; pointors to images and skeleton are passed in the constructor
MCloth *mcloth = new MCloth(cImg, dImg, uImg, skel);
MCloth3D *pCloth3D = new MCloth3D(skel, camera);

HANDLE hAudio=NULL;

//added by chong 
LPCWSTR processpath = L"..\\Debug\\ftpupload.exe";//path to the file ftpupload.exe for facebook upload
LPCWSTR processpath1 = L"..\\Debug\\Cutscene.exe";//path to the file ftpupload.exe for facebook upload
static const int NUM_3DCLOTHES = 8; 
const int NUM_BAGS = 3;
const int NUM_DRESSES_MALE = 5;
const int NUM_DRESSES_FEMALE = 11; 


unsigned char* detailsDataFemale[NUM_DRESSES_FEMALE];
unsigned char* detailsDataMale[NUM_DRESSES_MALE];
unsigned char* detailsDataBag[NUM_BAGS];
unsigned char* detailsData3D[NUM_3DCLOTHES];
unsigned char* qrCodeData= 0;
string detailsFemale[] ={"C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF1.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF2.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF3.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF4.png",
					     "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF5.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF6.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF7.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF8.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF9.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF10.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Female/DF11.png"};
string detailsMale[] ={"C:/Users/Magic Mirror/Desktop/Data/Details/Male/DM1.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Male/DM2.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Male/DM3.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Male/DM4.png",
					     "C:/Users/Magic Mirror/Desktop/Data/Details/Male/DM5.png"};
string detailsBag[] ={   "C:/Users/Magic Mirror/Desktop/Data/Details/Bag/DB1.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Bag/DB2.png",
						 "C:/Users/Magic Mirror/Desktop/Data/Details/Bag/DB3.png" };
string details3D[] = {"C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG1.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG2.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG3.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG4.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG5.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG6.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG7.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG8.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG9.png",
					  "C:/Users/Magic Mirror/Desktop/Data/Details/3D/DG10.png",};

string mfilename[]={	"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/M01_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/M02_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/M03_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/M04_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/M05_icon.png"};

string bfilename[] ={	"C:/Users/Magic Mirror/Desktop/Data/Pictures/bag1.png",
						"C:/Users/Magic Mirror/Desktop/Data/Pictures/bag2.png",
						"C:/Users/Magic Mirror/Desktop/Data/Pictures/bag3.png"};

string ffilename[] = {	"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F01_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F02_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F03_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F04_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F05_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F06_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F07_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F08_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F09_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F10_icon.png",
						"C:/Users/Magic Mirror/Desktop/Data/Parco/icons/F11_icon.png"};

string dfilename[]	={	//"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment1.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment2.png",
						//"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment3.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment4.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment5.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment6.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment7.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment8.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment9.png",
						"C:/Users/Magic Mirror/Desktop/Data/Models/Icons/Garment10.png",
									};
int ids3DGarment[]={2,4,5,6,7,8,9,10};

bool bBagIsActive = 0;
bool bShowBag = 0;
bool bShowDress3D = 0;
bool bShowDress2D = 1;
bool ismale = 0;
int garmentid = 1; 
 
//variables for gui
int counter = 0;

//Added by Mobeen - 16 July 2012
GLuint bgTexID;			//background texture ID
GLuint rgbTexID;		//live scene texture ID
GLuint clothTexID;		//cloth texture ID
GLuint accessoryTexID;	//bag or other accessory texture ID
GLuint framesTexID;		//chong 
GLuint detailsTexID;
DWORD* colorBuffer=NULL;//data buffer to store the live RGB scene
GLuint pboID;			//pbo to clear the texture data

//----------------------------------Chong 
//variables for gui
//int counter = 0;
int counter_r = 0;
int counter_s1 = 0;
int counter_s2 = 0;
int counter_s3 = 0;
int counter_s4 = 0;

int counter_b1 = 0;
int counter_b2 = 0;
int counter_b3 = 0;
int counter_b4 = 0;

int counter_bl1 = 0;
int counter_bl2 = 0;
int counter_bl3 = 0;
int counter_bl4 = 0;

int counter_trans = 0;

int counter_sh = 0; 
int counter_save = 0; 
bool flag1 = false;
bool flag2 = false;
bool flag3 = false; 
bool flag4 = false;
bool flag5 = false; 
bool detail1flag = false; 
bool frame1flag = false; 
bool frame2flag = false; 
bool previewflag = false;
bool photoflag = false;
bool set1flag = false;
bool set2flag = false; 
bool skeletonflag = false;
bool screenshotflag = false;
bool savingflag = false; 
bool showclothesflag = false; 

IplImage *detail1, *frame1, *frame2, *frame3;
unsigned char *dataf1c;
unsigned char *dataf1n;
unsigned char *dataf1;
unsigned char *dataf2;
unsigned char *datap1;
unsigned char *datap2;
unsigned char *datad1;
unsigned char *datad2;
unsigned char *datadbag;
unsigned char *datafinger; 
unsigned char *datafinger1;
unsigned char *datafinger2;
unsigned char *datafinger3;
unsigned char *datafinger4;
unsigned char *datasaving1;
unsigned char *datacount1;
unsigned char *datacount2;
unsigned char *datacount3;

Stats  statsBags[NUM_BAGS];
Stats  statsFemale[NUM_DRESSES_FEMALE];
Stats  statsMale[NUM_DRESSES_MALE];
Stats  stats3D[NUM_3DCLOTHES];

time_t t1;
time_t t;

int oldGarmentID_3D = -1;
int oldGarmentID_F = -1;
int oldGarmentID_M = -1;
int oldGarmentID_B = -1;

void set_timer(){
	t = time(0);
}

bool playvid = false; 
//-------------------------------------------------------------------------------------------------------------------
void showCircle(int x, int y, int r)
//---------------------------------------- ---------------------------------------------------------------------------
{
	int count = 0; 
	if(r == 1){
		count = counter;
	}
	else if(r == 2){
		count = counter_r;
	}else if(r == 3){
		count = counter_s1;
	}else if(r == 4){
		count = counter_s2;
	}else if(r == 5){
		count = counter_s3;
	}else if(r == 6){
		count = counter_s4;
	}
	
	glBegin(GL_LINE_LOOP);
	glColor4f(1,0,0,1);
	for(float t = 0; t <= 6.28; t += 0.1)
		glVertex2f(30 * cos(t) + x, 30 * sin(t) + y);
	glEnd();
	glBegin(GL_POLYGON);
	glColor4f(1,1,0,1);
	for(float t = 0; t <= 6.28*(counter-10)/10; t += 0.1)
		glVertex2f(25 * cos(t) + x, 25 * sin(t) + y);
	glEnd();
	
}
void playVideo()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	// Start the child process. 
	if( !CreateProcess( processpath1,   // No module name (use command line)
			NULL,	
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure

			) 
	{
		printf( "CreateProcess failed (%d).\n", GetLastError() );
		return;
	}

/*
	int VIEWPORT_WIDTH = 1280;
	int VIEWPORT_HEIGHT = 960; 
	//video 
	IplImage *image = cvQueryFrame(g_Capture);
	if(image == NULL){
		cvReleaseCapture(&g_Capture);
		g_Capture = cvCaptureFromAVI("C:/Users/Magic Mirror/Desktop/Data/Pictures/Screensaver.mov");
		image = cvQueryFrame(g_Capture);
	}

	//cvCvtColor(image, image, CV_BGR2RGB);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image->width, image->height,GL_BGR, GL_UNSIGNED_BYTE, image->imageData);
	glColor4f(1,1,1,1);
		glBegin(GL_QUADS);

		// upper left
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);
	
		// upper right
		glTexCoord2f((float)1280/(float)2048, 0);
		glVertex2f(1280, 0);

		// bottom right
		glTexCoord2f((float)1280/(float)2048, (float)960/(float)2048);
		glVertex2f(1280, 960);

		// bottom left
		glTexCoord2f(0, (float)960/(float)2048);
		glVertex2f(0, 960);

	glEnd();
	*/
}
//-------------------------------------------------------------------------------------------------------------------
void PrintOnScreen(char *str, int posX=500, int posY=200)
//-------------------------------------------------------------------------------------------------------------------
{
	glColor3f( 55, 155, 55 );
	glRasterPos2f(posX, posY);
	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}
void PrintOnScreen(double d, char *title = NULL, int posX=500, int posY=200)
{
	char str[100]={'\0'};
	if(title == NULL) sprintf_s(str, "%lg", d);
	else sprintf_s(str, "%s%lg", title, d);
	PrintOnScreen(str, posX, posY);
}
void addbag(){
	bBagIsActive = 1;
	bShowBag = 1;
	mcloth->Update(MCLOTH_BAGS);
	garmentid = 1; 
}
void addcloth(){
	garmentid = 1;
	bBagIsActive = 0;
	if(bShowDress3D){ 
		garmentid = 1 + pCloth3D->GetGarmentID();
	}else{
		bShowDress2D=1;
		if(ismale)
			mcloth->Update(MCLOTH_DRESS_MALE);
		else
			mcloth->Update(MCLOTH_DRESS_FEMALE);
	}
}
void bagbutton(){
	garmentid = 1; 
	bShowDress3D = bShowDress2D = 0;
	bShowBag = bBagIsActive = 1;
	ismale = 0;
	mcloth->Update(MCLOTH_BAGS);
}
void femalebutton(){
	garmentid = 1; 
	bShowDress3D = bShowBag = bBagIsActive = 0;
	bShowDress2D = 1;
	ismale = 0; 
	mcloth->Update(MCLOTH_DRESS_FEMALE);
}
void malebutton(){
	garmentid = 1; 
	bBagIsActive = 0;
	bShowBag = 0;
	bShowDress3D = 0;
	bShowDress2D = 1;
	ismale = 1;
	mcloth->Update(MCLOTH_DRESS_MALE);
}
void button3D(){
	garmentid = 1; 
	bShowBag = 0;
	bBagIsActive = 0;
	bShowDress3D = 1;
	ismale = 0;
	bShowDress2D = 0;
	garmentid = 1 + pCloth3D->GetGarmentID();//added 23aug
	printf("button3D garmentid = %d",garmentid);
}
//-------------------------------------------------------------------------------------------------------------------
void gui()
//-------------------------------------------------------------------------------------------------------------------
{
	if(skel->status == false){
		printf("no skeleton detected\n");
		skeletonflag = false;
	}else{
	//top left button s1 main
	if(detail1flag==false && frame2flag == false){
		if(4*skel->handL.u > 370 && 4*skel->handL.u < 370+125 && 4*skel->handL.v < 140 &&  4*skel->handL.v > 0) counter_s1++; //counter_s1++;
		else if(4*skel->handR.u > 370 && 4*skel->handR.u < 370+125 && 4*skel->handR.v < 140 &&  4*skel->handR.v > 0) counter_s1++;//counter_s1++;
		else counter_s1 = 0;
		if(counter_s1 > 20){
			if(frame1flag){
				frame2flag = true; //categories
				frame1flag = false;
			}
			if(previewflag){ //discard
				//frame2flag = true; 
				frame1flag = true; 
				previewflag = false; 
				photoflag = false; 
			}
			counter_s1 = 0;//counter_s1 = 0;
		}
	}

	//top left button s2
	if(frame2flag==false){
		if(4*skel->handL.u > 370+135 && 4*skel->handL.u < 370+270 && 4*skel->handL.v < 140 && 4*skel->handL.v > 0) 
			counter_s2++;//s2
		else if(4*skel->handR.u > 370+135 && 4*skel->handR.u < 370+270 && 4*skel->handR.v < 140 && 4*skel->handR.v > 0) 
			counter_s2++;//s2
		else
			counter_s2 = 0;
		if(counter_s2 > 20){
			if(frame1flag){//show/hide details 
				if(detail1flag)
					detail1flag = false; 
				else
					detail1flag = true; //details 
			}
			if(previewflag){//discard photo
				frame1flag = true; 
				previewflag = false; 
				photoflag = false; 
			}
			counter_s2 = 0;
		}
	}
	//bottom left button  
	if(frame1flag){
		if(4*skel->handL.u < 470 && 4*skel->handL.v > 400 && 4*skel->handL.v < 600) counter++;
		else if(4*skel->handR.u < 470 && 4*skel->handR.v > 400 && 4*skel->handR.v < 600) counter++;
		else counter = 0;
		if(counter > 20)
		{
			detail1flag = false; 
			garmentid++; 
			if(bShowBag && bBagIsActive)
			{
				if(garmentid >mcloth->NUM_BAGS)
					garmentid = 1;
				mcloth->Update(MCLOTH_BAG_NEXT);
		}else if(bShowDress3D){
			pCloth3D->ShowNextModel();	
			garmentid = 1 + pCloth3D->GetGarmentID();
		}else{
			if(ismale)
			{
				if(garmentid >mcloth->NUM_DRESSES_MALE)
				{
					garmentid = 1;
				}
			}else{ 
				if(garmentid > mcloth->NUM_DRESSES_FEMALE)
					garmentid = 1;
			}
			mcloth->Update(MCLOTH_DRESS_NEXT);
		}
		counter=0;
	}
	//bottom right button 
	if(4*skel->handR.u > 370+540-100 && 4*skel->handR.v > 400 && 4*skel->handR.v < 600) counter_r++;//_r
	else if(4*skel->handL.u > 370+540-100 && 4*skel->handL.v > 400 && 4*skel->handL.v < 600) counter_r++;
	else counter_r = 0;
	if(counter_r > 20)
	{
		detail1flag = false; 
		garmentid--;
		if(bShowBag && bBagIsActive)
		{
			if(garmentid <1)
				garmentid = mcloth->NUM_BAGS;
			mcloth->Update(MCLOTH_BAG_PREV);
		}else if(bShowDress3D){
			pCloth3D->ShowPreviousModel();	
			garmentid = 1 + pCloth3D->GetGarmentID();
		}else{
			if(garmentid <1)
			{
				if(ismale)
					garmentid = mcloth->NUM_DRESSES_MALE;
				else 
					garmentid = mcloth->NUM_DRESSES_FEMALE;
			}
			mcloth->Update(MCLOTH_DRESS_PREV);
		}
		counter_r = 0;
	}
	}//end of if(frame1flag)

	if(detail1flag == false && frame2flag == false){
		//top right button s3 take photo
		if(4*skel->handR.u > 370+270 && 4*skel->handR.u < 370+405 && 4*skel->handR.v < 140 && 4*skel->handR.v > 0) counter_s3++;//_s3
		else if(4*skel->handL.u > 370+270 && 4*skel->handL.u < 370+405 && 4*skel->handL.v < 140 && 4*skel->handL.v > 0) counter_s3++;
		else counter_s3 = 0;
		if(counter_s3 > 20){
			if(frame1flag){
				frame1flag = false;
				screenshotflag = true; //takephoto 
			}
			if(previewflag){//share to facebook
				//frame2flag = true; 
				previewflag = false; 
				photoflag = false; 
				savingflag = true; 
			}
			counter_s3 = 0;
		}
	//top right s4 add bags or add clothes
		if(frame1flag && ismale==false){
			if(4*skel->handR.u > 370+405 && 4*skel->handR.u < 370+540 && 4*skel->handR.v < 140 && 4*skel->handR.v > 0) counter_s4++;//s4
			else if(4*skel->handL.u > 370+405 && 4*skel->handL.u < 370+540 && 4*skel->handL.v < 140 && 4*skel->handL.v > 0) counter_s4++;
			else counter_s4 = 0;
			if(counter_s4 > 20)
			{
				if(frame1flag){//add bags or add clothes
					if(bBagIsActive){
						addcloth();
					}else {
						addbag();
					}
				}
				if(previewflag){//share to facebook
					previewflag = false; 
					photoflag = false; 
					savingflag = true; 
				}
				counter_s4 = 0; 
			}
		}
	}
	//button 1 on main menu 
	if(frame2flag){
		if(4*skel->handR.u > 370+110 && 4*skel->handR.u < 370+250 && 4*skel->handR.v > 140 && 4*skel->handR.v < 280) counter_b1++;//b1
		else if(4*skel->handL.u > 370+110 && 4*skel->handL.u < 370+250 && 4*skel->handL.v > 140 && 4*skel->handL.v < 280) counter_bl1++;
		else counter_b1 = counter_bl1 = 0;
		if(counter_b1 > 20 || counter_bl1 > 20)
		{
			frame2flag = false;
			frame1flag = true;
			femalebutton(); 
			counter_b1 = counter_bl1 = 0; 
		}
		//button 2 
		if(4*skel->handR.u > 370+310 && 4*skel->handR.u < 370+450 && 4*skel->handR.v > 140 && 4*skel->handR.v < 280) counter_b2++;//b2
		else if(4*skel->handL.u > 370+310 && 4*skel->handL.u < 370+450 && 4*skel->handL.v > 140 && 4*skel->handL.v < 280) counter_bl2++;
		else counter_b2 = counter_bl2 = 0;
		if(counter_b2 > 20 || counter_bl2 > 20 )
		{
			frame1flag = true;
			frame2flag = false;
			malebutton(); 
			counter_b2 = counter_bl2 = 0; 
		}
		//button 3
		if(4*skel->handR.u > 370+110 && 4*skel->handR.u < 370+250 && 4*skel->handR.v > 300 && 4*skel->handR.v < 520) counter_b3++;//b3
		else if(4*skel->handL.u > 370+110 && 4*skel->handL.u < 370+250 && 4*skel->handL.v > 300 && 4*skel->handL.v < 520) counter_bl3++;
		else counter_b3 = counter_bl3 = 0;
		if(counter_b3 > 20 || counter_bl3 > 20)
		{
			frame1flag = true;
			frame2flag = false;
			bagbutton(); 
			counter_b3 = counter_bl3 = 0; 

		}
		//button 4 
		if(4*skel->handR.u > 370+310 && 4*skel->handR.u < 370+450 && 4*skel->handR.v > 300 && 4*skel->handR.v < 520) counter_b4++;//b4
		else if(4*skel->handL.u > 370+310 && 4*skel->handL.u < 370+450 && 4*skel->handL.v > 300 && 4*skel->handL.v < 520) counter_bl4++;
		else counter_b4 = counter_bl4 = 0;
		if(counter_b4 > 20 || counter_bl4 > 20)
		{
			frame1flag = true;
			frame2flag = false;
			button3D();
			counter_b4 = counter_bl4 = 0; 
		}
	}
		skeletonflag = true;
	}//end if of no skeleton
}
//-------------------------------------------------------------------------------Chong 
float Rotate = 0;
float trans = -500;
void transistion(unsigned char* data){
	
		trans += 100.0f;
		glPushMatrix();
		glRotatef(3.0f,0.0f,0.0f,1.0f);
		glTranslatef(0.0f,trans,0.0f); 
	
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 432,768,GL_RGB, GL_UNSIGNED_BYTE, data); 
		//glRotatef(0.8f,0.0f,0.0f,1.0f);
		
		glColor4f(1,1,1,1);

		glBegin(GL_QUADS);
		
		// upper left
		glTexCoord2f(0, 0);
		glVertex2f(370+60, 0+150);
	
		// upper right
		glTexCoord2f((float)432/(float)2048, 0);
		glVertex2f(432+370+60, 0+150);

		// bottom right
		glTexCoord2f((float)432/(float)2048, (float)768/(float)2048);
		glVertex2f(432+370+60, 768+150);

		// bottom left
		glTexCoord2f(0, (float)768/(float)2048);
		glVertex2f(0+370+60, 768+150);

		glEnd();
	glPopMatrix();
	
	if(counter_trans > 5){
		counter_trans = 0; 
		photoflag = true; 
		Rotate = 0; 
		trans = -500;
	}
}
void showphoto(unsigned char* data){
	glPushMatrix();	
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		
		glRotatef(3.0f,0.0f,0.0f,1.0f);


		
		
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 432,768,GL_RGB, GL_UNSIGNED_BYTE, data); 
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);

		// upper left
		glTexCoord2f(0, 0);
		glVertex2f(370+60, 0+150);//glVertex2f(370+54, 0+150);
	
		// upper right
		glTexCoord2f((float)432/(float)2048, 0);
		glVertex2f(432+370+60, 0+150);//		glVertex2f(432+370+54, 0+150);

		// bottom right
		glTexCoord2f((float)432/(float)2048, (float)768/(float)2048);
		glVertex2f(432+370+60, 768+150);//		glVertex2f(432+370+54, 768+150);

		// bottom left
		glTexCoord2f(0, (float)768/(float)2048);
		glVertex2f(0+370+60, 768+150);//		glVertex2f(0+370+54, 768+150);

		glEnd();

		int width = 164, height = 164;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, qrCodeData); 
		glColor4f(1,1,1,1);
		
		float xOffset = 430;
		float yOffset = 150;
		//QR Code rendering
		glBegin(GL_QUADS); 
			glTexCoord2f(0, 0);													glVertex2f(xOffset+10, yOffset+10);
			glTexCoord2f((float)width/(float)2048, 0);							glVertex2f(xOffset+110, yOffset+10);
			glTexCoord2f((float)width/(float)2048, (float)height/(float)2048);	glVertex2f(xOffset+110, yOffset+110);
			glTexCoord2f(0, (float)height/(float)2048);							glVertex2f(xOffset+10,yOffset+110);
		glEnd();

	 
	glPopMatrix();
}
void showleft(){//next
	unsigned char *data=0; 
	int width = 0;
	int height = 0; 
	int nchannels = 0; 
	//bool showleftflag = false;
	char *filename=0;
	if(bBagIsActive){
		if(garmentid == mcloth->NUM_BAGS){
			filename = new char [bfilename[0].size()+1];
			strcpy(filename, bfilename[0].c_str());
		}else{
			filename = new char [bfilename[garmentid].size()+1];
			strcpy(filename, bfilename[garmentid].c_str());
		}
	}else if(bShowDress3D){
		if(garmentid == NUM_3DCLOTHES)
		{
			filename = new char [dfilename[0].size()+1];
			strcpy(filename, dfilename[0].c_str());
		}else{
			filename = new char [dfilename[garmentid].size()+1];
			strcpy(filename, dfilename[garmentid].c_str());
		}
	}else if(ismale){
		if(garmentid == mcloth->NUM_DRESSES_MALE){
			filename = new char [mfilename[0].size()+1];
			strcpy(filename, mfilename[0].c_str());
		}else{
			filename = new char [mfilename[garmentid].size()+1];
			strcpy(filename, mfilename[garmentid].c_str());
		}
	}else{
		if(garmentid == mcloth->NUM_DRESSES_FEMALE)
		{
			filename = new char [ffilename[0].size()+1];
			strcpy(filename, ffilename[0].c_str());
		}else{
			filename = new char [ffilename[garmentid].size()+1];
			strcpy(filename, ffilename[garmentid].c_str());
		}
	}
	//if(showleftflag){
		data = SOIL_load_image(
				filename,
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,height,GL_RGBA, GL_UNSIGNED_BYTE, data); 
		delete [] data;

		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);

		//TODO
		float u = (float)width/(float)2060;
		float v = (float)height/(float)2060;
		// upper left
		glTexCoord2f(u, 0);
		glVertex2f(0+370, 415);
	
		// upper right
		glTexCoord2f(0, 0);
		glVertex2f(width+370, 415);

		// bottom right
		glTexCoord2f(0, v);
		glVertex2f(width+370, height+415);

		// bottom left
		glTexCoord2f(u, v);
		glVertex2f(0+370, height + 415);

		glEnd();
	//}
}
void showright(){	
	unsigned char * data=0; 
	int width = 0;
	int height = 0; 
	int nchannels = 0; 
	char *filename=0; 
//	bool showrightflag = false;
	if(bBagIsActive){
		if(garmentid == 1){
			filename = new char [bfilename[mcloth->NUM_BAGS-1].size()+1];
			strcpy(filename, bfilename[mcloth->NUM_BAGS-1].c_str());
		}else{
			filename = new char [bfilename[garmentid-2].size()+1];
			strcpy(filename, bfilename[garmentid-2].c_str());
		}
	}else if(bShowDress3D){
		//garmentid = pCloth3D->GetGarmentID();
		printf("current garmentid = %d\n",garmentid);
		if(garmentid == 1){
			filename = new char [dfilename[NUM_3DCLOTHES-1].size()+1];
			strcpy(filename, dfilename[NUM_3DCLOTHES-1].c_str());
		}else{
			filename = new char [dfilename[garmentid-2].size()+1];
			strcpy(filename, dfilename[garmentid-2].c_str());
		}
		
	}else if(ismale){
		if(garmentid == 1){
			filename = new char [mfilename[mcloth->NUM_DRESSES_MALE-1].size()+1];
			strcpy(filename, mfilename[mcloth->NUM_DRESSES_MALE-1].c_str());
		}else{
			filename = new char [mfilename[garmentid-2].size()+1];
			strcpy(filename, mfilename[garmentid-2].c_str());
		}
	}else{
		if(garmentid == 1){
			filename = new char [ffilename[mcloth->NUM_DRESSES_FEMALE-1].size()+1];
			strcpy(filename, ffilename[mcloth->NUM_DRESSES_FEMALE-1].c_str());
		}else{
			filename = new char [ffilename[garmentid-2].size()+1];
			strcpy(filename, ffilename[garmentid-2].c_str());
		}
	}
	//if(showrightflag){
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	data = SOIL_load_image(
				filename,
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);	
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,height,GL_RGBA, GL_UNSIGNED_BYTE, data); 
		delete [] data;
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);

		// upper left
		glTexCoord2f(0, 0);
		glVertex2f(540+370-width, 415);
	
		// upper right
		glTexCoord2f((float)width/(float)2060, 0);
		glVertex2f(540+370, 415);

		// bottom right
		glTexCoord2f((float)width/(float)2060, (float)height/(float)2060);
		glVertex2f(540+370, height+415);

		// bottom left
		glTexCoord2f(0, (float)height/(float)2060);
		glVertex2f(540+370-width, height + 415);

		glEnd();
	//}
}
void showframe(unsigned char* data){
	//printf("showframe"); 
	//	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 540,960,GL_RGBA, GL_UNSIGNED_BYTE, data); 

		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);

		// upper left
		glTexCoord2f(0, 0);
		glVertex2f(370, 0);
	
		// upper right
		glTexCoord2f((float)540/(float)2048, 0);
		glVertex2f(540+370, 0);

		// bottom right
		glTexCoord2f((float)540/(float)2048, (float)960/(float)2048);
		glVertex2f(540+370, 960);

		// bottom left
		glTexCoord2f(0, (float)960/(float)2048);
		glVertex2f(0+370, 960);

	glEnd();
		
}
void showHand(void){
	bool showing = false;
	unsigned char * dataf=0;
	dataf = datafinger;
	int countf = 0; 
	int handx = 0;
	int handy = 0;
	if(detail1flag){
		showing = true; 
		handx = 4*skel->handL.u;
		handy = 4*skel->handL.v;
	}
	if(counter > 8){
		countf = counter;
		showing = true;
		handx = 4*skel->handL.u;
		handy = 4*skel->handL.v;
	}
	if(counter_r > 8){
		countf = counter_r;
		showing = true;
		handx = 4*skel->handR.u;
		handy = 4*skel->handR.v;
	}
	if(counter_s1 > 8) 
	{
		countf = counter_s1;
		showing = true;
		if(4*skel->handL.v < 140){
			handx = 4*skel->handL.u;
			handy = 4*skel->handL.v;
		}else{
			handx = 4*skel->handR.u;
			handy = 4*skel->handR.v;		
		}
	}
	if(counter_s2 > 8){
		countf = counter_s2;
		showing = true;
		if(4*skel->handL.v < 140){
			handx = 4*skel->handL.u;
			handy = 4*skel->handL.v;
		}else{
			handx = 4*skel->handR.u;
			handy = 4*skel->handR.v;		
		}
	}
	if(counter_s3 > 8){
		countf = counter_s3;
		showing = true;
		if(4*skel->handL.v < 140){
			handx = 4*skel->handL.u;
			handy = 4*skel->handL.v;
		}else{
			handx = 4*skel->handR.u;
			handy = 4*skel->handR.v;		
		}
	}
	if(counter_s4 > 8){
		countf = counter_s4;
		showing = true;
		if(4*skel->handL.v < 140){
			handx = 4*skel->handL.u;
			handy = 4*skel->handL.v;
		}else{
			handx = 4*skel->handR.u;
			handy = 4*skel->handR.v;		
		}
	}
	if(counter_b1 > 8) 
	{
		countf = counter_b1;
		showing = true;
		handx = 4*skel->handR.u;
		handy = 4*skel->handR.v;
	}
	if(counter_b2 > 8){
		countf = counter_b2;
		showing = true;
		handx = 4*skel->handR.u;
		handy = 4*skel->handR.v;
	}
	if(counter_b3 > 8){
		countf = counter_b3;
		showing = true;
		handx = 4*skel->handR.u;
		handy = 4*skel->handR.v;
	}
	if(counter_b4 > 8){
		countf = counter_b4;
		showing = true;
		handx = 4*skel->handR.u;
		handy = 4*skel->handR.v;
	}
	if(counter_bl1 > 8) 
	{
		countf = counter_bl1;
		showing = true;
		handx = 4*skel->handL.u;
		handy = 4*skel->handL.v;
	}
	if(counter_bl2 > 8){
		countf = counter_bl2;
		showing = true;
		handx = 4*skel->handL.u;
		handy = 4*skel->handL.v;
	}
	if(counter_bl3 > 8){
		countf = counter_bl3;
		showing = true;
		handx = 4*skel->handL.u;
		handy = 4*skel->handL.v;
	}
	if(counter_bl4 > 8){
		countf = counter_bl4;
		showing = true;
		handx = 4*skel->handL.u;
		handy = 4*skel->handL.v;
	}
	if(countf>18){
		dataf = datafinger4;
	}else if(countf>16){
		dataf = datafinger3;
	}else if(countf>14){
		dataf = datafinger2;
	}else if(countf>12){
		dataf = datafinger1;
	}
	if(showing)
	{
	//	printf("show hand image");
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 50,50,GL_RGBA, GL_UNSIGNED_BYTE, dataf); 

		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
		//printf("%d %d \n",handx, handy);
		// upper left
		glTexCoord2f(0, 0);
		glVertex2f(handx-25, handy-25);
	
		// upper right
		glTexCoord2f((float)50/(float)2060, 0);
		glVertex2f(handx+25, handy-25);

		// bottom right
		glTexCoord2f((float)50/(float)2060, (float)50/(float)2060);
		glVertex2f(handx+25, handy+25);

		// bottom left
		glTexCoord2f(0, (float)50/(float)2060);
		glVertex2f(handx-25, handy+25);

		glEnd();
	}
}
void screenshot(void)
{
	frame1flag = false; 
	unsigned char * dataf=0;
	dataf = datacount3;
	bool dontshow = true;
	if(counter_sh>40){
		dontshow = false; 
	}else if(counter_sh>30){
		dataf = datacount1;
	}else if(counter_sh>20){
		dataf = datacount2;
	}else if(counter_sh>10){
		dataf = datacount3;
	}
	//	printf("show hand image");
	if(dontshow){
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 540,960,GL_RGBA, GL_UNSIGNED_BYTE, dataf); 

		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			// upper left
		glTexCoord2f(0, 0);
		glVertex2f(370, 0);

		// upper right
		glTexCoord2f((float)540/(float)2048, 0);
		glVertex2f(540+370, 0);
			// bottom right
		glTexCoord2f((float)540/(float)2048, (float)960/(float)2048);
		glVertex2f(540+370, 960);
			// bottom left
		glTexCoord2f(0, (float)960/(float)2048);
		glVertex2f(370, 960);
		glEnd();
	}

	if(counter_sh>40){
		//play sound 
		PlaySound(L"tone.wav", NULL, SND_FILENAME | SND_SYNC);
	
		counter_sh = 0; 
		screenshotflag = false; 
		int nWidth = glutGet( GLUT_WINDOW_WIDTH );
		int nHeight = glutGet( GLUT_WINDOW_HEIGHT );
		IplImage* screenshot =cvCreateImage(cvSize((int)nWidth,(int)nHeight), IPL_DEPTH_8U, 3);
		//IplImage* screenshot =cvCreateImage(cvSize(540,960), IPL_DEPTH_8U, 3);
	
		glReadBuffer( GL_BACK );

		glReadPixels( 0, 0, 
			nWidth, nHeight,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			(GLvoid*)screenshot->imageData);
		cvFlip(screenshot, NULL, 0);
		cvSetImageROI(screenshot, cvRect((nWidth-1080)/2, 0, 1080, 1920));
 
		/* create destination image
		Note that cvGetSize will return the width and the height of ROI */
		IplImage *img2 = cvCreateImage(cvGetSize(screenshot),
								   screenshot->depth,
								   screenshot->nChannels);
 
		/* copy subimage */
		cvCopy(screenshot, img2, NULL);
 
		/* always reset the Region of Interest */
		cvResetImageROI(screenshot);
		IplImage *destination = cvCreateImage
								( cvSize((int)((img2->width*80)/100) , (int)((img2->height*80)/100) ),
										 img2->depth, img2->nChannels );
		cvResize(img2, destination);
		/* Add both images
	    Note that now both images have 'same' width & height */
		IplImage *img1 = cvLoadImage("C:/Users/Magic Mirror/Desktop/Data/Pictures/photo2.png", 1);	
		
		CvRect rect = cvRect(108, 108, img1->width-216, img1->height-384);
 
		/* sets Region of Interest */
		cvSetImageROI(img1, rect);
		cvCvtColor(destination,destination,CV_RGB2BGR);
		cvAdd(img1, destination, img1, NULL);
		cvResetImageROI(img1);
		IplImage *dest = cvCreateImage
								( cvSize((int)((540*80)/100) , (int)((960*80)/100) ),
										 img1->depth, img1->nChannels );
		cvResize(img1, dest);
		char *filename = "screenshot.jpg"; 
		cvSaveImage(filename, dest);
		previewflag = true; 
		//delete []dataf;

	}
}
void savingcountdown(void){
	unsigned char * dataf=0;
	dataf = datasaving1;
	bool dontshow = true;
	if(counter_save>20){
		dontshow = false; 
	}else{
		dataf = datasaving1;
	}
		
	/*if(counter_save>16){
		dataf = datasaving4;
	}else if(counter_save>14){
		dataf = datasaving3;
	}else if(counter_save>12){
		dataf = datasaving2;
	}else if(counter_save>10){
		dataf = datasaving1;
	}*/
	if(dontshow){
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 540,960,GL_RGBA, GL_UNSIGNED_BYTE, dataf); 

		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			// upper left
		glTexCoord2f(0, 0);
		glVertex2f(370, 0);

		// upper right
		glTexCoord2f((float)540/(float)2048, 0);
		glVertex2f(540+370, 0);
			// bottom right
		glTexCoord2f((float)540/(float)2048, (float)960/(float)2048);
		glVertex2f(540+370, 960);
			// bottom left
		glTexCoord2f(0, (float)960/(float)2048);
		glVertex2f(370, 960);
		glEnd();
	}

	if(counter_save>20){
		time_t t = time(0);
		struct tm * now = localtime( & t );
		char directory[20]={'\0'};
		sprintf(directory,".\\%d%d%d",now->tm_mday,now->tm_mon+1,now->tm_year + 1900 );
		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, directory, -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		MultiByteToWideChar (CP_ACP, 0, directory, -1, pwText, dwNum );
		CreateDirectory(pwText,NULL); 
		char filename1[30]={'\0'};
		sprintf(filename1,".\\%d%d%d\\%d%d%d%d.jpg", now->tm_mday,  now->tm_mon+1, now->tm_year + 1900,  now->tm_sec, now->tm_mday, now->tm_mon+1, now->tm_year + 1900);
		DWORD fdwNum = MultiByteToWideChar (CP_ACP, 0, filename1, -1, NULL, 0);
		wchar_t *fText;
		fText = new wchar_t[fdwNum];
		MultiByteToWideChar (CP_ACP, 0, filename1, -1, fText, fdwNum );
		CopyFile(L"screenshot.jpg",fText,false);
		counter_save = 0; 
		savingflag = false;
		//frame2flag = true;
		frame1flag = true; 
		//-------------------Facebook upload Code 
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
	
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );
		// Start the child process. 
		if( !CreateProcess( processpath,   // No module name (use command line)
			NULL,	
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure

			) 
		{
			printf( "CreateProcess failed (%d).\n", GetLastError() );
			return;
		}

		// Wait until child process exits.
		//WaitForSingleObject( pi.hProcess, INFINITE );
		WaitForSingleObject( pi.hProcess, 10000 );

		// Close process and thread handles. 
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	
		HWND hWnd = FindWindowEx( NULL, NULL, L"IEFrame", NULL );
	
		PostMessage(hWnd,WM_KEYDOWN,VK_F5,0);
		//ShellExecute(NULL, L"open", L"http://www.edart.sg/htdocs/Test3.php", NULL, NULL, SW_FORCEMINIMIZE );
	}
}
//-------------------------------------------------------------------------------------------------------------------
void showImage(char *data, int nwidth, int nheight, int x_start, int x_end, int y_start, int y_end, int nchannels)
//-------------------------------------------------------------------------------------------------------------------
{	 
	int nTextImageSize = 2048;

	if(nchannels == 3)
	{
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, nTextImageSize, nTextImageSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nwidth, nheight,GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if(nchannels == 4)
	{
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, nTextImageSize, nTextImageSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nwidth,nheight,GL_RGBA, GL_UNSIGNED_BYTE, data); 
	}
	else if(nchannels == 1)
	{
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, nTextImageSize, nTextImageSize, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nwidth,nheight,GL_LUMINANCE, GL_UNSIGNED_BYTE, data); 
	}
	glBegin(GL_QUADS);	
		glColor4f(1,1,1,1);
		glTexCoord2f(0, 0); // upper left
		glVertex2f(x_start, y_start);	

		glTexCoord2f((float)nwidth/(float)nTextImageSize, 0); // upper right
		glVertex2f(x_end, y_start);	

		glTexCoord2f((float)nwidth/(float)nTextImageSize, (float)nheight/(float)nTextImageSize);// bottom right
		glVertex2f(x_end, y_end);

		glTexCoord2f(0, (float)nheight/(float)nTextImageSize);// bottom left
		glVertex2f(x_start, y_end);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

//Added by Mobeen - 16 July 2012
void DrawFullScreenQuad() {
	glBegin(GL_QUADS);	
		glColor4f(1,1,1,1);
		// upper left
		glTexCoord2f(0, 0); glVertex2f(0, 0);	
		// upper right
		glTexCoord2f(1, 0); glVertex2f(COLOR_RES_X, 0);	
		// bottom right
		glTexCoord2f(1, 1);	glVertex2f(COLOR_RES_X, COLOR_RES_Y);
		// bottom left
		glTexCoord2f(0, 1);	glVertex2f(0, COLOR_RES_Y);
	glEnd();
}

//------------------------------------------------------------------------
void glutDisplay (void)
//------------------------------------------------------------------------
{
	CHECK_GL_ERROR

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update the buffers passed in constructor
	camera->Update();		
	skel->Update(camera->bUserChanged);
	camera->GetNeckOffset();

	//Render the backdrop first
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bgTexID);
	DrawFullScreenQuad();
	CHECK_GL_ERROR
	//Render the live scene	
	glBindTexture(GL_TEXTURE_2D, rgbTexID); 
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,COLOR_RES_X, COLOR_RES_Y, GL_BGRA, GL_UNSIGNED_BYTE, (DWORD*)cImg->imageData);
	CHECK_GL_ERROR
	DrawFullScreenQuad();
	CHECK_GL_ERROR
	glDisable(GL_TEXTURE_2D);
	
	if(showclothesflag){

		if(bShowDress3D) 
		{
			if(oldGarmentID_3D != garmentid) {
				oldGarmentID_3D = garmentid;
				stats3D[garmentid-1].counts++;
			}
			CHECK_GL_ERROR
			pCloth3D->Draw();
			CHECK_GL_ERROR
		}else if( bShowDress2D ){
			if(ismale) {
				if(oldGarmentID_M != garmentid) {
					oldGarmentID_M = garmentid;
					statsMale[garmentid-1].counts++;			 
				}
			} 
			else {
				if(oldGarmentID_F != garmentid) {
					oldGarmentID_F = garmentid;
					statsFemale[garmentid-1].counts++;			 
				}
			}

			CHECK_GL_ERROR
			int nwidth = mcloth->garment->iplImg_4ch->width;
			int nheight = mcloth->garment->iplImg_4ch->height;
			glEnable(GL_BLEND);		
			glEnable(GL_TEXTURE_2D); 
			glBindTexture(GL_TEXTURE_2D, clothTexID);

			CHECK_GL_ERROR

			//bind the pbo so that the texture is cleared
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,2048, 2048, GL_BGRA, GL_UNSIGNED_BYTE, 0);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		
			CHECK_GL_ERROR

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nwidth, nheight,GL_RGBA, GL_UNSIGNED_BYTE, mcloth->garment->iplImg_4ch->imageData); 
			
			GLuint error = glGetError();

			CHECK_GL_ERROR
			mcloth->Draw();
			CHECK_GL_ERROR
			glDisable(GL_TEXTURE_2D); 
			glDisable(GL_BLEND);		
		}
		CHECK_GL_ERROR
		if(bShowBag) 
		{
			if(oldGarmentID_B != garmentid) {
				oldGarmentID_B = garmentid;
				statsBags[garmentid-1].counts++;			 
			}
			   
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, accessoryTexID);
			//the bag
			int x1=0, y1=0, x2=0, y2=0;
			mcloth->GetPosition(1, &x1, &x2, &y1, &y2); 
			glEnable(GL_BLEND);
			CHECK_GL_ERROR
				showImage(mcloth->bag->iplImg_4ch->imageData, mcloth->bag->iplImg_4ch->width, mcloth->bag->iplImg_4ch->height, x1, x2, y1, y2, mcloth->bag->iplImg_4ch->nChannels);	
			CHECK_GL_ERROR
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}
		CHECK_GL_ERROR
	}	
	CHECK_GL_ERROR

	//gesture based interaction
	gui();

	CHECK_GL_ERROR
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, framesTexID);
		glEnable(GL_BLEND);
	CHECK_GL_ERROR
	if(frame1flag){
		showclothesflag = true; 
		showleft();
		showright();
		if(bBagIsActive){
			showframe(dataf1);
		}else{
			if(ismale){
				showframe(dataf1n);//male clothes no bags option
			}else{
				showframe(dataf1c);//female
			}
		}
	}
	CHECK_GL_ERROR
	if(frame2flag){
		showclothesflag = false;
		detail1flag = false; 
		oldGarmentID_3D = -1;
		oldGarmentID_F = -1;
		oldGarmentID_M = -1;
		oldGarmentID_B = -1;
		showframe(dataf2);
	}
	CHECK_GL_ERROR
	if(detail1flag){
		if(bBagIsActive){			
			//showframe(datadbag);
			//added by Mobeen 23 Oct 2012			
			showframe(detailsDataBag[garmentid-1]);
		}else{
			if(ismale){
				//showframe(datad2);
				showframe(detailsDataMale[garmentid-1]);
			}else{
				//showframe(datad1);
				if(bShowDress3D)
					showframe(detailsData3D[garmentid-1]);
				else
					showframe(detailsDataFemale[garmentid-1]);
			}
		}
	}
	CHECK_GL_ERROR
	if(previewflag){
		detail1flag = false;
		showclothesflag = false;
		showframe(datap1);
		if(photoflag == false){
			//photoflag = true; 
			int width = 0;
			int height = 0;
			int nchannels = 0; 
			delete [] datap2;
			datap2 = SOIL_load_image(
					"../KinectSDK/screenshot.jpg",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
			counter_trans++; 
			transistion(datap2); 
			
		}
	}
	CHECK_GL_ERROR
	if(photoflag){
		showphoto(datap2);
	}
	if(screenshotflag){
		counter_sh++;
		screenshot(); 
	}
	if(savingflag){
		showclothesflag = false;
		counter_save++;
		savingcountdown(); 
	}
	if(skeletonflag){
		showHand();//under skeletonflag
		set_timer();
		if(playvid)
		{
			//stop video
			HWND hWnd_video = FindWindowEx( NULL, NULL, L"VideoRenderer", NULL );
			PostMessage(hWnd_video,WM_KEYDOWN,VK_ESCAPE,0);

			playvid = false; 
			//startfrompageone();
			frame2flag = true; //categories
			frame1flag = false; 
			savingflag = false;
			screenshotflag = false;
			photoflag = false;
			previewflag = false;
			detail1flag = false; 
			showclothesflag = false;

		}
	}else{
		//timer to play video 
		t1 = time(0);
		time_t t2 = t+10;//10 second count down 
		if(t1 > t2){
			//play video
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, detailsTexID);
			glEnable(GL_BLEND);
			if(playvid==false){
				playVideo(); 	
				playvid = true;
			}
		}
	}
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	CHECK_GL_ERROR
	//prinitng something on screen
	
	//PrintOnScreen(sizeof(Skeleton), "Sizeof(vec6) = ", 500, 250);
	//PrintOnScreen(skel->bInStandardPose, "Standard Pose = ", 500, 250);
	//PrintOnScreen(skel->shoulder_rotation, "Shoulder Rotation = ", 500, 300);
	PrintOnScreen(garmentid-1, "ID = ", 500, 200);
	PrintOnScreen(statsMale[garmentid-1].counts, "Male Count = ", 500, 250);
	PrintOnScreen(statsFemale[garmentid-1].counts, "Female Count = ", 500, 275);
	PrintOnScreen(stats3D[garmentid-1].counts, "3D Count = ", 500, 300);
	PrintOnScreen(statsBags[garmentid-1].counts, "Bag Count = ", 500, 325);

	glutSwapBuffers();

	//frame rate
	camera->GetFrameRate();

}

//----------------------------------------------------
void glutSpecialKey(int key, int x, int y)
//----------------------------------------------------
{
	switch(key)
	{

	int mdf;

	//show 2d female
	case GLUT_KEY_F1:
		bBagIsActive = 0;
		bShowDress3D = 0;
		bShowDress2D = 1;
		mcloth->Update(MCLOTH_DRESS_FEMALE);
		break;

	//2d male
	case GLUT_KEY_F2:
		bBagIsActive = 0;
		bShowDress3D = 0;
		bShowDress2D = 1;
		mcloth->Update(MCLOTH_DRESS_MALE);
		break;

	//bag + existing selection 2d/3d
	case GLUT_KEY_F3:
		bBagIsActive = 1;
		bShowBag = 1;
		mcloth->Update(MCLOTH_BAGS);
		break;

	//3d obj + existing status of bag
	case GLUT_KEY_F4:
		bBagIsActive = 0;
		bShowDress3D = 1;
		break;

	//2d female only
	case GLUT_KEY_F5:
		bShowDress3D = bShowBag = bBagIsActive = 0;
		bShowDress2D = 1;
		mcloth->Update(MCLOTH_DRESS_FEMALE);
		break;

	//bag only
	case GLUT_KEY_F6:
		bShowDress3D = bShowDress2D = 0;
		bShowBag = bBagIsActive = 1;
		mcloth->Update(MCLOTH_BAGS);
		break;

	case GLUT_KEY_F10:
		if(bShowDress2D||bShowBag) mcloth->Update(MCLOTH_DRESS_SAVE);
		if(bShowDress3D) {
			sprintf(newpath,"%sModels/FemaleYOffsets.csv", path);
			pCloth3D->SaveYOffsets(newpath);
		}
		break;

	case GLUT_KEY_F9:
		//press F9 to manually align/scale the clothes using (alt+) arrow keys
		mcloth->Update(MCLOTH_ADJUST_PARAMETERS);
		break;

	case GLUT_KEY_UP:		
		mdf = glutGetModifiers();
		if(bShowDress2D || bShowBag) {
			if(mdf == GLUT_ACTIVE_ALT) 
				mcloth->Update(MCLOTH_TRANSLATE_U);
			else 
				mcloth->Update(MCLOTH_SCLAE_Y_UP);
		} else {
			if(mdf == GLUT_ACTIVE_ALT) 
				if(bShowDress3D)
					pCloth3D->IncrementYOffsets(); 
				/*else
					pCloth3DFemale->IncrementYOffsets(); */

		}
		break;

	case GLUT_KEY_DOWN:
		mdf = glutGetModifiers();
		if(bShowDress2D || bShowBag) {
			if(mdf == GLUT_ACTIVE_ALT) 
				mcloth->Update(MCLOTH_TRANSLATE_D);
			else 
				mcloth->Update(MCLOTH_SCLAE_Y_DOWN);
		} else {
			if(mdf == GLUT_ACTIVE_ALT) 
				if(bShowDress3D)
					pCloth3D->DecrementYOffsets(); 
				/*else
					pCloth3DFemale->DecrementYOffsets(); */
		}
		break;

	case GLUT_KEY_RIGHT:
		mdf = glutGetModifiers();
		if(mdf == GLUT_ACTIVE_ALT) 
			mcloth->Update(MCLOTH_TRANSLATE_R);
		else 
			mcloth->Update(MCLOTH_SCLAE_X_UP);
		break;

	case GLUT_KEY_LEFT:
		mdf = glutGetModifiers();
		if(mdf == GLUT_ACTIVE_ALT)
			mcloth->Update(MCLOTH_TRANSLATE_L);
		else 
			mcloth->Update(MCLOTH_SCLAE_X_DOWN);
		break;

	default:
		break;
	}
}

void fnExit()
{
	//load the details of bags
	for(int i=0;i<NUM_BAGS;i++) {
		delete [] detailsDataBag[i];
	}
	for(int i=0;i<NUM_DRESSES_FEMALE;i++) {
		delete [] detailsDataFemale[i];		
	}
	for(int i=0;i<NUM_DRESSES_MALE;i++) {
		delete [] detailsDataMale[i];		
	}
	for(int i=0;i<NUM_3DCLOTHES;i++) {
		delete [] detailsData3D[i];		
	}

	delete [] qrCodeData;

	//video
	//	cvReleaseCapture(&g_Capture);
	
	if(camera)
		delete camera;
	delete mcloth;
	delete pCloth3D;

	delete [] colorBuffer;
	colorBuffer = NULL;

	glDeleteTextures(1, &bgTexID);
	glDeleteTextures(1, &rgbTexID);
	glDeleteTextures(1, &accessoryTexID);
	glDeleteTextures(1, &clothTexID);
	glDeleteTextures(1, &framesTexID);
	glDeleteTextures(1, &detailsTexID); 
	glDeleteBuffers(1, &pboID);

	//close audio process	
	TerminateProcess(hAudio, EXIT_SUCCESS);
}

//--------------------------------------------------------------
void glutKeyboard (unsigned char key, int x, int y)
//--------------------------------------------------------------
{
	HWND hWnd = FindWindowEx( NULL, NULL, L"IEFrame", NULL );
	HWND hWnd1 = FindWindowEx( NULL, NULL, L"VideoRenderer", NULL );
	int static dx = 0;
	switch (key)
	{
	case 27:
		//Commented by Mobeen
		//This causes memory leak due to multiple deletions on exit 
		//since the camera is deleted again in the close function
		//Another solution is to set the deleted camera pointer to NULL
		//after deletion.
		//delete camera;
		fnExit();
		exit (1);
		break;
	case 'e':
		//refresh internet explorer for facebook upload
		//ShellExecute(NULL, L"open", L"http://www.edart.sg/htdocs/Test3.php", NULL, NULL, SW_FORCEMINIMIZE );
		PostMessage(hWnd,WM_KEYDOWN,VK_F5,0);
		break; 
	case 's':
		if(playvid){
			playvid = false;
		}else{
			playvid = true; 
		}
		break;
	case 'f':
		//adjustment to display user in center of TV screen
		//glutPositionWindow(-2030,-400);
		glutPositionWindow(-(750),-35);
		//set the size to large enugh to cover TV length and 
		glutReshapeWindow(1280*2, 960*2);//2.35
		break;

	case 'n':
		if(bShowBag && bBagIsActive)
			mcloth->Update(MCLOTH_BAG_NEXT);
		else if(bShowDress3D)
			pCloth3D->ShowNextModel();	
		else
			mcloth->Update(MCLOTH_DRESS_NEXT);
		break;

	case 'b':
		if(bShowBag && bBagIsActive)
			mcloth->Update(MCLOTH_BAG_PREV);
		else if(bShowDress3D)
			pCloth3D->ShowPreviousModel();	
		else
			mcloth->Update(MCLOTH_DRESS_PREV);
		break;

	case 't': 
		pCloth3D->ToggleTransparent(); 
		break;
	case 'v': 
		//test play video 		
		playVideo(); 
		break;
	case 'c':
		//stop video 
		PostMessage(hWnd1,WM_KEYDOWN,VK_ESCAPE,0);
		break;

		
	}
}

//----------------------------------------------------------------
void glutIdle (void)
//----------------------------------------------------------------
{
	//printf("glutIdle");
	glutPostRedisplay();
}


//Added by Mobeen
void InitGL() {	
	 
	//Setup initial OpenGL states
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//Load the background image using OpenCV
	IplImage* bgImage = cvLoadImage("back.jpg", 1);	
	GLenum format = GL_RGBA;
	GLenum internalFormat = GL_BGRA;
	switch(bgImage->nChannels) {
		case 3: internalFormat = GL_RGB; format = GL_RGB; break;
		case 4: internalFormat = GL_RGBA; format = GL_BGRA; break;
		case 1: internalFormat = GL_INTENSITY; format = GL_LUMINANCE; break;
	}
	 
	//Generate the background texture
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &bgTexID);
		glBindTexture(GL_TEXTURE_2D, bgTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		
		glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, COLOR_RES_X, COLOR_RES_Y, 0, format, GL_UNSIGNED_BYTE, bgImage->imageData);
	//Delete the OpenCV image
	cvReleaseImage(&bgImage);

	colorBuffer = new DWORD[COLOR_RES_X*COLOR_RES_Y];
	memset(colorBuffer,0, sizeof(DWORD)*COLOR_RES_X*COLOR_RES_Y);

	//Generate the live scene rgba texture
	glGenTextures(1, &rgbTexID);
	glBindTexture(GL_TEXTURE_2D, rgbTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, COLOR_RES_X, COLOR_RES_Y, 0, GL_BGRA, GL_UNSIGNED_BYTE, (DWORD*) cImg->imageData);
	 
	//Generate the cloth texture 
	glGenTextures(1, &clothTexID);
	glBindTexture(GL_TEXTURE_2D, clothTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, 2048, 2048, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	

	//Generate the accessory texture 
	glGenTextures(1, &accessoryTexID);
	glBindTexture(GL_TEXTURE_2D, accessoryTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, 2048, 2048, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	

	//Generate frames texture
	glGenTextures(1, &framesTexID);
	glBindTexture(GL_TEXTURE_2D, framesTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, 2048, 2048, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
	

//	IplImage *image = cvQueryFrame(g_Capture);
//	cvCvtColor(image, image, CV_BGR2RGB);
	glGenTextures(1, &detailsTexID);
	glBindTexture(GL_TEXTURE_2D, detailsTexID);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glDisable(GL_TEXTURE_2D);
	
	//wglSwapIntervalEXT(0);	//disable vsync
	glGenBuffers(1, &pboID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, 2048*2048*4, NULL, GL_STREAM_DRAW);
		GLubyte* pData = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
			memset(pData,0, 2048*2048*4);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

//----------------------------------------------------------------
void main(int argc, char* argv[])
//----------------------------------------------------------------
{ 
	//-------------timer
	set_timer();

	//-------------
//	g_Capture = cvCaptureFromAVI("C:/Users/Magic Mirror/Desktop/Data/Pictures/Screensaver.mov");

	detail1 = frame1 = frame2 = frame3 = NULL;
	frame2flag = true; //categories

	int width = 0;
	int height = 0;
	int nchannels = 0; 
	int width1 = 0;
	int height1 = 0;
	int nchannels1 = 0; 
	qrCodeData = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/QRCode.bmp",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);

	datad1 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/detail1.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datad2 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/detail2.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datadbag = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/detail3.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);

	dataf1c =  SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/interfaceclothes.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	dataf1n =  SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/interfacenone.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	dataf1 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/interfacebag.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	dataf2 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/categories.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datap1 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/preview.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				); 
	datap2 = SOIL_load_image(
				"../KinectSDK/screenshot.jpg",
				&width1,&height1,&nchannels1
				,SOIL_LOAD_AUTO
				);
	datafinger = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/circle0.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger1 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/circle1.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger2 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/circle2.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger3 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/circle3.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger4 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/circle4.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datasaving1 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/uploadsuccessful.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datacount1 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/count1.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datacount2 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/count2.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datacount3 = SOIL_load_image(
				"C:/Users/Magic Mirror/Desktop/Data/Pictures/count3.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	
	//load the details of bags
	for(int i=0;i<NUM_BAGS;i++) {
		detailsDataBag[i]= SOIL_load_image(detailsBag[i].c_str(),&width,&height,&nchannels,SOIL_LOAD_AUTO);		
	}
	for(int i=0;i<NUM_DRESSES_FEMALE;i++) {
		detailsDataFemale[i]= SOIL_load_image(detailsFemale[i].c_str(),&width,&height,&nchannels,SOIL_LOAD_AUTO);		
	}
	for(int i=0;i<NUM_DRESSES_MALE;i++) {
		detailsDataMale[i]= SOIL_load_image(detailsMale[i].c_str(),&width,&height,&nchannels,SOIL_LOAD_AUTO);		
	}
	for(int i=0;i<NUM_3DCLOTHES;i++) {
		detailsData3D[i]= SOIL_load_image(details3D[i].c_str(),&width,&height,&nchannels,SOIL_LOAD_AUTO);		
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(COLOR_RES_X, COLOR_RES_Y);
	glutCreateWindow ("Magic Mirror");
	
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutSpecialFunc(glutSpecialKey);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
	glutCloseFunc(fnExit);

	//Added by Mobeen - 16 July 2012
	glewInit(); 
	InitGL(); 
	mcloth->SetNumItems(NUM_BAGS, NUM_DRESSES_FEMALE, NUM_DRESSES_MALE);
	
	
	for(int i=0;i<NUM_3DCLOTHES;i++) {	 
		int id = ids3DGarment[i];
		sprintf(newpath,"%sModels/Garment%d/Garment%d.obj", path, id, id);
		
		pCloth3D->LoadObjMesh(newpath);		
	}
	pCloth3D->InitReferenceValues();
	sprintf(newpath,"%sModels/FemaleYOffsets.csv", path);
	pCloth3D->LoadYOffsets(newpath);

	for(int i=0;i<NUM_3DCLOTHES;i++) {
		int id = ids3DGarment[i];
		if((id==19 || id==20) ||  id<=10) {
			pCloth3D->SetReferenceHeight(i, 1.22f); 
			pCloth3D->SetReferenceShoulderWidth(i, 0.277f);  
		}
	}

	/*
	pCloth3D->LoadObjMesh("C:/Users/Magic Mirror/Desktop/Data/Models/Garment1/Garment1.obj");  
	pCloth3D->InitReferenceValues();

	for(int i=0;i<NUM_3DCLOTHES;i++) {
		if(i!=2 && i<10) {
			pCloth3D->SetReferenceHeight(i, 1.22f); 
			pCloth3D->SetReferenceShoulderWidth(i, 0.277f);  
		}
	}
	*/

	// Setup the OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(0, COLOR_RES_X, COLOR_RES_Y, 0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0,0, COLOR_RES_X, COLOR_RES_Y);
		
	glutPositionWindow(-(750),-35);
	glutReshapeWindow(1280*2, 960*2);

	//now play the audio 
	SHELLEXECUTEINFO pInfo;
	pInfo.hwnd = NULL;
	pInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    pInfo.fMask = SEE_MASK_FLAG_NO_UI|SEE_MASK_NOCLOSEPROCESS ;
    pInfo.lpVerb = L"open";
	pInfo.lpFile = L"Kalimba.mp3";
	pInfo.lpParameters = L"/prefetch:1";
	pInfo.lpDirectory = L"\"C:\\Program Files (x86)\\Windows Media Player\\\"";
	pInfo.nShow = SW_HIDE; 
	
	ShellExecuteEx(&pInfo);

	hAudio = pInfo.hProcess;

	// Per frame code is in glutDisplay
	glutMainLoop();
}