#pragma once
#include "board.h"

// Heuristic - how to valuate one position
struct Eval {
    // Return + when white is winning
    double evaluate(const Board &board) {

        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                double multiplier = piece->color == Color::White ? 1 : -1;
                sum += multiplier; // TODO piece value
            }
        }
        return sum;
    }
};
