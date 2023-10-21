#pragma once

#include "board.h"
// An agent gets a board and decides what to do. I want to do a monte carlo tree
// search What a monte carlo tree search

struct MonteCarloNode {

    Board board;
    std::vector<Move> moves;
    std::vector<MonteCarloNode *> children;

    bool isTerminal = false;
    double initialV = 0;
    std::vector<double> P;    // probability of each move
    std::vector<double> N;    // number of visits
    std::vector<double> sumV; // number of visits

    MonteCarloNode(const Board &board) {

        // When you initialise a node, you "predict" each move first

        this->board = board;
        moves = board.possibleMoves();

        if (board.getStatus() == Status::WHITE_WIN) {
            isTerminal = true;
            if (board.turn == Color::WHITE) {
                initialV = 1000;
            } else {
                initialV = -1000;
            }
            return;

        } else if (board.getStatus() == Status::BLACK_WIN) {
            isTerminal = true;
            if (board.turn == Color::WHITE) {
                initialV = -1000;
            } else {
                initialV = 1000;
            }

            return;
        }

        // you predict a pi (value of each move) as well as current v
        P.assign(moves.size(), 0);
        N.assign(moves.size(), 0);
        children.assign(moves.size(), 0);

        for (int i = 0; i < moves.size(); i++) {
            const auto &move = moves[i];

            Board nextBoard = board.makeMove(move);

            // populate pi
        }
    }

    double getConfidence(const int moveIndex) const {}

    // Return the valuation of the board. For now, we'll use point count, and
    // win/loss is +- 1000 points
    double searchOnce() {
        if (isTerminal) {
            return initialV;
        }

        //  choose the move with the highest confidence and explore it
        double bestConfidence = -1e9;
        int bestIndex = -1;
        for (int i = 0; i < P.size(); i++) {
            const auto curConfidence = getConfidence(i);
            if (curConfidence >= bestConfidence) {
                bestConfidence = curConfidence;
                bestIndex = i;
            }
        }

        // if game finishes

        if (children[bestIndex] == nullptr) {
            children[bestIndex] =
                new MonteCarloNode(board.makeMove(moves[bestIndex]));

            return children[bestIndex]->initialV;
        } else {
            // Note flipped because change players
            double childResult = -children[bestIndex]->searchOnce();

            sumV[bestIndex] += childResult;
            N[bestIndex] += 1;

            return childResult;
        }
    }

    Move bestMove() const {
        int biggest = -1;
        std::vector<int> indices;
        for (int i = 0; i < N.size(); i++) {
            if (N[i] > biggest) {
                biggest = N[i];
                indices = {i};

            } else if (N[i] == biggest) {
                indices.push_back(i);
            }
        }

        std::cout << "There are " << indices.size() << " best moves"
                  << std::endl;
        return moves[indices[0]];
    }
};

struct Agent {
    // An agent makes a MCTS node, explores it, and returns the best move
    Move makeBestMove(const Board &board, int numSearches) {
        auto root = new MonteCarloNode(board);
        for (int i = 0; i < numSearches; i++) {
            root->searchOnce();
        }

        return root->bestMove();
    }
};
