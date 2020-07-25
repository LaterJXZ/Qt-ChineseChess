#include "scene.h"
#include "board.h"
#include "netgame.h"
#include <QHBoxLayout>

Scene::Scene(bool isLocal, bool isServer)
{
    setFixedSize(600,500);
    if(isLocal){
        Board *board = new Board();
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(board);
    }
    else{
        NetGame *netGame = new NetGame(isServer);
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->addWidget(netGame);
    }
}
