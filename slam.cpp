#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    int frameNum = -1;
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

    for(;;)
    {
        vid >> frame;

        if( frame.empty() )
        {
            cout << "Video Over" << endl;
        }

        frameNum = frameNum + 1;
        cout << "Frame: " << frameNum << endl;
        resize( frame, frame, Size(480, 270) );

        imshow( WIN, frame );

        char c = (char) waitKey(0);
        if( c == 27 ) break;

    }
    return 0;
}
