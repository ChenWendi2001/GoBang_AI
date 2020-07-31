#include "gamemodel.h"

void GameModel::startGame(GameType type,int aiside){
    gameType = type;
    turn = 0;
    playerFlag = false;
    N = 8;
    memset(board, -1, sizeof(board));

    if(gameType == AI){
        ai_side = 1-aiside;
    }

    playerFlag = false;

    if(gameType== AI && ai_side==0){
        updateboard(7,7);
    }

}

GameModel::GameModel(){
    last_col = last_row = -1;
}



void GameModel::actionByplayer(int row, int col){
    updateboard(row,col);
}

void GameModel::actionByAI(){
    score temp;
    temp = iterative_DFS(DEPTH);
    updateboard(temp.row,temp.col);
    last_row = temp.row;
    last_col = temp.col;
}



bool GameModel::isWin(int row, int col){
    for(int i = 0;i<5;++i){
        if(
                row-i>=0 && row-i+4<BOARD_GRAD_SIZE
                && board[row-i][col]==board[row-i+1][col]
                && board[row-i][col]==board[row-i+2][col]
                && board[row-i][col]==board[row-i+3][col]
                && board[row-i][col]==board[row-i+4][col])
        {

            win_x[0] = row-i;win_y[0]=col;
            win_x[1] = row-i+1;win_y[1]=col;
            win_x[2] = row-i+2;win_y[2]=col;
            win_x[3] = row-i+3;win_y[3]=col;
            win_x[4] = row-i+4;win_y[4]=col;

            return true;
        }
    }
    for(int i = 0;i<5;++i){
        if(
                col-i>=0 && col-i+4<BOARD_GRAD_SIZE
                && board[row][col-i]==board[row][col-i+1]
                && board[row][col-i]==board[row][col-i+2]
                && board[row][col-i]==board[row][col-i+3]
                && board[row][col-i]==board[row][col-i+4])
        {

            win_x[0] = row;win_y[0]=col-i;
            win_x[1] = row;win_y[1]=col-i+1;
            win_x[2] = row;win_y[2]=col-i+2;
            win_x[3] = row;win_y[3]=col-i+3;
            win_x[4] = row;win_y[4]=col-i+4;

            return true;
        }
    }
    for(int i = 0;i<5;++i){
        if(
                row-i>=0 && row-i+4<BOARD_GRAD_SIZE
                &&col-i>=0 && col-i+4<BOARD_GRAD_SIZE
                && board[row-i][col-i]==board[row-i+1][col-i+1]
                && board[row-i][col-i]==board[row-i+2][col-i+2]
                && board[row-i][col-i]==board[row-i+3][col-i+3]
                && board[row-i][col-i]==board[row-i+4][col-i+4])
        {

            win_x[0] = row-i;win_y[0]=col-i;
            win_x[1] = row-i+1;win_y[1]=col-i+1;
            win_x[2] = row-i+2;win_y[2]=col-i+2;
            win_x[3] = row-i+3;win_y[3]=col-i+3;
            win_x[4] = row-i+4;win_y[4]=col-i+4;

            return true;
        }
    }
    for(int i = 0;i<5;++i){
        if(
                row+i-4>=0 && row+i<BOARD_GRAD_SIZE
                &&col-i>=0 && col-i+4<BOARD_GRAD_SIZE
                && board[row+i][col-i]==board[row+i-1][col-i+1]
                && board[row+i][col-i]==board[row+i-2][col-i+2]
                && board[row+i][col-i]==board[row+i-3][col-i+3]
                && board[row+i][col-i]==board[row+i-4][col-i+4])
        {

            win_x[0] = row+i;win_y[0]=col-i;
            win_x[1] = row+i-1;win_y[1]=col-i+1;
            win_x[2] = row+i-2;win_y[2]=col-i+2;
            win_x[3] = row+i-3;win_y[3]=col-i+3;
            win_x[4] = row+i-4;win_y[4]=col-i+4;

            return true;
        }
    }

    return false;
}

bool GameModel::_assert(int row,int col){
    if(row>=0 && col>=0 && row<BOARD_GRAD_SIZE && col< BOARD_GRAD_SIZE){
        return true;
    }
    else{
        return false;
    }
}

void GameModel::next_point(int &row, int &col,int d,int e){//d for direction s , e for step
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

void GameModel::farthest(int &length,int &ans_row, int &ans_col, int start_row, int start_col, int role, int d, int e){
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

void GameModel::get_neighbour(int left_row, int left_col, int right_row, int right_col, int *left, int *right, int d, int role){
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


config GameModel::analyze(int length, int *left, int *right, int role){
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

void GameModel::evaluate_point(int row, int col, int role){
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

void GameModel::reve(){
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

void GameModel::updateboard(int row, int col){
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


void GameModel::remove(int row, int col){
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


int GameModel::evaluate(int role){
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


bool GameModel::has_neighbour(int row, int col){

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



void GameModel::find_all_pos(std::vector<std::pair<int, int> > &vec){
    for(int i = 0;i<BOARD_GRAD_SIZE;i++){
        for(int j = 0;j<BOARD_GRAD_SIZE;j++){
            if(board[i][j]==-1 && has_neighbour(i,j)){
                vec.push_back(std::make_pair(i,j));
            }
        }
    }
}


score GameModel::alpha_beta_search(int row, int col, int deep, int alpha, int beta){
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

        if(p.empty()){
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

score GameModel::iterative_DFS(int deep){
    score temp;
    for(int i = 2;i<=deep;i+=2){
        cur_depth = i;
        temp = alpha_beta_search(0,0,i,NGIFF,IFF);
        if(temp.s>=200000||temp.immKILL) return temp;
    }
    return temp;
}
