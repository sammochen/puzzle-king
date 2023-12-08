# Puzzle King

My attempt at coding a chess bot!

Starting as a smaller project and will try get puzzles working first

## Run

```sh
mkdir build
cd build
cmake .. && cmake --build .
```

To run a specific test case

```sh
./all_tests --gtest_filter="*Fork*"
```

## TODOs

- [ ] Implement all the rules of chess
  - [ ] Special rules: Castling, En Passent, Promotions
  - [ ] Optimise for time/space complexity
- [ ] Load puzzles + solve
