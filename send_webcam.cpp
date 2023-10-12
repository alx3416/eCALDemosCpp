#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <iostream>
#include "mensaje_main.pb.h"


int main(int argc, char *argv[])
{
    // Subscribers names
    const char *topic_mi_mensaje = "webcam_data";

    // Creating Process
    eCAL::Initialize(argc, argv, "C++ webcam Publisher");
    eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, " ");
    // Creating subscribers and its message
    eCAL::protobuf::CPublisher<pb::webcam> pub_mensaje(topic_mi_mensaje);
    pb::webcam protobuf_message;

    eCAL::Process::SleepMS(1000);

    cv::Mat frame, frame_gray;
    cv::VideoCapture cap;

    int deviceID = 0; // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    for (;;)
    {
        cap.read(frame);

        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        cv::imshow("Live", frame);

        cv::uint8_t image[640 * 480 * 3];
        int idx = 0;
        for(int col=0; col<frame.cols; col++){
            for(int row=0; row<frame.rows; row++){
                image[idx++] = frame.at<cv::Vec3b>(row,col)[0];
                image[idx++] = frame.at<cv::Vec3b>(row,col)[1];
                image[idx++] = frame.at<cv::Vec3b>(row,col)[2];
            }
        }

        protobuf_message.mutable_frame()->set_height(frame.cols);
        protobuf_message.mutable_frame()->set_width(frame.rows);
        protobuf_message.mutable_frame()->set_name("Logitech C920");
        protobuf_message.mutable_frame()->set_imagecompression(pb::UNCOMPRESSED);
        protobuf_message.mutable_frame()->set_imageformat(pb::RGB);
        protobuf_message.mutable_frame()->set_data(&image, frame.rows * frame.cols * 3);
        pub_mensaje.Send(protobuf_message);
        eCAL::Process::SleepMS(5);

        if (cv::waitKey(5) >= 0)
            break;
    }
    return 0;
}