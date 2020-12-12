#ifndef MCTS_H
#define MCTS_H

#include "Reversi.h"
#include <thread>

//vector<int> getSortedVectorIndex(const vector<int>& v);

class MCTS {
public:
    static Reversi* playToEnd(const Reversi& reversi, Reversi::Square color) {
        Reversi* tmpBoard = new Reversi(reversi);
        bool noMove = false;
        while (true) {
            auto possibleMove = tmpBoard->getPossibleMove(color);
            if (possibleMove.empty()) {
                if (noMove) { // no move in opposite
                    return tmpBoard;
                } else {
                    noMove = true;
                    color = Reversi::getOppositeColor(color);
                    continue;
                }
            }
            int r = rand() % possibleMove.size();
            tmpBoard->makeMove(possibleMove[r], color);
            color = Reversi::getOppositeColor(color);
            noMove = false;
        }
    }

    static Reversi::Coordinate treeSearch(const Reversi& reversi, const Reversi::Square& color) {
        auto possibleMove = reversi.getPossibleMove(color);

        vector<thread> threads;
        vector<heuristic> scores;
        for (auto&& move : possibleMove) {
            thread t(calculateScore, ref(reversi), ref(color), ref(scores), ref(move));
            threads.push_back(std::move(t));
        }
        for (auto&& t : threads) {
            t.join();
        }

        auto maxIt = max_element(scores.begin(),scores.end());
        return maxIt->getCoordinate();
    }

    class heuristic {
    public:
        heuristic(const Reversi::Square& color, const Reversi::Coordinate& coordinate)
                : color(color), score(0), coordinate(coordinate) {}

        void calculateScore(const Reversi& reversi) {
            winning(reversi);
            corner(reversi);
        }

        double getScore() const {
            return score;
        }

        const Reversi::Coordinate& getCoordinate() const {
            return coordinate;
        }

        // for compare score
        bool operator==(const heuristic& rhs) const {
            return getScore() == rhs.getScore();
        }

        bool operator!=(const heuristic& rhs) const {
            return !(rhs == *this);
        }

        bool operator<(const heuristic& rhs) const {
            return getScore() < rhs.getScore();
        }

        bool operator>(const heuristic& rhs) const {
            return rhs < *this;
        }

        bool operator<=(const heuristic& rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const heuristic& rhs) const {
            return !(*this < rhs);
        }

    private:
        void winning(const Reversi& reversi) {
            if (reversi.getWinner() == color) {
                score += 10;
            } else if (reversi.getWinner() != Reversi::blank){
                score -= 10;
            }
        }

        void corner(const Reversi& reversi) {
            auto cor = reversi.getCorners();
            for (int i = 0; i < 4; ++i) {
                if (cor[i] == color) {
                    score += 2;
                }
            }
            delete cor;
        }

        double score;
        const Reversi::Square& color;
        const Reversi::Coordinate& coordinate;
    };

private:
    static void calculateScore(const Reversi& reversi, const Reversi::Square& color, vector<heuristic>& scores,
                               const Reversi::Coordinate& move) {
        heuristic h(color, move);
        Reversi tmpBoard = reversi;
        tmpBoard.makeMove(move, color);
        Reversi* tmpRes;
        for (int i = 0; i < 2000; ++i) {
            tmpRes = playToEnd(tmpBoard, color);
            h.calculateScore(*tmpRes);
            delete tmpRes;
        }
        scores.push_back(h);
    }
};


#endif //MCTS_H
