#include "AIController.h"
#include <utility>
#include <vector>
#include <cstring>
#include <queue>

extern int ai_side; //0: black, 1: white
std::string ai_name = "baby_ai";

bool playerFlag;

int turn = 0;
int board[15][15];

int cur_depth;
int N;
const int DEPTH = 6;

const int boundary[2] = {50,70};

const int BOARD_GRAD_SIZE = 15;


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

config scoreMap[15][15];

struct score{
    int s=0;
    int row;
    int col;
    int alpha;
    int beta;

    bool immKILL = false;
};

bool _assert(int row,int col){
    if(row>=0 && col>=0 && row<BOARD_GRAD_SIZE && col< BOARD_GRAD_SIZE){
        return true;
    }
    else{
        return false;
    }
}

void next_point(int &row, int &col,int d,int e){//d for direction s , e for step
    switch(d){
    case 1://-
        row += e;
        break;
    case 2://|
        col += e;
        break;
    case 3:// "\"
        row += e;
        col += e;
        break;
    case 4:// "/"
        row += e;
        col -= e;
        break;
    }
}

void farthest(int &length,int &ans_row, int &ans_col, int start_row, int start_col, int role, int d, int e){
    ans_row = start_row;
    ans_col = start_col;

    next_point(start_row,start_col,d,e);
    while(_assert(start_row,start_col) && board[start_row][start_col] == role){
        
        ans_row = start_row;
        ans_col = start_col;

        next_point(start_row,start_col,d,e);
        length ++;

    }

}

void get_neighbour(int left_row, int left_col, int right_row, int right_col, int *left, int *right, int d, int role){
    for(int i = 1;i<=4;++i){
        next_point(left_row,left_col,d,-1);
        if(_assert(left_row,left_col)){
            left[i] = board[left_row][left_col];
        }
        else{
            left[i] = 1-role;
        }
        next_point(right_row,right_col,d,1);
        if(_assert(right_row,right_col)){
            right[i] = board[right_row][right_col];
        }
        else{
            right[i] = 1-role;
        }
    }
}


