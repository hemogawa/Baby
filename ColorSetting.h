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
	uchar GetColorR(IplImage *img, int x, int y);
	uchar GetColorG(IplImage *img, int x, int y);
	uchar GetColorB(IplImage *img, int x, int y);
};