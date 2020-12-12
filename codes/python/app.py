import json
import threading

import rpc
import time
import reversi
from flask import Flask, render_template, request, abort

app = Flask(__name__)
time.sleep(10)
r = rpc.Rpc()
games_turn = dict()
games_status = dict()


@app.route('/')
def hello_world():
    return render_template("index.html")


@app.route('/api/games', methods=['POST'])
def new_game():
    data = request.data.decode("utf-8")
    game_id = r.call("N " + data)
    games_status[game_id] = 0
    return game_id, 201


@app.route('/api/games/<game_id>')
def get_game(game_id):
    if games_status[game_id] == 1:
        return "", 204

    res = r.call("G " + game_id)
    if res == "Invalid":
        abort(404)
    game = reversi.Reversi(json.loads(res))
    game.generate_display_board()

    games_turn[game_id] = game.turn
    if games_turn[game_id] == 'computer':
        threading.Thread(target=computer_move, args=(game_id,)).start()

    return json.dumps(game.to_dict())


@app.route('/api/games/<game_id>/move', methods=['POST'])
def move(game_id):
    if games_turn[game_id] != 'user':
        abort(403, description="Not your turn")

    data = request.json
    args = "U " + game_id + " " + str(data['x']) + " " + str(data['y'])
    res = r.call(args)

    if res == 'Invalid':
        abort(400, description="Move Invalid")

    game = reversi.Reversi(json.loads(res))
    game.generate_display_board()
    games_turn[game_id] = game.turn

    threading.Thread(target=computer_move, args=(game_id,)).start()

    return json.dumps(game.to_dict())


@app.route('/api/games/<game_id>', methods=['DELETE'])
def delete_game(game_id):
    r.call("R " + game_id)
    games_status.pop(game_id)
    games_turn.pop(game_id)
    return '', 204


def computer_move(game_id):
    if games_turn[game_id] == 'user':
        return

    res = r.call("C " + game_id)
    if res == 'Finish':
        games_status[game_id] = 0


if __name__ == '__main__':
    app.run(host='0.0.0.0')
