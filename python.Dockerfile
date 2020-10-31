FROM python:3.8

WORKDIR /app
COPY . .

CMD python3 codes/hello.py
