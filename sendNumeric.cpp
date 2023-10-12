#include <iostream>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/publisher.h>
#include "mi_mensaje.pb.h"


int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    // Subscribers names
    const char *topic_mi_mensaje = "mensaje 1";

    // Creating Process
    eCAL::Initialize(argc, argv, "C++ Publisher");
    eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, " ");

    // Creating subscribers and its message
    eCAL::protobuf::CPublisher<pb::HelloWorld> pub_mensaje(topic_mi_mensaje);
    pb::HelloWorld protobuf_message;


    eCAL::Process::SleepMS(1000);

    long long time = 0;
    int i {0};
    while (eCAL::Ok()) {
        protobuf_message.set_name("zutano menganillo");
        protobuf_message.set_id(i++);
        protobuf_message.set_msg(-987.654);
        protobuf_message.set_state(true);
        pub_mensaje.Send(protobuf_message);
        eCAL::Process::SleepMS(1000);
    }

    return 0;
}
