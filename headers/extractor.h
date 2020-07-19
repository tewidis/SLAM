#include <vector>

#include <opencv2/core.hpp>

class Frame
{
    public:
        std::vector<cv::KeyPoint> kps;
        cv::Mat des;
        cv::Mat frame;

        void extract( cv::Mat );
        void match_frames( Frame*, Frame*, std::vector<std::vector<cv::DMatch>> );
        Frame( cv::Mat );
};

void match_frames( Frame* f1, Frame* f2, std::vector<std::vector<cv::DMatch>>* );
