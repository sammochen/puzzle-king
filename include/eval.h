#pragma once
#include "board.h"
#include "piece.h"

// Heuristic - how to valuate one position
struct Eval {
    // Return + when white is winning
    double evaluate(const Board &board) {

        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = board.pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                double multiplier = piece->color == Color::White ? 1 : -1;
                sum += multiplier * value(piece->piece);
            }
        }
        return sum;
    }

    double value(Piece piece) {
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
        return 0;
    }
};
