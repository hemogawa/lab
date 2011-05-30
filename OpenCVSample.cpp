#define CV_NO_BACKWARD_COMPATIBILITY

#include "cv.h"
#include "highgui.h"
#include "ColorSetting.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>

#ifdef _EiC
#define WIN32
#endif

using namespace std;
using namespace cv;
bool first = true;
int clearCir=90;
int mouseX = 50, mouseY = 50;

struct colors {
	double r;
	double g;
	double b;
};

void drawClearSpot(void *dstImg, IplImage *clImg, IplImage *gauImg, int x, int y, int radius){
	ColorSetting colorSetting;
	int i,j;
	struct colors clearColors = { 0, 0, 0};
	struct colors gauseColors = { 0, 0, 0};
	for(i=0; i<=radius; i++){
		for(j=0; j<=radius-i; j++){
			if(x+i+radius < clImg ->width && y+j+radius < clImg->height){
				colorSetting.getRGB(clImg, x+i, y+j, &clearColors.r, &clearColors.g, &clearColors.b);
				cvSet2D(dstImg,j+y,i+x,CV_RGB(clearColors.r,clearColors.g,clearColors.b));
			}
			if(x-i-radius > 0 && y-i-radius > 0){
				colorSetting.getRGB(clImg, x-i, y-j, &clearColors.r, &clearColors.g, &clearColors.b);
				cvSet2D(dstImg,y-j,x-i,CV_RGB(clearColors.r,clearColors.g,clearColors.b));
			}
			if( x+i+radius < clImg ->width && y-j-radius >0){
				colorSetting.getRGB(clImg, x+i, y-j, &clearColors.r, &clearColors.g, &clearColors.b);
				cvSet2D(dstImg,y-j,x+i,CV_RGB(clearColors.r,clearColors.g,clearColors.b));
			}
			if( x-i-radius > 0 && y+j+radius < clImg ->height){
				colorSetting.getRGB(clImg, x-i, y+j, &clearColors.r, &clearColors.g, &clearColors.b);
				cvSet2D(dstImg,y+j,x-i,CV_RGB(clearColors.r,clearColors.g,clearColors.b));
			}
		}
	}
	cvShowImage("dst", dstImg);
	for(i=0; i<=radius; i++){
		for(j=0; j<=radius-i; j++){
			if(x+i+radius < clImg ->width && y+j+radius < clImg->height){
				colorSetting.getRGB(gauImg, x+i, y+j, &gauseColors.r, &gauseColors.g, &gauseColors.b);
				cvSet2D(dstImg,j+y,i+x,CV_RGB(gauseColors.r,gauseColors.g,gauseColors.b));
			}
			if(x-i-radius > 0 && y-i-radius > 0){
				colorSetting.getRGB(gauImg, x-i, y-j, &gauseColors.r, &gauseColors.g, &gauseColors.b);
				cvSet2D(dstImg,y-j,x-i,CV_RGB(gauseColors.r,gauseColors.g,gauseColors.b));
			}
			if( x+i+radius < clImg ->width && y-j-radius >0){
				colorSetting.getRGB(gauImg, x+i, y-j, &gauseColors.r, &gauseColors.g, &gauseColors.b);
				cvSet2D(dstImg,y-j,x+i,CV_RGB(gauseColors.r,gauseColors.g,gauseColors.b));
			}
			if( x-i-radius > 0 && y+j+radius < clImg ->height){
				colorSetting.getRGB(gauImg, x-i, y+j, &gauseColors.r, &gauseColors.g, &gauseColors.b);
				cvSet2D(dstImg,y+j,x-i,CV_RGB(gauseColors.r,gauseColors.g,gauseColors.b));
			}	
		}
	}	
}

void Mouse(int event, int x, int y, int flags, void *dstImage){
	IplImage *srcImage, *gauImage;
	srcImage = cvLoadImage("../../00.png", CV_LOAD_IMAGE_COLOR);
	gauImage = cvLoadImage("../../gause.png", CV_LOAD_IMAGE_COLOR);
	mouseX = x; mouseY = y;
	ColorSetting colorSetting;
	int i=0,j=0;
	if (event == CV_EVENT_MOUSEMOVE) 
		drawClearSpot(dstImage, srcImage, gauImage, x, y, clearCir);
}

