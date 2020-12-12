## Topic idea

A online Reversi game that user play with AI.

## Language used

C++, Python, JavaScript

## Communication method

**Restful API** connect between Python and JavaScript

**RabbitMQ** connect between C++ and Python

## Implementation

**JavaScript** manage the front end (browser). And send request to Python through Restful API. **Python** hold the Restful API in server and manage the requests. And Python past required calculation part back to C++ through RabbitMQ. **C++** can calculate what next step computer will make. Because the large calculation required, C++ can be much faster than Python.

## Deployment technology

Docker Compose + Containers

## Running Command

docker-compose build && docker-compose up

## Application Detail

This is the reversi game, user can click the start button to start a game

Click the cells on board to play the game.

Users can end the game by clicking the end game button at anytime

The application allow multiple users play at the same time.

