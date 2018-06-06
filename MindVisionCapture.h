//
// Created by scientificrat on 18-6-4.
//

#ifndef MINDVISIONCAPTURE_MINDVISIONCAPTURE_H
#define MINDVISIONCAPTURE_MINDVISIONCAPTURE_H

#include <mindvision/CameraApi.h>
#include <mindvision/CameraDefine.h>
#include <mindvision/CameraStatus.h>
#include <opencv2/core.hpp>

class MindVisionCapture {

private:
    unsigned char *imageBuffer = nullptr;
    int hCamera = 0; //handle of camera
    int channel = 0;

public:
    explicit MindVisionCapture();

    ~MindVisionCapture();

    MindVisionCapture &operator>>(cv::Mat &frame);

    bool read(cv::Mat &frame);

    void info();

    void setExposureTime(double time);

private:

    bool initCamera();
};


#endif //MINDVISIONCAPTURE_MINDVISIONCAPTURE_H
