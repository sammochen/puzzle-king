#pragma once
#include "game.h"
#include "piece.h"

// Heuristic - how to valuate one position
struct Eval {
    // Return 1 when white is winning and 0 when black is winning
    double evaluate(const Game &game) {
        const auto status = game.getStatus();

        if (status == GameStatus::WhiteWin) {
            return 1.0;
        }
        if (status == GameStatus::BlackWin) {
            return -1.0;
        }

        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = game.board.pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                double multiplier = piece->color == Color::White ? 1 : -1;
                sum += multiplier * value(piece->piece);
            }
        }

        // sigmoid normalise the score
        double result = sigmoid(sum);
        assert(result >= 0 && result <= 1);
        return result;
    }

    double sigmoid(double x) const { return 1.0 / (1.0 + std::exp(-x / 4)); }

    double value(Piece piece) const {
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
