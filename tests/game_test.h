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
                "RNBQKBNR",
                Color::White);

    Game game(board);
}
