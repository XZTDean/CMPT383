FROM debian:10

RUN apt-get update \
  && apt-get install -y build-essential gcc g++ clang git libssl-dev libevent-dev \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git \
  && cd AMQP-CPP && make && make install

WORKDIR /app
COPY . .

RUN g++ -Wall -O2 -levent -lamqpcpp -lpthread -ldl -o x codes/cpp/main.cpp

CMD ./x
