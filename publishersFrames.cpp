#include <iostream>
#include <vector>
#include <stdio.h>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include "SvcImageRearPb.pb.h"

void reshapeArray(std::vector<int> &out_array, const int height, const int width, const std::vector<std::vector<int>> &vraw);

int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;

    // Publisher names
    const char* topicFrame = "SvcImageRearPb";


    // Creating Process
    eCAL::Initialize(argc, argv, "CPP_PublisherFrame");
    eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, " ");

    // Creating publishers and their messages
    eCAL::protobuf::CPublisher<pb::SvcImageRearPb> publisher(topicFrame);
    pb::SvcImageRearPb message;

    eCAL::Process::SleepMS(1000);

    message.mutable_data()->set_height(32);
    message.mutable_data()->set_width(32);
    message.mutable_data()->set_datasize(32*32);
    message.mutable_data()->set_bitsperpixel(8);
    message.mutable_data()->set_channelformat(pb::BaseImage_ChannelFormat_RAW);
    message.mutable_data()->set_pixelformat(pb::BaseImage_PixelFormat_UNSIGNED);
    message.mutable_data()->set_compression(pb::BaseImage_Compression_UNCOMPRESSED);

    // std::vector<std::vector<int>> arrayRaw(message.data().height(), std::vector<int>(message.data().width(), 0));
    // std::vector<int> vectorRaw(message.data().datasize(), 0);
    // message.SerializeToArray(&arrayRaw[0], arrayRaw.size());
    // message.mutable_data()->set_imagedata(&arrayRaw, arrayRaw.size());

    long long time = 0;
    int i { 1 };

    while (eCAL::Ok()) {
      // timestamp is inside header message, see TCD_detection.proto and header.proto
      message.mutable_header()->set_timestamp(++i);
      if (i>=255){i=1;}

      int patch[32][32] = { 250, 128, 25, 123 };
      int arrSize = sizeof(patch)/sizeof(patch[0][0]);
      // std::vector<std::vector<int>> arrayRaw(message.data().height(), std::vector<int>(message.data().width(), i));
      // std::vector<int> vectorRaw(message.data().datasize(), 34);
      // reshapeArray(vectorRaw, message.data().height(), message.data().width(), arrayRaw);

      message.mutable_data()->set_imagedata(&patch, arrSize);
      std::cout << "sending header.timestamp: " << i << std::endl;

      publisher.Send(message);
      eCAL::Process::SleepMS(100);

    }

    return 0;
}

void reshapeArray(std::vector<int> &out_array, const int height, const int width, const std::vector<std::vector<int>> &vRaw){
    int idx {0};
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            out_array[idx++] = vRaw[i][j];
        }
    }
}