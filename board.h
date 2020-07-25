#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "piece.h"
#include "step.h"
#include <QPushButton>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    void init(bool isplayRed);
    void drawPieces(QPainter& painter,int id);//绘制棋子
    QPoint center(int row,int col);//返回棋盘上行列的坐标点
    QPoint center(int id);//返回棋子所在的位置中心点
    bool isBottomSide(int id);//判断是否在棋盘下面半区，即是否过河
    int countPiecesAtLine(int row1,int col1,int row2,int col2);//计算直线上的棋子数量
    bool getRowCol(QPoint pt,int &row,int &col);//获取某个坐标点对应棋盘上的行列值
    int idAtRowCol(int row,int col);//获取指定行列值上面的棋子id
    //各个类型棋子走棋的规则
    bool canMove(int moveID,int row,int col,int killID);

    bool canMoveJIANG(int moveID,int row,int col,int killID);
    bool canMoveSHI(int moveID,int row,int col,int killID);
    bool canMoveXIANG(int moveID,int row,int col,int killID);
    bool canMoveCHE(int moveID,int row,int col,int killID);
    bool canMoveMA(int moveID,int row,int col,int killID);
    bool canMovePAO(int moveID,int row,int col,int killID);
    bool canMoveBING(int moveID,int row,int col,int killID);

    Piece pieces[32];//32个棋子
    QVector<Step *> steps;
    int d = 40;//棋盘方格的边长
    int selectedID;//当前选中的棋子id，未选中任何棋子为-1
    bool isRedTurn;//是否轮到红方行动
    bool playRed;//是否执红方
    bool isBack;

    QPushButton *stepBack;

signals:
    void gameOver();

public slots:
    void end();
    virtual void slotStepBack();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    virtual void click(int id,int row,int col);

};

#endif // BOARD_H
