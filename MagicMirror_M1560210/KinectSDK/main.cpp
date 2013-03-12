 
#include "stdafx.h" 
#include "SOIL.h"
#include "gui.h"
#include <gl/glew.h> 
#include <gl/wglew.h> 
#include <gl/freeglut.h>
#include "tinyxml2.h"
//kevin
#include "math.h"
//end


//video player ffmpeg
//extern "C" {
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//}

#include <mmsystem.h> //for playing wav file 
#include <time.h> 
#pragma comment(lib, "glew32")
//PhysX libs
#pragma comment(lib, "PhysX3_x86.lib")
#pragma comment(lib, "PhysX3Common_x86.lib") 
#pragma comment(lib, "PhysX3Extensions.lib") 

//APEX libs
#pragma comment(lib, "ApexFramework_x86.lib")
#pragma comment(lib, "PhysX3Cooking_x86.lib")
#pragma comment(lib, "PxTask.lib")
#pragma comment(lib, "PhysXProfileSDK.lib")
#include <cassert>
//#define //check_gl_error assert(glGetError() == GL_NO_ERROR);

char* path = ::getenv("MAGIC_MIRROR_DATA_ROOT");
char newpath[MAX_PATH]={'\0'};

//play video 
//CvCapture* g_Capture;// = "C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/Screensaver.avi";

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


float translate=0;
float scale=1;
//HANDLE hAudio=NULL;

//added by chong 
LPCWSTR processpath = L"..\\Debug\\ftpupload.exe";//path to the file ftpupload.exe for facebook upload
//LPCWSTR processpath = L"..\\ftpupload.exe";//path to the file ftpupload.exe for facebook upload
LPCWSTR processpath1 = L"..\\Debug\\Cutscene.exe";//path to the file ftpupload.exe for facebook upload

//Modified by CCL
//"const" removed by CCL as these will be determined by XML Load
/*static const int NUM_3DCLOTHES = 1;
const int NUM_BAGS = 8;
const int NUM_DRESSES_MALE = 2;
const int NUM_DRESSES_FEMALE = 16; */
static int NUM_3DCLOTHES=0;
int NUM_BAGS=0;
int NUM_DRESSES_MALE=0;
int NUM_DRESSES_FEMALE=0;

//Removed by CCL-
//Actaully this is not even needed
/*unsigned char* detailsDataFemale[NUM_DRESSES_FEMALE];
unsigned char* detailsDataMale[NUM_DRESSES_MALE];
unsigned char* detailsDataBag[NUM_BAGS];
unsigned char* detailsData3D[NUM_3DCLOTHES];*/
unsigned char* qrCodeData= 0;
 
  
//int ids3DGarment[]={  6,2,4,5,7,8,9,10,21,22,23,24,26,28,14,15,16,17};
int ids3DGarment[]={6,2,4,5,8,10,22,23};
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

//Declarations NEW 240113
bool showWheel = true;	//set to true for test purposes. set to off afterwards. --SF
bool showPanel = false;
int panelFlag = 0;
int menuFlag = 0;
//END

//GestureDetection
bool isDetected = false;
//end

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

//Declarations NEW 240113
unsigned char *dataDefault;
unsigned char *dataHome;
unsigned char *dataPhoto;
unsigned char *dataDetails;
unsigned char *dataAdd;
unsigned char *dataP;
unsigned char *dataPHome;
unsigned char *dataPLadies;
unsigned char *dataPMen;
unsigned char *dataPBag;
unsigned char *dataP3d;
unsigned char *dataAlbum;
unsigned char *dataABack;
unsigned char *dataALike;
unsigned char *dataALikeOn;
unsigned char *dataAShare;
unsigned char *dataADiscard;

unsigned char *dataFrame;
int wFrame = 71;
int hFrame = 122;
unsigned char *dataFrameBig;
int wFrameBig = 101;
int hFrameBig = 173;

unsigned char *dataWDefault;
unsigned char *dataWHome;
unsigned char *dataWPhoto;
unsigned char *dataWDetails;
unsigned char *dataWAdd;
unsigned char *dataWP;
unsigned char *dataWPHome;
unsigned char *dataWPLadies;
unsigned char *dataWPMen;
unsigned char *dataWPBag;
unsigned char *dataWP3d;
unsigned char *dataWAlbum;
unsigned char *dataWABack;
unsigned char *dataWALike;
unsigned char *dataWALikeOn;
unsigned char *dataWAShare;
unsigned char *dataWADiscard;
//END

unsigned char *datafinger; 
unsigned char *datafinger1;
unsigned char *datafinger2;
unsigned char *datafinger3;
unsigned char *datafinger4;
unsigned char *datasaving1;
unsigned char *datacount1;
unsigned char *datacount2;
unsigned char *datacount3;

//kevin vars start
//speed of rotation multipliers
//rotateIncrement must positive integer, default 2 is very slow. Adjust higher to turn wheel faster
const int rotateIncrement = 6;
//do not change these multipliers
const float offsetCenterMultiplier = 1.0;
const float offsetCenterLargeMultiplier = 1.0;
const float normalMultiplier = 1.0;
//frame scaling control
const float fscaleMin=0.7;
const float fscaleMax=1.0;
float fscale1 = fscaleMax;
float fscale2 = fscaleMin;
float fscale3 = fscaleMin;
float fscale4 = fscaleMin;
float fscale5 = fscaleMin;
float fscale6 = fscaleMin;
float fscale1b = fscaleMax;
float fscale2b = fscaleMin;
float fscale3b = fscaleMin;
float fscale4b = fscaleMin;
float fscale5b = fscaleMin;
float fscale6b = fscaleMin;
float fscale1m = fscaleMax;
float fscale2m = fscaleMin;
float fscale3m = fscaleMin;
float fscale4m = fscaleMin;
float fscale5m = fscaleMin;
float fscale6m = fscaleMin;
float fscale1f = fscaleMax;
float fscale2f = fscaleMin;
float fscale3f = fscaleMin;
float fscale4f = fscaleMin;
float fscale5f = fscaleMin;
float fscale6f = fscaleMin;
//control wheel's frame icons cycle movement (do not change)
const float rUpperBound = 264.0;
const float rLowerBound = 96.0;
const float rOffset = 168.0;
//distance to center of rotation (do not change)
const int orbitRange = 300;
//offset for each frame distance from the center at 180 degress(do not change)
const int frameOffset1 = 28;
const int frameOffset2 = 56;
const int frameOffset3 = 84;
//special regions near center to adjust speed
const int centerRegionUpperBound = 180-10+frameOffset1;
const int centerRegionLowerBound = 180+10-frameOffset1;
const int centerLargeRegionUpperBound = 180+frameOffset2;
const int centerLargeRegionLowerBound = 180-frameOffset2;
//angle of rotation in degrees
double clockwiseTraversing = 180.0;
double clockwiseTraversing2 = clockwiseTraversing+frameOffset2;
double clockwiseTraversing3 = clockwiseTraversing+frameOffset1;
double clockwiseTraversing4 = clockwiseTraversing-frameOffset1;
double clockwiseTraversing5 = clockwiseTraversing-frameOffset2;
double clockwiseTraversing6 = clockwiseTraversing+frameOffset3;
double clockwiseTraversingb = 180.0;
double clockwiseTraversing2b = clockwiseTraversingb+frameOffset2;
double clockwiseTraversing3b = clockwiseTraversingb+frameOffset1;
double clockwiseTraversing4b = clockwiseTraversingb-frameOffset1;
double clockwiseTraversing5b = clockwiseTraversingb-frameOffset2;
double clockwiseTraversing6b = clockwiseTraversingb+frameOffset3;
double clockwiseTraversingm = 180.0;
double clockwiseTraversing2m = clockwiseTraversingm+frameOffset2;
double clockwiseTraversing3m = clockwiseTraversingm+frameOffset1;
double clockwiseTraversing4m = clockwiseTraversingm-frameOffset1;
double clockwiseTraversing5m = clockwiseTraversingm-frameOffset2;
double clockwiseTraversing6m = clockwiseTraversingm+frameOffset3;
double clockwiseTraversingf = 180.0;
double clockwiseTraversing2f = clockwiseTraversingf+frameOffset2;
double clockwiseTraversing3f = clockwiseTraversingf+frameOffset1;
double clockwiseTraversing4f = clockwiseTraversingf-frameOffset1;
double clockwiseTraversing5f = clockwiseTraversingf-frameOffset2;
double clockwiseTraversing6f = clockwiseTraversingf+frameOffset3;
double clockwiseTraversingIncrement = 0;
//storage
const float DEG2RAD = 3.14159/180;
float degInRad = 0.0;
float degInRad2 = 0.0;
float degInRad3 = 0.0;
float degInRad4 = 0.0;
float degInRad5 = 0.0;
float degInRad6 = 0.0;
//clothes icons id tracking
//bags, male, female
int b_id1, b_id2, b_id3, b_id4, b_id5, b_id6;
int m_id1, m_id2, m_id3, m_id4, m_id5, m_id6;
int f_id1, f_id2, f_id3, f_id4, f_id5, f_id6;
//max count of bags, male, female
int maxBags, maxMale, maxFemale;
//track to alter current selection with wheel scrolls
int fSelect = 0;
int fSelect_b = fSelect;
int fSelect_m = fSelect;
int fSelect_f = fSelect;
//aspect ratio fix
double ar;
bool key_state[256]={false};
void keyupdate();
void keyUpdateFromGesture();
void smoothWheelToCenter();
void showicon(int w1, int h1, int frameId);
void DrawFrames(unsigned char* data, int width, int height, int frameId);
void loadWheel();
void moveWheel();
void saveWheel();
void checkfSelect();
void showcurrent();
int garmentid_b = garmentid;
int garmentid_m = garmentid;
int garmentid_f = garmentid;
//end kevin vars

//Edwin values
double tempValueTurnedA = 0.0;
double tempValueTurnedB = 0.0;
//end Edwin


//Removed by CCL- we will allocate these fixed size
/*Stats  statsBags[NUM_BAGS];
Stats  statsFemale[NUM_DRESSES_FEMALE];
Stats  statsMale[NUM_DRESSES_MALE];
Stats  stats3D[NUM_3DCLOTHES];*/
//Stats  *statsBags;
//Stats  *statsFemale;
//Stats  *statsMale;
//Stats  *stats3D;
Stats statsBags[100];
Stats statsFemale[100];
Stats statsMale[100];
Stats stats3D[100];

time_t t1;
time_t t;

int oldGarmentID_3D = -1;
int oldGarmentID_F = -1;
int oldGarmentID_M = -1;
int oldGarmentID_B = -1;

class Timer {
public:
	Timer() {
		start=0;
		stop =0;
	}

	void Start() {
		start = clock(); 
	}
	void Stop() {
		stop = clock();	 
	}
	float GetElapsedTime() {
		return float(stop-start)/CLOCKS_PER_SEC;
	}

	clock_t start, stop;
};

void set_timer(){
	t = time(0);
}

Timer timer;

//----------------------------
// CCL Added for XML Load
//------------------------------
tinyxml2::XMLDocument xmldoc;
const char clothesXML[] = "ClothesList.xml";
char* DATAPATH = ::getenv("MAGIC_MIRROR_DATA_ROOT");
void XMLGetFullPath( tinyxml2::XMLElement* inElement, const char* attr, std::string& fullattrpath)
{
	
	fullattrpath = DATAPATH;
	const char* pathAttr = inElement->Attribute(attr);
	if (pathAttr)
		fullattrpath.append(pathAttr);
}

