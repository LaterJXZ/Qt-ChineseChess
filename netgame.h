#ifndef NETGAME_H
#define NETGAME_H
#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>

class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool isServer);
    void slotStepBack();

    void click(int id, int row, int col);

    QTcpServer *server;
    QTcpSocket *socket;

public slots:
    void slotNewConnection();
    void slotReceiveInfo();
};

#endif // NETGAME_H
