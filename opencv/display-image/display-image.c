#include "highgui.h"

int main(int argc, char **argv) {
  IplImage *image = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
  cvNamedWindow("display-image", CV_WINDOW_AUTOSIZE);
  cvShowImage("display-image", image);
  cvWaitKey(0);
  cvReleaseImage(&image);
  cvDestroyWindow("display-image");

  return 0;
}
