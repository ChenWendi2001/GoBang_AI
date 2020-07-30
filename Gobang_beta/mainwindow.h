#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSound>
#include <QPixmap>
#include <Qtimer>
#include "gamemodel.h"
#include "time.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int modeIndex,int sideIndex,QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    GameModel *game;

    QPushButton * close;
    QPushButton * swap;

    QSound *sound;
    QSound *sound2;

    QPixmap black;
    QPixmap white;
    QPixmap board_pic;

    GameType game_type;

    int clickPosRow, clickPosCol;
    int last_row,last_col;

    bool selectPos = false;
    int mode,side;

    double ai_time;

    void initUI();

    void initSignalSlots();

    void paintEvent(QPaintEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void initGame(int modeIndex,int sideIndex);

    void initAIGame(int aiside);

    void initPVPGame();

    void chessOneByPerson();

    void chessOneByAI();

    void actionByplayer(int row,int col);

    void swap_func();


};

#endif // MAINWINDOW_H
