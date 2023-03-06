#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <random>
#include <math.h>
#include "bar.h"

/**
  Creates a new Point object with coordinates x and y
  @param x int x coordinate
  @param y int y coordinate
*/
Bar::Bar(QColor color, const int x, const int y, int height) {
  this->color_ = color;
  x_ = x;
  y_=y;
  height_=height;
}

QRectF Bar::boundingRect() const
{
    return QRectF(x_, y_, width_, height_);
}

QPainterPath Bar::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, height_);
    return path;
}


void Bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    //shape the bar with given x, y, width and height values
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->height_));
    painter->setBrush(b);
}

void Bar::update_condition(int x, int y, int height){
    //update with the given values for x, y coordinates and the height of the bar
    x_=x;
    y_=y;
    height_=height;
    update();
}





