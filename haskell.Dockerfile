FROM haskell:8

WORKDIR /app
COPY . .

CMD runhaskell hello-world/hello.hs
