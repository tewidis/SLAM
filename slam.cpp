#include <iostream>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

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
    const char* WIN = "Original Video";
    namedWindow( WIN, WINDOW_AUTOSIZE );
    moveWindow( WIN, 420, 240 );
    int isOver;
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
        isOver = process_frame( WIN, f, all_frames );
    }
    while( !isOver );

    delete( all_frames );
    return 0;
}

int process_frame( const char* WIN, Frame* f, vector<Frame*>* all_frames )
{
    // detect and extract the features
    f->extract( f->frame );
    all_frames->push_back( f );

    // if this isn't the first frame, match
    Mat outimg = f->frame;
    vector<vector<DMatch>> matches;

    if( all_frames->size() > 1 )
    {
        match_frames( all_frames->end()[-1], all_frames->end()[-2], &matches );
        cout << matches.size() << endl;
        // plot the matches
        drawMatches( all_frames->end()[-1]->frame, all_frames->end()[-1]->kps, \
                     all_frames->end()[-2]->frame, all_frames->end()[-2]->kps, \
                     matches, outimg, Scalar(255,0,0) );
    }

    // plot the key points over the frame
    //drawKeypoints( outimg, f->kps, outimg, Scalar(0,255,0) );
    imshow( WIN, outimg );

    char c = (char) waitKey(1);
    if( c == 27 ) return 1;
    return 0;
}
