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

// a move for a board is well defined by the to and from
struct Move {
    Square to, from;
};

enum Status { WHITE_WIN = 0, BLACK_WIN = 1, IN_PROGRESS = 2 };

enum Color { WHITE = 0, BLACK = 1 };

struct Piece {
    virtual char display() const = 0;

    // true if you can move from->to
    virtual bool canMove(const Square &from, const Square &to,
                         const Board &board) {
        throw std::runtime_error("unimplemented");
    };
};

struct Pawn : Piece {
    char display() const override { return 'P'; }
};
struct Knight : Piece {
    char display() const override { return 'N'; }
};
struct Bishop : Piece {
    char display() const override { return 'B'; }
};
struct Rook : Piece {
    char display() const override { return 'R'; }

    bool canMove(const Square &from, const Square &to, const Board &board) {
        return true;
    };
};
struct Queen : Piece {
    char display() const override { return 'Q'; }
};
struct King : Piece {
    char display() const override { return 'K'; }
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
            const int bottom_row = color == Color::WHITE ? 0 : 7;
            pieces[bottom_row][0] = std::make_pair(color, new Rook());
            pieces[bottom_row][1] = std::make_pair(color, new Knight());
            pieces[bottom_row][2] = std::make_pair(color, new Bishop());
            pieces[bottom_row][3] = std::make_pair(color, new Queen());
            pieces[bottom_row][4] = std::make_pair(color, new King());
            pieces[bottom_row][5] = std::make_pair(color, new Bishop());
            pieces[bottom_row][6] = std::make_pair(color, new Knight());
            pieces[bottom_row][7] = std::make_pair(color, new Rook());

            const int top_row = color == Color::WHITE ? 1 : 6;
            for (int col = 0; col < 8; col++) {
                pieces[top_row][col] = std::make_pair(color, new Pawn());
            }
        }
    }

    // as an interface, all it cares about is: what are the legal moves, and
    // applying the legal move
    std::vector<Move> possibleMoves() const {}

    // move should be legal - throws error if illegal
    Board makeMove(const Move &move) const {}

    // and check if the game is over
    Status getStatus() const {}

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
};