void LoadGarments( const std::string& fileName )
{
	std::string fullIconPath;
	std::string fullIconName;
	std::string fullpath;
	std::string fullGarmentName;
	std::string fullDetailPath;
	std::string fullDetailName;
	tinyxml2::XMLElement* childNode;
	tinyxml2::XMLElement* conceptElement ;
	std::string fullfilename = DATAPATH;
	fullfilename.append(fileName);
	int index3d;

	//Load and read XML

	if (xmldoc.LoadFile( fullfilename.c_str() )!=tinyxml2::XML_NO_ERROR)
	{
		fprintf(stdout, "cannot open %s\n", fullfilename.c_str() );
		fflush(stdout);
		//exit(1);
	}
	
	//get root element
	tinyxml2::XMLElement* rootElement = xmldoc.RootElement();
	if (rootElement==NULL)
	{
		fprintf(stdout, "Error in %s defining 'CLOTHESLIST'\n", fullfilename.c_str() );
		fflush(stdout);
		//exit(1);
	}
	
	//look for "concept" elements (3D Male and Female
	std::vector<std::string> conceptTags;
	conceptTags.clear();
	conceptTags.push_back( "CONCEPT" );


	for (int i = 0; i< conceptTags.size() ; i++)
	{
		conceptElement = rootElement->FirstChildElement(conceptTags.at(i).c_str());
		//concept has path too
		fullpath.clear();
		XMLGetFullPath( conceptElement, "path", fullpath );
		

		fullIconPath.clear();
		XMLGetFullPath( conceptElement, "icon_path", fullIconPath);

		fullDetailPath.clear();
		XMLGetFullPath( conceptElement, "detail_path", fullDetailPath );
		

		//going through "ITEM"s under "CONCEPT_FEMALE"
		childNode = conceptElement->FirstChildElement("ITEM");
		while (childNode)
		{
			//if (childNode->FirstChild())
			//{
				tinyxml2::XMLElement* garmentElement = childNode->FirstChildElement( "GARMENT" );
				tinyxml2::XMLElement* detailElement = childNode->FirstChildElement( "DETAIL" );
				tinyxml2::XMLElement* iconElement = childNode->FirstChildElement( "ICON" );

				fullGarmentName.clear();
				fullGarmentName = fullpath;
				fullGarmentName.append( garmentElement->FirstChild()->ToText()->Value() );

				fprintf(stdout, "%s loading %s\n",conceptTags.at(i).c_str(), fullGarmentName.c_str() );
				fflush(stdout);
				index3d = pCloth3D->LoadObjMesh( fullGarmentName );

				if (iconElement)
				{
					fullIconName.clear();
					fullIconName = fullIconPath;
					fullIconName.append( iconElement->FirstChild()->ToText()->Value() );
					//m_3DIconList.push_back( fullIconName );
					pCloth3D->SetIconPath( index3d-1, fullIconName );
				}
				else
				{
					fprintf(stdout, " Pleaes Specify Icon for for %s\n", fullGarmentName );
					fflush(stdout);
					exit(2);
				}

				if (detailElement)
				{
					fullDetailName.clear();
					fullDetailName = fullDetailPath;
					fullDetailName.append( detailElement->FirstChild()->ToText()->Value() );
					fprintf(stdout, "with detail path = %s\n", fullDetailName.c_str() );
					fflush(stdout);
					pCloth3D->SetDetailPath( index3d-1, fullDetailName );
				}
				else
				{
					fprintf(stdout, " Pleaes Specify Detail for for %s\n", fullGarmentName );
					fflush(stdout);
					exit(2);
				}

				//Now the attributes
				//0 will be returned if there is no such attribute
				pCloth3D->SetYOffsets( index3d-1, childNode->FloatAttribute( "yoffset" ) ) ;
				pCloth3D->SetReferenceHeight( index3d-1, childNode->FloatAttribute( "ref_height" ) );
				pCloth3D->SetReferenceShoulderWidth( index3d-1, childNode->FloatAttribute( "ref_shoulder_width" ) ); 

				NUM_3DCLOTHES++;
			//}
			childNode = childNode->NextSiblingElement("ITEM");
			
		}
	}


	//Load 2D Female, Male and Bags
	//=============================
	std::vector<std::string> ParcoTags;
	ParcoTags.push_back("PARCO_FEMALE");
	ParcoTags.push_back("PARCO_MALE");
	ParcoTags.push_back("PARCO_BAGS");
	for (int i=0; i<ParcoTags.size(); i++)
	{
		conceptElement=rootElement->FirstChildElement(ParcoTags.at(i).c_str());
		fullpath.clear();
		XMLGetFullPath( conceptElement, "path", fullpath);

		fullIconPath.clear();
		XMLGetFullPath( conceptElement, "icon_path", fullIconPath );

		fullDetailPath.clear();
		XMLGetFullPath( conceptElement, "detail_path", fullDetailPath );

		//going through each item
		childNode = conceptElement->FirstChildElement("ITEM");
		while( childNode  )
		{
			//if (childNode->FirstChild() )
			//{
				tinyxml2::XMLElement* garmentElement = childNode->FirstChildElement( "GARMENT" );
				tinyxml2::XMLElement* alphaElement = childNode->FirstChildElement("ALPHA" );
				tinyxml2::XMLElement* detailElement = childNode->FirstChildElement( "DETAIL" );

				Garment* newGarment;
				if (i==0)
					newGarment=mcloth->NewGarment( MCLOTH_DRESS_FEMALE );
				else if (i==1)
					newGarment=mcloth->NewGarment( MCLOTH_DRESS_MALE );
				else if (i==2)
					newGarment=mcloth->NewGarment( MCLOTH_BAGS );
				std::string fullGarmentName = fullpath;
				fullGarmentName.append( garmentElement->FirstChild()->ToText()->Value() );
				fprintf(stdout, "Parco Female finding %s\n", fullGarmentName.c_str() );
				fflush(stdout);
				newGarment->img_name = fullGarmentName;

				//alpha image
				if (alphaElement)
				{
					fullGarmentName.clear();
					fullGarmentName = fullpath;
					fullGarmentName.append( alphaElement->FirstChild()->ToText()->Value() );
					newGarment->alpha_name = fullGarmentName;
				}
				else
				{
					fprintf(stdout, "Please specify Alpha for %s\n", newGarment->img_name.c_str() );
					fflush(stdout);
					exit(2);
				}

				//icon image
				fullIconName.clear();
				fullIconName = fullIconPath;
				fullIconName.append( garmentElement->FirstChild()->ToText()->Value() );
				newGarment->icon_name = fullIconName;

				//detail image
				if (detailElement)
				{
					fullDetailName.clear();
					fullDetailName = fullDetailPath;
					fullDetailName.append( detailElement->FirstChild()->ToText()->Value() );
					newGarment->detail_name = fullDetailName;
				}
				else
				{
					fprintf(stdout, " Pleaes Specify Detail for for %s\n", newGarment->img_name.c_str() );
					fflush(stdout);
					exit(2);
				}

				//attributes tx, ty, sx, sy
				if (i<=1)
				{
					newGarment->tx=childNode->FloatAttribute("tx");
					newGarment->ty=childNode->FloatAttribute("ty");
					newGarment->sx=childNode->FloatAttribute("sx");
					newGarment->sy=childNode->FloatAttribute("sy");
					//fprintf(stdout, "tx=%f ty = %f sx = %f sy=%f\n",newGarment->tx, newGarment->ty, newGarment->sx, newGarment->sy );
					//fflush(stdout);
					//Should compare 
					//if (i==0)
					//	mcloth->AddGarment( newGarment, MCLOTH_DRESS_FEMALE );
					//else if (i==1)
					//	mcloth->AddGarment( newGarment, MCLOTH_DRESS_MALE );
				}
				else if (i==2)
				{
					newGarment->u0=childNode->IntAttribute("u0");
					newGarment->v1=childNode->IntAttribute("v1");
					newGarment->z1=childNode->FloatAttribute("z1");
					newGarment->sx1=childNode->FloatAttribute("sx1");
					newGarment->sy1=childNode->FloatAttribute("sy1");
					//fprintf(stdout, "u0=%d v1=%d z1=%f sx1=%f sx1=%f\n", newGarment->u0, newGarment->v1, newGarment->z1, newGarment->sx1, newGarment->sy1 );
					//fflush(stdout);
					//mcloth->AddGarment( newGarment, MCLOTH_BAGS );
				}

			//}
			childNode = childNode->NextSiblingElement("ITEM");
		}
	}
}

//CCL-This file saving is for 2D garments
void Save2DGarmentsXML(const std::string& fileName)
{
	
	tinyxml2::XMLElement* rootElement = xmldoc.RootElement();
	if (!rootElement)
	{
		fprintf(stdout, "XML error.\n");
		return;
	}
	tinyxml2::XMLElement* parcoTagElement=NULL;
	tinyxml2::XMLElement* toAmendItem=NULL;
	int id=0; //in mcloth, id starts from 1
	int count=0;
	std::string parcoTag;
	//get the current ID
	if (mcloth->GetAccessoryType()==MCLOTH_DRESS_FEMALE)
	{
		parcoTag = "PARCO_FEMALE";
		id=mcloth->GetGarmentId() - 1;
	}
	else if (mcloth->GetAccessoryType()==MCLOTH_DRESS_MALE)
	{
		parcoTag = "PARCO_MALE";
		id=mcloth->GetGarmentId() -1;
	}
	else if (mcloth->GetAccessoryType()==MCLOTH_BAGS)
	{
		parcoTag = "PARCO_BAGS";
		id=mcloth->GetBagId() - 1;
	}
	//look for the TAG
	parcoTagElement=rootElement->FirstChildElement( parcoTag.c_str() );

	if ( !parcoTagElement)
	{
		fprintf(stdout, "WARNING no such tag : %s in XML. Not saving.\n", parcoTag.c_str() );
		fflush(stdout);
		return;
	}
	//then run through the items
	toAmendItem=parcoTagElement->FirstChildElement("ITEM"); //number 0
	if (!toAmendItem)
	{
		fprintf(stdout, "No child nodes under tag %s in XML. Not saving.\n", parcoTag.c_str() );
		fflush(stdout);
		return;
	}
	while (count!=id && toAmendItem)
	{
		toAmendItem=toAmendItem->NextSiblingElement("ITEM");
		count++;
	}
	//in case of discrepancy
	if (!toAmendItem)
	{
		fprintf(stdout, "error in saving information on garment id %d %s\n", mcloth->garment->img_name.c_str() );
		fflush(stdout);
		return;
	}
	
	if (mcloth->GetAccessoryType()!=MCLOTH_BAGS)
	{
		toAmendItem->ToElement()->SetAttribute( "tx", (double) mcloth->garment->tx );
		toAmendItem->ToElement()->SetAttribute( "ty", (double) mcloth->garment->ty );
		toAmendItem->ToElement()->SetAttribute( "sx", (double) mcloth->garment->sx );
		toAmendItem->ToElement()->SetAttribute( "sy", (double) mcloth->garment->sy );
		fprintf(stdout, "saving attributes %f %f %f %f [%d] %s\n", mcloth->garment->tx, \
			mcloth->garment->ty, mcloth->garment->sx, mcloth->garment->sy,  id, mcloth->garment->img_name.c_str() );
		fflush(stdout);
	}
	else
	{
		toAmendItem->ToElement()->SetAttribute( "u0", (int) mcloth->bag->u0 );
		toAmendItem->ToElement()->SetAttribute( "v1", (int) mcloth->bag->v1 );
		toAmendItem->ToElement()->SetAttribute( "z1", (double) mcloth->bag->z1 );
		toAmendItem->ToElement()->SetAttribute( "sx1", (double) mcloth->bag->sx1 );
		toAmendItem->ToElement()->SetAttribute( "sy1", (double) mcloth->bag->sy1 );
		fprintf(stdout, "saving attributes %d %d %f %f %f [%d] %s\n", mcloth->bag->u0, \
			mcloth->bag->v1, mcloth->bag->z1, mcloth->bag->sx1, mcloth->bag->sy1,  id, mcloth->bag->img_name.c_str() );
		fflush(stdout);
	}
	std::string fullfilename = DATAPATH;
	
	fullfilename.append(fileName);
	xmldoc.SaveFile(fullfilename.c_str());
}

