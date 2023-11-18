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

TEST(GameTests, InCheck) {
    {
        Board board("----k---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "----Q---"
                    "--------"
                    "----K---");

        Game gameBlack(board, Color::Black);
        EXPECT_EQ(gameBlack.inCheck(), true);

        Game gameWhite(board, Color::White);
        EXPECT_EQ(gameWhite.inCheck(), false);
    }

    {
        Board board("-Q--k---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "-q--K---");

        Game gameBlack(board, Color::Black);
        EXPECT_EQ(gameBlack.inCheck(), true);

        Game gameWhite(board, Color::White);
        EXPECT_EQ(gameWhite.inCheck(), true);
    }

    {
        Board board("----k---"
                    "----K---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------");

        Game gameBlack(board, Color::Black);
        EXPECT_EQ(gameBlack.inCheck(), true);

        Game gameWhite(board, Color::White);
        EXPECT_EQ(gameWhite.inCheck(), true);
    }
}

TEST(GameTests, Checkmate) {
    {
        Board board("----k---"
                    "---RQ---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "----K---");

        Game gameBlack(board, Color::Black);
        EXPECT_EQ(gameBlack.inCheck(), true);
        EXPECT_EQ(gameBlack.inCheckmate(), true);
    }

    {
        Board board("----k---"
                    "----Q---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "----K---");

        Game gameBlack(board, Color::Black);
        EXPECT_EQ(gameBlack.inCheck(), true);
        EXPECT_EQ(gameBlack.inCheckmate(), false);
    }
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
        EXPECT_EQ(game.legalMoves().size(), 5);
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
        EXPECT_EQ(game.legalMoves().size(), 8);
    }

    {
        Board board("--------"
                    "--------"
                    "--------"
                    "----k---"
                    "--------"
                    "----K---"
                    "--------"
                    "--------");

        Game game(board, Color::White);
        EXPECT_EQ(game.possibleMoves().size(), 8);
        EXPECT_EQ(game.legalMoves().size(), 5);
    }
}

TEST(GameTests, QueenNumMoves) {
    {
        Board board("----k---"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "Q---K---");

        Game game(board, Color::White);

        EXPECT_EQ(game.possibleMoves().size(), 17 + 5);
        EXPECT_EQ(game.legalMoves().size(), 17 + 5);
    }
}
