#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

int process_frame( const char* WIN, Mat frame, int frameNum )
{
    Ptr<ORB> orb = ORB::create();
    if( frame.empty() )
    {
        cout << "Video Over" << endl;
        return 1;
    }

    cout << "Frame: " << frameNum << endl;
    resize( frame, frame, Size(480, 270) );

    // make the frame black and white
    Mat framebw;
    cvtColor( frame, framebw, COLOR_BGR2GRAY );

    // detecting the "good" features from the frame
    vector<Point2f> features;
    goodFeaturesToTrack( framebw, features, 3000, 0.01, 7);

    // convert features to KeyPoints and extract the descriptors
    vector<KeyPoint> kps;
    for( size_t i = 0; i < features.size(); i++ )
    {
        kps.push_back( KeyPoint( features[i], 20 ) );
    }
    Mat descriptors;
    orb->compute( frame, kps, descriptors );

    // matching
    vector<DMatch> matches;
    BFMatcher bf(NORM_HAMMING);
    bf.match( descriptors, matches );

    for( auto it = matches.begin(); it != matches.end(); ++it )
    {
        cout << it->queryIdx << " " << it->trainIdx << endl;
    }

    // plot the features over the frame
    for( auto it = kps.begin(); it != kps.end(); ++it )
    {
        circle( frame, it->pt, 3, Scalar(0,255,0) );
    }

    imshow( WIN, frame );

    char c = (char) waitKey(1);
    if( c == 27 ) return 1;
    return 0;
}

int main(int argc, char** argv)
{
    VideoCapture vid( "videos/test_countryroad.mp4" );
    if( !vid.isOpened() )
    {
        cout << "Could not open reference" << endl;
        return -1;
    }

    Mat frame;
    const char* WIN = "Original Video";
    namedWindow( WIN, WINDOW_AUTOSIZE );
    moveWindow( WIN, 420, 240 );
    int frameNum = -1;
    int isOver;

    do
    {
        vid >> frame;
        isOver = process_frame( WIN, frame, frameNum );
        frameNum = frameNum + 1;
    }
    while( !isOver );

    return 0;
}

