#include "include/agent.h"
#include "include/board.h"

using namespace std;

int main() {
    Board board;
    board.print();
    std::cout << "HI" << std::endl;

    Agent agent;
    auto move = agent.chooseBestMove(board, 10);
    std::cout << move << std::endl;

    auto nextBoard = board.makeMove(move);
    nextBoard.print();
}
