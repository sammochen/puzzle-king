#pragma once

#include "cmath"
#include "game.h"
// An agent gets a board and decides what to do. I want to do a monte carlo tree
// search What a monte carlo tree search

struct MonteCarloNode {
    Game game;
    std::vector<Move> moves;
    std::vector<MonteCarloNode *> children;

    // number of children
    int numMoves;
    bool expanded = false;
    bool isTerminal = false;
    double initialV = 0;

    // probability of each move - the higher the more favourable the move
    std::vector<double> P;
    // number of visits
    std::vector<double> N;
    // sum of the scores from the n visits
    std::vector<double> sumV;

    MonteCarloNode(const Game &game) {

        // When you initialise a node, you "predict" each move first

        this->game = game;
        moves = game.possibleMoves();
        numMoves = moves.size();
        std::cout << "making new node, numMoves: " << numMoves << std::endl;
    }

    double getConfidence(const int moveIndex) const {
        int totalN = std::accumulate(N.begin(), N.end(), 0);
        int n = N[moveIndex];

        double p = P[moveIndex];
        double q = sumV[moveIndex] == 0 ? 0 : sumV[moveIndex] / n;

        double result = q + 1.41 * p * (std::sqrt(totalN)) / (1 + n);

        assert(isfinite(result));
        return result;
    }

    double expandNode() {
        assert(!expanded);
        expanded = true;

        // terminal conditions
        if (game.getStatus() == Status::WHITE_WIN) {
            isTerminal = true;
            if (game.turn() == Color::White) {
                initialV = 1000;
            } else {
                initialV = -1000;
            }
            return initialV;

        } else if (game.getStatus() == Status::BLACK_WIN) {
            isTerminal = true;
            if (game.turn() == Color::White) {
                initialV = -1000;
            } else {
                initialV = 1000;
            }

            return initialV;
        }

        

        initialV = board.heuristic();

        // when you expand, you use a heuristic
        P.assign(numMoves, 0);
        N.assign(numMoves, 0);
        sumV.assign(numMoves, 0);
        children.assign(numMoves, nullptr);

        for (int i = 0; i < numMoves; i++) {
            Board nextBoard = board.makeMove(moves[i]);
            P[i] = -nextBoard.heuristic(); // heuristic
        }

        double maxP = *max_element(P.begin(), P.end());
        double minP = *min_element(P.begin(), P.end());
        double pRange = maxP - minP;

        if (pRange == 0) {
            P.assign(P.size(), 0);
        } else {
            for (double &x : P) {
                x = (x - minP) / pRange;
            }
        }

        return initialV;
    }

    // Return the valuation of the board. For now, we'll use point count, and
    // win/loss is +- 1000 points
    double searchOnce() {
        if (!expanded) {
            return expandNode();
        }

        if (isTerminal) {
            return initialV;
        }

        //  choose the move with the highest confidence and explore it
        double bestConfidence = -1e9;
        int bestIndex = -1;
        for (int i = 0; i < numMoves; i++) {
            const auto curConfidence = getConfidence(i);
            if (curConfidence >= bestConfidence) {
                bestConfidence = curConfidence;
                bestIndex = i;
            }
        }
        std::cout << bestConfidence << ' ' << bestIndex << std::endl;

        // if game finishes

        if (children[bestIndex] == nullptr) {
            children[bestIndex] =
                new MonteCarloNode(game.makeMove(moves[bestIndex]));

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
    Move chooseBestMove(const Game &game, int numSearches) const {
        auto *root = new MonteCarloNode(game);
        for (int i = 0; i < numSearches; i++) {
            std::cout << "searching " << i << std::endl;
            root->searchOnce();
        }

        return root->bestMove();
    }
};
