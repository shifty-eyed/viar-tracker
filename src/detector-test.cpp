
#include <iostream>
#include <fstream> 
#include <filesystem> 
#include <math.h>
#include <stdio.h>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/videoio/registry.hpp"


#include "../MyArucoDetector.h"
#include "../BodyPoseDetector.h"

using namespace std;
using namespace cv;

#define WINDOW_NAME "Testing Detector"

#define YELLOW Scalar(0,255,200)
#define BLUE Scalar(255,0,0)
#define GREEN Scalar(0,255,0)
#define RED Scalar(0,0,255)

#define EXPECTED_DETECTION_HITS 1
int actualDetectedHits = 0;
//MyArucoDetector arucoDetector = MyArucoDetector();
//BodyPoseDetector bodyDetector = BodyPoseDetector();

const int POSE_PAIRS[NUM_PAIRS][2] = {   // COCO body
    {1,2}, {1,5}, {2,3},
    {3,4}, {5,6}, {6,7},
    {1,8}, {8,9}, {9,10},
    {1,11}, {11,12}, {12,13},
    {1,0}, {0,14},
    {14,16}, {0,15}, {15,17}
};

int doMagic(Mat &src, Mat& dst) {

    vector<ArucoMarkerPosition> arucos;
    vector<BodyPose> bodies;

    //arucoDetector.detect(src, arucos);
    //string metrics = bodyDetector.detect(src, bodies);

    src.copyTo(dst);



    for (ArucoMarkerPosition m : arucos) {
        circle(dst, Point2f(m.x,m.y), 4, YELLOW, 2);
        for (int i = 0;i < 4;i++) {
            int next = i == 3 ? 0 : i + 1;
            line(dst, m.corners[i], m.corners[next], BLUE);
        }
    }


    //dro
    for (BodyPose body : bodies) {
        for (int n = 0; n < body.points.size(); n++ ) {
            Point2f p = body.points[n];
            if (p.x > 0 && p.y > 0) {
                circle(dst, p, 3, RED, 1);
                putText(dst, to_string(n), p, FONT_HERSHEY_SIMPLEX, 0.8f, BLUE, 2);
            }
        }
    }

    //putText(dst, metrics, Point(30, 30), FONT_HERSHEY_SIMPLEX, 0.5f, YELLOW, 1);

    return arucos.size();
}


int main(int argc, char* argv[]) {

    int camId = 0;//atoi(argv[1]);
    cout << "opening cam " << camId << std::endl;

    //CAP_MSMF = 1400,
    //CAP_GSTREAMER    = 1800, 
    //CAP_DSHOW        = 700
    //CAP_UEYE = 2500,         //!< uEye Camera API

    /*CAP_PROP_FOURCC MJPG
CAP_PROP_FORMAT -1
CAP_PROP_ISO_SPEED -1
CAP_PROP_FPS 0
CAP_PROP_APERTURE -1
CAP_PROP_EXPOSURE -6
CAP_PROP_BACKEND 700
CAP_PROP_BITRATE -1
CAP_PROP_BUFFERSIZE -1
Details: DSHOW*/

    /*CAP_PROP_FOURCC YUY2
CAP_PROP_FORMAT -1
CAP_PROP_ISO_SPEED -1
CAP_PROP_FPS 0
CAP_PROP_APERTURE -1
CAP_PROP_EXPOSURE -6
CAP_PROP_BACKEND 700
CAP_PROP_BITRATE -1
CAP_PROP_BUFFERSIZE -1*/

    cout << "CV build info:" << endl << cv::getBuildInformation() << std::endl << "--" << endl;

    VideoCapture camera(camId, CAP_ANY, {
        CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        CAP_PROP_FRAME_WIDTH, 1920,
        CAP_PROP_FRAME_HEIGHT, 1080
        });


    auto fourcc = (int)camera.get(CAP_PROP_FOURCC);
    cout << "props:" << endl;
    cout << "CAP_PROP_FOURCC " << static_cast<char>(fourcc & 0xFF) << static_cast<char>(fourcc >> 8 & 0xFF)
        << static_cast<char>(fourcc >> 16 & 0xFF) << static_cast<char>(fourcc >> 24 & 0xFF) << endl;
    cout << "CAP_PROP_FORMAT " << camera.get(CAP_PROP_FORMAT) << endl;
    cout << "CAP_PROP_ISO_SPEED " << camera.get(CAP_PROP_ISO_SPEED) << endl;

    cout << "CAP_PROP_FPS " << camera.get(CAP_PROP_FPS) << endl;
    cout << "CAP_PROP_APERTURE " << camera.get(CAP_PROP_APERTURE) << endl;
    cout << "CAP_PROP_EXPOSURE " << camera.get(CAP_PROP_EXPOSURE) << endl;
    cout << "CAP_PROP_BACKEND " << camera.get(CAP_PROP_BACKEND) << endl;
    cout << "CAP_PROP_BITRATE " << camera.get(CAP_PROP_BITRATE) << endl;
    cout << "CAP_PROP_BUFFERSIZE " << camera.get(CAP_PROP_BUFFERSIZE) << endl;
    

    //camera.set(CAP_PROP_FRAME_WIDTH, 1920); camera.set(CAP_PROP_FRAME_HEIGHT, 1080);
    //camera.set(CAP_PROP_FPS, 30);
    //camera.set(CAP_PROP_FRAME_WIDTH, 1280); camera.set(CAP_PROP_FRAME_HEIGHT, 720);
    if (!camera.isOpened()) {
        cerr << "ERROR: Could not open camera" << endl;
        return 1;
    }
    else {
        cout << "Details: " << camera.getBackendName() << endl;
    }

    namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);

    Mat src;
    Mat dst;

    clock_t t = clock();
    int frameCount = 0;
    int fps = 0;
    while (1) {
        camera >> src;
        frameCount++;
        actualDetectedHits += doMagic(src, dst);

        cv::imshow(WINDOW_NAME, dst);


        if (clock() - t > CLOCKS_PER_SEC) {
            t = clock();
            fps = frameCount;
            double hitRate = (double)actualDetectedHits / (double)(EXPECTED_DETECTION_HITS * frameCount);
            printf("fps: %d, hitRate=%d%%\n", fps, (int)(hitRate*100.0));
            frameCount = 0;
            actualDetectedHits = 0;
        }
        
        char key = (char)cv::waitKey(1);
        if (key != -1) {
            break;
        }
    }
    return 0;

}

