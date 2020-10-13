FROM debian:10

RUN apt-get update \
  && apt-get install -y ocaml-nox \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*


WORKDIR /app
COPY . .

RUN ocamlc -o ./hello hello-world/hello.ml
CMD ./hello
