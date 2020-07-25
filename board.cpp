#include "board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

Board::Board(QWidget *parent) : QWidget(parent)
{
    stepBack = new QPushButton(this);
    stepBack->setGeometry(450,100,100,40);
    stepBack->setText("悔棋");
    playRed = true;
    init(playRed);
    connect(this,&Board::gameOver,this,&Board::end);
    connect(stepBack,&QPushButton::clicked,this,&Board::slotStepBack);
}

void Board::init(bool isplayRed)
{
    selectedID = -1;
    isRedTurn = true;
    isBack = false;
    for(int i = 0; i < 32; i++){
        pieces[i].init(i,isplayRed);
    }
}

void Board::drawPieces(QPainter &painter, int id)
{
    if(pieces[id]._isDead)
        return;
    QPoint c = center(id);
    QRect rect = QRect(c.x() - d/2,c.y() - d/2,d,d);
    if(id == selectedID)
        painter.setBrush(Qt::gray);
    else
        painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(Qt::black);
    painter.drawEllipse(c,d/2,d/2);
    if(pieces[id]._isRed)
        painter.setPen(Qt::red);
    painter.setFont(QFont("宋体",d / 2,75));
    painter.drawText(rect,pieces[id].getText(),QTextOption(Qt::AlignCenter));
}

QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.setX((col + 1) * d);
    ret.setY((row + 1) * d);
    return ret;
}

QPoint Board::center(int id)
{
    return center(pieces[id]._row,pieces[id]._col);
}

bool Board::isBottomSide(int id)
{
    if(playRed)
        return id > 15;
    else
        return id < 16;
}

int Board::countPiecesAtLine(int row1, int col1, int row2, int col2)
{
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;
    int ret = 0;
    if(row1 == row2){
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int c = min + 1; c < max; ++c){
            if(idAtRowCol(row1,c) != -1)
                ++ret;
        }
    }
    else{
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int r = min + 1; r < max; ++r){
            if(idAtRowCol(r,col1) != -1)
                ++ret;
        }
    }
    return ret;
}

bool Board::getRowCol(QPoint pt, int &row, int &col)
{
    row = qRound(static_cast<qreal>(pt.ry()) / static_cast<qreal>(d)) - 1;
    col = qRound(static_cast<qreal>(pt.rx()) / static_cast<qreal>(d)) - 1;
    if(row < 0 || row > 9 || col < 0 || col > 8)
        return false;
    return true;
}

int Board::idAtRowCol(int row, int col)
{
    for(int i = 0 ;i < 32; ++i){
        if(!pieces[i]._isDead && pieces[i]._row == row && pieces[i]._col == col){
            return i;
        }
    }
    return -1;
}

bool Board::canMove(int moveID, int row, int col, int killID)
{
    if(killID != -1){
        if(pieces[moveID]._isRed == pieces[killID]._isRed){
            selectedID = killID;
            update();
            return false;
        }
    }
    switch (pieces[moveID]._type) {
    case Piece::JIANG:
        return canMoveJIANG(moveID,row,col,killID);
        break;
    case Piece::SHI:
        return canMoveSHI(moveID,row,col,killID);
        break;
    case Piece::XIANG:
        return canMoveXIANG(moveID,row,col,killID);
        break;
    case Piece::CHE:
        return canMoveCHE(moveID,row,col,killID);
        break;
    case Piece::MA:
        return canMoveMA(moveID,row,col,killID);
        break;
    case Piece::PAO:
        return canMovePAO(moveID,row,col,killID);
        break;
    case Piece::BING:
        return canMoveBING(moveID,row,col,killID);
        break;
    }
    return true;
}

bool Board::canMoveJIANG(int moveID, int row, int col, int killID)
{
    if((moveID + killID == 24) && (countPiecesAtLine(pieces[moveID]._row,pieces[moveID]._col,row,col) == 0))
        return true;
    if(col < 3 || col > 5)
        return false;
    if(pieces[moveID]._isRed){
        if(row < 7) return false;
    }
    else{
        if(row > 2) return false;
    }
    int dr = pieces[moveID]._row - row;
    int dc = pieces[moveID]._col - col;
    int diff = abs(dr) * 10 + abs(dc);
    if(diff == 1 || diff == 10)
        return true;
    return false;
}

bool Board::canMoveSHI(int moveID, int row, int col, int )
{
    if(col < 3 || col > 5)
        return false;
    if(pieces[moveID]._isRed){
        if(row < 7) return false;
    }
    else{
        if(row > 2) return false;
    }
    int dr = pieces[moveID]._row - row;
    int dc = pieces[moveID]._col - col;
    int diff = abs(dr) * 10 + abs(dc);
    if(diff == 11)
        return true;
    return false;
}