//CCL - this file saving is for 3D Garment
void Save3DGarmentsXML(const std::string& fileName)
{
	tinyxml2::XMLElement* rootElement = xmldoc.RootElement();
	if (!rootElement)
	{
		fprintf(stdout, "XML error.\n");
		return;
	}
	tinyxml2::XMLElement* conceptElement=NULL;
	tinyxml2::XMLElement* toAmendItem=NULL;
	int id=0; //in mcloth3d, id starts from 0
	int count=0;
	//get the current ID

	id = pCloth3D->GetGarmentID();
	conceptElement = rootElement->FirstChildElement( "CONCEPT" );
	if ( !conceptElement )
	{
		fprintf(stdout, "WARNING no such tag : CONCEPT in XML. Not saving.\n" );
		fflush(stdout);
		return;
	}
	//go thru the items
	toAmendItem = conceptElement->FirstChildElement("ITEM"); //number 0
	if (!toAmendItem)
	{
		fprintf(stdout, "No child nodes under CONCEPT in XML. Not saving\n");
		fflush(stdout);
		return;
	}
	while (count!=id && toAmendItem)
	{
		toAmendItem=toAmendItem->NextSiblingElement("ITEM");
		count++;
	}
	//in case of discrepancy
	if (!toAmendItem)
	{
		fprintf(stdout, "error in saving information on 3D garment id %d \n", id );
		fflush(stdout);
		return;
	}

	//lets save
	toAmendItem->SetAttribute( "yoffset", (double) pCloth3D->GetYOffsets( id ) );

	std::string fullfilename = DATAPATH;	
	fullfilename.append(fileName);
	xmldoc.SaveFile(fullfilename.c_str());
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
//kevin menu change edited buttons start
void bagbutton(){
	saveWheel();
	garmentid = garmentid_b; 
	bShowDress3D = bShowDress2D = 0;
	bShowBag = bBagIsActive = 1;
	ismale = 0;
	mcloth->Update(MCLOTH_BAGS);
	//load wheel
	loadWheel();
	//load bag icons
	maxBags = mcloth->NUM_BAGS;
	//generate icon placements in which frames based on center frame
	checkfSelect();
}
void femalebutton(){
	saveWheel();
	garmentid = garmentid_f; 
	bShowDress3D = bShowBag = bBagIsActive = 0;
	bShowDress2D = 1;
	ismale = 0; 
	mcloth->Update(MCLOTH_DRESS_FEMALE);
	//load wheel
	loadWheel();
	//load female icons
	maxFemale = mcloth->NUM_DRESSES_FEMALE;
	//generate icon placements in which frames based on center frame
	checkfSelect();
}
void malebutton(){
	saveWheel();
	garmentid = garmentid_m; 
	bBagIsActive = 0;
	bShowBag = 0;
	bShowDress3D = 0;
	bShowDress2D = 1;
	ismale = 1;
	mcloth->Update(MCLOTH_DRESS_MALE);
	//load wheel
	loadWheel();
	//load male icons
	maxMale = mcloth->NUM_DRESSES_MALE;
	//generate icon placements in which frames based on center frame
	checkfSelect();
}
//end kevin menu change edited

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

void mainMenuButton() {
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
}

void infoButton() {
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
}

//kevin scroll
void scrollLeftButton(){
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
}

void scrollRightButton(){
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
}
//end kevin scroll

void photoButton(){
	if(frame1flag){
		frame1flag = false;
		screenshotflag = true; //takephoto 

		////////////////////////////////////
		if(bShowBag) {
			statsBags[garmentid-1].nPictures ++;		
		} 
		if(bShowDress2D){
			if(ismale) {
				statsMale[garmentid-1].nPictures ++;
			} else {
				statsFemale[garmentid-1].nPictures ++;
			}
		}
		else if(bShowDress3D) {
			stats3D[garmentid-1].nPictures ++;
		} 
		///////////////////////////////////////

	}
	if(previewflag){//share to facebook
		//frame2flag = true; 
		previewflag = false; 
		photoflag = false; 
		savingflag = true; 
	}
}

void recommendButton(){
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
}
void UpdateTime() {
	

	timer.Stop();
	 
	if(bBagIsActive && bShowBag) {
		statsBags[garmentid-1].totalTimeSpent += timer.GetElapsedTime();		
	} else if(ismale) {
		statsMale[garmentid-1].totalTimeSpent += timer.GetElapsedTime();
	} 
	else if(bShowDress3D) {
		stats3D[garmentid-1].totalTimeSpent += timer.GetElapsedTime();
	} else {
		statsFemale[garmentid-1].totalTimeSpent += timer.GetElapsedTime();
	} 
	timer.Start();
	
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

				////////////////////////////////////
				if(bShowBag) {
					statsBags[garmentid-1].nPictures ++;		
				} 
				if(bShowDress2D){
					if(ismale) {
						statsMale[garmentid-1].nPictures ++;
					} else {
						statsFemale[garmentid-1].nPictures ++;
					}
				}
				else if(bShowDress3D) {
					stats3D[garmentid-1].nPictures ++;
				} 
				///////////////////////////////////////

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
		/*
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
		*/
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

		 
		//QR Code drawing function
		int width = 148, height = 148;
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

//kevin icon thumbnails each drawing on each frame
void showicon(int w1, int h1, int frameId){
	unsigned char *data=0; 
	int width = 0;
	int height = 0; 
	int nchannels = 0;
	//if bag wheel
	if(bBagIsActive){
		switch(frameId)
		{
			case 1:
				if(b_id1<=0) b_id1=maxBags+b_id1;
				else if (b_id1>maxBags) b_id1=b_id1-maxBags;
				sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, b_id1);
				break;
			case 2:
				if(b_id2<=0) b_id2=maxBags+b_id2;
				else if (b_id2>maxBags) b_id2=b_id2-maxBags;
				sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, b_id2);
				break;
			case 3:
				if(b_id3<=0) b_id3=maxBags+b_id3;
				else if (b_id3>maxBags) b_id3=b_id3-maxBags;
				sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, b_id3);
				break;
			case 4:
				if(b_id4<=0) b_id4=maxBags+b_id4;
				else if (b_id4>maxBags) b_id4=b_id4-maxBags;
				sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, b_id4);
				break;
			case 5:
				if(b_id5<=0) b_id5=maxBags+b_id5;
				else if (b_id5>maxBags) b_id5=b_id5-maxBags;
				sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, b_id5);
				break;
			case 6:
				if(b_id6<=0) b_id6=maxBags+b_id6;
				else if (b_id6>maxBags) b_id6=b_id6-maxBags;
				sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, b_id6);
				break;
		}
	}else if(bShowDress3D){		 
		//sprintf(newpath,"%s/Models/icons/Garment%d.png", path, ids3DGarment[( garmentid % NUM_3DCLOTHES)] );
		//sprintf( newpath, "%s", pCloth3D->GetIconPathNext().c_str() );
	}else if(ismale){
		//if male 2D wheel
		switch(frameId)
		{
			case 1:
				if(m_id1<=0) m_id1=maxMale+m_id1;
				else if (m_id1>maxMale) m_id1=m_id1-maxMale;
				sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, m_id1);
				break;
			case 2:
				if(m_id2<=0) m_id2=maxMale+m_id2;
				else if (m_id2>maxMale) m_id2=m_id2-maxMale;
				sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, m_id2);
				break;
			case 3:
				if(m_id3<=0) m_id3=maxMale+m_id3;
				else if (m_id3>maxMale) m_id3=m_id3-maxMale;
				sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, m_id3);
				break;
			case 4:
				if(m_id4<=0) m_id4=maxMale+m_id4;
				else if (m_id4>maxMale) m_id4=m_id4-maxMale;
				sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, m_id4);
				break;
			case 5:
				if(m_id5<=0) m_id5=maxMale+m_id5;
				else if (m_id5>maxMale) m_id5=m_id5-maxMale;
				sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, m_id5);
				break;
			case 6:
				if(m_id6<=0) m_id6=maxMale+m_id6;
				else if (m_id6>maxMale) m_id6=m_id6-maxMale;
				sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, m_id6);
				break;
		}
	}else{
		//if female 2D wheel
		switch(frameId)
		{
			case 1:
				if(f_id1<=0) f_id1=maxFemale+f_id1;
				else if (f_id1>maxFemale) f_id1=f_id1-maxFemale;
				sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, f_id1);
				break;
			case 2:
				if(f_id2<=0) f_id2=maxFemale+f_id2;
				else if (f_id2>maxFemale) f_id2=f_id2-maxFemale;
				sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, f_id2);
				break;
			case 3:
				if(f_id3<=0) f_id3=maxFemale+f_id3;
				else if (f_id3>maxFemale) f_id3=f_id3-maxFemale;
				sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, f_id3);
				break;
			case 4:
				if(f_id4<=0) f_id4=maxFemale+f_id4;
				else if (f_id4>maxFemale) f_id4=f_id4-maxFemale;
				sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, f_id4);
				break;
			case 5:
				if(f_id5<=0) f_id5=maxFemale+f_id5;
				else if (f_id5>maxFemale) f_id5=f_id5-maxFemale;
				sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, f_id5);
				break;
			case 6:
				if(f_id6<=0) f_id6=maxFemale+f_id6;
				else if (f_id6>maxFemale) f_id6=f_id6-maxFemale;
				sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, f_id6);
				break;
		}
	}
		data = SOIL_load_image(
				newpath,
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	float u = (float)width/(float)2060;
	float v = (float)height/(float)2060;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	//Save icon texture image as current texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,height,GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete [] data;
	//Draw icons with this texture
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);
		//upper left
		glTexCoord2f(0, 0);
		glVertex2f(-w1/(float)2, -h1/(float)2);
		//upper right
		glTexCoord2f(u, 0);
		glVertex2f(w1/(float)2, -h1/(float)2);
		//bottom right
		glTexCoord2f(u, v);
		glVertex2f(w1/(float)2, h1/(float)2);
		//bottom left
		glTexCoord2f(0, v);
		glVertex2f(-w1/(float)2, h1/(float)2);
	glEnd();		
}
//end kevin icons drawing

//kevin start
//Draw current selection at leftside of screen for prototype testing only
void showcurrent(){
	unsigned char *data=0; 
	int width = 0;
	int height = 0; 
	int nchannels = 0;   
	if(bBagIsActive){ 
		sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, mcloth->GetBagId());
	}else if(bShowDress3D){		 
		//do nothing for now, no 3D dress
	}else if(ismale){
		sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png",path, mcloth->GetGarmentId());
	}else{ 
		sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png",path, mcloth->GetGarmentId());
	}
		data = SOIL_load_image(
				newpath,
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width,height,GL_RGBA, GL_UNSIGNED_BYTE, data); 
		delete [] data;
		float u = (float)width/(float)2060;
		float v = (float)height/(float)2060;
		glColor4f(1,1,1,1);
		glBegin(GL_QUADS);
			if(bShowDress3D) {
				//upper left
				glTexCoord2f(u, 0);
				glVertex2f(0+370, 415);
				//upper right
				glTexCoord2f(0, 0);
				glVertex2f(width+370, 415);
				//bottom right
				glTexCoord2f(0, v);
				glVertex2f(width+370, height+415);
				//bottom left
				glTexCoord2f(u, v);
				glVertex2f(0+370, height + 415);
			} else {
				//upper left
				glTexCoord2f(0, 0);
				glVertex2f(0+370, 415);
				//upper right
				glTexCoord2f(u, 0);
				glVertex2f(width+370, 415);
				//bottom right
				glTexCoord2f(u, v);
				glVertex2f(width+370, height+415);
				//bottom left
				glTexCoord2f(0, v);
				glVertex2f(0+370, height + 415);
			}
		glEnd();
}
//end kevin

