#ifndef REVERSI_H
#define REVERSI_H

#include <cstdint>
#include <ostream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

/**
 * Reversi Class stores the board of the game.
 * It also contain a enum for element in each grid.
 * It contain Coordinate class.
 */
class Reversi {
public:
    /// @enum element in every grid
    enum Square : uint_least8_t {blank, white, black};

    /**
     * Coordinate class shows the coordinate in the board
     * @var x - x-axis
     * @var y - y-axis
     */
    class Coordinate {
    public:
        enum Direction : uint_least8_t {U, D, L, R, UL, UR, DL, DR};

        Coordinate(int x, int y) : x(x), y(y) {}

        vector<Direction> getDirection() const {
            vector<Direction> dir{U, D, L, R, UL, UR, DL, DR}; // up down left right
            if (x <= 0) {
                eraseDirection(dir, L);
                eraseDirection(dir, UL);
                eraseDirection(dir, DL);
            }
            if (x >= edgeSize - 1) {
                eraseDirection(dir, R);
                eraseDirection(dir, UR);
                eraseDirection(dir, DR);
            }
            if (y <= 0) {
                eraseDirection(dir, U);
                eraseDirection(dir, UL);
                eraseDirection(dir, UR);
            }
            if (y >= edgeSize - 1) {
                eraseDirection(dir, D);
                eraseDirection(dir, DL);
                eraseDirection(dir, DR);
            }
            return dir;
        }

        Coordinate getNeighbour(Direction dir) const {
            switch (dir) {
                case U:
                    return {x, y - 1};
                case D:
                    return {x, y + 1};
                case L:
                    return {x - 1, y};
                case R:
                    return {x + 1, y};
                case UL:
                    return {x - 1, y - 1};
                case UR:
                    return {x + 1, y - 1};
                case DL:
                    return {x - 1, y + 1};
                case DR:
                    return {x + 1, y + 1};
            }
        }

        friend ostream& operator<<(ostream& os, const Coordinate& coordinate) {
            char tmp = coordinate.x + 'A';
            os << '(' << tmp << ' ' << coordinate.y + 1 << ')';
            return os;
        }

        bool operator==(const Coordinate& rhs) const {
            return x == rhs.x &&
                   y == rhs.y;
        }

        bool operator!=(const Coordinate& rhs) const {
            return !(rhs == *this);
        }

        int x, y;

    private:
        static void eraseDirection(vector<Direction>& dirList, Direction direction) {
            vector<Direction>::iterator p = find(dirList.begin(), dirList.end(), direction);
            if (p != dirList.end())
                dirList.erase(p);
        }
    };

    Reversi(Square user) : user(user) {
        Square* s = new Square[edgeSize * edgeSize];
        for (int i = 0; i < edgeSize * edgeSize; ++i) {
            s[i] = blank;
        }
        board = new Square*[edgeSize];
        for (int i = 0; i < edgeSize; ++i) {
            board[i] = s + edgeSize * i;
        }
        //center starting place
        int center = edgeSize / 2;
        board[center - 1][center - 1] = board[center][center] = white;
        board[center][center - 1] = board[center - 1][center] = black;
        next = black;
    }

    Reversi(const Reversi& reversi) {
        Square* s = new Square[edgeSize * edgeSize];
        this->board = new Square*[edgeSize];
        for (int i = 0; i < edgeSize; ++i) {
            this->board[i] = s + edgeSize * i;
        }
        for (int i = 0; i < edgeSize; ++i) {
            for (int j = 0; j < edgeSize; ++j) {
                this->board[i][j] = reversi.board[i][j];
            }
        }
        this->next = reversi.next;
        this->user = reversi.user;
    }

    virtual ~Reversi() {
        delete[] board[0];
        delete[] board;
    }

    bool makeMove(const Coordinate& coor, const Square& color) {
        if (coor.x >= edgeSize || coor.y >= edgeSize || coor.x < 0 || coor.y < 0) {
            return false;
        } else if (!isPossibleMove(coor, color)) {
            return false;
        }
        vector<Coordinate::Direction> directions = coor.getDirection();
        for (auto&& dir : directions) {
            if (isFlipableDirection(coor, color, dir)) {
                flipDirection(coor, color, dir);
            }
        }
        setColor(coor, color);

        if (color == next) {
            getNext();
        }
        return true;
    }

    Square getColor(Coordinate coor) const {
        if (coor.x >= edgeSize || coor.y >= edgeSize || coor.x < 0 || coor.y < 0) {
            return blank;
        }
        return board[coor.y][coor.x];
    }

    vector<Coordinate> getPossibleMove(Square color) const {
        vector<Coordinate> moves;
        if (color == blank) {
            return moves;
        }
        for (int i = 0; i < edgeSize; ++i) { //y
            for (int j = 0; j < edgeSize; ++j) { //x
                Coordinate coordinate(j, i);
                if (isPossibleMove(coordinate, color)) {
                    moves.push_back(coordinate);
                }
            }
        }
        return moves;
    }

    bool isEnd() const {
        int possibleMove = getPossibleMove(white).size() + getPossibleMove(black).size();
        return !possibleMove;
    }

    Square getWinner() const {
        if (!isEnd()) {
            return blank;
        }
        int wNum = 0;
        int bNum = 0;
        for (int i = 0; i < edgeSize; ++i) { //y
            for (int j = 0; j < edgeSize; ++j) { //x
                switch (board[i][j]) {
                    case black: ++bNum; break;
                    case white: ++wNum;
                }
            }
        }
        if (wNum > bNum) {
            return white;
        } else if (bNum > wNum) {
            return black;
        } else {
            return blank;
        }
    }

