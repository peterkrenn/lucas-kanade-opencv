#include <stdio.h>

#include <highgui.h>

#define APP_NAME "lucas-kanade-opencv"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "%s: %s\n", APP_NAME, "No file name given");
    fprintf(stderr, "Usage: %s <file name>\n", APP_NAME);

    exit(EXIT_FAILURE);
  }

  CvCapture *file = cvCaptureFromFile(argv[1]);

  if (file == NULL) {
    exit(EXIT_FAILURE);
  }

  cvReleaseCapture(&file);

  return 0;
}
