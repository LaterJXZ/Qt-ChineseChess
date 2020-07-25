#include <QApplication>
#include "board.h"
#include "netgame.h"
#include <QMessageBox>
#include "scene.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    bool isLocal = false;
    bool isServer = false;
    if(QMessageBox::question(nullptr,"local or net","本地游戏请选是，网络对战请选否") == QMessageBox::Yes){
        isLocal = true;
    }
    else{
        if(QMessageBox::question(nullptr,"server or client","服务器请选是，客户端请选否") == QMessageBox::Yes){
            isServer = true;
        }
    }
    Scene w(isLocal,isServer);
    w.show();
    return app.exec();
}
