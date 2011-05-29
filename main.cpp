#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "cvaux.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "ColorSetting.h"

using namespace std;	//Stringって実はstd::stringって呼び出してたんですねー
using namespace cv;

char* srcMov = "../../inputfile.avi";
char saveUrl[256];

struct colors {
	uchar r;
	uchar g;
	uchar b;
};

int main (int argc, char * const argv[]) {
	IplImage *imgFrm, *imgGray, *imgResize;
	Mat frame, frameCopy, image;
	struct colors imgColor={0,0,0};
	ColorSetting colSet;	//引数無しのコンストラクタは()無しで呼出すんですってよ
    CvCapture* capture = 0;
	int x=0, y=0, i=0;
	capture = cvCaptureFromFile(srcMov);
	if (capture == NULL) {
		printf("%s is can't load", srcMov);
		exit(0);
	}
	
	cvNamedWindow("window", CV_WINDOW_AUTOSIZE);
	char key;
	for (;;) {
		if (NULL == (imgFrm=cvQueryFrame(capture))) {
			printf("can't get frame");
			break;
		}
		imgGray = cvCreateImage(cvGetSize(imgFrm), IPL_DEPTH_8U, 1);
		for (y=0; y < imgFrm->height; y++) {
			for (x=0; x<imgFrm->width; x++) {
				imgColor.r = colSet.GetColorR(imgFrm, x, y);
				imgColor.g = colSet.GetColorG(imgFrm, x, y);
				imgColor.b = colSet.GetColorB(imgFrm, x, y);
				if (imgColor.r >imgColor.g *1.1 &&
					imgColor.g>imgColor.b *1.1 )
					cvSetReal2D(imgGray, y, x, 255);
				else
					cvSetReal2D(imgGray, y, x, 0);
			}
		}
		imgResize = cvCreateImage(cvSize(imgFrm->width/2, imgFrm->height/2), 8, 1);
		cvResize(imgGray, imgResize, CV_INTER_CUBIC);
		sprintf(saveUrl, "../../captures/saveImage%d.jpg",i);
		printf("%s",saveUrl);
		cvSaveImage(saveUrl, imgGray);
		cvShowImage("window", imgResize);
		key = (char)cvWaitKey(41);
		if(key == '\033'){
			exit(0);
		}
		i++;
	}
	cvWaitKey(0);
	cvReleaseCapture(& capture);
	cvDestroyWindow("window");
	cvReleaseCapture(&capture);
	cvReleaseImage(&imgFrm);
	cvReleaseImage(&imgGray);
    return 0;
}