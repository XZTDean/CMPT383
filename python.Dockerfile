FROM python:3.8

WORKDIR /app
COPY . .

RUN pip install flask \
    && pip install pika

CMD python3 codes/hello.py
