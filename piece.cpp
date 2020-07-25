#include "piece.h"

Piece::Piece()
{

}

Piece::~Piece()
{

}

QString Piece::getText()
{
    switch (this->_type) {
    case CHE:
        return "车";
    case MA:
        return "马";
    case XIANG:{
        if(_isRed)
            return "相";
        else
            return "象";
    }
    case SHI:
        return "士";
    case JIANG:{
        if(_isRed)
            return "帅";
        else
            return "将";
    }
    case PAO:
        return "炮";
    case BING:{
        if(_isRed)
            return "兵";
        else
            return "卒";
    }
    }
    return "错误";
}

//id的棋子初始化，根据是否执红方来调整红色在下还是黑色在下
void Piece::init(int id,bool playRed)
{
    _id = id;
    _isDead = false;
    _isRed = id >= 16;
    if(playRed){
        if(id < 16){
            _row = pos[id].row;
            _col = pos[id].col;
            _type = pos[id].type;
        }
        else{
            _row = 9 - pos[id - 16].row;
            _col = 8 - pos[id - 16].col;
            _type = pos[id - 16].type;
        }
    }
    else{
        if(id < 16){
            _row = 9 - pos[id].row;
            _col = 8 - pos[id].col;
            _type = pos[id].type;
        }
        else{
            _row = pos[id - 16].row;
            _col = pos[id - 16].col;
            _type = pos[id - 16].type;
        }
    }
}