config analyze(int length, int *left, int *right, int role){
    config ans;
    if(length>=5)
        ans.five++;
    else if(length == 4){
        if(left[1]==-1 && right[1]==-1){
            ans.four++;
        }
        else if(left[1]==-1 || right[1]==-1){
            ans.b_four++;
        }
    }
    else if (length == 3){
        if ((left[1] == -1 && left[2] == role)||(right[1] == -1 &&right[2]==role))
            ans.b_four++;
        else if (left[1] == -1 && right[1] == -1 && (left[2] == -1 || right[2] == -1))
            ans.three++;
        else if ((left[1] == -1 && left[2] ==-1) || (right[1] ==-1 && right[2] == -1))
            ans.b_three++;
    }
    else if (length == 2){
        if ((left[1] == -1 && left[2] == role && left[3] == role) &&
            (right[1] == -1 && right[2] == role && right[3] == role))
            ans.b_four++;
        else if (left[1] == -1 && right[1] == -1 &&
            ((left[2] == role && left[3] == -1) || (right[2] == role && right[3] == -1)))
            ans.three++;

        else if ((left[1] ==-1 && left[3] ==-1 && left[2] == role) ||
                    (right[1] ==-1 && right[3] ==-1 && right[2] == role))
            ans.b_three++;
        else if ((left[1] ==-1 && right[1] == -1) &&
                    (left[2] == role || right[2] == role))
            ans.b_three++;
        else if ((left[1] ==-1 && left[2] ==-1 && left[3] == role) ||
                    (right[1] ==-1 && right[2] ==-1 && right[3] == role))
            ans.b_three++;
        else if ((left[1] ==-1 && right[1] ==-1 && right[2] ==-1 && right[3] ==-1) ||
                 (left[1] == -1  && right[1] ==-1 && left[2] ==-1&& left[3] ==-1)||
            (left[1] == -1  && right[1] == -1 && (right[2] == -1|| left[2] == -1)) )
            ans.two++;
        else if ((left[1] ==-1 && left[2] ==-1 && left[3] ==-1) ||
                    (right[1] ==-1 && right[2] ==-1 && right[3] == -1))
            ans.b_two++;
    }
    else if (length == 1) {
        if ((left[1] ==  -1 && left[2] == role && left[3] == role && left[4] == role) ||
                (right[1] == -1 && right[2] == role && right[3] == role && right[4] == role))
            ans.b_four++;
        else if ((left[1] == -1 && right[1] == -1) && ((left[2] == role && left[3] == role && left[4] == -1) ||
                (right[2] == role && right[3] == role && right[4] == -1)))
            ans.three++;
        else if ((left[1] == -1 && right[1] == -1) &&
                ((left[2] == role && left[3] == role) || (right[2] == role && right[3] == role)))
            ans.b_three++;
        else if ((left[1] == -1 && left[4] == -1 && left[2] == role && left[3] == role) ||
                (right[1] == -1 && right[4] == -1 && right[2] == role && right[3] == role))
            ans.b_three++;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == role && left[4] == role) ||
                (right[1] == -1 && right[2] == -1 && right[3] == role && right[4] == role))
            ans.b_three++;
        else if ((left[1] == -1 && left[3] == -1 && left[2] == role && left[4] == role) ||
                (right[1] == -1 && right[3] == -1 && right[2] == role && right[4] == role))
            ans.b_three++;
        else if ((left[1] == -1 && right[1] == -1 && right[3] == -1 && right[2] == role) && (left[2] == -1 || right[4] == -1))
            ans.two++;
        else if ((right[1] == -1 && left[1] == -1 && left[3] == -1 && left[2] == role) &&
                (right[2] == -1 || left[4] == -1))
            ans.two++;
        else if ((left[1] == -1 && right[1] == -1 && right[2] == -1 && right[4] == -1 && right[3] == role) ||
                (right[1] == -1 && left[1] == -1 && left[2] == -1 && left[4] == -1 && left[3] == role))
            ans.two++;
        else if ((left[1] == -1 && left[3] == -1 && left[4] == -1 && left[2] == role) ||
                (right[1] == -1 && right[3] == -1 && right[4] == -1 && right[2] == role))
            ans.b_two++;
        else if ((left[1] == -1 && right[1] == -1 && right[2] == -1 && left[2] == role) ||
                (right[1] == -1 && left[1] == -1 && left[2] == -1 && right[2] == role))
            ans.b_two++;
        else if ((left[1] == -1 && left[2] == -1 && left[4] == -1 && left[3] == role) ||
                (right[1] == -1 && right[2] == -1 && right[4] == -1 && right[3] == role))
            ans.b_two++;
        else if ((left[1] == -1 && left[2] == -1 && right[1] == -1 && left[3] == role) ||
                (right[1] == -1 && right[2] == -1 && left[1] == -1 && right[3] == role))
            ans.b_two++;
        else if ((left[1] == -1 && left[2] == -1 && left[3] == -1 && left[4] == role) ||
                (right[1] == -1 && right[2] == -1 && right[3] == -1 && right[4] == role))
            ans.b_two++;
        else if (left[1] == -1 && right[1] == -1)
            ans.one++;
        else if (left[1] == -1 || right[1] == -1)
            ans.b_one++;
    }

    return ans;
}

void evaluate_point(int row, int col, int role){
    if(role == -1){
        config zero;
        scoreMap[row][col] = zero;
        return;
    }
    int sum=0;
    config ans;
    for(int i = 1;i<=4;++i){
        int left_row,left_col,right_row,right_col;
        int length = 1;
        farthest(length,left_row,left_col,row,col,role,i,-1);
        farthest(length,right_row,right_col,row,col,role,i,1);
        int left[5]; int right[5];
        get_neighbour(left_row,left_col,right_row,right_col,left,right,i,role);
        ans += analyze(length,left,right,role);
    }
    if(role == ai_side){
        sum += ans.one * ONE + ans.two * TWO +ans.three *THREE+ans.four * FOUR
            +ans.b_one * BLOCKED_ONE+ans.b_two * BLOCKED_TWO+ans.b_three * BLOCKED_THREE
            +ans.b_four * BLOCKED_FOUR + ans.five *FIVE;
    }
    else if(role == 1-ai_side){
        sum += ans.one * one + ans.two * two +ans.three *three+ans.four * four
            +ans.b_one * blocked_one +ans.b_two * blocked_two+ans.b_three * blocked_three
            +ans.b_four * blocked_four+ ans.five * five;
    }

    ans.v = sum;
    scoreMap[row][col] = ans;
    return;
}

void reve(){
    for(int i = 0;i<BOARD_GRAD_SIZE;i++){
        for(int j = 0;j<BOARD_GRAD_SIZE;j++){
            if(board[i][j]!=-1){
                board[i][j] = 1-board[i][j];
            }
        }
    }
    for(int i = 0;i<BOARD_GRAD_SIZE;i++){
        for(int j = 0;j<BOARD_GRAD_SIZE;j++){
            if(board[i][j]!=-1){
                evaluate_point(i,j,board[i][j]);
            }
        }
    }
    
    playerFlag = !playerFlag;
    //turn++;
}

