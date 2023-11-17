#pragma once

#include "piece.h"
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

// struct Piece {
//     virtual char display() const = 0;
//     virtual double getValue() const = 0;

//     // true if you can move from->to
//     virtual std::vector<Square> getMovableSquares(const Square &cur,
//                                                   const Board &board) const {
//         // throw std::runtime_error("unimplemented");
//         return {{0, 0},
//                 {7, 4},
//                 {7, 5},
//                 {7, 7},
//                 {7, 6}}; // every piece can move to 0,0  lol
//     };
// };

// struct Pawn : Piece {
//     char display() const override { return 'P'; }
//     double getValue() const override { return 1; }
// };
// struct Knight : Piece {
//     char display() const override { return 'N'; }
//     double getValue() const override { return 3; }
// };
// struct Bishop : Piece {
//     char display() const override { return 'B'; }
//     double getValue() const override { return 3; }
// };
// struct Rook : Piece {
//     char display() const override { return 'R'; }
//     double getValue() const override { return 5; }
//     std::vector<Square> getMovableSquares(const Square &cur,
//                                           const Board &board) const override
//                                           {

//         const auto curColor = board.pieces[cur.row][cur.col]->first;

//         vector<int> di = {0, 0, 1, -1};
//         vector<int> dj = {1, -1, 0, 0};

//         std::vector<Square> result;

//         for (int d = 0; d < 4; d++) {
//             int row = cur.row + di[d];
//             int col = cur.col + dj[d];
//             while (1) {
//                 if (row < 0 || row >= 8 || col < 0 || col >= 8) {
//                     break;
//                 }
//                 if (board.pieces[row][col] &&
//                     board.pieces[row][col]->first == curColor) {
//                     break; // cannot go past own pieces
//                 }

//                 result.push_back({row, col});
//                 if (board.pieces[row][col]) {
//                     break; // other piece
//                 }
//             }
//         }

//         return result;
//     };
// };
// struct Queen : Piece {
//     char display() const override { return 'Q'; }
//     double getValue() const override { return 9; }
// };
// struct King : Piece {
//     char display() const override { return 'K'; }
//     double getValue() const override { return 10000; }
// };

// Captures the board state
struct Board {
    // a1 = 00, h8 = 77
    std::vector<std::vector<std::optional<ColoredPiece>>> pieces;
    Color turn = Color::WHITE;

    Board(const std::string &s, const Color &turn) : turn(turn) {
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
                            "RNBQKBNR"),
                Color::WHITE) {}

    // possible by piece movement, but not by legality
    // e.g. might move but leave king in check
    std::vector<Move> possibleMoves() const {
        std::vector<Move> result;

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here
                if (piece->color != turn)
                    continue; // can't move other color piece

                // for (auto &square :
                //      piece->second->getMovableSquares({row, col}, *this)) {
                //     result.push_back({{row, col}, square});
                // }
            }
        }

        return result;
    }

    // legal moves are moves where your king is not in check
    std::vector<Move> legalMoves() const {
        std::vector<Move> result;

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here
                if (piece->color != turn)
                    continue; // can't move other color piece

                // TODO move logic
                // for (auto &square :
                //      piece->piece->getMovableSquares({row, col}, *this)) {
                //     result.push_back({{row, col}, square});
                // }
            }
        }

        return result;
    }

    // move should be legal - throws error if illegal
    Board makeMove(const Move &move) const {
        // note - doesnt handle en passent and castling and promotion
        Board copy = *this;
        copy.turn = turn == Color::BLACK ? Color::WHITE : Color::BLACK;
        copy.pieces[move.to.row][move.to.col] =
            copy.pieces[move.from.row][move.from.col];
        copy.pieces[move.from.row][move.from.col] = std::nullopt;
        return copy;
    }

    Color other_color(const Color &c) const {
        return c == Color::WHITE ? Color::BLACK : Color::WHITE;
    }

    bool inCheck() const {
        // if it is white's turn, you are in check if some piece can capture the
        // king
        Board copy = *this;
        copy.turn = other_color(copy.turn);

        for (auto move : copy.possibleMoves()) {
            auto &target = copy.pieces[move.to.row][move.to.col];
            if (target && target->color == turn &&
                target->piece.getChar() == 'K') {
                // if the target is your king, then you are in check
                return true;
            }
        }
        return false;
    }

    // and check if the game is over
    Status getStatus() const {
        // if the king is in check, and there are no legal moves, game over
        return Status::IN_PROGRESS;
    }

    void print_char(int row, int col) {
        const auto &colored_piece = pieces[row][col];
        if (colored_piece) {
            std::cout << (colored_piece->color == Color::WHITE
                              ? TerminalColor::Modifier(TerminalColor::FG_WHITE)
                              : TerminalColor::Modifier(TerminalColor::FG_BLUE))
                      << colored_piece->piece.getChar();
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

    double heuristic() const { // return positive when WHITE is winning
        double sum = 0;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                const auto &piece = pieces[row][col];
                if (piece == std::nullopt)
                    continue; // no piece here

                double multiplier = piece->color == Color::WHITE ? 1 : -1;
                sum += multiplier; // TODO piece value
            }
        }
        return sum;
    }
};
