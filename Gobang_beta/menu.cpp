#include "menu.h"
#include "ui_menu.h"
#include <QPainter>
#include <QFont>

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    iniUI();
    iniSignalSlots();

    ui->setupUi(this);
    setFixedSize((MARGIN*2 + BLOCK_SIZE * (BOARD_GRAD_SIZE-1)),MARGIN*2 + BLOCK_SIZE * (BOARD_GRAD_SIZE-1));
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,QColor("#B2C8A7"));
    setPalette(palette);
    this->setWindowTitle(QString("Gobang V1.2 beta"));
    this->setWindowIcon(QIcon(".\\source\\inc.png"));
}

menu::~menu()
{
    delete ui;
}

void menu::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QFont font1("Times New Roman",28,QFont::Bold);

    painter.setFont(font1);
    painter.setRenderHint(QPainter::Antialiasing,true);

    painter.drawText(0,offset,this->width(),this->height()/6+offset,Qt::AlignCenter,"Gobang V1.2 beta");

    QFont font2("Times New Roman",15,QFont::Bold,true);
    painter.setFont(font2);

    painter.drawText(0,this->height()/20+offset,this->width(),this->height()/5+offset,Qt::AlignCenter,"A PPCA Project");

    QFont font3("Times New Roman",15,QFont::Bold,false);
    painter.setFont(font3);

    painter.drawText(0,this->height()/6+offset,this->width(),this->height()/5+offset,Qt::AlignCenter,"Rule:");

    QFont font4("Times New Roman",15);
    painter.setFont(font4);
    QString text = "Freestyle is too simple, so we adopt Swap2 Rule. "
            "In Swap2 Rule, after the first player places the third stone, "
            "the second player has the chance to swap all the stones on the board.\n"
            "Good Luck! :)";
    QRect rect((this->width())/8,this->height()/5+offset,this->width() - (this->width())/4, this->height()/3+offset);
    int flags = Qt::TextWordWrap | Qt::AlignCenter;
    painter.drawText(rect, flags, text);

    painter.setFont(font3);
    painter.drawText(0,offset,this->width(),this->height()+offset,Qt::AlignCenter,"Please Choose Game Mode:");

}

void menu::iniUI(){
    chooseMode = new QComboBox(this);//this 可以把combobox嵌入当前窗口
    chooseSide = new QComboBox(this);

    QFont font1("Times New Roman",13);
    chooseMode->setFont(font1);
    chooseMode->setFixedSize(150,25);
    chooseSide->setFont(font1);
    chooseSide->setFixedSize(150,25);

    chooseMode->addItem("       Challenge AI");
    chooseMode->addItem("             PVP");
    chooseMode->show();
    chooseMode->move(this->width()/2 - 58 ,this->height()-60);

    chooseSide->addItem("        Take Black");
    chooseSide->addItem("        Take White");
    chooseSide->show();
    chooseSide->move(this->width()/2 - 58 ,this->height()-20);


    start = new QPushButton("Start",this);//this 嵌入当前窗口
    start->setFixedSize(100,45);
    QFont font("Times New Roman",18,QFont::Bold,false);
    start->setFont(font);
    start->show();//显示
    start->move(this->width()/2 - 32 ,this->height()+40);//移动到合适位置


/*
    start2 = new QPushButton("Close",this);
    start2->setFixedSize(100,50);

    start2->setFont(font);
    start2->show();
    start2->move(this->width()/2 - 24 ,this->height()+90);
*/

}

void menu::iniSignalSlots(){
    connect(start,&QPushButton::clicked,this,&start_game);
}


void menu::start_game(){
    w = new MainWindow(chooseMode->currentIndex(),chooseSide->currentIndex());
    w->show();
    //this->close();

}
