#include "graphicsscene.h"
#include <QGraphicsItem>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
    addRect(0,0,1,1,QPen(QColor("black")),QBrush(QColor("transparent")));
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    addLine(prevX,prevY,event->scenePos().x(),event->scenePos().y(),pen);
    path.lineTo(event->scenePos().x(),event->scenePos().y());
    prevX=event->scenePos().x();
    prevY=event->scenePos().y();
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    prevX=event->scenePos().x();
    prevY=event->scenePos().y();
    path=QPainterPath();
    path.moveTo(prevX,prevY);
    switch(event->button()){
        case Qt::LeftButton:
            pen=QPen(QColor("Black"));
            break;
        case Qt::RightButton:
            pen=QPen(QColor("Red"));
            break;
        default:
            break;
    }
    //addRect(event->scenePos().x(),event->scenePos().y(),1,1);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    addLine(prevX,prevY,event->scenePos().x(),event->scenePos().y(),pen);
    switch(event->button()){
        case Qt::LeftButton:

            break;
        case Qt::RightButton:
            QGraphicsItem *item;
            foreach(item,items(path)){
                if(item!=bg){
                    removeItem(item);
                    delete item;
                }
            };
            break;
        default:
            break;
    }



}
