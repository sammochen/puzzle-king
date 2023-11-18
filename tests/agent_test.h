#include "agent.h"

#include <gtest/gtest.h>

TEST(AgentTests, AgentTest) {

    Board board("------k-"
                "-----ppp"
                "--------"
                "--------"
                "--------"
                "----Q---"
                "--------"
                "----K---");
    Game game(board, Color::White);

    Agent agent;
    const Move move = agent.chooseBestMove(game, 1000);

    EXPECT_EQ(move.to.row, 7);
    EXPECT_EQ(move.to.col, 4);
}
