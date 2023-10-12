#include <iostream>
#include <vector>
#include <stdio.h>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include <ecal/msg/protobuf/subscriber.h>
#include "TCD_tracking.pb.h"
#include "TCD_detection.pb.h"
#include "TopViewCalibRearPb.pb.h"
#include "VehicleDynamicsInPb.pb.h"
// #include "ini/include/INIReader.h"


int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    std::cout <<"loading program options from: " << argv[1] << std::endl;
    // INIReader reader("test.ini");

    // auto iniValue = static_cast<float>(reader.GetReal("user", "pi", -1));
    // std::cout <<"pi value: " << iniValue << std::endl;


    // Publisher names
    const char* topicTCD_tracking = "TCD_tracking";
    const char* topicTCD_detection = "TCD_detection";
    const char* topicCameraCalibration = "TopViewCalibRearPb";
    const char* topicVehicleDynamics = "VehicleDynamicsInPb";

    // Creating Process
    eCAL::Initialize(argc, argv, "CPP_Publishers");
    eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, " ");

    // Creating publishers and their messages
    eCAL::protobuf::CPublisher<pb::TCD_tracking> pubTracking(topicTCD_tracking);
    pb::TCD_tracking msgTracking;

    eCAL::protobuf::CPublisher<pb::TCD_detection> pubDetection(topicTCD_detection);
    pb::TCD_detection msgDetection;

    eCAL::protobuf::CPublisher<pb::Calibration::CameraCalibration> pubCalibration(topicCameraCalibration);
    pb::Calibration::CameraCalibration msgCalibration;

    eCAL::protobuf::CPublisher<pb::SensorNearData::VehicleDynamics> pubVehicleDynamics(topicVehicleDynamics);
    pb::SensorNearData::VehicleDynamics msgVehicleDynamics;

    eCAL::Process::SleepMS(1000);

    long long time = 0;
    int i { 0 };
    while (eCAL::Ok()) {
      msgTracking.set_frameid(++i);
      // In proto3, 0 is the default value, each field not defined is set to 0 thus not appearing in ecal monitor
      msgTracking.mutable_header()->set_timestamp(123456);
      msgTracking.set_worldposx(0.0);
      msgTracking.set_worldposy(0.5);
      msgTracking.set_worldposz(-0.5);
      // TCD_detection imported in TCD_tracking
      msgTracking.mutable_tcd_detection()->set_frameid(654987);
      std::cout << "sending from TCD_tracking frameid: " << i << std::endl;

      // timestamp is inside header message, see TCD_detection.proto and header.proto
      msgDetection.mutable_header()->set_timestamp(i);
      msgDetection.set_xmax(5);
      msgDetection.set_ymax(7);
      msgDetection.set_xmin(9);
      msgDetection.set_ymin(-8);
      std::cout << "sending from TCD_detection header.timestamp: " << i << std::endl;

      pubTracking.Send(msgTracking);
      pubDetection.Send(msgDetection);
      eCAL::Process::SleepMS(500);

    }

    return 0;
}
