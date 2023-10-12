#include <iostream>
#include <ecal/ecal.h>
#include <ecal/msg/protobuf/subscriber.h>
#include "mi_mensaje.pb.h"


int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;

    // Subscribers names
    const char *topic_mi_mensaje = "mensaje 1";

    // Creating Process
    eCAL::Initialize(argc, argv, "C++ Subscriber");
    eCAL::Process::SetState(proc_sev_healthy, proc_sev_level1, " ");

    // Creating subscribers and its message
    eCAL::protobuf::CSubscriber<pb::HelloWorld> sub_mensaje(topic_mi_mensaje);
    pb::HelloWorld protobuf_message;


    eCAL::Process::SleepMS(1000);

    long long time = 0;
    size_t flag_mi_mensaje = 0;

    while (eCAL::Ok()) {
        flag_mi_mensaje = sub_mensaje.Receive(protobuf_message, &time, 50);

        if (flag_mi_mensaje != 0) {
            std::cout << "TimeStamp received: " << protobuf_message.name() << std::endl;
            std::cout << "TimeStamp received: " << protobuf_message.id() << std::endl;
            std::cout << "TimeStamp received: " << protobuf_message.msg() << std::endl;
            std::cout << "TimeStamp received: " << protobuf_message.state() << std::endl;
        }
    }

    return 0;
}