void showleft(){//next
	UpdateTime();
	unsigned char *data=0; 
	int width = 0;
	int height = 0; 
	int nchannels = 0;   
	if(bBagIsActive){ 
		//sprintf(newpath,"%sParco/icons/Bags/bag%d.png",path, ((garmentid% mcloth->NUM_BAGS) + 1) ); 
		//sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, mcloth->GetBagIdNext() ); //ccl removed
		sprintf(newpath, "%s", mcloth->GetBagIconNext() );	//ccl added
	}else if(bShowDress3D){		 
		//sprintf(newpath,"%s/Models/icons/Garment%d.png", path, ids3DGarment[( garmentid % NUM_3DCLOTHES)] ); //ccl removed
		sprintf( newpath, "%s", pCloth3D->GetIconPathNext().c_str() );	//ccl added
	}else if(ismale){
		//sprintf(newpath,"%sParco/icons/Male/M%d.png", path, ((garmentid% NUM_DRESSES_MALE) + 1)  );
		//sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png", path, mcloth->GetGarmentIdNext()  );		//ccl removed
		sprintf( newpath, "%s", mcloth->GetGarmentIconNext() );	//ccl added
	}else{ 
		//sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png", path, ((garmentid% NUM_DRESSES_FEMALE) + 1)  ); 
		//sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png", path, mcloth->GetGarmentIdNext() );	//ccl removed
		sprintf( newpath, "%s", mcloth->GetGarmentIconNext() ); //ccl added
	}
	//if(showleftflag){
		data = SOIL_load_image(
				newpath,
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
		if(bShowDress3D) {
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
		} else {
			// upper left
			glTexCoord2f(0, 0);
			glVertex2f(0+370, 415);
	
			// upper right
			glTexCoord2f(u, 0);
			glVertex2f(width+370, 415);

			// bottom right
			glTexCoord2f(u, v);
			glVertex2f(width+370, height+415);

			// bottom left
			glTexCoord2f(0, v);
			glVertex2f(0+370, height + 415);
		}
		glEnd();
	//}
}

void showright(){	
	UpdateTime();
	unsigned char * data=0; 
	int width = 0;
	int height = 0; 
	int nchannels = 0; 
	 
//	char *filename=0; 
//	bool showrightflag = false;
	if(bBagIsActive){
		 
		int id = (garmentid-1)% mcloth->NUM_BAGS;
		if(id == 0)
			id = mcloth->NUM_BAGS;

		//sprintf(newpath,"%sParco/icons/Bags/bag%d.png", path, id ); 
		//sprintf(newpath,"%sParco/icons/Bags/bag%02d_icon.png",path, mcloth->GetBagIdPrev() ); //ccl removed
		sprintf( newpath, "%s", mcloth->GetBagIconPrev() );	//ccl aaded
	}else if(bShowDress3D){
		 
		 
		int id = (garmentid-1)% NUM_3DCLOTHES;
		if(id == 0)
			id = NUM_3DCLOTHES;

		//sprintf(newpath,"%s/Models/icons/Garment%d.png", path, ids3DGarment[ id-1 ] );	//ccl removed
		sprintf( newpath, "%s", pCloth3D->GetIconPathPrev().c_str() );	//ccl added
	}else if(ismale){
		/*if(garmentid == 1){
			filename = new char [mfilename[mcloth->NUM_DRESSES_MALE-1].size()+1];
			strcpy(filename, mfilename[mcloth->NUM_DRESSES_MALE-1].c_str());
		}else{
			filename = new char [mfilename[garmentid-2].size()+1];
			strcpy(filename, mfilename[garmentid-2].c_str());
		}*/
		int id = (garmentid-1)% NUM_DRESSES_MALE;
		if(id == 0)
			id = NUM_DRESSES_MALE;
		//sprintf(newpath,"%sParco/icons/Male/M%02d_icon.png", path, mcloth->GetGarmentIdPrev()  );	//ccl removed
		//sprintf(newpath,"%sParco/icons/Male/M%d.png", path, id ); 

		sprintf(newpath, "%s", mcloth->GetGarmentIconPrev() );	//ccl added
	}else{
		/*if(garmentid == 1){
			filename = new char [ffilename[mcloth->NUM_DRESSES_FEMALE-1].size()+1];
			strcpy(filename, ffilename[mcloth->NUM_DRESSES_FEMALE-1].c_str());
		}else{
			filename = new char [ffilename[garmentid-2].size()+1];
			strcpy(filename, ffilename[garmentid-2].c_str());
		}*/
		int id = (garmentid-1)% NUM_DRESSES_FEMALE;
		if(id == 0)
			id = NUM_DRESSES_FEMALE;

		//sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png", path, id ); 
		//sprintf(newpath,"%sParco/icons/Female/F%02d_icon.png", path, mcloth->GetGarmentIdPrev() );	//ccl removed

		sprintf(newpath, "%s", mcloth->GetGarmentIconPrev() );	//ccl added
 
	}
	//if(showrightflag){
		//glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	data = SOIL_load_image(
				newpath,
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

// NEW 240113
//Change function name from showframe to avoid confusion between drawing the BG and drawing the
//big/small frames in the new GUI
void DrawBG(unsigned char* data){
	//printf("DrawBG"); 
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

//DrawFrames NEW 240113 by Shawn
//kevin edited fixed white lines around border
void DrawFrames(unsigned char* data, int width, int height, int frameId) {
	float width2 = (float)width/(float)2060;
	float height2 = (float)height/(float)2060;
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);

		// upper left
		glTexCoord2f(0, 1/(float)2048);
		glVertex2f(-width/(float)2, -height/(float)2);
	
		// upper right
		glTexCoord2f((width-1)/(float)2048, 1/(float)2048);
		glVertex2f(width/(float)2, -height/(float)2);

		// bottom right
		glTexCoord2f((width-1)/(float)2048, (height-1)/(float)2048);
		glVertex2f(width/(float)2, height/(float)2);

		// bottom left
		glTexCoord2f(0.0, (height-1)/(float)2048);
		glVertex2f(-width/(float)2, height/(float)2);
	glEnd();
	//comment out this line if it lags too much
	showicon(width-30, height-30, frameId);
}
//END
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
		IplImage *img1 = cvLoadImage("C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/photo2.png", 1);	
		
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
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
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

		if(bShowBag) {
			statsBags[garmentid-1].nUploads ++;		
		} 
		if(bShowDress2D){
			if(ismale) {
				statsMale[garmentid-1].nUploads ++;
			} else {
				statsFemale[garmentid-1].nUploads ++;
			}
		}
		else if(bShowDress3D) {
			stats3D[garmentid-1].nUploads ++;
		} 
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

		glTexCoord2f((float)(nwidth-5)/(float)nTextImageSize, 0); // upper right
		glVertex2f(x_end, y_start);	

		glTexCoord2f((float)(nwidth-5)/(float)nTextImageSize, (float)(nheight-5)/(float)nTextImageSize);// bottom right
		glVertex2f(x_end, y_end);

		glTexCoord2f(0, (float)(nheight-5)/(float)nTextImageSize);// bottom left
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

int cnt = 0;
wchar_t buffer[MAX_PATH]={L'\0'};
//------------------------------------------------------------------------
void glutDisplay (void)
//------------------------------------------------------------------------
{
	

	//kevin keypress
	keyupdate();
	//end
	
	//Edwin 06032013
	//keyUpdateFromGesture();
	//end

	cnt++;
	//print memory info. every 100 frames
	if(cnt>100) {
		//print the total graphics memory available
		GLint vidMemAvailable = 0;
		GLint totMemAvailable = 0;
		GLint curMemAvailable = 0;
		GLint       evicCount = 0;
		GLint      evicMemory = 0;
		glGetIntegerv(GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &vidMemAvailable);
		glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totMemAvailable);
		glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &curMemAvailable);
		glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX, &evicCount);
		glGetIntegerv(GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX, &evicMemory);
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");
		OutputDebugString(L"| Memory usage information: |             Memory (in kbs)          |\r\n");
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");
		wsprintf(buffer, L"|   Dedicated Video Memory  |                 %d              |\r\n",vidMemAvailable);
		OutputDebugString(buffer);
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");
		wsprintf(buffer, L"|   Total Avail. Memory     |                 %d              |\r\n", totMemAvailable);
		OutputDebugString(buffer);		
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");
		
		wsprintf(buffer, L"|   Avail. Current Memory   |                 %d              |\r\n", curMemAvailable);
		OutputDebugString(buffer);
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");

		wsprintf(buffer, L"|   Eviction Count          |                 %d                  |\r\n", evicCount);
		OutputDebugString(buffer);
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");

		wsprintf(buffer, L"|   Eviction Memory         |                 %d               |\r\n", evicMemory);
		OutputDebugString(buffer);
		OutputDebugString(L"+---------------------------+--------------------------------------+\r\n");
		cnt=0;
	}

	//check_gl_error

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update the buffers passed in constructor
	camera->Update();		
	skel->Update(camera->bUserChanged);
	camera->GetNeckOffset();

	//Render the backdrop first
	glEnable(GL_TEXTURE_2D);
	//kevin
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	//end
	glBindTexture(GL_TEXTURE_2D, bgTexID);
	DrawFullScreenQuad();
	//check_gl_error
	//Render the live scene	
	glBindTexture(GL_TEXTURE_2D, rgbTexID); 
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,COLOR_RES_X, COLOR_RES_Y, GL_BGRA, GL_UNSIGNED_BYTE, (DWORD*)cImg->imageData);
	//check_gl_error
	DrawFullScreenQuad();
	//check_gl_error
	glDisable(GL_TEXTURE_2D);

	if(showclothesflag){

		if(bShowDress3D) 
		{
			if(oldGarmentID_3D != garmentid) {
				
				stats3D[garmentid-1].counts++;
				oldGarmentID_3D = garmentid;
			}
			//check_gl_error
			pCloth3D->Draw();
			//check_gl_error
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

			//check_gl_error
			int nwidth = mcloth->garment->iplImg_4ch->width;
			int nheight = mcloth->garment->iplImg_4ch->height;
			glEnable(GL_BLEND);		
			glEnable(GL_TEXTURE_2D);
			//kevin
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ); 
			//end
			glBindTexture(GL_TEXTURE_2D, clothTexID);

			//check_gl_error

			//bind the pbo so that the texture is cleared
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
			glTexSubImage2D(GL_TEXTURE_2D,0,0,0,2048, 2048, GL_BGRA, GL_UNSIGNED_BYTE, 0);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		
			//check_gl_error


				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nwidth, nheight,GL_RGBA, GL_UNSIGNED_BYTE, mcloth->garment->iplImg_4ch->imageData); 
			
			
			GLuint error = glGetError();

			//check_gl_error
			mcloth->Draw();
			//check_gl_error
			glDisable(GL_TEXTURE_2D); 
			glDisable(GL_BLEND);
						
	
		}
		//check_gl_error
		if(bShowBag) 
		{
			if(oldGarmentID_B != garmentid) {
				oldGarmentID_B = garmentid;
				statsBags[garmentid-1].counts++;	
			}
			   
			glEnable(GL_TEXTURE_2D);
			//kevin
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			//end
			glBindTexture(GL_TEXTURE_2D, accessoryTexID);
			//the bag
			int x1=0, y1=0, x2=0, y2=0;
			mcloth->GetPosition(1, &x1, &x2, &y1, &y2); 
			glEnable(GL_BLEND);
			//check_gl_error
				//removed by CCL
				//showImage(mcloth->bag->iplImg_4ch->imageData, mcloth->bag->iplImg_4ch->width, mcloth->bag->iplImg_4ch->height, x1, x2, y1, y2, mcloth->bag->iplImg_4ch->nChannels);	
			//Added by CCL -- because the bag texture has a faint line. texture coordinates
			//modified to removed it
				int nwidth=mcloth->bag->iplImg_4ch->width;
			int nheight=mcloth->bag->iplImg_4ch->height;

				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, nwidth, nheight,GL_RGBA, GL_UNSIGNED_BYTE, mcloth->bag->iplImg_4ch->imageData); 

			//CCL-The entire chunk of"showImage" is replicated here.
			int nTextImageSize = 2048;
			glBegin(GL_QUADS);	
			glColor4f(1,1,1,1);
			glTexCoord2f(0, 0); // upper left
			glVertex2f(x1, y1);	
			
			glTexCoord2f((float)(nwidth-2)/(float)nTextImageSize, 0); // upper right //ccl minus 2px to removed thin line
			glVertex2f(x2, y1);	

			glTexCoord2f((float)(nwidth-2)/(float)nTextImageSize, (float)(nheight-2)/(float)nTextImageSize);// bottom right
			glVertex2f(x2, y2);

			glTexCoord2f(0, (float)(nheight-2)/(float)nTextImageSize);// bottom left
			glVertex2f(x1, y2);
			glEnd();
			
			//check_gl_error
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
		}
		//check_gl_error
	}	
	//check_gl_error

	//gesture based interaction
	gui();

	//check_gl_error
		glEnable(GL_TEXTURE_2D);
	//kevin start
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glBindTexture(GL_TEXTURE_2D, framesTexID);
		glEnable(GL_BLEND);
	//check_gl_error
	if(frame1flag){
		showclothesflag = true; 
		showcurrent();
		//showleft();
		//showright();
	//kevin end
		//NEW 240113
/*		//replaced by v2
		if(bBagIsActive){
			DrawBG(dataf1);
		}else{

			if(ismale){
				DrawBG(dataf1n);//male clothes no bags option
			}else{
				DrawBG(dataf1c);//female
			}
		}//end of if(bBagIsActive)
	*/
		//v2 by SF
		if (showWheel) {
			if (showPanel) {
				switch (panelFlag) {
					case 0:
						DrawBG(dataWPHome);
						break;
					case 1:
						DrawBG(dataWPLadies);
						break;
					case 2:
						DrawBG(dataWPMen);
						break;
					case 3:
						DrawBG(dataWPBag);
						break;
					case 4:
						DrawBG(dataWP3d);
						break;
					default:
						DrawBG(dataWP);
				}
			}
			else {
				switch (menuFlag) {
					case 0:
						DrawBG(dataWHome);
						break;
					case 1:
						DrawBG(dataWDetails);
						break;
					case 2:
						DrawBG(dataWPhoto);
						break;
					case 3:
						DrawBG(dataWAdd);
						break;
					default:
						DrawBG(dataWDefault);
				}
			}

			//kevin wheel frames start
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				glTranslatef(370 + 400 +orbitRange, 960/2 + 25, 0);
				//Draw Middle Frame 1
				glPushMatrix();
				degInRad = clockwiseTraversing*DEG2RAD;
				glTranslatef(cos(degInRad)*orbitRange,sin(degInRad)*orbitRange,0);
				glScalef(1.0*fscale1, 1.0*fscale1, 1.0*fscale1);
				DrawFrames(dataFrameBig, wFrameBig, hFrameBig, 1);
				glPopMatrix();
				//Draw two up from middle Frame 2
				glPushMatrix();
					degInRad2 = clockwiseTraversing2*DEG2RAD;
					glTranslatef(cos(degInRad2)*orbitRange,sin(degInRad2)*orbitRange,0);
					glScalef(1.0*fscale2, 1.0*fscale2, 1.0*fscale2);
					DrawFrames(dataFrameBig, wFrameBig, hFrameBig, 2);
				glPopMatrix();
				//Draw one up from middle Frame 3
				glPushMatrix();
					degInRad3 = clockwiseTraversing3*DEG2RAD;
					glTranslatef(cos(degInRad3)*orbitRange,sin(degInRad3)*orbitRange,0);
					glScalef(1.0*fscale3, 1.0*fscale3, 1.0*fscale3);
					DrawFrames(dataFrameBig, wFrameBig, hFrameBig, 3);
				glPopMatrix();
				//Draw one down from middle Frame 4
				glPushMatrix();
					degInRad4 = clockwiseTraversing4*DEG2RAD;
					glTranslatef(cos(degInRad4)*orbitRange,sin(degInRad4)*orbitRange,0);
					glScalef(1.0*fscale4, 1.0*fscale4, 1.0*fscale4);
					DrawFrames(dataFrameBig, wFrameBig, hFrameBig, 4);
				glPopMatrix();
				//Draw two down from middle Frame 5
				glPushMatrix();
					degInRad5 = clockwiseTraversing5*DEG2RAD;
					glTranslatef(cos(degInRad5)*orbitRange,sin(degInRad5)*orbitRange,0);
					glScalef(1.0*fscale5, 1.0*fscale5, 1.0*fscale5);
					DrawFrames(dataFrameBig, wFrameBig, hFrameBig, 5);
				glPopMatrix();
				//Draw three up from middle Frame 6
				glPushMatrix();
					degInRad6 = clockwiseTraversing6*DEG2RAD;
					glTranslatef(cos(degInRad6)*orbitRange,sin(degInRad6)*orbitRange,0);
					glScalef(1.0*fscale6, 1.0*fscale6, 1.0*fscale6);
					DrawFrames(dataFrameBig, wFrameBig, hFrameBig, 6);
				glPopMatrix();
			glPopMatrix();
			//move wheel
			moveWheel();
		//end kevin wheel frames

		}
		else {
			if (showPanel) {
				switch (panelFlag) {
					case 0:
						DrawBG(dataPHome);
						break;
					case 1:
						DrawBG(dataPLadies);
						break;
					case 2:
						DrawBG(dataPMen);
						break;
					case 3:
						DrawBG(dataPBag);
						break;
					case 4:
						DrawBG(dataP3d);
						break;
					default:
						DrawBG(dataP);
				}
			}
			else {
				switch (menuFlag) {
					case 0:
						DrawBG(dataHome);
						break;
					case 1:
						DrawBG(dataDetails);
						break;
					case 2:
						DrawBG(dataPhoto);
						break;
					case 3:
						DrawBG(dataAdd);
						break;
					default:
						DrawBG(dataDefault);
				}
			}
		}

	}
	//check_gl_error
	//for splash page
	if(frame2flag){
		showclothesflag = false;
		detail1flag = false; 
		oldGarmentID_3D = -1;
		oldGarmentID_F = -1;
		oldGarmentID_M = -1;
		oldGarmentID_B = -1;
		DrawBG(dataWDefault);
	}
	//check_gl_error
