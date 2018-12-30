#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QKeyEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QtGlobal>
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QTime>
#include <QEventLoop>
#include <QTimer>
#include <QtAlgorithms>
#include <QMessageBox>


namespace Ui {
class Widget;
}

struct array{int a[4][4];};
struct tile
{
    int power=0;//存放方格是2的几次方
    int value=0;//存放方格的具体值
    QLabel *label;
    QPropertyAnimation *ani;
};

struct position
{
    int x=0;
    int y=0;
};

class Widget : public QWidget
{
    Q_OBJECT

public:

    QString tilecolor[11]={"QLabel{background-color:#eee4da;color:#4D4D4D}",
                          "QLabel{background-color:#ede0c8;color:#4D4D4D}",
                          "QLabel{background-color:#f2b179;color:#f9f6f2}",
                          "QLabel{background-color:#f59563;color:#f9f6f2}",
                          "QLabel{background-color:#f67c5f;color:#f9f6f2}",
                          "QLabel{background-color:#f65e3b;color:#f9f6f2}",
                          "QLabel{background-color:#edcf72;color:#f9f6f2}",
                          "QLabel{background-color:#edcc61;color:#f9f6f2}",
                          "QLabel{background-color:#edc850;color:#f9f6f2}",
                          "QLabel{background-color:#edc53f;color:#f9f6f2}",
                          "QLabel{background-color:#edc22e;color:#f9f6f2}"};

    explicit Widget(QWidget *parent = 0);
    ~Widget();
    tile grid[4][4],clone[4][4];
    position direct;
    void randomGrid();
    void animationAppear(int x,int y);
    void clearall();
    void Mov();
    int search();
    void tryMov();
    //int waiting;
    int checkWin();
    int checkLose();
    array trymov(array node,int i);
    int direction=-1;
    int ifchange,stop;
    int animationMov(int x,int y,position a,bool merge);
    int alphaBeta(array node,int alpha,int beta,int depth,int player);
    int valuation(array temp);
    //int ifchange(int x,int y,position a,bool merge);
protected:

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_newgame_clicked();

    void on_up_clicked();

    void on_right_clicked();

    void on_down_clicked();

    void on_left_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
