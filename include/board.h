#pragma once

#include "terminal_color.h"

#include <iostream>
#include <numeric>
#include <optional>
#include <stdlib.h>
#include <utility>
#include <vector>

struct Board;

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

enum Status { WHITE_WIN = 0, BLACK_WIN = 1, IN_PROGRESS = 2 };

enum Color { WHITE = 0, BLACK = 1 };

struct Piece {
    virtual char display() const = 0;
    virtual double getValue() const = 0;

    // true if you can move from->to
    virtual std::vector<Square> getMovableSquares(const Square &cur,
                                                  const Board &board) {
        // throw std::runtime_error("unimplemented");
        return {{0, 0}}; // every piece can move to 0,0  lol
    };
};

struct Pawn : Piece {
    char display() const override { return 'P'; }
    double getValue() const override { return 1; }
};
struct Knight : Piece {
    char display() const override { return 'N'; }
    double getValue() const override { return 3; }
};
struct Bishop : Piece {
    char display() const override { return 'B'; }
    double getValue() const override { return 3; }
};
struct Rook : Piece {
    char display() const override { return 'R'; }
    double getValue() const override { return 5; }
};
struct Queen : Piece {
    char display() const override { return 'Q'; }
    double getValue() const override { return 9; }
};
struct King : Piece {
    char display() const override { return 'K'; }
    double getValue() const override { return 10000; }
};

using ColoredPiece = std::pair<Color, Piece *>;

// Captures the board state
struct Board {
    std::vector<std::vector<std::optional<ColoredPiece>>> pieces;
    Color turn = Color::WHITE;

    // initialises the board
    Board() {
        pieces.assign(
            8, std::vector<std::optional<ColoredPiece>>(8, std::nullopt));

        for (Color color : {Color::WHITE, Color::BLACK}) {
            const int back_row = color == Color::WHITE ? 0 : 7;
            pieces[back_row][0] = std::make_pair(color, new Rook());
            pieces[back_row][1] = std::make_pair(color, new Knight());
            pieces[back_row][2] = std::make_pair(color, new Bishop());
            pieces[back_row][3] = std::make_pair(color, new Queen());
            pieces[back_row][4] = std::make_pair(color, new King());
            pieces[back_row][5] = std::make_pair(color, new Bishop());
            pieces[back_row][6] = std::make_pair(color, new Knight());
            pieces[back_row][7] = std::make_pair(color, new Rook());

            const int front_row = color == Color::WHITE ? 1 : 6;
            for (int col = 0; col < 8; col++) {
                pieces[front_row][col] = std::make_pair(color, new Pawn());
            }
        }
    }

    // as an interface, all it cares about is: what are the legal moves, and
    // applying the legal move
    std::vector<Move> possibleMoves() const {
        std::vector<Move> result;

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here
                if (piece->first != turn)
                    continue; // can't move other color piece

                for (auto &square :
                     piece->second->getMovableSquares({row, col}, *this)) {
                    result.push_back({{row, col}, square});
                }
            }
        }

        return result;
    }

    // move should be legal - throws error if illegal
    Board makeMove(const Move &move) const {
        // note - doesnt handle en passent and castling and promotion
        Board copy = *this;
        copy.pieces[move.to.row][move.to.col] =
            copy.pieces[move.from.row][move.from.col];
        copy.pieces[move.from.row][move.from.col] = std::nullopt;
        return copy;
    }

    // and check if the game is over
    Status getStatus() const { return Status::IN_PROGRESS; }

    void print_char(int row, int col) {
        const auto &colored_piece = pieces[row][col];
        if (colored_piece) {
            std::cout << (colored_piece->first == Color::WHITE
                              ? TerminalColor::Modifier(TerminalColor::FG_WHITE)
                              : TerminalColor::Modifier(TerminalColor::FG_BLUE))
                      << colored_piece->second->display();
        } else {
            std::cout << TerminalColor::Modifier(TerminalColor::FG_RED) << '-';
        }

        std::cout << TerminalColor::Modifier(TerminalColor::FG_DEFAULT)
                  << TerminalColor::Modifier(TerminalColor::BG_DEFAULT);
    }

    void print_char(const std::string &pos) {
        assert(pos.size() == 2);
        const int row = pos[1] - '1';
        const int col = pos[0] - 'a';
        return print_char(row, col);
    }

    void print() {
        for (char i = '8'; i >= '1'; i--) {
            for (char j = 'a'; j <= 'h'; j++) {
                print_char(std::string(1, j) + i);
            }
            std::cout << std::endl;
        }
    }

    double heuristic() const {
        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                double multiplier = piece->first == turn ? 1 : -1;
                sum += piece->second->getValue() * multiplier;
            }
        }
        return sum;
    }
};
