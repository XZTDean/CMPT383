FROM php:7

WORKDIR /app
COPY . .

CMD php hello-world/hello.php
