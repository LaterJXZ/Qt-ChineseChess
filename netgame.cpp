#include "netgame.h"
#include <QMessageBox>

NetGame::NetGame(bool isServer)
{
    server = nullptr;
    socket = nullptr;
    if(isServer){
        playRed = true;
        server = new QTcpServer(this);
        server->listen(QHostAddress::Any,9999);
        connect(server,&QTcpServer::newConnection,this,&NetGame::slotNewConnection);
    }
    else{
        playRed = false;
        socket = new QTcpSocket(this);
        socket->connectToHost("localhost",9999);
        connect(socket,&QTcpSocket::readyRead,this,&NetGame::slotReceiveInfo);
    }
    init(playRed);
    disconnect(stepBack,&QPushButton::clicked,this,&Board::slotStepBack);
    connect(stepBack,&QPushButton::clicked,this,&NetGame::slotStepBack);
}

void NetGame::slotStepBack()
{
    char buff[2];
    buff[0] = 2;
    buff[1] = 0;
    socket->write(buff,2);
}

void NetGame::click(int id, int row, int col)
{
    if(selectedID == -1 && id != -1){
        if(pieces[id]._isRed != playRed)
            return;
    }
    Board::click(id,row,col);
    char buff[4];
    buff[0] = 1;
    buff[1] = 9 - row;
    buff[2] = 8 - col;
    buff[3] = id;
    socket->write(buff,4);
}

void NetGame::slotNewConnection()
{
    if(socket) return;
    socket = server->nextPendingConnection();
    connect(socket,&QTcpSocket::readyRead,this,&NetGame::slotReceiveInfo);
}

void NetGame::slotReceiveInfo()
{
    QByteArray info = socket->readAll();
    int cmd = info[0];
    if(cmd == 1){
        int row = info[1];
        int col = info[2];
        int id = info[3];
        Board::click(id,row,col);
    }
    else if(cmd == 2) {
        int op = info[1];
        if(op == 0){
            QMessageBox::StandardButtons ret;
            ret = QMessageBox::question(this,"悔棋","对方申请悔棋，是否同意?");
            if(ret == QMessageBox::Yes){
                char buff[2] = {2,1};
                socket->write(buff,2);
                Board::slotStepBack();
            }
            else{
                char buff[2] = {2,-1};
                socket->write(buff,2);
            }
        }
        else if(op == 1){
            Board::slotStepBack();
        }
        else if(op == -1){
            QMessageBox::warning(this,"悔棋","对方拒绝悔棋！");
        }
    }
}
