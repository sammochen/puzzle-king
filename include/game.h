#pragma once

#include "board.h"
#include "piece.h"

// The entire game. outside should only interact with a game
struct Game {
    Game() {}
    Game(const Board &board) : board(board) {}

    Color turn() const { return board.turn; }

    bool inCheck() const {
        // // if it is White's turn, you are in check if some piece can capture
        // the
        // // king
        // Board copy = board;
        // copy.turn = copy.turn.other();

        // for (auto move : copy.possibleMoves()) {
        //     auto &target = copy.pieces[move.to.row][move.to.col];
        //     if (target && target->color == turn &&
        //         target->piece.getChar() == 'K') {
        //         // if the target is your king, then you are in check
        //         return true;
        //     }
        // }
        return false;
    }

    std::vector<Move> possibleMoves() { return {}; }

    std::vector<Move> legalMoves() { return {}; }

    // and check if the game is over
    Status getStatus() const {
        // if the king is in check, and there are no legal moves, game over
        return Status::IN_PROGRESS;
    }

    Board board; // a game has a board
};
