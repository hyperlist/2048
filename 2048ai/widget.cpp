#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    //waiting=0;
    this->grabKeyboard();
    ui->setupUi(this);

}

Widget::~Widget()
{
    delete ui;
}
/******初始化，重置所有方块*****/
void Widget::clearall(){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++){
            if(grid[i][j].value!=0) {
                delete grid[i][j].ani;
                delete grid[i][j].label;
                grid[i][j].power=0;
                grid[i][j].value=0;
            }
        }
}
/*****按下NEWGAME,游戏重新开始，随机产生两个方块*****/
void Widget::on_newgame_clicked()
{
    clearall();

    randomGrid();
    randomGrid();//初始化两个数字格


}
/******在没有方块的位置随机产生方块******/
void Widget::randomGrid(){

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int x=qrand()%4;
    int y=qrand()%4;
    while(grid[x][y].value!=0) {//产生的方块与已经存在的方块不能重叠
        x=qrand()%4;
        y=qrand()%4;
    }
    int k=qrand()%100;
    grid[x][y].power=k>10?1:2;
    grid[x][y].value=k>10?2:4;
    animationAppear(x,y);//产生动画播放
}
/*******播放动画******/
void Widget::animationAppear(int x,int y){

    //Tile *label1 = new Tile(this);

    QLabel *label1=new QLabel(this);
    QFont ft;
    ft.setFamily("Consolas");
    ft.setBold(true);
    ft.setPointSize(30);
    label1->setFont(ft);
    label1->setStyleSheet(tilecolor[grid[x][y].power-1]);
    label1->setText(QString::number(grid[x][y].value));
    label1->setAlignment(Qt::AlignCenter);
    label1->raise();
    label1->show();
    grid[x][y].label=label1;

    //delete grid[x][y].label;

    QPropertyAnimation *q1 = new QPropertyAnimation(grid[x][y].label, "geometry");
    q1->setDuration(50);
    q1->setStartValue(QRect(25+50+x*105, 145+50+y*105, 0, 0));
    q1->setEndValue(QRect(25+x*105, 145+y*105, 100, 100));
    grid[x][y].ani=q1;
    q1->start();
    //delete label1;
}

/*****移动动画*****/
int Widget::animationMov(int x,int y,position a,bool merge){

    //Tile *label1 = new Tile(this);

/******两个相同数字方块相遇，消除其中的一个******/
    if(merge) {
        delete grid[x][y].ani;
        QPropertyAnimation *q1 = new QPropertyAnimation(grid[a.x][a.y].label, "geometry");
        q1->setDuration(50*qAbs(a.x-x+a.y-y));
        q1->setStartValue(QRect(25+x*105, 145+y*105, 100, 100));
        q1->setEndValue(QRect(25+a.x*105, 145+a.y*105, 100, 100));
        grid[a.x][a.y].ani=q1;
        grid[a.x][a.y].value<<=1;
        grid[a.x][a.y].power++;
        //qDebug()<<grid[a.x][a.y].value;
        q1->start();

        //QEventLoop loop;
        //QTimer::singleShot(50*qAbs(a.x-x+a.y-y), &loop, SLOT(quit()));
        //loop.exec();

        delete grid[x][y].label;
        //qDebug()<<grid[a.x][a.y].power;
        grid[a.x][a.y].label->setStyleSheet(tilecolor[grid[a.x][a.y].power-1]);
        grid[a.x][a.y].label->setText(QString::number(grid[a.x][a.y].value));
        grid[a.x][a.y].label->show();
        grid[x][y].value=0;
        grid[x][y].power=0;
        return 1;

    }
/*******方块移动到所能移动的最远处*******/
    else if(!(a.x==x&&a.y==y)){
        delete grid[x][y].ani;
        QPropertyAnimation *q1 = new QPropertyAnimation(grid[x][y].label, "geometry");
        q1->setDuration(50*qAbs(a.x-x+a.y-y));
        q1->setStartValue(QRect(25+x*105, 145+y*105, 100, 100));
        q1->setEndValue(QRect(25+a.x*105, 145+a.y*105, 100, 100));
        grid[a.x][a.y].ani=q1;
        grid[a.x][a.y].value=grid[x][y].value;
        grid[a.x][a.y].power=grid[x][y].power;
        //qDebug()<<"zhiwei"<<grid[a.x][a.y].value

        grid[a.x][a.y].label=grid[x][y].label;
        grid[x][y].value=0;
        grid[x][y].power=0;

        q1->start();
        return 1;
    }
    return 0;
    //delete label1;
}

