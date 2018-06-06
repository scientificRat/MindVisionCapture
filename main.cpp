#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "MindVisionCapture.h"

using namespace cv;

int main() {
    MindVisionCapture capture;
    Mat frame;
    namedWindow("test", CV_WINDOW_NORMAL);
    double exposure_time = 4000;
    while (true) {
        time_t t1 = clock();
        if (!capture.read(frame)) {
            printf("error!\n");
            break;
        }
        std::cout<<clock()-t1<<std::endl;
        imshow("test", frame);
        int key = (char) waitKey(1);
        if (key == 'q') {
            break;
        } else if (key == 'w') {
            exposure_time += 500;
            capture.setExposureTime(exposure_time);
            printf("exposure time: %.2lf ms\n", exposure_time / 1000);

        } else if (key == 's') {
            exposure_time -= 500;
            capture.setExposureTime(exposure_time);
            printf("exposure time: %.2lf ms\n", exposure_time / 1000);
        }


    }
    return 0;
}