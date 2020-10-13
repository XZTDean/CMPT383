FROM erlang:23

WORKDIR /app

COPY . .

RUN erlc hello-world/hello.erl
CMD erl -noshell -eval "hello:hello()" -eval 'init:stop()'
