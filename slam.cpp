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
    vector<KeyPoint> kp;
    Mat desc;

    if( frame.empty() )
    {
        cout << "Video Over" << endl;
        return 1;
    }

    cout << "Frame: " << frameNum << endl;
    resize( frame, frame, Size(480, 270) );
    orb->detectAndCompute( frame, noArray(), kp, desc );

    for( vector<KeyPoint>::iterator it = kp.begin(); it != kp.end(); ++it )
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