//END
	if(detail1flag){
		if(bBagIsActive){			
			//showframe(datadbag);
			//added by Mobeen 23 Oct 2012			
			//showframe(detailsDataBag[mcloth->GetBagId()-1]);
			//sprintf(newpath,"%sDetails/Bag/DB%d.png",path, mcloth->GetBagId()); //ccl removed
			sprintf(newpath, "%s", mcloth->bag->detail_name.c_str() );	//ccl added
		}else{
			if(ismale){
				//showframe(datad2);
				//showframe(detailsDataMale[garmentid-1]);
				///showframe(detailsDataMale[mcloth->GetGarmentId()-1]);
				//sprintf(newpath,"%sDetails/Male/DM%d.png",path, mcloth->GetGarmentId());	//ccl removed
				sprintf(newpath, "%s", mcloth->garment->detail_name.c_str() );	//ccl added
			}else{
				//showframe(datad1);
				if(bShowDress3D)
					///showframe(detailsData3D[garmentid-1]);
					//sprintf(newpath,"%sDetails/3D/D3D%d.png",path, ids3DGarment[( (garmentid-1) % NUM_3DCLOTHES)]);	//ccl removed
					sprintf(newpath, "%s", pCloth3D->GetDetailPath().c_str() );	//ccl added
				else
					///showframe(detailsDataFemale[mcloth->GetGarmentId()-1]);
					//sprintf(newpath,"%sDetails/Female/DF%d.png",path, mcloth->GetGarmentId());	//ccl removed
					sprintf(newpath, "%s", mcloth->garment->detail_name.c_str() ); //ccl added
			}
		}

		int w=0, h=0, c=0;
		unsigned char* data = SOIL_load_image(newpath,&w,&h,&c,SOIL_LOAD_AUTO);		
		//showframe(data);
		DrawBG(data);
		delete [] data; 

	}
	//check_gl_error
	if(previewflag){
		detail1flag = false;
		showclothesflag = false;
		DrawBG(datap1);
		//showframe(datap1);
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
	//check_gl_error
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
			//kevin
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			//end
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
	//check_gl_error
	//prinitng something on screen
	
	//PrintOnScreen(sizeof(Skeleton), "Sizeof(vec6) = ", 500, 250);
	//PrintOnScreen(skel->bInStandardPose, "Standard Pose = ", 500, 250);
	//PrintOnScreen(skel->shoulder_rotation, "Shoulder Rotation = ", 500, 300);
	
	 
	//PrintOnScreen(garmentid-1, "ID = ", 500, 200);
	
	//PrintOnScreen(statsMale[garmentid-1].counts, "Male Count = ", 500, 250);
	//PrintOnScreen(statsFemale[garmentid-1].counts, "Female Count = ", 500, 275);

	/*PrintOnScreen(statsMale[garmentid-1].totalTimeSpent, "Time Male = ", 500, 250);
	PrintOnScreen(statsFemale[garmentid-1].totalTimeSpent, "Time Female = ", 500, 275);
	PrintOnScreen(stats3D[garmentid-1].totalTimeSpent, "Time 3D = ", 500, 300);
	PrintOnScreen(statsBags[garmentid-1].totalTimeSpent, "Time Bags= ", 500, 325); 
*/
	/*PrintOnScreen(statsMale[garmentid-1].nUploads, "Pictures Male = ", 500, 250);
	PrintOnScreen(statsFemale[garmentid-1].nUploads, "Pictures Female = ", 500, 275);
	PrintOnScreen(statsBags[garmentid-1].nUploads, "Pictures Bags = ", 500, 300);
	PrintOnScreen(stats3D[garmentid-1].nUploads, "Pictures 3D = ", 500, 350);*/
 
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
		//CCL Changed for XML Load
		//if(bShowDress2D||bShowBag) mcloth->Update(MCLOTH_DRESS_SAVE);
		//if(bShowDress3D) {
		//	sprintf(newpath,"%sModels/YOffsets.csv", path);
		//	pCloth3D->SaveYOffsets(newpath);
		//}
		if (bShowDress2D||bShowBag)
			Save2DGarmentsXML(clothesXML); 
		else if (bShowDress3D)
			Save3DGarmentsXML(clothesXML);
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
	timer.Stop();

	//ccl removed for XML Load
	//load the details of bags
	//for(int i=0;i<NUM_BAGS;i++) {
	//	delete [] detailsDataBag[i];
	//}
	//for(int i=0;i<NUM_DRESSES_FEMALE;i++) {
	//	delete [] detailsDataFemale[i];		
	//}
	//for(int i=0;i<NUM_DRESSES_MALE;i++) {
	//	delete [] detailsDataMale[i];		
	//}
	//for(int i=0;i<NUM_3DCLOTHES;i++) {
	//	delete [] detailsData3D[i];		
	//}
	 
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
	//TerminateProcess(hAudio, EXIT_SUCCESS);

}

bool DirectoryExists(const char* dirName) {
   DWORD attribs = ::GetFileAttributesA(dirName);
   if (attribs == INVALID_FILE_ATTRIBUTES) {
	  return false;
   }
   return (attribs & FILE_ATTRIBUTE_DIRECTORY);
} 

char directory[MAX_PATH]={'\0'};

