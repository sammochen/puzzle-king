#pragma once

#include "cmath"
#include "eval.h"
#include "game.h"

struct ExpandedNodeStats {
    int numMoves;

    // the "prior"
    // the initial policy. should be normalised i think
    std::vector<double> P;
    // number of visits
    std::vector<double> N;
    // sum of the scores from the n visits
    std::vector<double> sumV;

    ExpandedNodeStats(std::vector<double> P) : numMoves(P.size()), P(P) {
        N.assign(numMoves, 0);
        sumV.assign(numMoves, 0);
    }

    void addObservation(const int moveIndex, double value) {
        sumV[moveIndex] += value;
        N[moveIndex]++;
    }

    int getMostVisitedMove() const {
        int move = std::max_element(N.begin(), N.end()) - N.begin();
        assert(move != N.size());
        return move;
    }

    // Return the move index with highest confidence
    int getMoveWithBestConfidence(bool isWhite) const {
        double bestConfidence = -1e9;
        int bestIndex = -1;

        for (int i = 0; i < numMoves; i++) {
            const auto curConfidence = getConfidence(i, isWhite);
            if (curConfidence > bestConfidence) {
                bestConfidence = curConfidence;
                bestIndex = i;
            }
        }
        assert(bestIndex != -1);
        return bestIndex;
    }

    double getAverageV(const int moveIndex) const {
        const int n = N[moveIndex];
        return n == 0 ? 0 : sumV[moveIndex] / n;
    }

    // Returns the "confidence" of choosing a move.
    // Exploration/exploitation - tries promising moves but will give others a
    // chance
    double getConfidence(const int moveIndex, const bool isWhite) const {
        const int totalN = std::accumulate(N.begin(), N.end(), 0);
        const int n = N[moveIndex];
        if (n == 0) {
            return 1000; // always prefer unexplored nodes
        }

        double p, averageV;
        if (isWhite) {
            p = P[moveIndex];
            averageV = sumV[moveIndex] / n;
        } else {
            // p is a probability, averageV is [-1, 1]
            p = 1 - P[moveIndex];
            averageV = -sumV[moveIndex] / n;
        }

        // the bigger the n, the less important p is
        // https://web.stanford.edu/~surag/posts/alphazero.html
        // TODO the sqrt(2) here is a hyperparameter
        const double result =
            averageV + 4.0 * p * (std::sqrt(totalN)) / (1 + n);

        assert(isfinite(result));
        return result;
    }
};

// One node is for one game state
struct Node {
    Game game;
    Eval eval;

    int depth;

    bool isTerminal = false;
    double initialV = 0;

    int numMoves;
    std::vector<Move> moves;
    std::vector<std::optional<Node>> children;

    std::optional<ExpandedNodeStats> expandedNodeStats = std::nullopt;

    Node(const Game &game, int depth = 0) : game(game), depth(depth) {
        // When you initialise a node, you don't unpack it yet
        moves = game.legalMoves();
        numMoves = moves.size();
        children.assign(numMoves, std::nullopt);

        initialV = eval.evaluate(game);
        isTerminal = game.getStatus() != GameStatus::InProgress;
    }

    double expandNode() {
        assert(!expandedNodeStats);

        std::vector<double> P(numMoves);

        if (isTerminal) {
            auto normalisedP = Eval::normalise(P);
            expandedNodeStats =
                ExpandedNodeStats(normalisedP); // I suspect this isnt necessary
            return initialV;
        }

        for (int i = 0; i < numMoves; i++) {
            Board nextBoard = game.board.makeMove(moves[i]);
            Game nextGame(nextBoard, game.turn.other());
            P[i] = eval.evaluate(nextGame); // this evaluation is -1 to 1
        }

        auto normalisedP = Eval::normalise(P);
        expandedNodeStats = ExpandedNodeStats(normalisedP);
        return initialV;
    }

    // Search once, and return the valuation of the board.
    double searchOnce() {
        if (!expandedNodeStats) {
            return expandNode();
        }

        if (isTerminal) {
            return initialV;
        }

        const int bestIndex = expandedNodeStats->getMoveWithBestConfidence(
            game.turn == Color::White);
        assert(bestIndex >= 0 && bestIndex < numMoves);

        if (!children[bestIndex]) {
            children[bestIndex] =
                Node(game.makeMove(moves[bestIndex]), depth + 1);
            return children[bestIndex]->initialV;
        }

        // No flip - node evaluates from white's perspective
        const double childResult = children[bestIndex]->searchOnce();
        expandedNodeStats->addObservation(bestIndex, childResult);
        return childResult;
    }

    Move bestMove() const {
        // the best move is the most traversed action
        return moves[expandedNodeStats->getMostVisitedMove()];
    }

    void print_agent_state() const {
        for (int i = 0; i < numMoves; i++) {
            std::cout << moves[i] << ' ' << expandedNodeStats->P[i] << ' '
                      << expandedNodeStats->getAverageV(i) << ' '
                      << expandedNodeStats->getConfidence(i, game.turn ==
                                                                 Color::White)
                      << ' ' << expandedNodeStats->N[i] << std::endl;

            // if (children[i]) {
            //     children[i]->game.board.print();
            // }
            // std::cout << std::endl;
            // std::cout << std::endl;
        }
    }
};

struct Agent {
    // An agent makes a MCTS node, explores it, and returns the best move
    Move chooseBestMove(const Game &game, int numSearches) const {
        auto root = Node(game, 0);
        for (int i = 0; i < numSearches; i++) {
            if (i % 1000 == 0)
                std::cout << i << ' ' << std::flush;
            root.searchOnce();
        }

        std::cout << std::endl;
        root.print_agent_state();
        return root.bestMove();
    }
};
