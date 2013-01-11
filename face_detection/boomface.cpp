#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <stdio.h>
#include <iostream>
#include <ctime> // library untuk waktu

// *** Change this to your install location! ***
// *********************************************

//#define OPENCV_ROOT  "C:/opencv"
#define OPENCV_ROOT  "C:/OpenCV/opencv"

// *********************************************

using namespace std;
using namespace cv;

const char * DISPLAY_WINDOW = "Unleashed Corporation";

IplImage *marker = cvLoadImage("C:/mask.png");    /* load marker */
IplImage *frame;
CvRect* r;
CvRect rect;

double elapsed;
int last_time;
int num_frames;
clock_t start;	// variabel waktu

CvHaarClassifierCascade * pCascade = 0;		// the face detector
CvMemStorage * pStorage = 0;				// memory for detector to use
CvSeq * face;								// memory-access interface	

void overlayImages(CvRect * point, IplImage * frame);
void detect_face(IplImage * frame, CvSeq * face);

int main(int argc, char** argv)
{
	CvCapture *capture = cvCaptureFromCAM(0);

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
		// mengambil waktu saat aloritma utama mulai dijalankan
		start = clock();

		// mengambil setiap frame dari video capture
		frame = cvQueryFrame( capture );

		// detect faces in image
		face = cvHaarDetectObjects
			(frame, pCascade, pStorage,
			1.5,						// increase search scale by 10% each pass
			3,							// merge groups of three detections
			CV_HAAR_DO_CANNY_PRUNING,	// skip regions unlikely to contain a face
			cvSize(100,100));			// smallest size face to detect = 40x40

		detect_face(frame, face);

		cvShowImage(DISPLAY_WINDOW, frame);

		//system("cls");
		//menampilkan waktu komputasi
		cout << "waktu komputasi: " << (clock() - start) / (double) CLOCKS_PER_SEC << "\t ms per proses" << endl;

		IplImage * hist = cvCreateImage(cvGetSize(frame), 8, 1);
		cvCvtColor( frame, hist, CV_BGR2GRAY );
		cvEqualizeHist(hist, hist);

		cvShowImage("hist",hist);

		if ( (cvWaitKey(10) & 255) == 27 ) break;
	}

	// clean up and release resources
	cvReleaseCapture(&capture);
	if(pCascade) cvReleaseHaarClassifierCascade(&pCascade);
	if(pStorage) cvReleaseMemStorage(&pStorage);
	
	cvDestroyWindow("show thumb");
	cvDestroyWindow(DISPLAY_WINDOW);
	cvDestroyWindow("hist");
	return 0;
}


void detect_face(IplImage * frame, CvSeq * face)
{
	int i;
	IplImage * src = cvLoadImage("C:/testimage/test.jpg");

	// draw a rectangular outline around each detection
	for(i=0;i<(face? face->total:0); i++ )
	{
		int total = face->total;
		r = (CvRect*)cvGetSeqElem(face, i);
		
		CvPoint pt1 = { r->x, r->y };
		// CvPoint pt2 = { r->x + 120, r->y + 120 };
		CvPoint pt2 = { r->x + r->height, r->y + r->height };
		cvRectangle(frame, pt1, pt2, CV_RGB(0,255,0), 3, 4, 0);
		
		//for(j=0;j<(face? face->total:0); j++ )
		//{

		CvRect recta = cvRect(r->x, r->y, 120, 120);
			
		cvSetImageROI(frame, recta );

		IplImage * gray = cvCreateImage(cvGetSize(frame), frame->depth, 1);
		cvCvtColor(frame, gray, CV_RGB2GRAY);
		cvSaveImage("C:/test.jpg", gray);

		cvShowImage("show thumb", gray);

		cvResetImageROI(frame);
		cvReleaseImage(&gray);

		//}

		
		//overlayImages(r, frame);
	}

	// Mengitung Framerate
	num_frames++;
	elapsed = clock() - last_time;
	int fps = 0;
	fps = floor(num_frames / (float)(1 + (float)elapsed / (float)CLOCKS_PER_SEC));
	num_frames = 0;
	last_time = clock() + 1 * CLOCKS_PER_SEC;

	CvFont font;

	// menampilkan jumlah fps
	char fpsnum[16];
	char myNum2 = fps;
	itoa (myNum2,fpsnum,10);
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 1.0, 0, 1, CV_AA);
	cvPutText(frame,"FPS",cvPoint(550,50), &font, cvScalar(0, 255, 0, 0));
	cvPutText(frame,fpsnum,cvPoint(500,50), &font, cvScalar(0, 255, 0, 0));

	// menampilkan jumlah wajah terdeteksi
	char txt[16];
	char myNum = i;
	itoa (i,txt,10);
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 1.0, 0, 1, CV_AA);
	cvPutText(frame,"Jumlah Bengeut: ",cvPoint(2,460), &font, cvScalar(0, 255, 0, 0));
	cvPutText(frame,txt,cvPoint(230,460), &font, cvScalar(0, 255, 0, 0));

}

// Fitur mengganti wajah
void overlayImages(CvRect * point, IplImage * frame)
{
		/* define rectangle for ROI */
		rect = cvRect(point->x, point->y,marker->width, marker->height);

		/* sets Region of Interest */
		cvSetImageROI(frame, rect);

		/* Add masked images */
		cvAddWeighted(marker,1, frame, 0, 0, frame);

		/* always reset the region of interest */
		cvResetImageROI(frame);
		
}
