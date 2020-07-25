#ifndef SCENE_H
#define SCENE_H
#include <QWidget>

class Scene:public QWidget
{
    Q_OBJECT
public:
    Scene(bool isLocal,bool isServer);
};

#endif // SCENE_H