    Square* getCorners() const {
        Square* corner = new Square[4];
        corner[0] = board[0][0];
        corner[1] = board[0][edgeSize - 1];
        corner[2] = board[edgeSize - 1][0];
        corner[3] = board[edgeSize - 1][edgeSize - 1];
        return corner;
    }

    static Square getOppositeColor(const Square& color) {
        switch (color) {
            case white:
                return black;
            case black:
                return white;
            default:
                return blank;
        }
    }

    Reversi& operator=(const Reversi& rhs){
        if (this == &rhs) {
            return *this;
        }
        for (int i = 0; i < edgeSize; ++i) {
            for (int j = 0; j < edgeSize; ++j) {
                this->board[i][j] = rhs.board[i][j];
            }
        }
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Reversi& reversi) {
        // top label
        os << ' ';
        for (int i = 0; i < Reversi::edgeSize; ++i) {
            char tmp = i + 'A';
            os << "   " << tmp;
        }
        os << endl;
        // top-edge
        os << "  ┏";
        for (int i = 0; i < Reversi::edgeSize; ++i) {
            os << "━━━";
            if (i < Reversi::edgeSize - 1) {
                os << "┯";
            } else {
                os << "┓\n";
            }
        }
        // board line
        for (int i = 0; i < Reversi::edgeSize; ++i) {
            os << i + 1 << " ┃";
            for (int j = 0; j < Reversi::edgeSize; ++j) {
                os << ' ' << reversi.board[i][j] << ' ';
                if (j < Reversi::edgeSize - 1) {
                    os << "│";
                } else {
                    os << "┃ " << i + 1 << endl;
                }
            }
            if (i < Reversi::edgeSize - 1) { // not bottom separate line
                os << "  ┠";
                for (int j = 0; j < Reversi::edgeSize; ++j) {
                    os << "───";
                    if (j < Reversi::edgeSize - 1) {
                        os << "┼";
                    } else {
                        os << "┨\n";
                    }
                }
            } else { // bottom-edge
                os << "  ┗";
                for (int j = 0; j < Reversi::edgeSize; ++j) {
                    os << "━━━";
                    if (j < Reversi::edgeSize - 1) {
                        os << "┷";
                    } else {
                        os << "┛\n";
                    }
                }
            }
        }
        // bottom label
        os << ' ';
        for (int i = 0; i < Reversi::edgeSize; ++i) {
            char tmp = i + 'A';
            os << "   " << tmp;
        }
        os << "\n\n";
        return os;
    }

    friend ostream& operator<<(ostream& os, Square square) {
        switch (square) {
            case white:
                os << '1';
                break;
            case black:
                os << '2';
                break;
            case blank:
                os << '0';
        }
        return os;
    }

    string toJson() {
        stringstream stream;
        stream << "{\"board\":[";
        for (int i = 0; i < edgeSize; ++i) {
            stream << "[";
            for (int j = 0; j < edgeSize; ++j) {
                stream << board[i][j];
                if (j < edgeSize - 1) {
                    stream << ',';
                } else {
                    stream << ']';
                }
            }
            if (i < edgeSize - 1) {
                stream << ',';
            } else {
                stream << "],";
            }
        }

        stream << "\"turn\": \"";
        if (isEnd()) {
            stream << "end\", \"win\":\"";
            if (getWinner() == user) {
                stream << "user\"}";
            } else {
                stream << "computer\"}";
            }
        } else {
            if (next == user) {
                stream << "user\"}";
            } else {
                stream << "computer\"}";
            }
        }
        return stream.str();
    }

    Square getUser() const {
        return user;
    }

    string debugString() const {
        string str;
        for (int i = 0; i < edgeSize; ++i) {
            for (int j = 0; j < edgeSize; ++j) {
                switch (board[i][j]) {
                    case blank: str.append("0"); break;
                    case white: str.append("1"); break;
                    case black: str.append("2");
                }
            }
        }
        return str;
    }

    static const int edgeSize = 8;

private:
    bool isPossibleMove(const Coordinate& coordinate, const Square& color) const {
        if (getColor(coordinate) == blank) {
            vector<Coordinate::Direction> directions = coordinate.getDirection();
            for (auto&& dir : directions) {
                if (isFlipableDirection(coordinate, color, dir)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isFlipableDirection(const Coordinate& coordinate, const Square& color, const Coordinate::Direction dir) const {
        Coordinate neighbour = coordinate.getNeighbour(dir);
        if (getColor(neighbour) != blank && getColor(neighbour) != color) { // neighbour is opposite color
            while (getColor(neighbour) != blank && getColor(neighbour) != color) {
                neighbour = neighbour.getNeighbour(dir); // keep find neighbour until is not opponent
            }
            if (getColor(neighbour) == color) { // end of a line
                return true;
            }
        }
        return false;
    }

    void flipDirection(const Coordinate& coordinate, const Square& color, const Coordinate::Direction dir) {
        Coordinate neighbour = coordinate.getNeighbour(dir);
        while (getColor(neighbour) != color) {
            setColor(neighbour, color);
            neighbour = neighbour.getNeighbour(dir);
        }
    }

    void setColor(const Coordinate& coor, const Square& color) {
        board[coor.y][coor.x] = color;
    }

    void getNext() {
        Square opposite = getOppositeColor(next);
        if (getPossibleMove(opposite).empty()) {
            if (getPossibleMove(next).empty()) {
                next = blank;
            }
        } else {
            next = opposite;
        }
    }

    Square** board;
    Square next;
    Square user;
};

#endif //REVERSI_H
