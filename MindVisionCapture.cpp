//
// Created by scientificrat on 18-6-4.
//

#include "MindVisionCapture.h"

MindVisionCapture::MindVisionCapture() {
    if (!initCamera()) {
        exit(-1);
    } else {
        printf("camera init success!\n");
    }
}

MindVisionCapture &MindVisionCapture::operator>>(cv::Mat &frame) {
    if (!this->read(frame)) {
        fprintf(stderr, "frame read fail!\n");
    }
    return *this;
}

bool MindVisionCapture::read(cv::Mat &frame) {
    tSdkFrameHead sFrameInfo;
    BYTE *pbyBuffer;
    IplImage *iplImage = NULL;
    const int waitTime = 1000; // timeout in ms
    if (CameraGetImageBuffer(hCamera, &sFrameInfo, &pbyBuffer, waitTime) != CAMERA_STATUS_SUCCESS) {
        return false;
    }
    CameraImageProcess(hCamera, pbyBuffer, this->imageBuffer, &sFrameInfo);
    frame = cv::Mat(sFrameInfo.iHeight, sFrameInfo.iWidth, CV_8UC(channel));
    frame.data = this->imageBuffer;
    CameraReleaseImageBuffer(hCamera, pbyBuffer);
    return true;
}

bool MindVisionCapture::initCamera() {

    int iCameraCounts = 1;
    int initStatus = -1;
    tSdkCameraDevInfo tCameraEnumList;
    tSdkCameraCapbility tCapability;      //设备描述信息
    int iDisplayFrames = 10000;

    CameraSdkInit(1);

    //枚举设备，并建立设备列表
    CameraEnumerateDevice(&tCameraEnumList, &iCameraCounts);

    //没有连接设备
    if (iCameraCounts == 0) {
        fprintf(stderr, "Device not found, make sure you are in root\n");
        return false;
    }

    //相机初始化,初始化成功后,才能调用任何其他相机相关的操作接口
    initStatus = CameraInit(&tCameraEnumList, -1, -1, &hCamera);

    //初始化失败
    if (initStatus != CAMERA_STATUS_SUCCESS) {
        fprintf(stderr, "init failed, code:%d\n", initStatus);
        return false;
    }

    //获得相机的特性描述结构体。该结构体中包含了相机可设置的各种参数的范围信息。决定了相关函数的参数
    CameraGetCapability(hCamera, &tCapability);


    this->imageBuffer = (unsigned char *) malloc(
            tCapability.sResolutionRange.iHeightMax * tCapability.sResolutionRange.iWidthMax * 3);
    //g_readBuf = (unsigned char*)malloc(tCapability.sResolutionRange.iHeightMax*tCapability.sResolutionRange.iWidthMax*3);

    CameraPlay(hCamera);

    setExposureTime(4000);
    int code = CameraSetOnceWB(hCamera);
    printf("%d\n", code);

    info();

    if (tCapability.sIspCapacity.bMonoSensor) {
        channel = 1;
        CameraSetIspOutFormat(hCamera, CAMERA_MEDIA_TYPE_MONO8);
    } else {
        channel = 3;
        CameraSetIspOutFormat(hCamera, CAMERA_MEDIA_TYPE_BGR8);
    }

    return true;
}

MindVisionCapture::~MindVisionCapture() {
    CameraUnInit(hCamera);
    free(this->imageBuffer);
}

void MindVisionCapture::info() {
    printf("\nMindVision Camera Info\n");
    double exposureTime = 0;
    if (!CameraGetExposureTime(hCamera, &exposureTime)) {
        printf("Exposure time: %lf us\n", exposureTime);
    }
    int gain_r, gain_g, gain_b = 0;
    if (!CameraGetGain(hCamera, &gain_r, &gain_g, &gain_b)) {
        printf("Gain(r,g,b): %d,%d,%d \n", gain_r, gain_g, gain_b);
    }
    int frame_speed;
    if (!CameraGetFrameSpeed(hCamera, &frame_speed)) {
        printf("Frame speed: %d \n", frame_speed);
    }

}

void MindVisionCapture::setExposureTime(double time) {
    if (int code = CameraSetAeState(hCamera, false)) {
        fprintf(stderr, "turn off auto exposure error,code: %d \n", code);
    }
    if (int code = CameraSetExposureTime(hCamera, time)) {
        fprintf(stderr, "set exposure time error,code: %d \n", code);
    }
}
