#include "include/terminal_color.h"

#include <iostream>
#include <numeric>
#include <optional>
#include <stdlib.h>
#include <utility>
#include <vector>

using namespace std;

enum Color { WHITE = 0, BLACK = 1 };
struct Piece {
  virtual char display() const = 0;
};

struct Pawn : Piece {
  char display() const override { return 'P'; }
};
struct Knight : Piece {
  char display() const override { return 'N'; }
};
struct Bishop : Piece {
  char display() const override { return 'B'; }
};
struct Rook : Piece {
  char display() const override { return 'R'; }
};
struct Queen : Piece {
  char display() const override { return 'Q'; }
};
struct King : Piece {
  char display() const override { return 'K'; }
};

using ColoredPiece = std::pair<Color, Piece *>;

struct Board {
  vector<vector<optional<ColoredPiece>>> pieces;

  Board() {
    pieces.assign(8, vector<optional<ColoredPiece>>(8, nullopt));

    for (Color color : {Color::WHITE, Color::BLACK}) {
      const int bottom_row = color == Color::WHITE ? 0 : 7;
      pieces[bottom_row][0] = make_pair(color, new Rook());
      pieces[bottom_row][1] = make_pair(color, new Knight());
      pieces[bottom_row][2] = make_pair(color, new Bishop());
      pieces[bottom_row][3] = make_pair(color, new Queen());
      pieces[bottom_row][4] = make_pair(color, new King());
      pieces[bottom_row][5] = make_pair(color, new Bishop());
      pieces[bottom_row][6] = make_pair(color, new Knight());
      pieces[bottom_row][7] = make_pair(color, new Rook());

      const int top_row = color == Color::WHITE ? 1 : 6;
      for (int col = 0; col < 8; col++) {
        pieces[top_row][col] = make_pair(color, new Pawn());
      }
    }
  }

  void print_char(int row, int col) {
    const auto &colored_piece = pieces[row][col];
    if (colored_piece) {
      cout << (colored_piece->first == Color::WHITE
                   ? TerminalColor::Modifier(TerminalColor::FG_WHITE)
                   : TerminalColor::Modifier(TerminalColor::FG_BLUE))
           << colored_piece->second->display();
    } else {
      cout << TerminalColor::Modifier(TerminalColor::FG_RED) << '-';
    }

    cout << TerminalColor::Modifier(TerminalColor::FG_DEFAULT)
         << TerminalColor::Modifier(TerminalColor::BG_DEFAULT);
  }

  void print_char(const std::string &pos) {
    assert(pos.size() == 2);
    const int row = pos[1] - '1';
    const int col = pos[0] - 'a';
    return print_char(row, col);
  }

  void print() {
    for (char i = '8'; i >= '1'; i--) {
      for (char j = 'a'; j <= 'h'; j++) {
        print_char(std::string(1, j) + i);
      }
      cout << endl;
    }
  }
};

int main() {
  Board board;
  board.print();
}
