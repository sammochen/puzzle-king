#pragma once

#include "board.h"
#include "piece.h"

// The entire game. outside should only interact with a game
struct Game {
    Board board;
    Color turn;

    Game() : board(Board()), turn(Color::White) {}
    Game(const Board &board, const Color &turn) : board(board), turn(turn) {}

    // you are in check if the opponent can
    bool inCheck() const {

        Game copy = *this;
        copy.turn = copy.turn.other();

        for (auto move : copy.possibleMoves()) {
            auto &target = copy.board.pieces[move.to.row][move.to.col];
            if (target && target->color == turn &&
                target->piece == Piece::King) {
                // if the target is your king, then you are in check
                return true;
            }
        }
        return false;
    }

    // possible moves go through each piece and go through their moves
    std::vector<Move> possibleMoves() const {
        std::vector<Move> result;

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (!board.pieces[row][col])
                    continue;
                auto piece = *board.pieces[row][col];
                if (piece.color != turn)
                    continue; // not your turn

                if (piece.piece == Piece::King) {
                    for (auto square : possibleKingSquares(row, col)) {
                        result.push_back({{row, col}, square});
                    }
                }
            }
        }
        return result;
    }

    // just go forward in a line
    std::vector<Square> possibleStraightSquares(const int startRow,
                                                const int startCol,
                                                const int di, const int dj,
                                                const int maxDistance) const {
        const auto color = board.pieces[startRow][startCol]->color;
        auto row = startRow + di;
        auto col = startCol + dj;
        int steps = 1;

        std::vector<Square> result;

        while (row >= 0 && row < 8 && col >= 0 && col < 8 &&
               steps <= maxDistance) {
            if (board.pieces[row][col] &&
                board.pieces[row][col]->color == color) {
                break; // cannot go on your own colour
            }

            result.push_back({row, col});
            if (board.pieces[row][col]) {
                break; // must stop after capture
            }

            steps++;
        }

        return result;
    }

    // perpendicular - rook like
    std::vector<Square>
    possiblePerpendicularSquares(const int startRow, const int startCol,
                                 const int maxDistance) const {

        const std::vector<int> di = {0, 0, 1, -1}, dj = {1, -1, 0, 0};
        std::vector<Square> result;
        for (int d = 0; d < 4; d++) {
            auto cur = possibleStraightSquares(startRow, startCol, di[d], dj[d],
                                               maxDistance);
            result.insert(result.end(), cur.begin(), cur.end());
        }
        return result;
    }

    // diagonal - bishop like
    std::vector<Square> possibleDiagonalSquares(const int startRow,
                                                const int startCol,
                                                const int maxDistance) const {

        const std::vector<int> di = {1, -1, 1, -1}, dj = {1, 1, -1, -1};
        std::vector<Square> result;
        for (int d = 0; d < 4; d++) {
            auto cur = possibleStraightSquares(startRow, startCol, di[d], dj[d],
                                               maxDistance);
            result.insert(result.end(), cur.begin(), cur.end());
        }
        return result;
    }

    // both
    std::vector<Square>
    possiblePerpendicularDiagonalSquares(const int startRow, const int startCol,
                                         const int maxDistance) const {
        std::vector<Square> result;
        {
            auto cur =
                possiblePerpendicularSquares(startRow, startCol, maxDistance);
            result.insert(result.end(), cur.begin(), cur.end());
        }
        {
            auto cur = possibleDiagonalSquares(startRow, startCol, maxDistance);
            result.insert(result.end(), cur.begin(), cur.end());
        }
        return result;
    }

    std::vector<Square> possibleKingSquares(const int startRow,
                                            const int startCol) const {
        return possiblePerpendicularDiagonalSquares(startRow, startCol, 1);
    }

    std::vector<Square> possibleQueenSquares(const int startRow,
                                             const int startCol) const {
        return possiblePerpendicularDiagonalSquares(startRow, startCol, 8);
    }

    // legal moves are possible moves that dont leave you in check
    std::vector<Move> legalMoves() const { return {}; }

    // and check if the game is over
    Status getStatus() const {
        // if the king is in check, and there are no legal moves, game over
        return Status::IN_PROGRESS;
    }
};