//kevin functions
//smoothly slide nearest frame to center when user stops scrolling wheel
void smoothWheelToCenter()
{
	if (clockwiseTraversingIncrement==0) return;
	if (clockwiseTraversing==180||clockwiseTraversing2==180||clockwiseTraversing3==180||clockwiseTraversing4==180||clockwiseTraversing5==180||clockwiseTraversing6==180)
	{
		clockwiseTraversingIncrement = 0;
		return;
	}
	if (clockwiseTraversingIncrement > 0){
		clockwiseTraversingIncrement = 2.0;
	}
	else if (clockwiseTraversingIncrement < 0){
		clockwiseTraversingIncrement = -2.0;
	}
}
//loads wheel positions from memory
void loadWheel()
{
	if(bBagIsActive){
		clockwiseTraversing=clockwiseTraversingb;
		clockwiseTraversing2=clockwiseTraversing2b;
		clockwiseTraversing3=clockwiseTraversing3b;
		clockwiseTraversing4=clockwiseTraversing4b;
		clockwiseTraversing5=clockwiseTraversing5b;
		clockwiseTraversing6=clockwiseTraversing6b;
		fscale1=fscale1b;
		fscale2=fscale2b;
		fscale3=fscale3b;
		fscale4=fscale4b;
		fscale5=fscale5b;
		fscale6=fscale6b;
		fSelect=fSelect_b;
	}
	else if(!bShowDress3D){
		if (ismale){
			clockwiseTraversing=clockwiseTraversingm;
			clockwiseTraversing2=clockwiseTraversing2m;
			clockwiseTraversing3=clockwiseTraversing3m;
			clockwiseTraversing4=clockwiseTraversing4m;
			clockwiseTraversing5=clockwiseTraversing5m;
			clockwiseTraversing6=clockwiseTraversing6m;
			fscale1=fscale1m;
			fscale2=fscale2m;
			fscale3=fscale3m;
			fscale4=fscale4m;
			fscale5=fscale5m;
			fscale6=fscale6m;
			fSelect=fSelect_m;
		}
		else {
			clockwiseTraversing=clockwiseTraversingf;
			clockwiseTraversing2=clockwiseTraversing2f;
			clockwiseTraversing3=clockwiseTraversing3f;
			clockwiseTraversing4=clockwiseTraversing4f;
			clockwiseTraversing5=clockwiseTraversing5f;
			clockwiseTraversing6=clockwiseTraversing6f;
			fscale1=fscale1f;
			fscale2=fscale2f;
			fscale3=fscale3f;
			fscale4=fscale4f;
			fscale5=fscale5f;
			fscale6=fscale6f;
			fSelect=fSelect_f;
		}
	}
}
//save current wheel
void saveWheel()
{
	if(bBagIsActive){
		fSelect_b=fSelect;
		garmentid_b=garmentid;
		clockwiseTraversingb=clockwiseTraversing;
		clockwiseTraversing2b=clockwiseTraversing2;
		clockwiseTraversing3b=clockwiseTraversing3;
		clockwiseTraversing4b=clockwiseTraversing4;
		clockwiseTraversing5b=clockwiseTraversing5;
		clockwiseTraversing6b=clockwiseTraversing6;
		fscale1b=fscale1;
		fscale2b=fscale2;
		fscale3b=fscale3;
		fscale4b=fscale4;
		fscale5b=fscale5;
		fscale6b=fscale6;
	}
	else if(!bShowDress3D){
		if (ismale){
			fSelect_m=fSelect;
			garmentid_m=garmentid;
			clockwiseTraversingm=clockwiseTraversing;
			clockwiseTraversing2m=clockwiseTraversing2;
			clockwiseTraversing3m=clockwiseTraversing3;
			clockwiseTraversing4m=clockwiseTraversing4;
			clockwiseTraversing5m=clockwiseTraversing5;
			clockwiseTraversing6m=clockwiseTraversing6;
			fscale1m=fscale1;
			fscale2m=fscale2;
			fscale3m=fscale3;
			fscale4m=fscale4;
			fscale5m=fscale5;
			fscale6m=fscale6;
		}
		else {
			fSelect_f=fSelect;
			garmentid_f=garmentid;
			clockwiseTraversingf=clockwiseTraversing;
			clockwiseTraversing2f=clockwiseTraversing2;
			clockwiseTraversing3f=clockwiseTraversing3;
			clockwiseTraversing4f=clockwiseTraversing4;
			clockwiseTraversing5f=clockwiseTraversing5;
			clockwiseTraversing6f=clockwiseTraversing6;
			fscale1f=fscale1;
			fscale2f=fscale2;
			fscale3f=fscale3;
			fscale4f=fscale4;
			fscale5f=fscale5;
			fscale6f=fscale6;
		}
	}
}
//check frame selection and load proper icon id for it
void checkfSelect()
{
	int selected_id;
	if(bBagIsActive){
		switch(fSelect)
		{
			case 2:
				selected_id=b_id2=mcloth->GetBagId();
				b_id5 = selected_id - 2;
				b_id6 = selected_id - 1;
				b_id3 = selected_id + 1;
				b_id1 = selected_id + 2;
				b_id4 = selected_id - 3;
				break;
			case 3:
				selected_id=b_id3=mcloth->GetBagId();
				b_id6 = selected_id - 2;
				b_id2 = selected_id - 1;
				b_id1 = selected_id + 1;
				b_id4 = selected_id + 2;
				b_id5 = selected_id - 3;
				break;
			case 4:
				selected_id=b_id4=mcloth->GetBagId();
				b_id3 = selected_id - 2;
				b_id1 = selected_id - 1;
				b_id5 = selected_id + 1;
				b_id6 = selected_id + 2;
				b_id2 = selected_id - 3;
				break;
			case 5:
				selected_id=b_id5=mcloth->GetBagId();
				b_id1 = selected_id - 2;
				b_id4 = selected_id - 1;
				b_id6 = selected_id + 1;
				b_id2 = selected_id + 2;
				b_id3 = selected_id - 3;
				break;
			case 6:
				selected_id=b_id6=mcloth->GetBagId();
				b_id4 = selected_id - 2;
				b_id5 = selected_id - 1;
				b_id2 = selected_id + 1;
				b_id3 = selected_id + 2;
				b_id1 = selected_id - 3;
				break;
			default:
				selected_id=b_id1=mcloth->GetBagId();
				b_id2 = selected_id - 2;
				b_id3 = selected_id - 1;
				b_id4 = selected_id + 1;
				b_id5 = selected_id + 2;
				b_id6 = selected_id - 3;
				break;
		}
		if(b_id1<=0) b_id1=maxBags+b_id1;
		else if (b_id1>maxBags) b_id1=b_id1-maxBags;
		if(b_id2<=0) b_id2=maxBags+b_id2;
		else if (b_id2>maxBags) b_id2=b_id2-maxBags;
		if(b_id3<=0) b_id3=maxBags+b_id3;
		else if (b_id3>maxBags) b_id3=b_id3-maxBags;
		if(b_id4<=0) b_id4=maxBags+b_id4;
		else if (b_id4>maxBags) b_id4=b_id4-maxBags;
		if(b_id5<=0) b_id5=maxBags+b_id5;
		else if (b_id5>maxBags) b_id5=b_id5-maxBags;
		if(b_id6<=0) b_id6=maxBags+b_id6;
		else if (b_id6>maxBags) b_id6=b_id6-maxBags;
	}
	else if(!bShowDress3D){
		if (ismale){
			switch(fSelect)
			{
				case 2:
					selected_id=m_id2=mcloth->GetGarmentId();
					m_id5 = selected_id - 2;
					m_id6 = selected_id - 1;
					m_id3 = selected_id + 1;
					m_id1 = selected_id + 2;
					m_id4 = selected_id - 3;
					break;
				case 3:
					selected_id=m_id3=mcloth->GetGarmentId();
					m_id6 = selected_id - 2;
					m_id2 = selected_id - 1;
					m_id1 = selected_id + 1;
					m_id4 = selected_id + 2;
					m_id5 = selected_id - 3;
					break;
				case 4:
					selected_id=m_id4=mcloth->GetGarmentId();
					m_id3 = selected_id - 2;
					m_id1 = selected_id - 1;
					m_id5 = selected_id + 1;
					m_id6 = selected_id + 2;
					m_id2 = selected_id - 3;
					break;
				case 5:
					selected_id=m_id5=mcloth->GetGarmentId();
					m_id1 = selected_id - 2;
					m_id4 = selected_id - 1;
					m_id6 = selected_id + 1;
					m_id2 = selected_id + 2;
					m_id3 = selected_id - 3;
					break;
				case 6:
					selected_id=m_id6=mcloth->GetGarmentId();
					m_id4 = selected_id - 2;
					m_id5 = selected_id - 1;
					m_id2 = selected_id + 1;
					m_id3 = selected_id + 2;
					m_id1 = selected_id - 3;
					break;
				default:
					selected_id=m_id1=mcloth->GetGarmentId();
					m_id2 = selected_id - 2;
					m_id3 = selected_id - 1;
					m_id4 = selected_id + 1;
					m_id5 = selected_id + 2;
					m_id6 = selected_id - 3;
					break;
			}
			if(m_id1<=0) m_id1=maxMale+m_id1;
			else if (m_id1>maxMale) m_id1=m_id1-maxMale;
			if(m_id2<=0) m_id2=maxMale+m_id2;
			else if (m_id2>maxMale) m_id2=m_id2-maxMale;
			if(m_id3<=0) m_id3=maxMale+m_id3;
			else if (m_id3>maxMale) m_id3=m_id3-maxMale;
			if(m_id4<=0) m_id4=maxMale+m_id4;
			else if (m_id4>maxMale) m_id4=m_id4-maxMale;
			if(m_id5<=0) m_id5=maxMale+m_id5;
			else if (m_id5>maxMale) m_id5=m_id5-maxMale;
			if(m_id6<=0) m_id6=maxMale+m_id6;
			else if (m_id6>maxMale) m_id6=m_id6-maxMale;
		}
		else {
			switch(fSelect)
			{
				case 2:
					selected_id=f_id2=mcloth->GetGarmentId();
					f_id5 = selected_id - 2;
					f_id6 = selected_id - 1;
					f_id3 = selected_id + 1;
					f_id1 = selected_id + 2;
					f_id4 = selected_id - 3;
					break;
				case 3:
					selected_id=f_id3=mcloth->GetGarmentId();
					f_id6 = selected_id - 2;
					f_id2 = selected_id - 1;
					f_id1 = selected_id + 1;
					f_id4 = selected_id + 2;
					f_id5 = selected_id - 3;
					break;
				case 4:
					selected_id=f_id4=mcloth->GetGarmentId();
					f_id3 = selected_id - 2;
					f_id1 = selected_id - 1;
					f_id5 = selected_id + 1;
					f_id6 = selected_id + 2;
					f_id2 = selected_id - 3;
					break;
				case 5:
					selected_id=f_id5=mcloth->GetGarmentId();
					f_id1 = selected_id - 2;
					f_id4 = selected_id - 1;
					f_id6 = selected_id + 1;
					f_id2 = selected_id + 2;
					f_id3 = selected_id - 3;
					break;
				case 6:
					selected_id=f_id6=mcloth->GetGarmentId();
					f_id4 = selected_id - 2;
					f_id5 = selected_id - 1;
					f_id2 = selected_id + 1;
					f_id3 = selected_id + 2;
					f_id1 = selected_id - 3;
					break;
				default:
					selected_id=f_id1=mcloth->GetGarmentId();
					f_id2 = selected_id - 2;
					f_id3 = selected_id - 1;
					f_id4 = selected_id + 1;
					f_id5 = selected_id + 2;
					f_id6 = selected_id - 3;
					break;
			}
			if(f_id1<=0) f_id1=maxFemale+f_id1;
			else if (f_id1>maxFemale) f_id1=f_id1-maxFemale;
			if(f_id2<=0) f_id2=maxFemale+f_id2;
			else if (f_id2>maxFemale) f_id2=f_id2-maxFemale;
			if(f_id3<=0) f_id3=maxFemale+f_id3;
			else if (f_id3>maxFemale) f_id3=f_id3-maxFemale;
			if(f_id4<=0) f_id4=maxFemale+f_id4;
			else if (f_id4>maxFemale) f_id4=f_id4-maxFemale;
			if(f_id5<=0) f_id5=maxFemale+f_id5;
			else if (f_id5>maxFemale) f_id5=f_id5-maxFemale;
			if(f_id6<=0) f_id6=maxFemale+f_id6;
			else if (f_id6>maxFemale) f_id6=f_id6-maxFemale;
		}
	}
}
//check if key is held down or released
void glutKeyboardUp (unsigned char key, int x, int y)
{
	key_state[key] = false;
}
void keyupdate()
{
	
	if (key_state['j']==true){
		clockwiseTraversingIncrement = (double)rotateIncrement;
	}
	else if (key_state['k']==true){
		clockwiseTraversingIncrement = (double)(-rotateIncrement);
	}

	else if (camera->gesCtrl->GetGesture(1)->isDetected() == true) {
		//take current clockwiseTraversing value and store it 
		//tempValueTurnedA = clockwiseTraversing;
		//printf_s("This is the degree traversed %d. \n", tempValueTurnedA);
		clockwiseTraversingIncrement = (double)6;
		//if current icons travelled for x amount distance
		if (clockwiseTraversing < 124) {
			//set it to false
			camera->gesCtrl->GetGesture(1)->alterDetectedVal(false);
		}
		
	}
	//else if the waving right gesture is detected
	else if (camera->gesCtrl->GetGesture(2)->isDetected() == true) {
		clockwiseTraversingIncrement = (double)(-6);
		if (clockwiseTraversing > 236) {
			camera->gesCtrl->GetGesture(2)->alterDetectedVal(false);
		}
	}
	else {
		smoothWheelToCenter();
	}
}

//Edwin 06032013
void keyUpdateFromGesture() 
{
	/*
	pseudocode
	if wave left detected
	turn wheel down for a certain amount of degrees
	set the gesture detected boolean value to false
	else if wave right detected
	turn wheel up for a certain amount of degrees
	set the gesture detected boolean value to false
	else
	smooth wheel to centre
	*/

	//edit accordingly
	//if the waving left gesture is detected
	
	if (camera->gesCtrl->GetGesture(1)->isDetected() == true) {
		clockwiseTraversingIncrement = (double)6;
		//camera->gesCtrl->GetGesture(1)->alterDetectedVal(false);
		//if current icons travelled for x amount distance
		if (clockwiseTraversing < 150) {
			camera->gesCtrl->GetGesture(1)->alterDetectedVal(false);
		}
		//set it to false
	}
	//else if the waving right gesture is detected
	else if (camera->gesCtrl->GetGesture(2)->isDetected() == true) {
		clockwiseTraversingIncrement = (double)(-6);
		if (clockwiseTraversing > 236) {
			camera->gesCtrl->GetGesture(2)->alterDetectedVal(false);
		}
		//camera->gesCtrl->GetGesture(2)->alterDetectedVal(false);
	}
	else {
		//camera->gesCtrl->GetGesture(1)->alterDetectedVal(false);
		//camera->gesCtrl->GetGesture(2)->alterDetectedVal(false);
		smoothWheelToCenter();
	}
	
}
//end

