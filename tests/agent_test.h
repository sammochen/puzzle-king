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
    const Move move = agent.chooseBestMove(game, 1000);
    const Move expectedMove{Square{1, 3}, Square{7, 3}};
    EXPECT_EQ(move, expectedMove);
}

TEST(AgentTests, MateInTwo) {

    Board board("r-----k-"
                "-----ppp"
                "--------"
                "--------"
                "----R---"
                "----Q---"
                "--------"
                "----K---");
    Game game(board, Color::White);

    Agent agent;
    const Move move = agent.chooseBestMove(game, 1000);
    const Move expectedMove{Square{3, 4}, Square{7, 4}};
    EXPECT_EQ(move, expectedMove);
}
