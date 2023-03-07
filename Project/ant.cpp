#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <random>
#include <math.h>
#include "ant.h"

/**
  Creates a new Point object with coordinates x and y
  @param x int x coordinate
  @param y int y coordinate
*/
Ant::Ant(QColor color, const int x, const int y, bool now, bool next) {
  this->color_ = color;
  x_ = x;
  y_ = y;
  current_alive_=now;
  next_alive_=next;
}

void Ant::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        qDebug() << "left clicked!";
        if(!current_alive_){
            qDebug()<<"it was dead but now it should be alive";
            current_alive_=true;
            color_=(QColor(0,0,255));
        }
    }
    else if(event->button()==Qt::RightButton){
        qDebug() << "right clicked!";
        if(current_alive_){
            qDebug()<<"it was alive but now it should be dead";
            current_alive_=false;
            color_=(QColor(255,255,255));
        }
    }
    update();
}

QRectF Ant::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

QPainterPath Ant::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}


void Ant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

//pre-set the condition for each ant while initializing the board
int Ant::set_condition(){
    //generate a value either 0 or 1
    int return_value=0;
    int random=rand()%2;
    if(random==0){
        //set to dead
        current_alive_=false;
        color_=QColor(255,255,255);
    }
    else{
        //set to alive
        current_alive_=true;
        color_=QColor(0,0,255);
        return_value=1;
    }
    next_alive_=false;
    stay_alive_=0;
    return return_value;
}

bool Ant::now_alive(){
    return current_alive_;
}

void Ant::set_next(bool condition){
    next_alive_=condition;
}

void Ant::update_condition(){
    //if the Ant was already alive, add to counter that counts
    if(current_alive_&&next_alive_){
        stay_alive_++;
    }
    else{
        stay_alive_=0;
    }
    current_alive_=next_alive_;
    next_alive_=false;
    //also update the color
    if(current_alive_){
        //if the ant has stayed alive for at least 3 turns, change it to orange
        if(stay_alive_>2){
            color_=QColor(255,128,0);
        }
        else{
            color_=QColor(0,0,255);
        }
    }
    else{
        color_=QColor(255,255,255);
    }
    update();
}


/**
  Makes it so the == operator will have the behavior that you
  expect when comparing points.
  You can overload pretty much any operator in c++
  @param first Point left hand side of the expression
  @param other Point right hand side of the expression
*/
bool operator==(const Ant &first, const Ant &other) {
  return first.x_ == other.x_ && first.y_ == other.y_;
}
