class Reversi:
    def __init__(self, dictionary: dict):
        self.board = dictionary['board']
        self.turn = dictionary['turn']
        self.win = dictionary.get('win')

    def generate_display_board(self):
        horizontal_border = [0, 11, 12, 13, 14, 15, 16, 17, 18, 0]
        disp = [horizontal_border]
        for i in range(8):
            row = self.board[i]
            disp_row = [8 - i]
            for j in range(8):
                ele = row[j]
                if (i + j) % 2 == 0:
                    ele += 20
                else:
                    ele += 30
                disp_row.append(ele)
            disp_row.append(8 - i)
            disp.append(disp_row)

        disp.append(horizontal_border)
        self.board = disp

    def to_dict(self):
        d = {"board": self.board, "turn": self.turn}
        if self.win is not None:
            d["win"] = self.win
        return d
