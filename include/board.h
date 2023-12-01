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
    bool operator==(const Square &rhs) const {
        return (row == rhs.row) && (col == rhs.col);
    }
    bool operator!=(const Square &rhs) const { return !operator==(rhs); }
};

std::ostream &operator<<(std::ostream &os, const Square &square) {
    return os << "(" << square.row << ", " << square.col << ")";
}

// a move for a board is well defined by the to and from
struct Move {
    Square from, to;
    bool operator==(const Move &rhs) const {
        return (from == rhs.from) && (to == rhs.to);
    }
    bool operator!=(const Move &rhs) const { return !operator==(rhs); }
};

std::ostream &operator<<(std::ostream &os, const Move &move) {
    return os << "(" << move.from << " -> " << move.to << ")";
}

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

    std::string toString() const {
        std::string result(64, '?');
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int row = 7 - i;
                int col = j;

                result[i * 8 + j] =
                    pieces[row][col] ? pieces[row][col]->getChar() : '-';
            }
        }
        return result;
    }

    Board makeMove(const Move &move) const {
        // note - doesnt handle en passent and castling and promotion
        Board copy = *this;
        copy.pieces[move.to.row][move.to.col] =
            copy.pieces[move.from.row][move.from.col];
        copy.pieces[move.from.row][move.from.col] = std::nullopt;
        return copy;
    }

    void printChar(int row, int col) const {
        if (row < 0 || row >= pieces.size() || col < 0 ||
            col >= pieces[0].size()) {
            std::cout << '?'; // shouldnt happen
            return;
        }

        const auto &coloredPiece = pieces[row][col];
        if (coloredPiece) {
            std::cout << (coloredPiece->color == Color::White
                              ? TerminalColor::Modifier(TerminalColor::FG_WHITE)
                              : TerminalColor::Modifier(TerminalColor::FG_BLUE))
                      << coloredPiece->getChar();
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

    void print() const {
        for (char i = '8'; i >= '1'; i--) {
            for (char j = 'a'; j <= 'h'; j++) {
                std::string s = std::string(1, j);
                s.push_back(i);

                printChar(s);
            }
            std::cout << std::endl;
        }
    }
};
