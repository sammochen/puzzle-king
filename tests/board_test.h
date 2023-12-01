#include "board.h"

#include <gtest/gtest.h>

TEST(BoardTests, DefaultConstructor) { Board board; }

TEST(BoardTests, StringConstructor) {

    std::string s = "rnbqkbnr"
                    "pppppppp"
                    "--------"
                    "--------"
                    "--------"
                    "--------"
                    "PPPPPPPP"
                    "RNBQKBNR";

    Board board(s);
    EXPECT_EQ(s, board.toString());
}

TEST(BoardTests, KingsConstructor) {

    std::string s = "--------"
                    "--------"
                    "---k----"
                    "--------"
                    "---K----"
                    "--------"
                    "--------"
                    "--------";

    Board board(s);
    EXPECT_EQ(s, board.toString());
}

TEST(BoardTests, MakeMove) {

    std::string s = "--------"
                    "--------"
                    "---k----"
                    "--------"
                    "---K----"
                    "--------"
                    "--------"
                    "--------";

    Board board(s);
    EXPECT_EQ(s, board.toString());

    std::string moved = "--------"
                        "--------"
                        "---k----"
                        "--------"
                        "----K---"
                        "--------"
                        "--------"
                        "--------";

    Board movedBoard = board.makeMove(Move{{3, 3}, {3, 4}});
    EXPECT_EQ(moved, movedBoard.toString());

    // assert old board still the same
    EXPECT_EQ(s, board.toString());
}
