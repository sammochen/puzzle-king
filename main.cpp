#include "include/agent.h"
#include "include/board.h"

using namespace std;

int main() {
    Board board;
    board.print();

    Agent agent;

    agent.makeBestMove(board, 10);
}
