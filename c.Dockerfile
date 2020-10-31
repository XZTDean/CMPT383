FROM debian:10

RUN apt-get update \
  && apt-get install -y build-essential gcc g++ clang \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN g++ -Wall -O2 -o hello codes/hello.cpp

CMD ./hello
