#include "highgui.h"

int main(int argc, char **argv) {
  cvNamedWindow("display-video", CV_WINDOW_AUTOSIZE);
  CvCapture *capture = cvCaptureFromFile(argv[1]);
  IplImage *frame;

  while (1) {
    frame = cvQueryFrame(capture);
    if (!frame) {
      break;
    }

    cvShowImage("display-video", frame);

    char c = cvWaitKey(33);
    if (c == 27) {
      break;
    }
  }

  cvReleaseCapture(&capture);
  cvDestroyWindow("display-video");

  return 0;
}
