#include "agent.h"

#include <gtest/gtest.h>

TEST(AgentTests, MateInOne) {
    Board board("------k-"
                "r----ppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "---R-PPP"
                "------K-");
    Game game(board, Color::White);

    Agent agent;
    const Move move = agent.chooseBestMove(game, 10000);
    const Move expectedMove{Square{1, 3}, Square{7, 3}};
    EXPECT_EQ(move, expectedMove);
}

TEST(AgentTests, MateInTwo) {
    Board board("r-----k-"
                "-----ppp"
                "--------"
                "--------"
                "--R-----"
                "--------"
                "--R-----"
                "----K---");
    Game game(board, Color::White);

    Agent agent;
    const Move move = agent.chooseBestMove(game, 10000);
    const Move expectedMove{Square{3, 4}, Square{7, 4}};
    EXPECT_EQ(move, expectedMove);
}

TEST(AgentTests, TakeAFreePiece) {
    Board board("------k-"
                "--------"
                "--------"
                "--------"
                "-r--R---"
                "--------"
                "--------"
                "----K---");
    Game game(board, Color::White);

    Agent agent;
    const Move move = agent.chooseBestMove(game, 10000);
    const Move expectedMove{Square{3, 4}, Square{3, 1}};
    EXPECT_EQ(move, expectedMove);
}

TEST(AgentTests, Fork) {
    Board board("-r------"
                "--------"
                "--------"
                "--------"
                "b-----k-"
                "--------"
                "---R----"
                "----K---");
    Game game(board, Color::White);

    Agent agent;
    const Move move = agent.chooseBestMove(game, 10000);
    const Move expectedMove{Square{1, 3}, Square{3, 3}};
    EXPECT_EQ(move, expectedMove);

    game = game.makeMove(move);
}