bool Board::canMoveXIANG(int moveID, int row, int col, int)
{
    if(isBottomSide(moveID)){
        if(row <= 4)
            return false;
    }
    else{
        if(row >= 5)
            return false;
    }
    int dr = pieces[moveID]._row - row;
    int dc = pieces[moveID]._col - col;
    int diff = abs(dr) * 10 + abs(dc);
    if(diff != 22)
        return false;
    int rEye = (row + pieces[moveID]._row) / 2;
    int cEye = (col + pieces[moveID]._col) / 2;
    if(idAtRowCol(rEye,cEye) != -1)
        return false;
    return true;
}

bool Board::canMoveCHE(int moveID, int row, int col, int)
{
    int ret = countPiecesAtLine(pieces[moveID]._row,pieces[moveID]._col,row,col);
    if(ret == 0)
        return true;
    return false;
}

bool Board::canMoveMA(int moveID, int row, int col, int)
{
    int dr = pieces[moveID]._row - row;
    int dc = pieces[moveID]._col - col;
    int diff = abs(dr) * 10 + abs(dc);
    if(diff != 12 && diff != 21)
        return false;
    if(diff == 12){
        if(idAtRowCol(pieces[moveID]._row,(pieces[moveID]._col + col) / 2) != -1)
            return false;
    }
    else{
        if(idAtRowCol((pieces[moveID]._row + row) / 2,pieces[moveID]._col) != -1)
            return false;
    }
    return true;
}

bool Board::canMovePAO(int moveID, int row, int col, int killID)
{
    int ret = countPiecesAtLine(pieces[moveID]._row,pieces[moveID]._col,row,col);
    if(killID != -1){
        if(ret == 1)
            return true;
    }
    else{
        if(ret == 0)
            return true;
    }
    return false;
}

bool Board::canMoveBING(int moveID, int row, int col, int)
{
    int dr = pieces[moveID]._row - row;
    int dc = pieces[moveID]._col - col;
    int diff = abs(dr) * 10 + abs(dc);
    if(diff != 1 && diff != 10)
        return false;
    if(isBottomSide(moveID)){
        if(row > pieces[moveID]._row)
            return false;
        if(pieces[moveID]._row >= 5 && pieces[moveID]._row == row)
            return false;
    }
    else{
        if(row < pieces[moveID]._row)
            return false;
        if(pieces[moveID]._row <= 4 && pieces[moveID]._row == row)
            return false;
    }
    return true;
}

void Board::end()
{
    init(playRed);
}

void Board::slotStepBack()
{
    if(steps.isEmpty()){
        return;
    }
    Step * s = steps.back();
    steps.removeLast();
    if(s->targetID != -1){
        pieces[s->targetID]._isDead = false;
    }
    pieces[s->startID]._row = s->fromRow;
    pieces[s->startID]._col = s->fromCol;
    isRedTurn = !isRedTurn;
    update();
    delete s;
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    for(int i = 1; i <= 10; i++){
        painter.drawLine(QPoint(d,i * d),QPoint(9 * d, i * d));
    }
    for(int i = 1; i <= 9; i++){
        if(i == 1 || i == 9)
            painter.drawLine(QPoint(i * d,d),QPoint(i * d,10 * d));
        else{
            painter.drawLine(QPoint(i * d,d),QPoint(i * d,5 * d));
            painter.drawLine(QPoint(i * d,6 * d),QPoint(i * d,10 * d));
        }
    }
    painter.drawLine(QPoint(4 * d,d),QPoint(6 * d,3 * d));
    painter.drawLine(QPoint(4 * d,3 * d),QPoint(6 * d,d));
    painter.drawLine(QPoint(4 * d,8 * d),QPoint(6 * d,10 * d));
    painter.drawLine(QPoint(4 * d,10 * d),QPoint(6 * d,8 * d));
    for(int i = 0; i < 32; i++){
        drawPieces(painter,i);
    }
}

void Board::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint pt = event->pos();
    int row,col;
    if(!getRowCol(pt,row,col))
        return;
    int i;
    int clickID = -1;
    for(i = 0; i < 32; ++i){
        if(pieces[i]._row == row && pieces[i]._col == col && !pieces[i]._isDead){
            clickID = i;
            break;
        }
    }
    click(clickID,row,col);
}

void Board::click(int id, int row, int col)
{
    if(selectedID == -1){
        if(id != -1){
            if(isRedTurn == pieces[id]._isRed){
                selectedID = id;
                update();
            }
        }
    }
    else{
        if(canMove(selectedID,row,col,id)){         
            Step * s = new Step;
            s->startID = selectedID;
            s->targetID = id;
            s->fromRow = pieces[selectedID]._row;
            s->fromCol = pieces[selectedID]._col;
            s->toRow = row;
            s->toCol = col;
            steps.push_back(s);
            pieces[selectedID]._row = row;
            pieces[selectedID]._col = col;
            if(id != -1){
                pieces[id]._isDead = true;
                if(id == 4){
                    QMessageBox::information(this,"游戏结束","红方获胜!");
                    emit gameOver();
                    return;
                }
                if(id == 20){
                    QMessageBox::information(this,"游戏结束","黑方获胜!");
                    emit gameOver();
                    return;
                }
            }
            selectedID = -1;
            isRedTurn = !isRedTurn;
            update();
        }
    }
}
