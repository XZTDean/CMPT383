#ifndef CPP_REVERSIMANAGER_H
#define CPP_REVERSIMANAGER_H

#include "Reversi.h"
#include "MCTS.h"
#include <unordered_map>
using namespace std;

class ReversiManager {
public:
    string newGame(const string& str) {
        long id = next;
        Reversi::Square user = (str == "white") ? Reversi::white : Reversi::black;
        map[id] = new Reversi(user);
        next++;
        return to_string(id);
    }

    string getGame(const string& str) {
        long id = stol(str);
        Reversi *reversi = map[id];
        return reversi->toJson();
    }

    string userMove(const string& str) {
        int first_space = str.find(' ');
        int sec_space = str.find(' ', first_space + 1);
        long id = stol(str.substr(0, first_space));
        int x = stoi(str.substr(first_space + 1, sec_space));
        int y = stoi(str.substr(sec_space + 1));
        Reversi::Coordinate coordinate(x, y);
        Reversi *reversi = map[id];
        if (reversi->makeMove(coordinate, reversi->getUser())) {
            return reversi->toJson();
        } else {
            return "Invalid";
        }
    }

    string computerMove(const string& str) {
        long id = stol(str);
        Reversi *reversi = map[id];
        Reversi::Square color = Reversi::getOppositeColor(reversi->getUser());
        Reversi::Coordinate coor = MCTS::treeSearch(*reversi, color);
        reversi->makeMove(coor, color);
        return "Finish";
    }

    void removeGame(const string& str) {
        long id = stol(str);
        delete map[id];
        map.erase(id);
    }
private:
    long next = 0;
    unordered_map<long, Reversi*> map;
};


#endif //CPP_REVERSIMANAGER_H
