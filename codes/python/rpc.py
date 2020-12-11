import uuid
import pika


class Rpc:
    def __init__(self):
        self.connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
        self.channel = self.connection.channel()
        self.channel.queue_declare(queue='reversi_call')

        result = self.channel.queue_declare(queue='reversi_result', exclusive=True)
        self.callback_queue = result.method.queue
        self.channel.basic_consume(
            queue=self.callback_queue,
            on_message_callback=self.on_response,
            auto_ack=True)

        self.return_value = dict()

    def call(self, body):
        corr_id = str(uuid.uuid4())
        self.return_value[corr_id] = None
        self.channel.basic_publish(exchange='', routing_key='reversi_call',
                                   properties=pika.BasicProperties(
                                       reply_to=self.callback_queue,
                                       correlation_id=corr_id,
                                   ), body=body)

        while self.return_value[corr_id] is None:
            self.connection.process_data_events()
        res = self.return_value[corr_id]
        self.return_value.pop(corr_id)
        return res

    def on_response(self, ch, method, props, body):
        if self.return_value[props.correlation_id] is None:
            self.return_value[props.correlation_id] = body
