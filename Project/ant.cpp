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
  current_decision_=now;
  next_decision_=next;
}

void Ant::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        qDebug() << "left clicked!";
        if(current_decision_ == 0){
            qDebug()<<"it was left but now it should be right";
            current_decision_=1;
            color_=(QColor(0,0,255));
        }
    }
    else if(event->button()==Qt::RightButton){
        qDebug() << "right clicked!";
        if(current_decision_ == 1){
            qDebug()<<"it was right but now it should be left";
            current_decision_=0;
            color_=(QColor(255,165,0));
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
        //set to left
        current_decision_=0;
        color_=QColor(255,165,0);
    }
    else{
        //set to right
        current_decision_=1;
        color_=QColor(0,0,255);
        return_value=1;
    }
    next_decision_=0;
    stay_right_=0;
    return return_value;
}

int Ant::now_decision(){
    return current_decision_;
}

void Ant::set_next(int condition){
    next_decision_=condition;
}

void Ant::update_condition(){
    //if the ant was already alive, add to counter that counts
    if(current_decision_&&next_decision_){
        stay_right_++;
    }
    else{
        stay_right_=0;
    }
    current_decision_=next_decision_;
    next_decision_=0;
    //also update the color
    if(current_decision_ == 1){
        //if the ant has stayed right for at least 3 turns, change it to orange
        if(stay_right_>2){
            color_=QColor(255,128,0);
        }
        else{
            color_=QColor(0,0,255);
        }
    }
    else if(current_decision_ == 0){ //update to white if choose to go left
        color_=QColor(255,165,0);
    }
    else{
        color_=QColor(0,255,0); //update to green if no decision needed and they are just moving forward
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
