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

        // this sum ranges from -1000 to 1000 - normalise to 0-1
        sum = std::max(sum, -1000.0);
        sum = std::min(sum, 1000.0);
        return (sum + 1000) / 2000;
    }

    double value(Piece piece) const {
        switch (piece) {
        case Piece::King:
            return 1000;
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
