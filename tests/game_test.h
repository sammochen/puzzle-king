#include "game.h"
#include <gtest/gtest.h>

TEST(GameTests, DefaultConstructor) { Game game; }

TEST(GameTests, StringConstructor) {
    Board board("rnbqkbnr"
                "pppppppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "PPPPPPPP"
                "RNBQKBNR");

    Game game(board, Color::White);
}

TEST(GameTests, KingNumMoves) {
    {
        Board board("----k---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "----K---");

        Game game(board, Color::White);
        EXPECT_EQ(game.possibleMoves().size(), 5);
    }

    {
        Board board("----k---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "----K---"
                    "--------"
                    "--------");

        Game game(board, Color::White);
        EXPECT_EQ(game.possibleMoves().size(), 8);
    }
}
