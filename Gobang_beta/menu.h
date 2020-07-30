#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include "gamemodel.h"
#include "mainwindow.h"

const int offset = 30;

namespace Ui {
class menu;
}

class menu : public QWidget
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();

private:
    Ui::menu *ui;

    QComboBox *chooseMode;
    QComboBox *chooseSide;

    QPushButton *start;

    MainWindow *w;

    void iniUI();
    void iniSignalSlots();
    void start_game();


    void paintEvent(QPaintEvent *event);
};

#endif // MENU_H
