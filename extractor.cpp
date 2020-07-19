#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

#include "headers/extractor.h"

using namespace std;
using namespace cv;

Frame::Frame( )
{
    vector<KeyPoint> kps;
    Mat des;
}

void Frame::extract( Mat window )
{
    // make the frame black and white
    Mat framebw;
    cvtColor( window, framebw, COLOR_BGR2GRAY );

    // detecting the "good" features from the frame
    vector<Point2f> features;
    goodFeaturesToTrack( framebw, features, 3000, 0.01, 7);

    // convert features to KeyPoints
    for( size_t i = 0; i < features.size(); i++ )
    {
        this->kps.push_back( KeyPoint( features[i], 20 ) );
    }

    // extract the descriptors
    Ptr<ORB> orb = ORB::create();
    orb->compute( window, this->kps, this->des );

    // clean up before exiting
    framebw.release();
    features.clear();
}

void Frame::match_frames( Frame* f1, Frame* f2 )
{
    // matching
    vector<DMatch> matches;
    BFMatcher bf(NORM_HAMMING);
    bf.match( f1->des, matches );
}
