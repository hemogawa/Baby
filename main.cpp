#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "cvaux.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;	//Stringって実はstd::stringって呼び出してたんですねー
using namespace cv;

char* srcMov = "../../inputfile.avi";
const char* cascadeName = "../../haarcascade_frontalface_alt.xml";
String nestedCascadeName = "../../haarcascade_eye_tree_eyeglasses.xml";

void detectAndDraw( IplImage* img, CvHaarClassifierCascade* cascade,
				   CvHaarClassifierCascade* nestedCascade, double scale);

int main (int argc, char * const argv[]) {
	IplImage* imgFrm;
	Mat frame, frameCopy, image;
    CvCapture* capture = 0;
	capture = cvCaptureFromFile(srcMov);
	CvHaarClassifierCascade *cascade, *nestedcascade;
	double scale = 1;
	/*if (!cascade.load(cascadeName)) {
		printf("can't load cascade");
	}*/
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
		detectAndDraw(imgFrm, cascade, nestedcascade, scale);
		cvShowImage("window", imgFrm);
		key = (char)cvWaitKey(41);
		if(key == '\033'){
			exit(0);
		}
	}
	cvWaitKey(0);
	cvReleaseCapture(& capture);
	cvDestroyWindow("window");
    return 0;
}

void detectAndDraw( IplImage* img, CvHaarClassifierCascade* cascade,
				   CvHaarClassifierCascade* nestedCascade, double scale){
	int i=0;
	CvSeq *faces;
	CvMemStorage *storage = 0;
	const static Scalar colors[] = { CV_RGB(0, 0, 255),
		CV_RGB( 0, 128, 255), CV_RGB( 0, 255, 255),
		CV_RGB(0, 255, 0), CV_RGB( 255, 128, 0),
		CV_RGB( 255, 255, 0), CV_RGB( 255, 0, 0), CV_RGB( 255, 0, 255)};
	IplImage* grayImg;
	grayImg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cascade = (CvHaarClassifierCascade *)cvLoad(cascadeName, 0, 0, 0);
	storage = cvCreateMemStorage(0);	//メモリ確保
	cvClearMemStorage(storage);
	cvCvtColor(img, grayImg,CV_BGR2GRAY);	//グレースケール化
	cvEqualizeHist(grayImg,grayImg);	//ヒストグラム均一化
	faces = cvHaarDetectObjects(grayImg, cascade, storage, 1.1, 4, 0, cvSize(30,30));
	for (i=0; i<(faces ? faces -> total : 0); i++) {
		CvRect *r = (CvRect *)cvGetSeqElem(faces, i);
		CvPoint center;
		int radius;
		center.x = cvRound(r->x + r->width * 0.5);
		center.y = cvRound(r->y + r->height * 0.5);
		radius = cvRound((r->width + r->height) * 0.25);
		cvCircle(img, center, radius, colors[i % 8], 3, 8, 0);
	}
}