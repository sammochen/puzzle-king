#pragma once
#include "game.h"
#include "piece.h"

// Heuristic - how to valuate one position
struct Eval {
    // Return 1 when white is winning and -1 when black is winning
    double evaluate(const Game &game) {
        const auto status = game.getStatus();

        if (status == GameStatus::WhiteWin) {
            return 1.0;
        }
        if (status == GameStatus::BlackWin) {
            return -1.0;
        }

        const double naive_sum = calc_naive_sum(game);
        const double result = sigmoid(naive_sum) * 2 - 1;
        assert(result >= -1 && result <= 1);
        return result;
    }

    double calc_naive_sum(const Game &game) const {
        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = game.board.pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                sum += (piece->color == Color::White ? 1 : -1) *
                       calc_naive_value(piece->piece);
            }
        }
        return sum;
    }

    static double flip(double value) { return -value; }

    double sigmoid(double x) const {
        const double res = 1.0 / (1.0 + std::exp(-x / 4));
        // if (res > 0.9)
        //     return 0.9;
        // if (res < 0.1)
        //     return 0.1;

        assert(res >= 0 && res <= 1);
        return res;
    }

    static std::vector<double> normalise(const std::vector<double> &P) {
        int n = P.size();
        // I want to normalise a vector such that the sum is 1
        // and it represents probability of wanting to try it
        // I think I should "centralise" the numbers and then "shrink"
        double lo = 1000, hi = -1000;
        for (const auto &p : P) {
            if (p < lo) {
                lo = p;
            }
            if (p > hi) {
                hi = p;
            }
        }
        if (hi == lo) {
            return std::vector<double>(n, 1.0 / n);
        }

        std::vector<double> result(n);

        double sumOfResult = 0;
        for (int i = 0; i < n; i++) {
            result[i] = (P[i] - lo) / (hi - lo);
            sumOfResult += result[i];
        }

        // and then divide by sumOfResult
        if (sumOfResult == 0) {
            return std::vector<double>(n, 1.0 / n);
        }
        if (sumOfResult < 0) {
            throw std::runtime_error("not allowed to be <0");
        }

        for (int i = 0; i < n; i++) {
            result[i] /= sumOfResult;
            if (result[i] < 0) {
                throw std::runtime_error("not allowed to be <0");
            }
        }

        return result;
    }

    double calc_naive_value(Piece piece) const {
        switch (piece) {
        case Piece::King:
            return 0;
        case Piece::Queen:
            return 9;
        case Piece::Rook:
            return 5;
        case Piece::Bishop:
            return 3;
        case Piece::Knight:
            return 3;
        case Piece::Pawn:
            return 1;
        }
        assert(false);
    }
};