//wheel movements
void moveWheel()
{
	//Middle center frame, Frame 1
	//region nearest to center
	if(clockwiseTraversing <= centerRegionUpperBound && clockwiseTraversing >= centerRegionLowerBound)
	{
		clockwiseTraversing -= clockwiseTraversingIncrement * offsetCenterMultiplier;
		if (clockwiseTraversingIncrement > 0 && clockwiseTraversing < 180) {
			//if direction down and is below center
			if (fscale1 > fscaleMin) fscale1 -= 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing < 180) {
			//if direction up and is below center
			if (fscale1 < fscaleMax) fscale1 += 0.1;
		}
		else if (clockwiseTraversingIncrement > 0 && clockwiseTraversing > 180) {
			//if direction down and is above center
			if (fscale1 < fscaleMax) fscale1 += 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing > 180) {
			//if direction up and is above center
			if (fscale1 > fscaleMin) fscale1 -= 0.1;
		}
		//check if old selection was adjacent frames, i.e if was previously frame3 above, go next	
		if (fSelect==3){
			scrollLeftButton(); //go next
		}
		else if (fSelect==4){
			scrollRightButton(); //go prev
		}
		fSelect=1;
	}
	//region abit further from center
	else if(clockwiseTraversing <= centerLargeRegionUpperBound && clockwiseTraversing >= centerLargeRegionLowerBound)
	{
		clockwiseTraversing -= clockwiseTraversingIncrement * offsetCenterLargeMultiplier;
	}
	//rest of region far from center
	else
	{
		clockwiseTraversing -= clockwiseTraversingIncrement * normalMultiplier;
	}
	//check if frame at bottom of wheel needs to flip back to top, else vice versa
	if (clockwiseTraversing <=rLowerBound)
	{
		clockwiseTraversing += rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id1 -= 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id1 -= 6;
			}
			else {
				f_id1 -= 6;
			}
		}
	}
	else if(clockwiseTraversing >=rUpperBound)
	{
		clockwiseTraversing -= rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id1 += 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id1 += 6;
			}
			else {
				f_id1 += 6;
			}
		}
	}
	//Two frames up from center, Frame 2
	if(clockwiseTraversing2 <= centerRegionUpperBound && clockwiseTraversing2 >= centerRegionLowerBound)
	{
		clockwiseTraversing2 -= clockwiseTraversingIncrement * offsetCenterMultiplier;
		if (clockwiseTraversingIncrement > 0 && clockwiseTraversing2 < 180) {
			//if direction down and is below center
			if (fscale2 > fscaleMin) fscale2 -= 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing2 < 180) {
			//if direction up and is below center
			if (fscale2 < fscaleMax) fscale2 += 0.1;
		}
		else if (clockwiseTraversingIncrement > 0 && clockwiseTraversing2 > 180) {
			//if direction down and is above center
			if (fscale2 < fscaleMax) fscale2 += 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing2 > 180) {
			//if direction up and is above center
			if (fscale2 > fscaleMin) fscale2 -= 0.1;
		}
		//check if old selection was adjacent frames
		if (fSelect==6){
			scrollLeftButton(); //go next
		}
		else if (fSelect==3){
			scrollRightButton(); //go prev
		}
		fSelect=2;
	}
	else if(clockwiseTraversing2 <= centerLargeRegionUpperBound && clockwiseTraversing2 >= centerLargeRegionLowerBound)
	{
		clockwiseTraversing2 -= clockwiseTraversingIncrement * offsetCenterLargeMultiplier;
	}
	else
	{
		clockwiseTraversing2 -= clockwiseTraversingIncrement * normalMultiplier;
	}
	if (clockwiseTraversing2 <=rLowerBound)
	{
		clockwiseTraversing2 += rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id2 -= 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id2 -= 6;
			}
			else {
				f_id2 -= 6;
			}
		}
	}
	else if(clockwiseTraversing2 >=rUpperBound)
	{
		clockwiseTraversing2 -= rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id2 += 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id2 += 6;
			}
			else {
				f_id2 += 6;
			}
		}
	}
	//One frame up from center, Frame 3
	if(clockwiseTraversing3 <= centerRegionUpperBound && clockwiseTraversing3 >= centerRegionLowerBound)
	{
		clockwiseTraversing3 -= clockwiseTraversingIncrement * offsetCenterMultiplier;
		if (clockwiseTraversingIncrement > 0 && clockwiseTraversing3 < 180) {
			//if direction down and is below center
			if (fscale3 > fscaleMin) fscale3 -= 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing3 < 180) {
			//if direction up and is below center
			if (fscale3 < fscaleMax) fscale3 += 0.1;
		}
		else if (clockwiseTraversingIncrement > 0 && clockwiseTraversing3 > 180) {
			//if direction down and is above center
			if (fscale3 < fscaleMax) fscale3 += 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing3 > 180) {
			//if direction up and is above center
			if (fscale3 > fscaleMin) fscale3 -= 0.1;
		}
		//check if old selection was adjacent frames
		if (fSelect==2){
			scrollLeftButton(); //go next
		}
		else if (fSelect==1){
			scrollRightButton(); //go prev
		}
		fSelect=3;
	}
	else if(clockwiseTraversing3 <= centerLargeRegionUpperBound && clockwiseTraversing3 >= centerLargeRegionLowerBound)
	{
		clockwiseTraversing3 -= clockwiseTraversingIncrement * offsetCenterLargeMultiplier;
	}
	else
	{
		clockwiseTraversing3 -= clockwiseTraversingIncrement * normalMultiplier;
	}
	if (clockwiseTraversing3 <=rLowerBound)
	{
		clockwiseTraversing3 += rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id3 -= 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id3 -= 6;
			}
			else {
				f_id3 -= 6;
			}
		}
	}
	else if(clockwiseTraversing3 >=rUpperBound)
	{
		clockwiseTraversing3 -= rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id3 += 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id3 += 6;
			}
			else {
				f_id3 += 6;
			}
		}
	}
	//One frame down from center, Frame4
	if(clockwiseTraversing4 <= centerRegionUpperBound && clockwiseTraversing4 >= centerRegionLowerBound)
	{
		clockwiseTraversing4 -= clockwiseTraversingIncrement * offsetCenterMultiplier;
		if (clockwiseTraversingIncrement > 0 && clockwiseTraversing4 < 180) {
			//if direction down and is below center
			if (fscale4 > fscaleMin) fscale4 -= 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing4 < 180) {
			//if direction up and is below center
			if (fscale4 < fscaleMax) fscale4 += 0.1;
		}
		else if (clockwiseTraversingIncrement > 0 && clockwiseTraversing4 > 180) {
			//if direction down and is above center
			if (fscale4 < fscaleMax) fscale4 += 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing4 > 180) {
			//if direction up and is above center
			if (fscale4 > fscaleMin) fscale4 -= 0.1;
		}
		//check if old selection was adjacent frames
		if (fSelect==1){
			scrollLeftButton(); //go next
		}
		else if (fSelect==5){
			scrollRightButton(); //go prev
		}
		fSelect=4;
	}
	else if(clockwiseTraversing4 <= centerLargeRegionUpperBound && clockwiseTraversing4 >= centerLargeRegionLowerBound)
	{
		clockwiseTraversing4 -= clockwiseTraversingIncrement * offsetCenterLargeMultiplier;
	}
	else
	{
		clockwiseTraversing4 -= clockwiseTraversingIncrement * normalMultiplier;
	}
	if (clockwiseTraversing4 <=rLowerBound)
	{
		clockwiseTraversing4 += rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id4 -= 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id4 -= 6;
			}
			else {
				f_id4 -= 6;
			}
		}
	}
	else if(clockwiseTraversing4 >=rUpperBound)
	{
		clockwiseTraversing4 -= rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id4 += 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id4 += 6;
			}
			else {
				f_id4 += 6;
			}
		}
	}
	//Two frames down from center, Frame5
	if(clockwiseTraversing5 <= centerRegionUpperBound && clockwiseTraversing5 >= centerRegionLowerBound)
	{
		clockwiseTraversing5 -= clockwiseTraversingIncrement * offsetCenterMultiplier;
		if (clockwiseTraversingIncrement > 0 && clockwiseTraversing5 < 180) {
			//if direction down and is below center
			if (fscale5 > fscaleMin) fscale5 -= 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing5 < 180) {
			//if direction up and is below center
			if (fscale5 < fscaleMax) fscale5 += 0.1;
		}
		else if (clockwiseTraversingIncrement > 0 && clockwiseTraversing5 > 180) {
			//if direction down and is above center
			if (fscale5 < fscaleMax) fscale5 += 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing5 > 180) {
			//if direction up and is above center
			if (fscale5 > fscaleMin) fscale5 -= 0.1;
		}
		//check if old selection was adjacent frames
		if (fSelect==4){
			scrollLeftButton(); //go next
		}
		else if (fSelect==6){
			scrollRightButton(); //go prev
		}
		fSelect=5;
	}
	else if(clockwiseTraversing5 <= centerLargeRegionUpperBound && clockwiseTraversing5 >= centerLargeRegionLowerBound)
	{
		clockwiseTraversing5 -= clockwiseTraversingIncrement * offsetCenterLargeMultiplier;
	}
	else
	{
		clockwiseTraversing5 -= clockwiseTraversingIncrement * normalMultiplier;
	}
	if (clockwiseTraversing5 <=rLowerBound)
	{
		clockwiseTraversing5 += rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id5 -= 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id5 -= 6;
			}
			else {
				f_id5 -= 6;
			}
		}
	}
	else if(clockwiseTraversing5 >=rUpperBound)
	{
		clockwiseTraversing5 -= rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id5 += 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id5 += 6;
			}
			else {
				f_id5 += 6;
			}
		}
	}
	//Three frames up from center, Frame6
	if(clockwiseTraversing6 <= centerRegionUpperBound && clockwiseTraversing6 >= centerRegionLowerBound)
	{
		clockwiseTraversing6 -= clockwiseTraversingIncrement * offsetCenterMultiplier;
		if (clockwiseTraversingIncrement > 0 && clockwiseTraversing6 < 180) {
			//if direction down and is below center
			if (fscale6 > fscaleMin) fscale6 -= 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing6 < 180) {
			//if direction up and is below center
			if (fscale6 < fscaleMax) fscale6 += 0.1;
		}
		else if (clockwiseTraversingIncrement > 0 && clockwiseTraversing6 > 180) {
			//if direction down and is above center
			if (fscale6 < fscaleMax) fscale6 += 0.1;
		}
		else if (clockwiseTraversingIncrement < 0 && clockwiseTraversing6 > 180) {
			//if direction up and is above center
			if (fscale6 > fscaleMin) fscale6 -= 0.1;
		}
		//check if old selection was adjacent frames
		if (fSelect==5){
			scrollLeftButton(); //go next
		}
		else if (fSelect==2){
			scrollRightButton(); //go prev
		}
		fSelect=6;
	}
	else if(clockwiseTraversing6 <= centerLargeRegionUpperBound && clockwiseTraversing6 >= centerLargeRegionLowerBound)
	{
		clockwiseTraversing6 -= clockwiseTraversingIncrement * offsetCenterLargeMultiplier;
	}
	else
	{
		clockwiseTraversing6 -= clockwiseTraversingIncrement * normalMultiplier;
	}
	if (clockwiseTraversing6 <=rLowerBound)
	{
		clockwiseTraversing6 += rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id6 -= 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id6 -= 6;
			}
			else {
				f_id6 -= 6;
			}
		}
	}
	else if(clockwiseTraversing6 >=rUpperBound)
	{
		clockwiseTraversing6 -= rOffset;
		//reassign icon id after flip
		if(bBagIsActive){
			b_id6 += 6;
		}
		else if(!bShowDress3D){
			if (ismale){
				m_id6 += 6;
			}
			else {
				f_id6 += 6;
			}
		}
	}
}
//end kevin functions

//--------------------------------------------------------------
void glutKeyboard (unsigned char key, int x, int y)
//--------------------------------------------------------------
{
	//kevin
	key_state[key] = true;
	//end
	static int counter =0;
	HWND hWnd = FindWindowEx( NULL, NULL, L"IEFrame", NULL );
	HWND hWnd1 = FindWindowEx( NULL, NULL, L"VideoRenderer", NULL );
	int static dx = 0;
	switch(key)
	{
		case 27:
		case 'q':
			//Commented by Mobeen
			//This causes memory leak due to multiple deletions on exit 
			//since the camera is deleted again in the close function
			//Another solution is to set the deleted camera pointer to NULL
			//after deletion.
			//delete camera;
			fnExit();
			exit(0);
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
		case 'w':
			//adjustment to display user in center of TV screen
			//glutPositionWindow(-2030,-400);
			glutPositionWindow(50, 0);
			//set the size to large enugh to cover TV length and 
			glutReshapeWindow((1280)/1.2*ar, (960)/1.2);//2.35
			break;

		case '/':
			//adjustment to display user in center of TV screen
			//glutPositionWindow(-2030,-400);
			glutPositionWindow(600, 0);
			//set the size to large enugh to cover TV length and 
			glutReshapeWindow((1280)/1.2*ar, (960)/1.2);//2.35
			break;

		case 'n':
			if(bShowBag && bBagIsActive)
			{
				mcloth->Update(MCLOTH_BAG_NEXT);

			}
			else if(bShowDress3D)
				pCloth3D->ShowNextModel();	
			else
			{
				mcloth->Update(MCLOTH_DRESS_NEXT);

			}
			break;

		case 'b':
			if(bShowBag && bBagIsActive)
			{
				mcloth->Update(MCLOTH_BAG_PREV);

			}
			else if(bShowDress3D)
				pCloth3D->ShowPreviousModel();	
			else
			{
				mcloth->Update(MCLOTH_DRESS_PREV);
			}
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

		case ',':translate -=0.01f; break;
		case '.':translate +=0.01f; break;
	
		case 'a':
			++counter;
			if(counter==1) {
				time_t t = time(0);
				struct tm * now = localtime( & t );
			 
				sprintf(directory,"%slogs/%d%d%d",path, now->tm_mday,now->tm_mon+1,now->tm_year + 1900 );
				//create directory only if it does not exists already
				if(!DirectoryExists(directory)) {
					DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, directory, -1, NULL, 0);
					wchar_t *pwText = new wchar_t[dwNum];
					MultiByteToWideChar (CP_ACP, 0, directory, -1, pwText, dwNum );
					CreateDirectory(pwText,NULL); 
					delete [] pwText;
				}
			}
			sprintf(newpath, "%s/colorImage%d.jpg", directory, counter);
			cvSaveImage(newpath, cImg);
		
			sprintf(newpath, "%s/depthImage%d.png", directory, counter);
			cvSaveImage(newpath, dImg);
		
			sprintf(newpath, "%s/userImage%d.png", directory, counter);
			cvSaveImage(newpath, uImg); 
			break;
			/*
		case 'j':
			clockwiseTraversingIncrement = (double)rotateIncrement;
			break;
		case 'k':
			clockwiseTraversingIncrement = (double)(-rotateIncrement);
			break;
			*/
		// ----------------------------------------------------
		// Daryl Mcintyre - Keypress access to menu buttons
		// ----------------------------------------------------
			//------------
			// Main Menu
			//------------
		case '1':
			// View female clothes
			frame2flag = false;
			frame1flag = true;
			femalebutton();
			break;
		case '2':
			// View male clothes
			frame2flag = false;
			frame1flag = true;
			malebutton();
			break;
		case '3':
			// View bags
			frame2flag = false;
			frame1flag = true;
			bagbutton();
			break;
		case '4':
			// View 3D clothes
			//frame2flag = false;
			//frame1flag = true;
			//button3D();
			break;
			//----------------------
			// Browser Menu
			//----------------------
		case '5':
			// Return to Main Menu
			mainMenuButton();
			break;
		case '6':
			// Item Information
			infoButton();
			break;
		case '7':
			// Scroll Left of item menu
			scrollLeftButton();
			break;
		case '8':
			// Scroll right of item menu
			scrollRightButton();
			break;
		case '9':
			// Photo Button
			//photoButton();
			break;
		case '0':
			// Makea recommmendation
			recommendButton();
			break;
		//end
	}
}

