#include <stdio.h>
#include <stdbool.h>
#include <highgui.h>

#define APP_NAME "lucas-kanade-opencv"
#define OUTPUT_FILE_NAME "output.avi"
#define FOURCC CV_FOURCC('X', 'V', 'I', 'D')
#define MAX_CORNERS 500
#define WINDOW_SIZE 10

IplImage *query_frame(CvCapture *video, IplImage **frame_buffer, IplImage *current_frame) {
  IplImage *frame = cvQueryFrame(video);

  if (frame) {
    cvConvertImage(frame, current_frame, 0);

    frame_buffer[1] = frame_buffer[0];
    cvConvertImage(frame, frame_buffer[0], 0);
  }

  return frame;
}

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
  long video_frame_count = cvGetCaptureProperty(video, CV_CAP_PROP_FRAME_COUNT);

  // Initialize video writer
  CvVideoWriter *video_writer = cvCreateVideoWriter(output_file_name,
    FOURCC, video_fps, video_frame_size, true);

  IplImage *frame_buffer[2];
  frame_buffer[0] = cvCreateImage(video_frame_size, IPL_DEPTH_8U, 1);
  frame_buffer[1] = cvCreateImage(video_frame_size, IPL_DEPTH_8U, 1);
  IplImage *current_frame = cvCreateImage(video_frame_size, IPL_DEPTH_8U, 3);

  IplImage *eigen_image = cvCreateImage(video_frame_size, IPL_DEPTH_32F, 1);
  IplImage *temp_image = cvCreateImage(video_frame_size, IPL_DEPTH_32F, 1);
  int corner_count = MAX_CORNERS;
  CvPoint2D32f corners[2][MAX_CORNERS];

  char features_found[MAX_CORNERS];
  float feature_errors[MAX_CORNERS];
  CvSize pyramid_size = cvSize(video_frame_size.width + 8, video_frame_size.height / 3);
  IplImage *pyramid_images[2];
  pyramid_images[0] = cvCreateImage(video_frame_size, IPL_DEPTH_32F, 1);
  pyramid_images[1] = cvCreateImage(video_frame_size, IPL_DEPTH_32F, 1);

  // Process video
  while (query_frame(video, frame_buffer, current_frame)) {
    cvGoodFeaturesToTrack(
      frame_buffer[0],
      eigen_image,
      temp_image,
      corners[0],
      &corner_count,
      0.01,
      5.0,
      0,
      3,
      0,
      0.4);

    cvFindCornerSubPix(
      frame_buffer[0],
      corners[0],
      corner_count,
      cvSize(WINDOW_SIZE, WINDOW_SIZE),
      cvSize(-1, -1),
      cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3));

    cvCalcOpticalFlowPyrLK(
      frame_buffer[0],
      frame_buffer[1],
      pyramid_images[0],
      pyramid_images[1],
      corners[0],
      corners[1],
      corner_count,
      cvSize(WINDOW_SIZE, WINDOW_SIZE),
      5,
      features_found,
      feature_errors,
      cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3),
      0);

    int i;
    for (i = 0; i < corner_count; i++) {
      if (features_found[i] == 0 || feature_errors[i] > 550) {
        continue;
      }

      CvPoint points[2];
      points[0] = cvPoint(cvRound(corners[0][i].x), cvRound(corners[0][i].y));
      points[1] = cvPoint(cvRound(corners[1][i].x), cvRound(corners[1][i].y));

      cvLine(current_frame, points[0], points[1], CV_RGB(255, 0, 0), 2, CV_AA, 0);
    }

    cvWriteFrame(video_writer, current_frame);
  }

  // Clean up
  // TODO: Release stuff
  cvReleaseCapture(&video);
  cvReleaseVideoWriter(&video_writer);

  return 0;
}
