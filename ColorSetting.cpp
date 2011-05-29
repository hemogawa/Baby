/*
 *  ColorSetting.cpp
 *  Baby
 *
 *  Created by 菊川 真理子 on 11/05/29.
 *  Copyright 2011 北陸先端科学技術大学院大学. All rights reserved.
 *
 */

#include "ColorSetting.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "cvaux.h"

ColorSetting::ColorSetting(){
}
uchar ColorSetting::GetColorR(IplImage *img, int x, int y){
	return ((uchar*)(img->imageData + img->widthStep * y))[x * 3 + 2];
}
uchar ColorSetting::GetColorG(IplImage *img, int x, int y){
	return ((uchar*)(img->imageData + img->widthStep * y))[x * 3 + 1];
}
uchar ColorSetting::GetColorB(IplImage *img, int x, int y){
	return ((uchar*)(img->imageData + img->widthStep * y))[x * 3];
}
