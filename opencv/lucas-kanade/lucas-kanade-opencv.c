#include <stdio.h>
#include <stdbool.h>
#include <highgui.h>

#define APP_NAME "lucas-kanade-opencv"
#define OUTPUT_FILE_NAME "output.avi"
#define FOURCC CV_FOURCC('X', 'V', 'I', 'D')

int main(int argc, char **argv) {
  // Check parameters
  if (argc < 2) {
    fprintf(stderr, "%s: %s\n", APP_NAME, "No video name given");
    fprintf(stderr, "Usage: %s <video file name> [output file name]\n", APP_NAME);

    exit(EXIT_FAILURE);
  }

  char *output_file_name;
  if (argc == 3) {
    output_file_name = argv[2];
  }
  else {
    output_file_name = OUTPUT_FILE_NAME;
  }

  // Load video
  char *file_name = argv[1];
  CvCapture *video = cvCaptureFromFile(file_name);

  if (!video) {
    exit(EXIT_FAILURE);
  }

  // Extract video parameters
  CvSize video_frame_size;
  video_frame_size.width = cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_WIDTH);
  video_frame_size.height = cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_HEIGHT);
  double video_fps = cvGetCaptureProperty(video, CV_CAP_PROP_FPS);

  // Initialize video writer
  CvVideoWriter *video_writer = cvCreateVideoWriter(output_file_name,
    FOURCC, video_fps, video_frame_size, true);

  // Process video
  IplImage *frame;
  while (frame = cvQueryFrame(video)) {
    cvWriteFrame(video_writer, frame);
  }

  // Clean up
  cvReleaseImage(&frame);
  cvReleaseCapture(&video);
  cvReleaseVideoWriter(&video_writer);

  return 0;
}
