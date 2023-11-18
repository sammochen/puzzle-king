#pragma once

#include "board.h"
#include "piece.h"

enum class GameStatus { WhiteWin = 0, BlackWin = 1, InProgress = 2 };

// The entire game. outside should only interact with a game
struct Game {
    Board board;
    Color turn;

    Game() : board(Board()), turn(Color::White) {}
    Game(const Board &board, const Color &turn) : board(board), turn(turn) {}

    // legal moves are possible moves that dont leave you in check
    std::vector<Move> legalMoves() const {
        std::vector<Move> res;
        for (auto move : possibleMoves()) {
            Board newBoard = board.makeMove(move);
            Game newGame(newBoard,
                         turn); // hypothetical new game in the same turn
            if (newGame.inCheck()) {
                continue;
            }
            res.push_back(move);
        }
        return res;
    }

    bool inCheck() const {
        Game copy(board, turn.other());

        for (auto move : copy.possibleMoves()) {
            const auto &target = copy.board.pieces[move.to.row][move.to.col];
            if (target && target->color == turn &&
                target->piece == Piece::King) {
                // if the target is your king, then you are in check
                return true;
            }
        }
        return false;
    }

    GameStatus getStatus() const {
        if (inCheckmate()) {
            if (turn == Color::White) {
                return GameStatus::BlackWin;
            } else {
                return GameStatus::WhiteWin;
            }
        }
        return GameStatus::InProgress;
    }

    Game makeMove(const Move &move) const {
        Board nextBoard = board.makeMove(move);
        return Game(nextBoard, turn.other());
    }

    bool inCheckmate() const { return inCheck() && legalMoves().size() == 0; }

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
                } else if (piece.piece == Piece::Queen) {
                    for (auto square : possibleQueenSquares(row, col)) {
                        result.push_back({{row, col}, square});
                    }
                } else if (piece.piece == Piece::Rook) {
                    for (auto square : possibleRookSquares(row, col)) {
                        result.push_back({{row, col}, square});
                    }
                } else if (piece.piece == Piece::Bishop) {
                    for (auto square : possibleBishopSquares(row, col)) {
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
        assert(color == turn);

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
            row += di;
            col += dj;
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

    std::vector<Square> possibleRookSquares(const int startRow,
                                            const int startCol) const {
        return possiblePerpendicularSquares(startRow, startCol, 8);
    }

    std::vector<Square> possibleBishopSquares(const int startRow,
                                              const int startCol) const {
        return possibleDiagonalSquares(startRow, startCol, 8);
    }
};
