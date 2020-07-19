#include <vector>

#include <opencv2/core.hpp>

class Frame
{
    public:
        std::vector<cv::KeyPoint> kps;
        cv::Mat des;

        void extract( cv::Mat );
        void match_frames( Frame*, Frame* );
        Frame( );
};

//int extract( Mat window, Frame output );

//void match_frames( Frame f1, Frame f2 );
