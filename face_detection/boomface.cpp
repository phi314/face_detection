#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <stdio.h>
#include <iostream>


// *** Change this to your install location! ***
// *********************************************
#define OPENCV_ROOT  "C:/OpenCV/opencv"
// *********************************************

using namespace std;
using namespace cv;

const char * DISPLAY_WINDOW = "Unleashed Corporation";

IplImage *marker = cvLoadImage("C:/mask.png");    /* load marker */
IplImage *frame;
CvRect* r;
CvRect rect;

CvHaarClassifierCascade * pCascade = 0;		// the face detector
CvMemStorage * pStorage = 0;				// memory for detector to use
CvSeq * face;								// memory-access interface	

void overlayImages(CvPoint point, IplImage * frame);
void displayDetections(IplImage * frame, CvSeq * face);

int main(int argc, char** argv)
{
	CvCapture *capture = cvCaptureFromCAM(0);

	/* usage check
	if(argc < 2)
	{
		printf("Missing name of image file!\n"
		       "Usage: %s <imagefilename>\n", argv[0]);
		exit(-1);
	}*/

	// initializations
	//frame = (argc > 1) ? cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR) : 0;

	pStorage = cvCreateMemStorage(0);
	pCascade = (CvHaarClassifierCascade *)cvLoad
	   ((OPENCV_ROOT"/data/haarcascades/haarcascade_frontalface_default.xml"),
	   0, 0, 0 );

	// validate that everything initialized properly
	if( !capture || !pStorage || !pCascade )
	{
		printf("Initialization failed: %s\n",
			(!capture)?  "can't load image file" :
			(!pCascade)? "can't load haar-cascade -- "
				         "make sure path is correct" :
			"unable to allocate memory for data storage", argv[1]);
		exit(-1);
	}

	// create a window to display detected faces
	cvNamedWindow(DISPLAY_WINDOW, CV_WINDOW_AUTOSIZE);

	// Show the image captured from the camera in the window and repeat
	while(1) {

		frame = cvQueryFrame( capture );

		// detect faces in image
		face = cvHaarDetectObjects
			(frame, pCascade, pStorage,
			1.2,						// increase search scale by 10% each pass
			3,							// merge groups of three detections
			CV_HAAR_DO_CANNY_PRUNING,	// skip regions unlikely to contain a face
			cvSize(50,50));				// smallest size face to detect = 40x40

		displayDetections(frame, face);

		cvShowImage(DISPLAY_WINDOW, frame);

		if ( (cvWaitKey(10) & 255) == 27 ) break;

	}

	// clean up and release resources
	cvReleaseCapture(&capture);
	if(pCascade) cvReleaseHaarClassifierCascade(&pCascade);
	if(pStorage) cvReleaseMemStorage(&pStorage);
	
	cvDestroyWindow(DISPLAY_WINDOW);
	return 0;
}


void displayDetections(IplImage * frame, CvSeq * face)
{
	int i, j;

	// draw a rectangular outline around each detection
	for(i=0;i<(face? face->total:0); i++ )
	{
		int total = face->total;
		r = (CvRect*)cvGetSeqElem(face, i);
		
		CvPoint pt1 = { r->x, r->y };
		CvPoint pt2 = { r->x + r->width, r->y + r->height };
		cvRectangle(frame, pt1, pt2, CV_RGB(0,255,0), 3, 4, 0);
		
		CvFont font;

		cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 1.0, 1.0, 0, 1, CV_AA);
		cvPutText(frame, "Keur Ngaca Lur! ", pt1, &font, cvScalar(255, 255, 255, 0));

		overlayImages(pt1, frame);

	}	

		
	
}

//ieu pitur nambah wajah ini acok
void overlayImages(CvPoint point, IplImage * frame)
{
		/* define rectangle for ROI */
		//rect = cvRect(point.x, point.y,marker->width, marker->height);

		/* sets Region of Interest */
		//cvSetImageROI(frame, rect);

		/* Add masked images */
		//cvAddWeighted(marker,1, frame, 0, 0, frame);

		/* always reset the region of interest */
		//cvResetImageROI(frame);
}
