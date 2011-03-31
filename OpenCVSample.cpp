/*
 *  OpenCVSample.cpp
 *  OpenGLSample
 *
 *  Created by 菊川 真理子 on 11/03/03.
 *  Copyright 2011 北陸先端科学技術大学院大学. All rights reserved.
 *
 */

#define CV_NO_BACKWARD_COMPATIBILITY

#include "OpenCVSample.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<GLUT/glut.h>
//#include<GL/gl.h>
#include<cv.h>
#include<highgui.h>

#ifdef _EiC
#define WIN32
#endif

using namespace std;
using namespace cv;
bool first = true;

int detectAndDraw( Mat& img,
				  CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
				  double scale);

String cascadeName =
"./haarcascade_frontalface_alt.xml";
String nestedCascadeName =
"./haarcascade_eye_tree_eyeglasses.xml";

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
char buff[255];
CascadeClassifier cascade, nestedCascade;
double scale = 1;
int fwidth,baseWidth = 190, zoomeStep = 0;

static int mouseX, mouseY;
static int width,height;
int VPSize = 20; 
int bulevel = 30;

void drowDamy(){
	glColor4f(0.0, 0.0, 0.0,0.5);
	glBegin(GL_LINE_STRIP);
	glVertex2d(-0.9,0);
	glVertex2d(0.9, 0);
	glEnd();
glEnd();
	
}

void disp(){
	glClearColor(1, 0.5, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int i=0;
	for (i=0; i<height/10; i++) {
		glViewport(0, i*10, width, VPSize);
		drowDamy();
	}
	glFlush();
}

void idle(void){
	glutPostRedisplay();
	
	if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=\"<cascade_path>\"]\n"
		"   [--nested-cascade[=\"nested_cascade_path\"]]\n"
		"   [--scale[=<image scale>\n"
		"   [filename|camera_index]\n" ;
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
        image = imread( "lena.jpg", 1 );	
   if( capture )
    {
            IplImage* iplImg = cvQueryFrame( capture );
            frame = iplImg;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );
			
            fwidth = detectAndDraw( frameCopy, cascade, nestedCascade, scale );
			if (first == true) {
				baseWidth = fwidth;
			} else {
				if((fwidth-baseWidth)/20 >= zoomeStep+1){
					zoomeStep++;
					printf("ZoomeIn!%d:",zoomeStep);
					system(buff);
				}else if ((fwidth-baseWidth)/20 < zoomeStep) {
					printf("ZoomeOut!%d:",zoomeStep);
					if(zoomeStep>0)
						zoomeStep--;
					system(buff);
				}
				bulevel = zoomeStep*10;
			}
			//fp = fopen("scripts/log.txt","a");
			//fprintf(fp,"%d->%d\n",baseWidth,width);
			//fclose(fp);
			first = false;
            //if( waitKey( 10 ) >= 0 )
            //    goto _cleanup_;
		
        //waitKey(0);
	//_cleanup_:
        //cvReleaseCapture( &capture );
    }
    else
    {
        if( !image.empty() )
        {
            fwidth = detectAndDraw( image, cascade, nestedCascade, scale );
            waitKey(0);
        }
        else if( !inputName.empty() )
        {
            // assume it is a text file containing the
			//list of the image filenames to be processed - one per line 
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
						fwidth = detectAndDraw( image, cascade, nestedCascade, scale );
                        c = waitKey(0);
                        if( c == 27 || c == 'q' || c == 'Q' )
                            break;
                    }
                }
                fclose(f);
            }
        }
    }	
	
	
}

void mouseMove(int x, int y){
	mouseX = x; mouseY = y;
	printf("(%d,%d)",mouseX,mouseY);
	glLoadIdentity();
	//glOrtho(-0.5, (GLdouble)width -0.5, (GLdouble)height -0.5, -0.5, -1.0, 1.0);
	glutIdleFunc(idle);
}

void resize(int w, int h){
	width=w; height=h;
	/*glViewport(0, 0, w, h/2);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w -0.5, (GLdouble)h -0.5, -0.5, -1.0, 1.0);*/
}

int detectAndDraw( Mat& img,
				  CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
				  double scale)
{
    int i = 0,faceSize = 190;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );
	
    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );
	
    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
							 1.1, 2, 0
							 //|CV_HAAR_FIND_BIGGEST_OBJECT
							 //|CV_HAAR_DO_ROUGH_SEARCH
							 |CV_HAAR_SCALE_IMAGE
							 ,
							 Size(30, 30) );
    t = (double)cvGetTickCount() - t;
    printf( "new detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
		if (r->width > 190) {
			faceSize = r->width;
		}
    }  
	return faceSize;
}

int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(200, 100);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("mouse");
	glutDisplayFunc(disp);	//コールバック関数
	glutReshapeFunc(resize);	//カレントウィンドウが生成・サイズ変更した際に呼び出される処理．幅・高さを渡す	
	glutPassiveMotionFunc(mouseMove);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();
	return 0;
}