const float TIMEOUT = 1000*10;//0.5f*60*60*1000;
//----------------------------------------------------------------
void glutIdle (void)
//----------------------------------------------------------------
{
	//printf("glutIdle");

	static float start = glutGet(GLUT_ELAPSED_TIME);
	float current = glutGet(GLUT_ELAPSED_TIME);

	

	//kevin
	keyupdate();
	//end

	//Edwin 06032013
	//keyUpdateFromGesture();
	//end

	if((current-start) > TIMEOUT) {
		//save stats
		time_t t = time(0);
		struct tm * now = localtime( &t );
		 
		sprintf(newpath,"%sStats/%d%d%d.csv",path, now->tm_mday,now->tm_mon+1,now->tm_year + 1900 );
		 
		FILE* f=fopen(newpath,"r");
		if(f!=NULL)
		{
			int dummy=0;
			char tmp[MAX_PATH]={'\0'};
			fgets(tmp, MAX_PATH, f);
			Stats s;
			for(int i=0;i<NUM_BAGS;i++)   {
				Stats& s1 = statsBags[i];
				fscanf(f, "Bag %d, %d, %d, %d, %f\n",&dummy, &s.counts, &s.nPictures, &s.nUploads, &s.totalTimeSpent);	 
				s1.counts += s.counts;
				s1.nPictures += s.nPictures;
				s1.nUploads += s.nUploads;
				s1.totalTimeSpent += s.totalTimeSpent;
			}
			for(int i=0;i<NUM_DRESSES_FEMALE;i++) {
				Stats& s1 = statsFemale[i];
				fscanf(f, "Cloth Female %d, %d, %d, %d, %f\n", &dummy, &s.counts, &s.nPictures, &s.nUploads, &s.totalTimeSpent);
				s1.counts += s.counts;
				s1.nPictures += s.nPictures;
				s1.nUploads += s.nUploads;
				s1.totalTimeSpent += s.totalTimeSpent;
			}
			for(int i=0;i<NUM_DRESSES_MALE;i++) {
				Stats& s1 = statsMale[i]; 
				fscanf(f, "Cloth Male %d, %d, %d, %d, %f\n", &dummy, &s.counts, &s.nPictures, &s.nUploads, &s.totalTimeSpent);
				s1.counts += s.counts;
				s1.nPictures += s.nPictures;
				s1.nUploads += s.nUploads;
				s1.totalTimeSpent += s.totalTimeSpent;
			}
			for(int i=0;i<NUM_3DCLOTHES;i++) {
				Stats& s1 = stats3D[i];
				fscanf(f, "Cloth3D %d, %d, %d, %d, %f\n", &dummy, &s.counts, &s.nPictures, &s.nUploads, &s.totalTimeSpent);
				s1.counts += s.counts;
				s1.nPictures += s.nPictures;
				s1.nUploads += s.nUploads;
				s1.totalTimeSpent += s.totalTimeSpent;
			}
			fclose(f);
		}
		

		FILE* fp=fopen(newpath,"w");

		fprintf(fp, "Category, Num Try-ons, Num Pictures, Num Uploads, Time spent\n");
		for(int i=0;i<NUM_BAGS;i++)   {
			Stats s = statsBags[i];
			fprintf(fp, "Bag %d, %d, %d, %d, %f\n",i+1, s.counts, s.nPictures, s.nUploads, s.totalTimeSpent);	
			printf("Bag %d, %d, %d, %d, %f\n",i+1, s.counts, s.nPictures, s.nUploads, s.totalTimeSpent);	
			statsBags[i].counts = statsBags[i].nPictures = statsBags[i].nUploads = statsBags[i].totalTimeSpent = 0;
		}
		for(int i=0;i<NUM_DRESSES_FEMALE;i++) {
			Stats s = statsFemale[i];
			fprintf(fp, "Cloth Female %d, %d, %d, %d, %f\n",i+1, s.counts, s.nPictures, s.nUploads, s.totalTimeSpent);
			statsFemale[i].counts = statsFemale[i].nPictures = statsFemale[i].nUploads = statsFemale[i].totalTimeSpent = 0;
		}
		for(int i=0;i<NUM_DRESSES_MALE;i++) {
			Stats s = statsMale[i]; 
			fprintf(fp, "Cloth Male %d, %d, %d, %d, %f\n",i+1, s.counts, s.nPictures, s.nUploads, s.totalTimeSpent);
			statsMale[i].counts = statsMale[i].nPictures = statsMale[i].nUploads = statsMale[i].totalTimeSpent = 0;
		}
		for(int i=0;i<NUM_3DCLOTHES;i++) {
			Stats s = stats3D[i];
			fprintf(fp, "Cloth3D %d, %d, %d, %d, %f\n",i+1, s.counts, s.nPictures, s.nUploads, s.totalTimeSpent);
			stats3D[i].counts = stats3D[i].nPictures = stats3D[i].nUploads = stats3D[i].totalTimeSpent = 0;
		}
		fclose(fp);

		start = current;
	}
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
		//kevin
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		//end
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
//	g_Capture = cvCaptureFromAVI("C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/Screensaver.mov");

	detail1 = frame1 = frame2 = frame3 = NULL;
	frame2flag = true; //categories

	int width = 0;
	int height = 0;
	int nchannels = 0; 
	int width1 = 0;
	int height1 = 0;
	int nchannels1 = 0; 
	qrCodeData = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/QRCode.bmp",
				
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
		/*		
	datad1 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/detail1.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datad2 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/detail2.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datadbag = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/detail3.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	*/
	dataf1c =  SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/interfaceclothes.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	dataf1n =  SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/interfacenone.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	dataf1 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/interfacebag.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	dataf2 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/Categories.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datap1 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/preview.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				); 
	datap2 = SOIL_load_image(
				"../KinectSDK/screenshot.jpg",
				&width1,&height1,&nchannels1
				,SOIL_LOAD_AUTO
				);
	datafinger = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/circle0.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger1 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/circle1.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger2 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/circle2.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger3 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/circle3.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datafinger4 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/circle4.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datasaving1 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/uploadsuccessful.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);

	//By SF
	dataDefault =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/default.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataHome =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/home.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataPhoto =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/photo.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataDetails =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/details.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataAdd =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/add.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);

	dataP =			SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/panel.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataPHome =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/panelplushome.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataPMen =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/panelplusmen.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataPLadies =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/panelplusladies.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataPBag =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/panelplusbag.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataP3d =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screen/panelplus3d.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);

	dataAlbum =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbum/a_default.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataABack =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbum/a_back.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataALike =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbum/a_like.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataALikeOn =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbum/a_like2.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataAShare =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbum/a_share.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataADiscard =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbum/a_discard.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);

	dataFrame =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/frames/frame_small.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataFrameBig =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/frames/frame_large.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);

	//images with wheels
	dataWDefault =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/default1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWHome =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/home1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWPhoto =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/photo1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWDetails =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/details1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWAdd =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/add1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);

	dataWP =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/panel1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWPHome =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/panelplushome1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWPMen =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/panelplusmen1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWPLadies =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/panelplusladies1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWPBag =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/panelplusbag1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWP3d =		SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenwheel/panelplus3d1.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);

	dataWAlbum =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbumwheel/aw_default.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWABack =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbumwheel/aw_back.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWALike =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbumwheel/aw_like.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWALikeOn =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbumwheel/aw_like2.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWAShare =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbumwheel/aw_share.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
	dataWADiscard =	SOIL_load_image(
					"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/v2/bg/screenalbumwheel/aw_discard.png",
					&width,&height,&nchannels
					,SOIL_LOAD_AUTO
					);
//END

	/*
	datacount1 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/count1.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datacount2 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/count2.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	datacount3 = SOIL_load_image(
				"C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Pictures/count3.png",
				&width,&height,&nchannels
				,SOIL_LOAD_AUTO
				);
	*/
	/*
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
	}*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(COLOR_RES_X, COLOR_RES_Y);
	glutCreateWindow ("Magic Mirror");
	
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	//kevin
	glutKeyboardUpFunc(glutKeyboardUp);
	//end
	glutSpecialFunc(glutSpecialKey);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
	glutCloseFunc(fnExit);

	//Added by Mobeen - 16 July 2012
	glewInit(); 
	InitGL(); 

	//removed by CCL to be replaced by XML Load
	//mcloth->SetNumItems(NUM_BAGS, NUM_DRESSES_FEMALE, NUM_DRESSES_MALE);
	
	
	//for(int i=0;i<NUM_3DCLOTHES;i++) {	 
	//	int id = ids3DGarment[i];
	//	if(id==6) {
	//		sprintf(newpath,"%sModels/Garment/Garment%d.obj", path, id, id);
	//	} else {
	//		//for existing 3d meshes
	//		sprintf(newpath,"%sModels/Garment%d/Garment%d.obj", path, id, id);
	//	}
	//	pCloth3D->LoadObjMesh(newpath);		
	//}
	//pCloth3D->InitReferenceValues();
	//sprintf(newpath,"%sModels/YOffsets.csv", path);
	//pCloth3D->LoadYOffsets(newpath);

	//for(int i=0;i<NUM_3DCLOTHES;i++) {
	//	int id = ids3DGarment[i]; 
	//	if(id<=10 || id>=19) {
	//		pCloth3D->SetReferenceHeight(i, 1.22f); 
	//		pCloth3D->SetReferenceShoulderWidth(i, 0.277f);  
	//	}
	//}

	/*
	pCloth3D->LoadObjMesh("C:/Users/Edwin Tan/Desktop/MagicMirror_Data_M1560210/Data/Models/Garment1/Garment1.obj");  
	pCloth3D->InitReferenceValues();

	for(int i=0;i<NUM_3DCLOTHES;i++) {
		if(i!=2 && i<10) {
			pCloth3D->SetReferenceHeight(i, 1.22f); 
			pCloth3D->SetReferenceShoulderWidth(i, 0.277f);  
		}
	}
	*/
	//CCL Added for XML Load
	LoadGarments(clothesXML);
	pCloth3D->InitReferenceValues();

	NUM_DRESSES_FEMALE=mcloth->GetNumDressesFemale();
	NUM_DRESSES_MALE=mcloth->GetNumDressesMale();
	NUM_BAGS=mcloth->GetNumBags();
	NUM_3DCLOTHES = pCloth3D->GetNumDresses();
	// Setup the OpenGL projection matrix

	//kevin edited
	//Aspect ratio fix
	ar = glutGet( GLUT_WINDOW_WIDTH ) / (double)glutGet( GLUT_WINDOW_HEIGHT );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, COLOR_RES_X *ar, COLOR_RES_Y, 0, -1.0, 1.0);
	//end

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0,0, COLOR_RES_X, COLOR_RES_Y);
	

	//for lab machine
	#ifdef LAB_MACHINE
	//Magic Mirror lab machine
	glutPositionWindow(-(1820), 0);
	glutReshapeWindow(1280*2, 960*2); 
    //for Magic Mirror
	#else	
	glutPositionWindow(-(750),-35);
	glutReshapeWindow(1280*2, 960*2);
	#endif

	
	//now play the audio 
	/*
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
	*/

	// Per frame code is in glutDisplay
	glutMainLoop();
}