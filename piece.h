#ifndef PIECE_H
#define PIECE_H
#include <QString>

//棋子类
class Piece
{
public:
    Piece();
    ~Piece();

    enum TYPE{CHE,MA,XIANG,SHI,JIANG,PAO,BING};
    struct{
        int row,col;
        Piece::TYPE type;
    }pos[16] = {
    {0,0,Piece::CHE},
    {0,1,Piece::MA},
    {0,2,Piece::XIANG},
    {0,3,Piece::SHI},
    {0,4,Piece::JIANG},
    {0,5,Piece::SHI},
    {0,6,Piece::XIANG},
    {0,7,Piece::MA},
    {0,8,Piece::CHE},
    {2,1,Piece::PAO},
    {2,7,Piece::PAO},
    {3,0,Piece::BING},
    {3,2,Piece::BING},
    {3,4,Piece::BING},
    {3,6,Piece::BING},
    {3,8,Piece::BING},
    };

    QString getText();//获取棋子的名字
    void init(int id,bool playRed);//初始化棋子

    int _row;//棋子行坐标
    int _col;//棋子列坐标
    int _id;//棋子id
    bool _isDead;//是否已经死掉了
    bool _isRed;//是不是红色棋子
    TYPE _type;//棋子的类型
};

#endif // PIECE_H
