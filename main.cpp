#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include "cvaux.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "ColorSetting.h"
#include "Labeling.h"
#include <math.h>

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
	IplImage *imgFrm, *imgGray, *imgResize, *imgTmp;
	short *dstLabel;
	Mat frame, frameCopy, image;
	struct colors imgColor={0,0,0};
	ColorSetting colSet;	//引数無しのコンストラクタは()無しで呼出すんですってよ
    CvCapture* capture = 0;
	CvPoint prePoint[5]={{0,0}}, fhlPoint[5] = {0,0}, crtPoint[5] = {0,0};
	int x=0, y=0, i=0, j=0, k=0;
	double minDist, crtDist;
	float size_x, size_y;
	LabelingBS labeling;
	RegionInfoBS *ri;
	
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
		imgTmp = cvCreateImage(cvGetSize(imgResize), IPL_DEPTH_8U, 1);
		cvDilate(imgResize, imgTmp, NULL, 8);
		cvErode(imgTmp, imgResize, NULL, 8);
		dstLabel = new short[imgResize->width * imgResize->height];
		labeling.Exec((uchar*)imgResize->imageData, dstLabel, imgResize->width, imgResize->height, true, 300);
		for (j=0;j<5;j++){
			ri = labeling.GetResultRegionInfo(j);
			ri->GetCenter( size_x, size_y);
			crtPoint[j].x = size_x; crtPoint[j].y = size_y;
		}
		for(j=0;j<5;j++){
			//1ばん最初のポイントを設定
			if (prePoint[j].x == 0 && prePoint[j].y == 0) {
				fhlPoint[j] = crtPoint[j];
			}
			else{
				for(k=0;k<5;k++){					
					crtDist = hypot(prePoint[j].x - crtPoint[k].x, prePoint[j].y - crtPoint[k].y);
					if (k==0) {
						minDist = crtDist;
						fhlPoint[j] = crtPoint[k];
					}else if (minDist > crtDist) {
						minDist = crtDist;
						fhlPoint[j] = crtPoint[k];
					}
				}
				if (minDist > 100) {
					fhlPoint[j] = prePoint[j];
				}
			}
			printf("%d:(%d,%d)",j,fhlPoint[j].x, fhlPoint[j].y);
			if (j==4)
				printf("\n");
			cvCircle(imgResize, fhlPoint[j], 5, CV_RGB(j*60,j*60,j*60), 3, 8, 0);
			prePoint[j] = fhlPoint[j];
		}
			/*else {
				// 前回からの距離が一番近い点をfhlPointとして記録
				for (k=0; k<5; k++) {
					crtDist = hypot(prePoint[k].x - size_x, prePoint[k].y - size_y);
					if (k == 0) {
						minDist = crtDist;
						fhlPoint[k].x = size_x; fhlPoint[k].y = size_y;
					} else if (minDist > crtDist) {
						minDist = crtDist;
						fhlPoint[k].x = size_x; fhlPoint[k].y = size_y;
					}
					printf("min:%3.0f (%d,%d)\t", minDist, fhlPoint[k].x, fhlPoint[k].y);
					if (k == 4) {
						printf("\n");
					}
				}
			}
			cvCircle(imgResize, fhlPoint[j], 5, CV_RGB(j*60,j*60,j*60), 3, 8, 0);
			prePoint[j].x = fhlPoint[j].x; prePoint[j].y = fhlPoint[j].y;
			printf("%d:(%d,%d)\n",j,fhlPoint[j].x, fhlPoint[j].y);
			if(j == 4)
				printf("\n");
			 */
		sprintf(saveUrl, "../../captures/saveLavel%d.jpg",i);
		cvSaveImage(saveUrl, imgResize);
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