#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

#include "headers/slam.h"
#include "headers/extractor.h"

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
    const char* WIN = "Original Video";
    namedWindow( WIN, WINDOW_AUTOSIZE );
    moveWindow( WIN, 420, 240 );
    int isOver;

    do
    {
        vid >> frame;
        isOver = process_frame( WIN, frame );
    }
    while( !isOver );

    return 0;
}

int process_frame( const char* WIN, Mat frame )
{
    if( frame.empty() )
    {
        cout << "Video Over" << endl;
        return 1;
    }

    resize( frame, frame, Size(480, 270) );

    Frame* f = new Frame();
    f->extract( frame );

    // plot the features over the frame
    for( auto it = f->kps.begin(); it != f->kps.end(); ++it )
    {
        circle( frame, it->pt, 3, Scalar(0,255,0) );
    }

    imshow( WIN, frame );

    char c = (char) waitKey(1);
    if( c == 27 ) return 1;
    return 0;
}
