FROM node:14-buster

#RUN npm install --build-from-source zeromq@6.0.0-beta.5 

WORKDIR /app
COPY demos .

#CMD node mycode.js