void updateboard(int row, int col){
    if(row==-1 && col == -1){
        reve();
        turn++;
        return;
    }
    if(playerFlag){
        board[row][col]=1;
    }
    else{
        board[row][col]=0;
    }

    int i,j;
    i = row;
    for(j = col-4;j<=col+4;j++){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    j = col;
    for(i = row-4;i<=row+4;i++){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    for(i = row-4,j = col-4;i<=row+4;i++,j++){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    for(i = row-4,j = col+4;i<=row+4;i++,j--){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    playerFlag = !playerFlag;
    turn++;
}


void remove(int row, int col){
    if(row==-1 && col == -1){
        reve();
        turn--;
        return;
    }
    board[row][col]=-1;
    int i,j;
    i = row;
    for(j = col-4;j<=col+4;j++){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    j = col;
    for(i = row-4;i<=row+4;i++){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    for(i = row-4,j = col-4;i<=row+4;i++,j++){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    for(i = row-4,j = col+4;i<=row+4;i++,j--){
        if(_assert(i,j))evaluate_point(i,j,board[i][j]);
    }
    playerFlag = !playerFlag;
    turn--;
}


int evaluate(int role){
    int playerScore = 0;
    int AIScore = 0;
    int player_ratio=1;
    int AI_ratio=1;

    for(int i = 0;i<BOARD_GRAD_SIZE;i++){
        for(int j = 0;j<BOARD_GRAD_SIZE;j++){
            if(board[i][j]==1-ai_side){
                //evaluate_point(i,j,board[i][j]);
                playerScore += scoreMap[i][j].v;
            }
            else if(board[i][j]==ai_side){
                //evaluate_point(i,j,board[i][j]);
                AIScore += scoreMap[i][j].v;
            }
        }
    }

    int result = AIScore - playerScore;

    config AI_state;
    config player_state;

    for(int i = 0;i<BOARD_GRAD_SIZE;i++){
        for(int j = 0;j<BOARD_GRAD_SIZE;j++){
            if(board[i][j]==1-ai_side){
                player_state += scoreMap[i][j];
            }
            else if(board[i][j]==ai_side){
                AI_state += scoreMap[i][j];
            }
        }
    }

    
    if(player_state.five>0) result -= 500000;
    else if(AI_state.five>0) result += 200000;
    else if(player_state.b_four>0 || player_state.four>0) result-=60000;
    else if(player_state.three>5) result-=30000;
    else if(player_state.b_four==0 && player_state.four==0){
        if(AI_state.four>0) result+=20000;
        else if(AI_state.b_four>=5) result+=20000;
        else if(AI_state.b_four>0 && AI_state.three>0)result+=20000;
        else if(player_state.three>0) result-=20000;
        else if(AI_state.three>=5) result+=10000;
    }

    return (role==ai_side?1:-1)*result ;
}


bool has_neighbour(int row, int col){

    for(int j = 1;j<=4;j++){
        int temp_row = row;
        int temp_col = col;
        next_point(temp_row,temp_col,j,-2);
        for(int i = 1;i<=5;++i){
            if(i == 3){
                next_point(temp_row,temp_col,j,1);
            }
            else{
                if(_assert(temp_row,temp_col)&&board[temp_row][temp_col]!=-1)
                    return true;
                next_point(temp_row,temp_col,j,1);
            }
        }
    }
    return false;
}



void find_all_pos(std::vector<std::pair<int, int> > &vec){
    for(int i = 0;i<BOARD_GRAD_SIZE;i++){
        for(int j = 0;j<BOARD_GRAD_SIZE;j++){
            if(board[i][j]==-1 && has_neighbour(i,j)){
                vec.push_back(std::make_pair(i,j));
            }
        }
    }
}



struct info{
    std::pair<int,int> a;
    int value;
};

struct cmp{
    bool operator()(info a,info b){
        return a.value<b.value;
    }
};

score alpha_beta_search(int row, int col, int deep, int alpha, int beta){
    int count = 0;
    score ans;
    ans.s = NGIFF;
    ans.alpha = alpha;
    ans.beta = beta;
    ans.row = -1;
    ans.col = -1;

    if(!deep){
        score temp;
        temp.s = evaluate(ai_side);
        temp.row = row;
        temp.col = col;
        return temp;
    }
    else{
        std::vector<std::pair<int,int>> v;
        find_all_pos(v);
        std::priority_queue<info,std::vector<info>,cmp> p;
        bool flag = true;//是否出现必防棋
        
        for(int i = 0;i<BOARD_GRAD_SIZE;i++){
            for(int j = 0;j<BOARD_GRAD_SIZE;j++){
                if(board[i][j]==1-(playerFlag?1:0)&&(scoreMap[i][j].b_four>=1||scoreMap[i][j].three>=1)){
                    ans.immKILL = true;
                    flag = false;
                    break;
                }
            }
        }
        
        if(flag||deep<cur_depth){
            for(std::pair<int,int> s:v){
                info temp;
                temp.a = s;
                updateboard(s.first,s.second);
                temp.value = evaluate(playerFlag?0:1);
                p.push(temp);
                remove(s.first,s.second);
            }
        
            if(turn == 3){
                info temp;
                temp.a.first = -1;
                temp.a.second = -1;
                reve();
                temp.value = evaluate(playerFlag?0:1);;
                p.push(temp);
                reve();
            }
        }
        else{
            for(std::pair<int,int> s:v){
                info temp;
                temp.a = s;
                updateboard(s.first,s.second);
                temp.value = evaluate(playerFlag?0:1);
                bool flag2 = true;
                for(int i = 0;i<BOARD_GRAD_SIZE;i++){
                    for(int j = 0;j<BOARD_GRAD_SIZE;j++){
                        if(board[i][j]==(playerFlag?1:0)&&(scoreMap[i][j].b_four>=1||scoreMap[i][j].three>=1)){
                            flag2 = false;
                            break;
                        }
                    }
                }
                if(flag2) p.push(temp);
                remove(s.first,s.second);
            }
        }

        int minimax;
        for(int i = 0;i<N;i++){
            if(p.empty()) break;
            std::pair<int,int> s = p.top().a;
            p.pop();
            if(ans.alpha<ans.beta){
                updateboard(s.first,s.second);
                minimax = alpha_beta_search(s.first,s.second,deep-1,ans.alpha,ans.beta).s;
                if(deep%2 == 0){
                    if(!count){
                        ans.s = minimax;
                        count++;
                    }
                    if(ans.alpha<minimax){
                        ans.alpha = minimax;
                        ans.s = minimax;
                        ans.row = s.first;
                        ans.col = s.second;
                    }
                }
                else{
                    if(!count){
                        ans.s = minimax;
                        count++;
                    }
                    if(ans.beta>minimax){
                        ans.beta = minimax;
                        ans.s = minimax;
                        ans.row = s.first;
                        ans.col = s.second;
                    }
                }
                remove(s.first,s.second);
                //if(ans.alpha>=ans.beta) break;
            }
        }
        return ans;
    }
}

score iterative_DFS(int deep){
    score temp;
    for(int i = 2;i<=deep;i+=2){
        cur_depth = i;
        temp = alpha_beta_search(0,0,i,NGIFF,IFF);
        if(temp.s>=200000||temp.immKILL) return temp;
    }
    return temp;
}

//init function is called once at the beginning
void init() {
	/* TODO: Replace this by your code */
    turn = 0;
    playerFlag = false;
    N = 8;
	memset(board, -1, sizeof(board));
    //memset(scoreMap, 0, sizeof(scoreMap));
}

// loc is the action of your opponent
// Initially, loc being (-1,-1) means it's your first move
// If this is the third step(with 2 black ), where you can use the swap rule, your output could be either (-1, -1) to indicate that you choose a swap, or a coordinate (x,y) as normal.



std::pair<int, int> action(std::pair<int, int> loc) {
	/* TODO: Replace this by your code */
	/* This is now a random strategy */
    if(turn == 0 && loc.first == -1 && loc.second == -1){
        ai_side = 0;
        updateboard(14,14);
        return std::make_pair(14,14);
    }
    else if(turn == 0){
        ai_side = 1;
    }
  
    updateboard(loc.first,loc.second);

    if(turn == 1 && ai_side == 1){
        updateboard(loc.first,loc.second-1);
        return std::make_pair(loc.first,loc.second-1);
    }

    if(turn == 3 && ai_side == 1){
        updateboard(-1,-1);
        return std::make_pair(-1,-1);
    }

    if(turn>boundary[ai_side])N=6;

    score temp;
    temp = iterative_DFS(DEPTH);
    updateboard(temp.row,temp.col);

    return std::make_pair(temp.row,temp.col);
}