/*****方块是否移动判断函数*****/
void Widget::Mov(){
    //while(waiting!=0);
    int change;
    position next;
    int x1,y1;
    x1=(direct.x==0?-1:direct.x);
    y1=(direct.y==0?-1:direct.y);
    int init_x=(direct.x==1?3:0);
    int init_y=(direct.y==1?3:0);
    //qDebug()<<x1<<y1;
    for(int i=init_x;i<=3&&i>=0;i-=x1){
        for(int j=init_y;j<=3&&j>=0;j-=y1){
            //qDebug()<<i<<j;
            if(grid[i][j].value!=0){
                next.x=i;
                next.y=j;
                bool merge=0;
                //qDebug()<<next.x<<next.y;
                while((next.x+direct.x)>=0&&(next.x+direct.x)<=3&&(next.y+direct.y)>=0&&(next.y+direct.y)<=3){
                    if(grid[next.x+direct.x][next.y+direct.y].value==0){//方块移动到此方向最远距离
                        next.x=next.x+direct.x;
                        next.y=next.y+direct.y;
                        //qDebug()<<i<<j;
                     }
                    else if(grid[next.x+direct.x][next.y+direct.y].value==grid[i][j].value){//两相同方块相遇
                        next.x=next.x+direct.x;
                        next.y=next.y+direct.y;
                        //qDebug()<<"chongdie";
                        merge=1;//两个相同的方块相遇了
                        break;
                    }
                    else break;

                }
                if(animationMov(i,j,next,merge)==1) change=1;//判断方块是否有移动或消除
            }
        }
    }

    if(change==1) {//如果方块有移动或消除，产生新方块
        QEventLoop loop;
        //waiting=1;
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();//延时，等待方块移动结束后产生新方块
        if(checkWin()==1) return;//检查是否胜利
        //waiting=0;
        randomGrid();//产生新方块
        if(checkLose()==1) return;//检查是否失败
    }
}
/*****上下左右四个方向键按钮控制*****/
void Widget::on_up_clicked()
{

    direct.x=0;
    direct.y=-1;
    Mov();

}
void Widget::on_right_clicked()
{
    direct.x=1;
    direct.y=0;
    Mov();
}

void Widget::on_down_clicked()
{
    direct.x=0;
    direct.y=1;
    Mov();
}

void Widget::on_left_clicked()
{
    direct.x=-1;
    direct.y=0;
    Mov();
}
/*****键盘上下左右键控制*****/
void Widget::keyPressEvent(QKeyEvent *event)
{
    //this->grabKeyboard();
    switch (event->key()) {
        case Qt::Key_Left:
            on_left_clicked();
        break;
        case Qt::Key_Right:
            on_right_clicked();
        break;
        case Qt::Key_Down:
            on_down_clicked();
        break;
        case Qt::Key_Up:
            on_up_clicked();
        break;
        default:
        break;
    }
}
/*****检测是否赢了*****/
int Widget::checkWin()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (grid[i][j].value== 2048){
                int r = QMessageBox::information(this, tr("win"), tr("you get 2048!!!"),QObject::tr(("重新开始")) , QObject::tr(("取消")));
                        if (r == QObject::tr(("重新开始")).toInt()) {
                            on_newgame_clicked();
                            //this->close();
                        }
                        else if (r == QObject::tr(("取消")).toInt()) {
                            //on_newgame_clicked();
                            this->close();
                        }
                 return 1;
            }
    return 0;
}

int Widget::checkLose(){
    array temp;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp.a[i][j]=grid[i][j].value;//复制一份局面
    ifchange=0;
    for(int i=0;i<4;i++){
        trymov(temp,i);
    }
    if(ifchange==0) {
        int r = QMessageBox::information(this, tr("lose"), tr("you lose!!!"),QObject::tr(("重新开始")) , QObject::tr(("取消")));
                if (r == QObject::tr(("重新开始")).toInt()) {
                    on_newgame_clicked();
                    //this->close();
                }
                else if (r == QObject::tr(("取消")).toInt()) {
                    //on_newgame_clicked();
                    this->close();
                }
         return 1;
    }
    return 0;
}
/*******以下是实现的AI自动玩2048的代码*******/
void Widget::on_pushButton_clicked()
{
    stop=0;
    while(!stop){
        QEventLoop loop;
        //waiting=1;
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();//延时，等待方块移动结束后产生新方块
        int i=search();
        switch (i) {
        case 0:
            on_up_clicked();
            break;
        case 1:
            on_down_clicked();
            break;
        case 2:
            on_left_clicked();
            break;
        case 3:
            on_right_clicked();
            break;
        }
        //qDebug()<<i;
    }
}
int Widget::search(){
    array temp;
    direction=-1;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            temp.a[i][j]=grid[i][j].power;//复制一份局面

    alphaBeta(temp,-10000,10000,5,0);
    return direction;
}


