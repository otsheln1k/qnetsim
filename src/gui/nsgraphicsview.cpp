#include "nsgraphicsview.h"

NSGraphicsView::NSGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    this->connection[0] = this->connection[1] = nullptr;
    this->mode = NSGraphicsViewMode::NONE;
    this->scene = new QGraphicsScene();
    this->setScene(this->scene);
    qDebug() << this->scene->width()<<" "<<this->scene->height();
}

NSGraphicsView::~NSGraphicsView()
{
    delete this->scene;
}

void NSGraphicsView::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Clicked";
    if (ev->button() == Qt::MouseButton::LeftButton){
        qDebug() << "Left button clicked!";

        switch(this->mode){
        case NSGraphicsViewMode::ADD_NODE:
            if (this->node == NSGraphicsViewNode::PC){
                this->scene->addItem(new class PC(this, ev->position()));
                this->scene->update(0,0,this->width(),this->height());
            }
            this->mode = NSGraphicsViewMode::NONE;
            break;

        case NSGraphicsViewMode::ADD_CONNECTION:
            qDebug() << "Try to select device to connect";
            if (this->connection[0] == nullptr){
                this->connection[0] = dynamic_cast<Node*>(this->scene->itemAt(ev->position(), QGraphicsView::transform()));
            } else {
                this->connection[1] = dynamic_cast<Node*>(this->scene->itemAt(ev->position(), QGraphicsView::transform()));
                if (this->connection[1] != nullptr){
                    this->scene->addLine(this->connection[0]->pos().x(),this->connection[0]->pos().y(),
                            this->connection[1]->pos().x(),this->connection[1]->pos().y(), Qt::SolidLine);
                    this->scene->update(0,0,this->width(),this->height());
                }
            }
            if (this->connection[0] != nullptr){
                qDebug()<< "1st item selected";
            }
            if (this->connection[1] != nullptr){
                qDebug()<< "2nd item selected";
            }
            break;
        }
    }
}

void NSGraphicsView::setMode(NSGraphicsViewMode mode)
{
    this->mode = mode;
}

void NSGraphicsView::setNode(NSGraphicsViewNode node)
{
    this->node = node;
}
