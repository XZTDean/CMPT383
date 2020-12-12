#include <iostream>
#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <string>
#include <unistd.h>
#include <thread>
#include "ReversiManager.h"

using namespace std;

ReversiManager manager;

void receiveMessage(AMQP::TcpChannel& channel, const AMQP::Message& m, uint64_t tag, const string& id) {
    string received = m.body();
    received = received.substr(0, m.bodySize());
    char first = received.at(0);
    string args = received.substr(2);
    string result;
    switch (first) {
        case 'N':
            result = manager.newGame(args);
            break;
        case 'G':
            result = manager.getGame(args);
            break;
        case 'U':
            result = manager.userMove(args);
            break;
        case 'C':
            result = manager.computerMove(args);
            break;
        case 'R':
            manager.removeGame(args);
            result = "Finish";
            break;
        default:
            result = "Invalid";
    }
    AMQP::Envelope env(result.data(), result.size());
    env.setCorrelationID(id);
    channel.publish("", m.replyTo(), env);
    channel.ack(tag);
}

void heartbeat(AMQP::TcpConnection& connection) {
    connection.heartbeat();
    std::this_thread::sleep_for (std::chrono::seconds(30));
}

int main() {
    sleep(10);
    event_base *base = event_base_new();
    AMQP::LibEventHandler handler(base);
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://rabbitmq/"));
    AMQP::TcpChannel channel(&connection);
    channel.setQos(5);
    channel.declareQueue("reversi_call");
    channel.consume("").onReceived([&channel](const AMQP::Message& m, uint64_t tag, bool){
        thread t(receiveMessage, ref(channel), ref(m), tag, m.correlationID());
        t.detach();
        cout << "onReceived end" << endl;
    });

    thread t(heartbeat, ref(connection));

    event_base_dispatch(base);
    return 0;
}
