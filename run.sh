# g++ --std=c++17 -O3 main.cpp  && ./a.out

rm a.out
g++ --std=c++17 --debug -fsanitize=address main.cpp  && ./a.out
