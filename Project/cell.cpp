#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <random>
#include <math.h>
#include "cell.h"

/**
  Creates a new Point object with coordinates x and y
  @param x int x coordinate
  @param y int y coordinate
*/
Cell::Cell(QColor color, const int x, const int y) {
  this->color_ = color;
  x_ = x;
  y_ = y;
}

//void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    if(event->button()==Qt::LeftButton){
//        qDebug() << "left clicked!";
//        if(!current_alive_){
//            qDebug()<<"it was dead but now it should be alive";
//            current_alive_=true;
//            color_=(QColor(0,0,255));
//        }
//    }
//    else if(event->button()==Qt::RightButton){
//        qDebug() << "right clicked!";
//        if(current_alive_){
//            qDebug()<<"it was alive but now it should be dead";
//            current_alive_=false;
//            color_=(QColor(255,255,255));
//        }
//    }
//    update();
//}

QRectF Cell::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

QPainterPath Cell::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}


void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

//pre-set the condition for each ant while initializing the board
int Cell::set_condition(){
    //generate a value either 0, 1, 2, 3, ..., or 9
    int return_value=0;
    int random=rand()%10;
    if(random==0){ // 10% chance of being as an obstacle
        //set to obstacle
        current_role_=1;
        color_=QColor(0,0,0);
        return_value=1;
    }
    else{
        //set to empty
        current_role_=0;
        color_=QColor(255,255,255);
    }
    next_role_=0;
    return return_value;
}

void Cell::set_ant_army(){
    current_role_ = 3;
    color_=QColor(255,0,0);
}

void Cell::set_food(){
    current_role_ = 2;
    color_=QColor(0,255,0);
}

int Cell::get_role(){
    return current_role_;
}

void Cell::set_next(int role){
    next_role_=role;
}

void Cell::update_condition(){
    current_role_=next_role_;
    //also update the color based on the updated role
    if(current_role_ == 0){
        color_=QColor(255,255,255); // white for empty
    }
    else if(current_role_ == 1){
        color_=QColor(0,0,0); // black for obstacle
    }
    else if(current_role_ == 2){
        color_=QColor(0,255,0); // green for food
    }
    else{
        color_=QColor(255,0,0); // red for ant army
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
bool operator==(const Cell &first, const Cell &other) {
  return first.x_ == other.x_ && first.y_ == other.y_;
}
