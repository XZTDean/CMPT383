#ifndef MCTS_H
#define MCTS_H

#include "Reversi.h"

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

    static Reversi::Coordinate treeSearch(const Reversi& reversi, const Reversi::Square& color, bool modify) {
        auto possibleMove = reversi.getPossibleMove(color);
//        Reversi::Square opponent = Reversi::getOppositeColor(color);
//        int size = possibleMove.size();
//        vector<int> mobility;
//        vector<int> opponentMobility;

        vector<heuristic> scores;
        for (auto&& move : possibleMove) {
            heuristic h(color, move, modify);
            Reversi tmpBoard = reversi;
            tmpBoard.makeMove(move, color);
//            int option = tmpBoard.getPossibleMove(color).size();
//            int opponentOption = tmpBoard.getPossibleMove(opponent).size();
//            mobility.push_back(option);
//            opponentMobility.push_back(opponentOption);
            Reversi* tmpRes;
            for (int i = 0; i < 2000; ++i) {
                tmpRes = playToEnd(tmpBoard, color);
                h.calculateScore(*tmpRes);
                delete tmpRes;
            }
            scores.push_back(h);
        }
//        vector<int> rankMobility = getSortedVectorIndex(mobility); // small rank better
//        vector<int> rankOpponentMobility = getSortedVectorIndex(opponentMobility); // large rank better
//        vector<int> rankOverall;
//        for (int i = 0; i < size; ++i) {
//            int overall = rankMobility[i] - rankOpponentMobility[i];
//            rankOverall.push_back(overall);
//        }
//        vector<int> rank = getSortedVectorIndex(rankOverall); // large rank better
//        double reward = 0.1;
//        for (int i = 0; i < size; ++i) {
//            scores[i].setWeight(0.95 + reward * i / (size - 1));
//        }

        auto maxIt = max_element(scores.begin(),scores.end());
        return maxIt->getCoordinate();
    }

private:
    class heuristic {
    public:
        heuristic(const Reversi::Square& color, const Reversi::Coordinate& coordinate, bool modified)
                : color(color), score(0), coordinate(coordinate), modified(modified) {}

        void calculateScore(const Reversi& reversi) {
            winning(reversi);
            if (modified) {
                corner(reversi);
            }
        }

        double getScore() const {
//            if (modified) {
//                return score * weight;
//            }
            return score;
        }

        const Reversi::Coordinate& getCoordinate() const {
            return coordinate;
        }

//        void setWeight(double weight) {
//            heuristic::weight = weight;
//        }

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

//        void coordinateCheck() {
//            if (coordinate.x == 0 || coordinate.x == Reversi::edgeSize - 1) {
//                if (coordinate.y == 0 || coordinate.y == Reversi::edgeSize - 1) {
//                    weight *= 1.1;
//                } else if (coordinate.y == 1 || coordinate.y == Reversi::edgeSize - 2) {
//                    weight *= 0.9;
//                } else if (coordinate.y == 2 || coordinate.y == Reversi::edgeSize - 3) {
//                    weight *= 1.05;
//                }
//            } else if (coordinate.x == 1 || coordinate.x == Reversi::edgeSize - 2) {
//                if (coordinate.y == 0 || coordinate.y == Reversi::edgeSize - 1) {
//                    weight *= 0.9;
//                } else if (coordinate.y == 1 || coordinate.y == Reversi::edgeSize - 2) {
//                    weight *= 0.95;
//                }
//            } else if (coordinate.x == 2 || coordinate.x == Reversi::edgeSize - 3) {
//                if (coordinate.y == 0 || coordinate.y == Reversi::edgeSize - 1) {
//                    weight *= 1.05;
//                }
//            }
//        }

        double score;
//        double weight;
        bool modified;
        const Reversi::Square& color;
        const Reversi::Coordinate& coordinate;
    };
};


//vector<int> getSortedVectorIndex(const vector<int>& v) {
//    int size = v.size();
//    vector<int> index;
//    vector<int> rank(size, -1);
//    for (int i = 0; i < size; ++i) {
//        int largestIndex = -1;
//        for (int j = 0; j < size; ++j) {
//            if (find(index.begin(), index.end(), j) == index.end()) {
//                if (largestIndex == -1) {
//                    largestIndex = j;
//                } else if (v[j] > v[largestIndex]) {
//                    largestIndex = j;
//                }
//            }
//        }
//        rank[largestIndex] = i;
//        index.push_back(largestIndex);
//    }
//    return rank;
//}

#endif //MCTS_H
