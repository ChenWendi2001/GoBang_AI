#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <utility>
#include <vector>
#include <cstring>
#include <queue>

enum GameType{
    PVP,
    AI
};

enum GameStatus{
    PLAYING,
    WIN,
    DEAD
};

const int DEPTH = 6;

const int boundary[2] = {50,70};

const int BOARD_GRAD_SIZE = 15;
const int MARGIN = 60;
const int BLOCK_SIZE = 40;
const int CHESS_RADIUS = 19;

const int NGIFF = -2147483648;
const int IFF = 2147483647;

const int ONE = 100;
const int TWO = 400;
const int THREE = 1000;
const int FOUR = 2000;
const int FIVE = 4000;
const int BLOCKED_ONE = 1;
const int BLOCKED_TWO = 100;
const int BLOCKED_THREE = 400;
const int BLOCKED_FOUR = 1000;

const int one = 150;
const int two  = 600;
const int three = 1000;
const int four = 2000;
const int five = 4000;
const int blocked_one = 1;
const int blocked_two = 150;
const int blocked_three = 600;
const int blocked_four = 1000;


struct config{
    int one=0;
    int two=0;
    int three=0;
    int four=0;
    int five=0;
    int b_one=0;
    int b_two=0;
    int b_three=0;
    int b_four=0;

    int v=0;

    void operator+=(const config & other){
        one+=other.one;
        two+=other.two;
        three+=other.three;
        four+=other.four;
        five+=other.five;
        b_one+=other.b_one;
        b_two+=other.b_two;
        b_three+=other.b_three;
        b_four+=other.b_four;
    }

    void operator=(const config & other){
        one=other.one;
        two=other.two;
        three=other.three;
        four=other.four;
        five=other.five;
        b_one=other.b_one;
        b_two=other.b_two;
        b_three=other.b_three;
        b_four=other.b_four;

        v = other.v;

    }

    config(){
        one=0;
        two=0;
        three=0;
        four=0;
        five=0;
        b_one=0;
        b_two=0;
        b_three=0;
        b_four=0;

        v=0;
    }
};



struct score{
    int s=0;
    int row;
    int col;
    int alpha;
    int beta;

    bool immKILL = false;
};


struct info{
    std::pair<int,int> a;
    int value;
};

struct cmp{
    bool operator()(info a,info b){
        return a.value<b.value;
    }
};


class GameModel{
public:

    int ai_side;
    bool playerFlag;
    int turn = 0;
    int board[15][15];
    config scoreMap[15][15];

    int cur_depth;
    int N;

    int last_row,last_col;

    int win_x[5];
    int win_y[5];

    GameModel();

    GameType gameType;
    GameStatus gameStatus;

    void startGame(GameType type,int aiside);
    void actionByplayer(int row,int col);
    void actionByAI();

    bool isWin(int row,int col);
    bool isDead();

    bool _assert(int row, int col);//检查该坐标是否合法
    void next_point(int &row,int &col,int d,int e);
    void farthest(int & length,int &ans_row,int &ans_col,int start_row,int start_col,int rol,int d, int e);
    void get_neighbour(int left_row,int left_col,int right_row,int right_col,int* left, int* right,int d,int role);

    bool has_neighbour(int row,int col);
    config analyze(int length,int*left,int *right,int role);


    void evaluate_point(int row,int col,int role);
    void reve();
    void updateboard(int row,int col);
    void remove(int row,int col);
    int evaluate(int role);
    void find_all_pos(std::vector<std::pair<int,int>> & vec);

    score alpha_beta_search(int row,int col,int deep,int alpha,int beta);
    score iterative_DFS(int deep);

};

#endif // GAMEMODEL_H
