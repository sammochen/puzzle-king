#include "agent.h"

#include <gtest/gtest.h>

std::vector<Move> simulateMoves(Game game, int num_moves, int num_iterations) {
    Agent agent;
    game.board.print();
    std::vector<Move> moves;
    for (int i = 0; i < num_moves; i++) {
        moves.emplace_back(agent.chooseBestMove(game, num_iterations));
        game = game.makeMove(moves.back());
        game.board.print();
    }
    return moves;
}

TEST(AgentTests, MateInOneWhite) {
    Board board("------k-"
                "r----ppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "---R-PPP"
                "------K-");
    Game game(board, Color::White);

    auto moves = simulateMoves(game, 1, 10000);

    const Move expectedMove{Square{1, 3}, Square{7, 3}};
    EXPECT_EQ(moves[0], expectedMove);
}

TEST(AgentTests, MateInOneBlack) {
    Board board("------k-"
                "---r-ppp"
                "--------"
                "--------"
                "--------"
                "--------"
                "R----PPP"
                "------K-");
    Game game(board, Color::Black);

    auto moves = simulateMoves(game, 1, 10000);
    const Move expectedMove{Square{6, 3}, Square{0, 3}};
    EXPECT_EQ(moves[0], expectedMove);
}

TEST(AgentTests, MateInTwoNoSack) {
    Board board("--------"
                "------k-"
                "R-------"
                "--------"
                "--------"
                "--------"
                "--R-----"
                "----K---");
    Game game(board, Color::White);

    Agent agent;
    auto moves = simulateMoves(game, 3, 10000);
    const Move expectedMove1{Square{1, 2}, Square{6, 2}};
    EXPECT_EQ(moves[0], expectedMove1);
    const Move expectedMove2{Square{5, 0}, Square{7, 0}};
    EXPECT_EQ(moves[2], expectedMove2);
}

TEST(AgentTests, DodgeMate) {
    Board board("--------"
                "R-----k-"
                "--------"
                "--------"
                "--------"
                "--------"
                "--R-----"
                "----K---");
    Game game(board, Color::Black);

    Agent agent;
    auto moves = simulateMoves(game, 1, 10000);
    const Move expectedMove1{Square{6, 6}, Square{5, 6}};
    EXPECT_EQ(moves[0], expectedMove1);
}

TEST(AgentTests, TakeAFreePieceWhite) {
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
    auto moves = simulateMoves(game, 1, 10000);
    const Move expectedMove1{Square{3, 4}, Square{3, 1}};
    EXPECT_EQ(moves[0], expectedMove1);
}

TEST(AgentTests, TakeAFreePieceBlack) {
    Board board("------k-"
                "--------"
                "--------"
                "--------"
                "-r--R---"
                "--------"
                "--------"
                "----K---");
    Game game(board, Color::Black);

    Agent agent;
    auto moves = simulateMoves(game, 1, 10000);
    const Move expectedMove1{Square{3, 1}, Square{3, 4}};
    EXPECT_EQ(moves[0], expectedMove1);
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