array Widget::trymov(array node,int i){
    //int change=0;
    position next;
    position temp;
    int init_x,init_y,x1,y1;
    switch (i) {
    case 0:
        init_x=0;init_y=0;x1=-1;y1=-1;temp.y=-1;
        break;
    case 1:
        init_x=0;init_y=3;x1=-1;y1=1;temp.y=1;
        break;
    case 2:
        init_x=0;init_y=0;x1=-1;y1=-1;temp.x=-1;
        break;
    case 3:
        init_x=3;init_y=0;x1=1;y1=-1;temp.x=1;
        break;
    }

    for(int i=init_x;i<=3&&i>=0;i-=x1){
        for(int j=init_y;j<=3&&j>=0;j-=y1){
            //qDebug()<<i<<j;
            if(node.a[i][j]!=0){
                next.x=i;
                next.y=j;
                bool merge=0;
                //qDebug()<<next.x<<next.y;
                while((next.x+temp.x)>=0&&(next.x+temp.x)<=3&&(next.y+temp.y)>=0&&(next.y+temp.y)<=3){
                    if(node.a[next.x+temp.x][next.y+temp.y]==0){//方块移动到此方向最远距离
                        next.x=next.x+temp.x;
                        next.y=next.y+temp.y;
                        //qDebug()<<i<<j;
                     }
                    else if(node.a[next.x+temp.x][next.y+temp.y]==node.a[i][j]){//两相同方块相遇
                        next.x=next.x+temp.x;
                        next.y=next.y+temp.y;
                        //qDebug()<<"chongdie";
                        ifchange=1;//两个相同的方块相遇了
                        merge=1;
                        break;
                    }
                    else break;

                }
            /*******方块合并*******/
                if(merge) {
                    node.a[next.x][next.y]+=1;
                    node.a[i][j]=0;

                }
            /*******方块移动到所能移动的最远处*******/
                else if(next.x!=i||next.y!=j){
                    ifchange=1;
                    node.a[next.x][next.y]=node.a[i][j];
                    node.a[i][j]=0;
                }

            }
        }
    }
    return node;
}

int Widget::alphaBeta(array node,int alpha,int beta,int depth,int player){

    if (depth == 0)
        return valuation(node);//调用估值函数
    else if (player == 0){//己方
            for(int i=0;i<4;i++){//搜索四个方向
                ifchange=0;
                array child=trymov(node,i);
                if(direction==-1&&ifchange!=0&&depth==5) direction=i;
                if(ifchange==0) continue;
                int value=alphaBeta(child,alpha,beta,depth-1,1);
                //qDebug()<<value;
                //if(t==0) value=-10000;
                if(value>alpha) {alpha=value;if(depth==5)direction=i;}
                if(alpha>=beta) break;
            }
            return alpha;
    }
        else{
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++){
                    array child=node;
                    //qDebug()<<&child<<"  "<<&node;
                    if(node.a[i][j]>0) continue;
                    else child.a[i][j]=1;
                    int value =alphaBeta(child,alpha,beta,depth-1,0);
                    //qDebug()<<value;
                    if(value<beta) beta=value;
                    if(alpha>=beta) break;}
                return beta;
     }
}


int Widget::valuation(array temp){

    int monotonicity;//单调性

    int totals[4] = {0, 0, 0, 0};

    //垂直方向
    for (int x=0; x<4; x++) {
      int current = 0;
      int next = current+1;
      while ( next<4 ) {
        while ( next<4 && temp.a[x][next]==0) {
          next++;
        }
        if (next>=4) { next--; }
        int currentpower = temp.a[x][current];
        int nextpower = temp.a[x][next];
        if (currentpower > nextpower) {
          totals[0] += nextpower - currentpower;
        } else if (nextpower > currentpower) {
          totals[1] += currentpower - nextpower;
        }
        current = next;
        next++;
      }
    }

    //水平方向
    for (int y=0; y<4; y++) {
      int current = 0;
      int next = current+1;
      while ( next<4 ) {
        while ( next<4 && temp.a[next][y]==0) {
          next++;
        }
        if (next>=4) { next--; }
        int currentpower = temp.a[current][y];
        int nextpower = temp.a[next][y];
        if (currentpower > nextpower) {
          totals[2] += nextpower - currentpower;
        } else if (nextpower > currentpower) {
          totals[3] += currentpower - nextpower;
        }
        current = next;
        next++;
      }
    }

    monotonicity= qMax(totals[0], totals[1]) + qMax(totals[2], totals[3]);

    int smoothness=0;//平滑性

    int emptygrid=0;//空格数

    int maxvalue=0;//最大值

    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
          if ( temp.a[i][j]==0) emptygrid++;
          else if ( temp.a[i][j]>0) {
            if(temp.a[i][j]>maxvalue) maxvalue=temp.a[i][j];
            int this_power=temp.a[i][j];
            //水平方向
            int target =i==3?i:i+1;
            while(temp.a[target][j]==0&&target<3) target++;
            //qDebug()<<target;
            smoothness -= qAbs(this_power - temp.a[target][j]);
            //垂直方向
            target =j==3?i:j+1;
            while(temp.a[i][target]==0&&target<3) target++;
            smoothness -= qAbs(this_power - temp.a[i][target]);


          }
        }
      }


    //qDebug()<<monotonicity<<smoothness<<maxvalue<<emptygrid;
    return monotonicity*1+smoothness*0.1+emptygrid+maxvalue*1;
}
void Widget::on_pushButton_2_clicked()
{
    stop=1;
}
