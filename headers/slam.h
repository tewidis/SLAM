#include <opencv2/opencv.hpp>
#include <vector>

#include "extractor.h"

bool process_frame( const char* WIN, Frame* f, std::vector<Frame*>* all_frames, cv::Mat Kinv );

double normalize( cv::Point2f pt, cv::Mat Kinv );
