FROM openjdk:14

WORKDIR /app
COPY . .

# for ZeroMQ server
#EXPOSE 5555

WORKDIR /app/hello-world

RUN javac Hello.java
CMD java Hello
