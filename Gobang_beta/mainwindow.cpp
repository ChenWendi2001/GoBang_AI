#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

MainWindow::MainWindow(int modeIndex,int sideIndex,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    mode = modeIndex;
    side = sideIndex;
    ai_time = 0;

    last_row = -1;
    last_col = -1;

    black.load(".\\source\\black.png");
    black = black.scaled(CHESS_RADIUS*2,CHESS_RADIUS*2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    white.load(".\\source\\white.png");
    white = white.scaled(CHESS_RADIUS*2,CHESS_RADIUS*2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    //board_pic.load("C:\\Users\\12779\\Desktop\\Qt\\board.jpg");


    ui->setupUi(this);
    initUI();
    initSignalSlots();
    setMouseTracking(true);

    setFixedSize((MARGIN*2 + BLOCK_SIZE * (BOARD_GRAD_SIZE-1))+200,MARGIN*2 + BLOCK_SIZE * (BOARD_GRAD_SIZE-1));
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B1723C"));
    //palette.setColor(QPalette::Background,QColor("#B2C8A7"));
    setPalette(palette);

    initGame(mode,side);

    clickPosCol = clickPosRow = -1;

    if(modeIndex == 0)
        this->setWindowTitle(QString("AI Game"));
    else
        this->setWindowTitle(QString("PVP Game"));
    this->setWindowIcon(QIcon(".\\source\\inc.png"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    QPen pen = painter.pen();

    pen.setColor(QColor("#8D5822"));
    pen.setWidth(7);
    painter.setPen(pen);

    QBrush brush;

    brush.setColor((QColor("#EEC085")));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(MARGIN/2,MARGIN/2,BLOCK_SIZE * (BOARD_GRAD_SIZE-1) + MARGIN,BLOCK_SIZE * (BOARD_GRAD_SIZE-1) + MARGIN);

/*
    painter.drawPixmap(MARGIN/2,MARGIN/2,
                       BLOCK_SIZE * (BOARD_GRAD_SIZE-1) + MARGIN,BLOCK_SIZE * (BOARD_GRAD_SIZE-1) + MARGIN,board_pic);
*/

    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    for(int i = 0;i < BOARD_GRAD_SIZE; ++i){
        painter.drawLine(MARGIN + BLOCK_SIZE * i,MARGIN,MARGIN + BLOCK_SIZE * i,size().height()-MARGIN);
        painter.drawLine(MARGIN ,MARGIN + BLOCK_SIZE * i,size().width()-200-MARGIN,MARGIN + BLOCK_SIZE * i);
    }

    brush.setColor(Qt::black);
    painter.setBrush(brush);
    painter.drawRect(MARGIN + BLOCK_SIZE * 7-5,MARGIN + BLOCK_SIZE * 7-5,10,10);
    painter.drawRect(MARGIN + BLOCK_SIZE * 3-5,MARGIN + BLOCK_SIZE * 3-5,10,10);
    painter.drawRect(MARGIN + BLOCK_SIZE * 11-5,MARGIN + BLOCK_SIZE * 11-5,10,10);
    painter.drawRect(MARGIN + BLOCK_SIZE * 3-5,MARGIN + BLOCK_SIZE * 11-5,10,10);
    painter.drawRect(MARGIN + BLOCK_SIZE * 11-5,MARGIN + BLOCK_SIZE * 3-5,10,10);

    pen.setColor(Qt::red);
    pen.setWidth(1);
    painter.setPen(pen);
    if(selectPos&&game->board[clickPosRow][clickPosCol] == -1){
           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol,
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow + BLOCK_SIZE/4, MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol);
           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow, MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol,
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol+ BLOCK_SIZE/4);

           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1) , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol,
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1) - BLOCK_SIZE/4, MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol);
           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1), MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol,
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1) , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosCol+ BLOCK_SIZE/4);

           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1),
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow + BLOCK_SIZE/4, MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1));
           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow, MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1),
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*clickPosRow , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1)- BLOCK_SIZE/4);

           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1) , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1),
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1) - BLOCK_SIZE/4, MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1));
           painter.drawLine(MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1), MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1),
                            MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosRow+1) , MARGIN - BLOCK_SIZE/2 + BLOCK_SIZE*(clickPosCol+1)- BLOCK_SIZE/4);
    }

    for(int i = 0;i<BOARD_GRAD_SIZE;++i){
        for(int j = 0;j<BOARD_GRAD_SIZE;++j){
            if(game->board[i][j]==1){
                pen.setColor(Qt::white);
                pen.setWidth(1);
                painter.setPen(pen);
                brush.setColor(Qt::white);
                painter.setBrush(brush);

                painter.drawPixmap(MARGIN + BLOCK_SIZE*i-CHESS_RADIUS,MARGIN + BLOCK_SIZE*j-CHESS_RADIUS,
                                   white);

                //painter.drawEllipse(MARGIN + BLOCK_SIZE*i-CHESS_RADIUS,MARGIN + BLOCK_SIZE*j-CHESS_RADIUS,CHESS_RADIUS*2,CHESS_RADIUS*2);
            }
            else if(game->board[i][j]==0){
                pen.setColor(Qt::black);
                pen.setWidth(1);
                painter.setPen(pen);
                brush.setColor(Qt::black);
                painter.setBrush(brush);

                painter.drawPixmap(MARGIN + BLOCK_SIZE*i-CHESS_RADIUS,MARGIN + BLOCK_SIZE*j-CHESS_RADIUS,
                                   black);

                //painter.drawEllipse(MARGIN + BLOCK_SIZE*i-CHESS_RADIUS,MARGIN + BLOCK_SIZE*j-CHESS_RADIUS,CHESS_RADIUS*2,CHESS_RADIUS*2);
            }
        }
    }



    if(game_type == AI){
        pen.setColor(Qt::black);
        painter.setPen(pen);
        QFont font1("Times New Roman",16,QFont::Bold);
        painter.setFont(font1);
        painter.drawText(430,0,680,this->height()*2/3,Qt::AlignCenter,"AI Time Consuming:");

        QString data;
        data.setNum(ai_time,'f',2);
        painter.drawText(430,0+30,680,this->height()*2/3+30,Qt::AlignCenter,data+" s");
    }


    if(last_row!=-1 && last_col !=-1 && game->gameStatus ==PLAYING){
        pen.setColor(Qt::red);
        painter.setPen(pen);
        brush.setColor(Qt::red);
        painter.setBrush(brush);
        painter.drawEllipse(MARGIN + BLOCK_SIZE*last_row-2,MARGIN + BLOCK_SIZE*last_col-2,4,4);
    }

    if(game->gameStatus == WIN){
        pen.setColor(Qt::red);
        pen.setWidth(2);
        painter.setPen(pen);
        brush.setColor(Qt::red);
        painter.setBrush(brush);
        for(int i = 0;i<=4;i++){
            painter.drawLine(MARGIN + BLOCK_SIZE*game->win_x[i]-6,MARGIN + BLOCK_SIZE*game->win_y[i],
                             MARGIN + BLOCK_SIZE*game->win_x[i]-3,MARGIN + BLOCK_SIZE*game->win_y[i]);
            painter.drawLine(MARGIN + BLOCK_SIZE*game->win_x[i]+3,MARGIN + BLOCK_SIZE*game->win_y[i],
                             MARGIN + BLOCK_SIZE*game->win_x[i]+6,MARGIN + BLOCK_SIZE*game->win_y[i]);
            painter.drawLine(MARGIN + BLOCK_SIZE*game->win_x[i],MARGIN + BLOCK_SIZE*game->win_y[i]+6,
                             MARGIN + BLOCK_SIZE*game->win_x[i],MARGIN + BLOCK_SIZE*game->win_y[i]+3);
            painter.drawLine(MARGIN + BLOCK_SIZE*game->win_x[i],MARGIN + BLOCK_SIZE*game->win_y[i]-6,
                             MARGIN + BLOCK_SIZE*game->win_x[i],MARGIN + BLOCK_SIZE*game->win_y[i]-3);

        }
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event){

    selectPos = false;

    int x = event->x();
    int y = event->y();

    if(x>MARGIN - BLOCK_SIZE/2 && x<BLOCK_SIZE * BOARD_GRAD_SIZE + MARGIN - BLOCK_SIZE/2
            && y>MARGIN - BLOCK_SIZE/2 && y<BLOCK_SIZE * BOARD_GRAD_SIZE + MARGIN - BLOCK_SIZE/2){
        x = (x- MARGIN + BLOCK_SIZE/2)/BLOCK_SIZE;
        y = (y- MARGIN + BLOCK_SIZE/2)/BLOCK_SIZE;
        if(game->board[x][y]==-1){
            selectPos = true;
            clickPosRow = x;
            clickPosCol = y;
            repaint();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(selectPos == false){
        return;
    }
    else{
        selectPos == false;
    }
    //QSound::play(".\\source\\mic.wav");
    chessOneByPerson();

    QTimer::singleShot(100,this,&chessOneByAI);//避免冲突
    repaint();
}

void MainWindow::initGame(int modeIndex,int sideIndex){
    game = new GameModel;
    if(modeIndex == 1){
        initPVPGame();
    }
    else{
        initAIGame(sideIndex);
    }
}

void MainWindow::initAIGame(int aiside){
    game_type = AI;
    game->gameStatus=PLAYING;
    game->startGame(game_type,aiside);
    if(game_type == AI && aiside==1){
        last_col = last_row = 7;
    }
    repaint();
}

void MainWindow::initPVPGame(){
    game_type = PVP;
    game->gameStatus=PLAYING;
    game->startGame(game_type,1);
    repaint();
}

void MainWindow::chessOneByPerson(){
    if(clickPosCol!=-1 && clickPosRow != -1 &&  game->board[clickPosRow][clickPosCol] == -1)
    {
        game->actionByplayer(clickPosRow,clickPosCol);
        sound2->play();
        last_row = clickPosRow;last_col = clickPosCol;
        if((game_type == PVP && game->turn==3)||(game_type == AI && game->turn==3)){
            swap->setEnabled(true);
        }
        else{
            swap->setEnabled(false);
        }
        if(clickPosCol!=-1 && clickPosRow!=-1 && game->board[clickPosRow][clickPosCol]!=-1){
            if(game->isWin(clickPosRow,clickPosCol) && game->gameStatus == PLAYING){
                game->gameStatus = WIN;
                repaint();
                QString str;
                if(game_type == AI) str ="You";
                else{
                    if(game->board[clickPosRow][clickPosCol] == 1){
                        str = "White";
                    }
                    else if(game->board[clickPosRow][clickPosCol]== 0){
                        str = "Black";
                    }
                }
                QMessageBox::StandardButton btnValue = QMessageBox::information(this,"Game Over", str + " wins!");
                if(btnValue == QMessageBox::Ok){
                    ai_time = 0;
                    last_row = -1;
                    last_col = -1;
                    game->startGame(game_type,side);
                    if(game_type == AI && side==1){
                        last_col = last_row = 7;
                    }
                    game->gameStatus = PLAYING;
                }
            }
        }

        repaint();
    }
}


void MainWindow::chessOneByAI(){
    if(game_type == AI){
        clock_t start = clock();
        game->actionByAI();
        clock_t end = clock();
        ai_time = (end-start)/double(CLOCKS_PER_SEC);
        sound->play();
        last_row = game->last_row;last_col = game->last_col;
        if(game->turn==3 && game->ai_side==0){
            swap->setEnabled(true);
        }
        else{
            swap->setEnabled(false);
        }

        if(game->isWin(game->last_row,game->last_col) && game->gameStatus == PLAYING){
            game->gameStatus = WIN;
            repaint();
            QString str;
            if(game->board[game->last_row][game->last_col] == game->ai_side){
                str = "AI";
            }
            else if(game->board[game->last_row][game->last_col]== 1-game->ai_side){
                str = "You";
            }

            QMessageBox::StandardButton btnValue = QMessageBox::information(this,"Game Over", str + " wins!");

            if(btnValue == QMessageBox::Ok){
                ai_time = 0;
                last_row = -1;
                last_col = -1;
                game->startGame(game_type,side);
                if(game_type == AI && side==1){
                    last_col = last_row = 7;
                }
                game->gameStatus = PLAYING;
            }
        }

        repaint();
    }
}

void MainWindow::swap_func(){
    game->updateboard(-1,-1);
    last_col=last_row = -1;
    swap->setEnabled(false);
    repaint();

    clock_t start = clock();
    chessOneByAI();
    clock_t end = clock();
    ai_time = (end-start)/double(CLOCKS_PER_SEC);
    repaint();
}

void MainWindow::initUI(){
    sound = new QSound(".\\source\\mic.wav", this);
    sound2 = new QSound(".\\source\\mic.wav", this);

    close = new QPushButton("Back",this);
    close->setFixedSize(100,45);
    QFont font("Times New Roman",18,QFont::Bold,false);
    close->setFont(font);
    close->show();
    close->move(this->width()* 1.8,this->height()*2-50);

    swap = new QPushButton("Swap",this);
    swap->setFixedSize(100,45);
    swap->setFont(font);
    swap->show();
    swap->move(this->width()* 1.8,this->height()*2-150);
}

void MainWindow::initSignalSlots(){
    swap->setEnabled(false);
    connect(close,&QPushButton::clicked,this,&QMainWindow::close);
    connect(swap,&QPushButton::clicked,this,&swap_func);
}