int detectAndDraw( Mat& img,
				  CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
				  double scale);

String cascadeName =
"../../haarcascade_frontalface_alt.xml";
String nestedCascadeName =
"../../haarcascade_eye_tree_eyeglasses.xml";

int main( int argc, const char** argv )
{
	struct colors sets = { 0, 0, 0};
	int extent=10;	//ぼやけ度
	
	ColorSetting colorSetting;
    CvCapture* capture = 0;
    Mat frame, frameCopy, image;
    const String scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const String cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();
    const String nestedCascadeOpt = "--nested-cascade";
    size_t nestedCascadeOptLen = nestedCascadeOpt.length();
    String inputName;
	FILE *fp;
	IplImage *srcImage=0,*dstImage=0,*gauImage=0;
	
	srcImage = cvLoadImage("../../00.png", CV_LOAD_IMAGE_COLOR);
	dstImage = cvCreateImage(cvSize(srcImage->width, srcImage->height), IPL_DEPTH_8U, 3);
	cvNamedWindow("dst", CV_WINDOW_AUTOSIZE);
	int i=0,j=0,k=0,l=0;
	for (i=0; i<srcImage->width; i++) {
		for (j=0; j<srcImage->height; j++) {
			struct colors sumPixel = { 0, 0, 0};
			int pixCnt = 0;
			for(k=0; k<=extent; k++){
				for(l=0; l<=extent-k; l++){
					if (i+k <= srcImage->width && j+l <= srcImage->height) {
						sumPixel.r += colorSetting.GetColorR(srcImage, i+k, j+l);
						sumPixel.g += colorSetting.GetColorG(srcImage, i+k, j+l);
						sumPixel.b += colorSetting.GetColorB(srcImage, i+k, j+l);
						pixCnt++;
					}
					if (extent != 0) {
						if(i-k >= 0 && j-l >=0){
							//printf("(%d,%d:%d,%d:%d)\n",k,l,i,j,j-l);
							sumPixel.r += colorSetting.GetColorR(srcImage, i-k, j-l);
							sumPixel.g += colorSetting.GetColorG(srcImage, i-k, j-l);
							sumPixel.b += colorSetting.GetColorB(srcImage, i-k, j-l);
							pixCnt++;
						}
					}
					if (extent != 0) {
						if(i+k <= srcImage->width && j-l >=0){
							//printf("(%d,%d:%d,%d:%d)\n",k,l,i,j,j-l);
							sumPixel.r += colorSetting.GetColorR(srcImage, i+k, j-l);
							sumPixel.g += colorSetting.GetColorG(srcImage, i+k, j-l);
							sumPixel.b += colorSetting.GetColorB(srcImage, i+k, j-l);
							pixCnt++;
						}
					}
					if (extent != 0) {
						if(i-k >= 0 && j+l <=srcImage->height){
							//printf("(%d,%d:%d,%d:%d)\n",k,l,i,j,j-l);
							sumPixel.r += colorSetting.GetColorR(srcImage, i-k, j+l);
							sumPixel.g += colorSetting.GetColorG(srcImage, i-k, j+l);
							sumPixel.b += colorSetting.GetColorB(srcImage, i-k, j+l);
							pixCnt++;
						}
					}
				}
			}
			sets.r = sumPixel.r / pixCnt;
			sets.g = sumPixel.g / pixCnt;
			sets.b = sumPixel.b / pixCnt;
			cvSet2D(dstImage,j,i,CV_RGB(sets.r,sets.g,sets.b));
		}
	}
	
	char buff[255];
    CascadeClassifier cascade, nestedCascade;
    double scale = 1;
	int width,baseWidth = 190, zoomeStep = 0;
	cvShowImage("dst", dstImage);
	cvSaveImage("../../gause.png", dstImage);
	gauImage = cvCloneImage(dstImage);
	cvSetMouseCallback("dst", Mouse,dstImage);
	
    /*for( int i = 1; i < argc; i++ )
	 {
	 if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 )
	 cascadeName.assign( argv[i] + cascadeOptLen );
	 else if( nestedCascadeOpt.compare( 0, nestedCascadeOptLen, argv[i], nestedCascadeOptLen ) == 0 )
	 {
	 if( argv[i][nestedCascadeOpt.length()] == '=' )
	 nestedCascadeName.assign( argv[i] + nestedCascadeOpt.length() + 1 );
	 if( !nestedCascade.load( nestedCascadeName ) )
	 cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
	 }
	 else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
	 {
	 if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
	 scale = 1;
	 }
	 else if( argv[i][0] == '-' )
	 {
	 cerr << "WARNING: Unknown option %s" << argv[i] << endl;
	 }
	 else
	 inputName.assign( argv[i] );
	 }*/
	
    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=\"<cascade_path>\"]\n"
		"   [--nested-cascade[=\"nested_cascade_path\"]]\n"
		"   [--scale[=<image scale>\n"
		"   [filename|camera_index]\n" ;
        return -1;
    }
	
    if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
        capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
    else if( inputName.size() )
    {
        image = imread( inputName, 1 );
        if( image.empty() )
            capture = cvCaptureFromAVI( inputName.c_str() );
    }
    else
        image = imread( "lena.png", 1 );
	
	
    if( capture )
    {
        for(;;)
        {
            IplImage* iplImg = cvQueryFrame( capture );
            frame = iplImg;
            if( frame.empty() )
                break;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );
			
            width = detectAndDraw( frameCopy, cascade, nestedCascade, scale );
			if (first == true) {
				baseWidth = width;
			} else {
				if((width-baseWidth)/20 >= zoomeStep+1){
					zoomeStep++;
					printf("ZoomeIn!%d:",zoomeStep);
					//sprintf(buff, "osascript scripts/zoomeIn.scpt %d",zoomeStep);
					//sprintf(buff, "osascript scripts/zoomeIn_top.scpt %d",zoomeStep);
					//system(buff);
				}else if ((width-baseWidth)/20 < zoomeStep) {
					printf("ZoomeOut!%d:",zoomeStep);
					zoomeStep--;
					//sprintf(buff, "osascript scripts/zoomeOut.scpt %d",zoomeStep);
					//sprintf(buff, "osascript scripts/zoomeOut_top.scpt %d",zoomeStep);
					//system(buff);
				}
				clearCir = 90 - (zoomeStep * 5);
				drawClearSpot(dstImage, srcImage, gauImage, mouseX, mouseY, clearCir);
			}
			/*fp = fopen("scripts/log.txt","a");
			fprintf(fp,"%d->%d\n",baseWidth,width);
			fclose(fp);*/
			first = false;
            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }
		
        waitKey(0);
	_cleanup_:
        cvReleaseCapture( &capture );
    }
    else
    {
        if( !image.empty() )
        {
            width = detectAndDraw( image, cascade, nestedCascade, scale );
            waitKey(0);
        }
        else if( !inputName.empty() )
        {
            /* assume it is a text file containing the
			 list of the image filenames to be processed - one per line */
            FILE* f = fopen( inputName.c_str(), "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
                    int len = (int)strlen(buf), c;
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
                    cout << "file " << buf << endl;
                    image = imread( buf, 1 );
                    if( !image.empty() )
                    {
						width = detectAndDraw( image, cascade, nestedCascade, scale );
                        c = waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                }
                fclose(f);
            }
        }
    }
   	cvWaitKey(0);
	cvDestroyWindow("dst");
	cvReleaseImage(&dstImage);
	return 0;
}

int detectAndDraw( Mat& img,
				  CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
				  double scale)
{
    int i = 0,faceSize = 190;
    vector<Rect> faces;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
	
    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );
	
    cascade.detectMultiScale( smallImg, faces,
							 1.1, 2, 0
							 //|CV_HAAR_FIND_BIGGEST_OBJECT
							 //|CV_HAAR_DO_ROUGH_SEARCH
							 |CV_HAAR_SCALE_IMAGE
							 ,
							 Size(190, 190) );
    //printf( "new detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
		faceSize = r->width;
    }  
	return faceSize;
}
