#include "include/agent.h"
#include "include/board.h"
#include "include/game.h"

using namespace std;

int main() {
    printf("Hello World!\n");

    // play a game with itself?

    Game game;
    while (1) {
        game.board.print();
        Agent agent;

        auto bestMove = agent.chooseBestMove(game, 2000);
        game = game.makeMove(bestMove);
    }
}
