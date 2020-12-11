#include <iostream>
#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <string>
#include <unistd.h>

using namespace std;

int main() {
    sleep(10);
    event_base *base = event_base_new();
    AMQP::LibEventHandler handler(base);
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://rabbitmq/"));
    AMQP::TcpChannel channel(&connection);
    channel.declareQueue("reversi_call");
    channel.consume("").onReceived([&channel](const AMQP::Message& m, uint64_t tag, bool){
        string received = m.body();
        received = received.substr(0, m.bodySize());
        cout << received << endl;
        string str = "THIS IS FROM C++";
        AMQP::Envelope env(str.data(), str.size());
        env.setCorrelationID(m.correlationID());
        channel.publish("", m.replyTo(), env);
        channel.ack(tag);
    });

    event_base_dispatch(base);
    return 0;
}
