/*
 *  ColorSetting.h
 *  Baby
 *
 *  Created by 菊川 真理子 on 11/05/29.
 *  Copyright 2011 北陸先端科学技術大学院大学. All rights reserved.
 *
 */

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "cvaux.h"

using namespace std;

class ColorSetting{
public:
	ColorSetting();
	double GetColorR(IplImage *img, int x, int y);
	double GetColorG(IplImage *img, int x, int y);
	double GetColorB(IplImage *img, int x, int y);
	void getRGB(IplImage *img, int x, int y, double *r, double *g, double *b);
};