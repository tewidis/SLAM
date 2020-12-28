#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

#include "headers/slam.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture vid( "videos/test_countryroad.mp4" );
    if( !vid.isOpened() )
    {
        cout << "Could not open reference" << endl;
        return -1;
    }

    Mat frame;
    int W = 1920/2;
    int H = 1080/2;
    int F = 200; // focal length
    Mat K = ( Mat_<double>(3,3) << F, 0, W/2, 0, F, H/2, 0, 0, 1 ); // camera intrinsic matrix
    Mat Kinv = K.inv();

    const char* WIN = "Original Video";
    namedWindow( WIN, WINDOW_NORMAL );
    resizeWindow( WIN, W, H );
    moveWindow( WIN, 0, 0 );
    bool isOver;
    vector<Frame*>* all_frames = new vector<Frame*>();

    do
    {
        vid >> frame;
        if( frame.empty() )
        {
            cout << "Video Over" << endl;
            break;
        }
        resize( frame, frame, Size(480, 270) );
        Frame* f = new Frame( frame );
        isOver = process_frame( WIN, f, all_frames, K );
    }
    while( !isOver );

    // clean up memory
    for( auto& it : *all_frames )
    {
        delete it;
    }
    delete all_frames;
    return 0;
}

double normalize( Point2f pt, Mat Kinv )
{
    Mat expanded = ( Mat_<double>(3,1) << double(pt.x), double(pt.y), 1 );
    return Kinv.dot( expanded );
}

bool process_frame( const char* WIN, Frame* f, vector<Frame*>* all_frames, Mat K )
{
    // detect and extract the features
    f->extract( f->frame );
    all_frames->push_back( f );

    // if this isn't the first frame, match
    Mat outimg = f->frame;
    vector<vector<DMatch>> matches;
    vector<Point2f> matches1;
    vector<Point2f> matches2;

    if( all_frames->size() > 1 )
    {
        match_frames( all_frames->end()[-1], all_frames->end()[-2], &matches );
    }

    // plot the key points over the frame
    drawKeypoints( outimg, f->kps, outimg, Scalar(0,255,0) );

    // plot the matches from one frame to the next
    for( int i = 0; i < matches.size(); i++ )
    {
        // TODO: is matches being indexed properly?
        Point2f pt1 = all_frames->end()[-1]->kps[matches[i][0].queryIdx].pt;
        Point2f pt2 = all_frames->end()[-2]->kps[matches[i][0].trainIdx].pt;

        line( outimg, pt1, pt2, Scalar(255,0,0) );
        matches1.push_back( pt1 );
        matches2.push_back( pt2 );
    }

    if( all_frames->size() > 1 )
    {
        //Mat h = findEssentialMat( matches1, matches2, K, RANSAC, 0.99, 1 );

        //Mat h, S, U, VT;
        Mat h = findFundamentalMat( matches1, matches2, RANSAC, 3, 0.99 );
        //SVDecomp( h, S, U, VT, cv::SVD::FULL_UV );
        cout << h << endl;
    }

    imshow( WIN, outimg );

    char c = (char) waitKey(1);
    if( c == 27 ) return true;
    return false;
}
