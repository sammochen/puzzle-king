#pragma once

#include "piece.h"
#include "terminal_color.h"

#include <iostream>
#include <numeric>
#include <optional>
#include <stdlib.h>
#include <utility>
#include <vector>

struct Square {
    int row, col;
};

std::ostream &operator<<(std::ostream &os, const Square &square) {
    return os << "Square { row: " << square.row << ", col: " << square.col
              << " }";
}

// a move for a board is well defined by the to and from
struct Move {
    Square from, to;
};

std::ostream &operator<<(std::ostream &os, const Move &move) {
    return os << "Move { to: " << move.to << ", from: " << move.from << " }";
}

enum Status { White_WIN = 0, Black_WIN = 1, IN_PROGRESS = 2 };

// Captures the board state
struct Board {
    // a1 = 00, h8 = 77
    std::vector<std::vector<std::optional<ColoredPiece>>> pieces;

    Board(const std::string &s) {
        assert(s.size() == 64);
        pieces.assign(
            8, std::vector<std::optional<ColoredPiece>>(8, std::nullopt));

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int row = 7 - i;
                int col = j;
                pieces[row][col] = ColoredPiece::fromChar(s[i * 8 + j]);
            }
        }
    }

    Board()
        : Board(std::string("rnbqkbnr"
                            "pppppppp"
                            "--------"
                            "--------"
                            "--------"
                            "--------"
                            "PPPPPPPP"
                            "RNBQKBNR")) {}

    // move should be legal
    Board makeMove(const Move &move) const {
        // note - doesnt handle en passent and castling and promotion
        Board copy = *this;
        copy.pieces[move.to.row][move.to.col] =
            copy.pieces[move.from.row][move.from.col];
        copy.pieces[move.from.row][move.from.col] = std::nullopt;
        return copy;
    }

    void printChar(int row, int col) const {
        const auto &coloredPiece = pieces[row][col];
        if (coloredPiece) {
            std::cout << (coloredPiece->color == Color::White
                              ? TerminalColor::Modifier(TerminalColor::FG_WHITE)
                              : TerminalColor::Modifier(TerminalColor::FG_BLUE))
                      << coloredPiece->piece.getChar();
        } else {
            std::cout << TerminalColor::Modifier(TerminalColor::FG_RED) << '-';
        }

        std::cout << TerminalColor::Modifier(TerminalColor::FG_DEFAULT)
                  << TerminalColor::Modifier(TerminalColor::BG_DEFAULT);
    }

    void printChar(const std::string &pos) const {
        assert(pos.size() == 2);
        const int row = pos[1] - '1';
        const int col = pos[0] - 'a';
        return printChar(row, col);
    }

    void print() {
        for (char i = '8'; i >= '1'; i--) {
            for (char j = 'a'; j <= 'h'; j++) {
                printChar(std::string(1, j) + i);
            }
            std::cout << std::endl;
        }
    }
};
