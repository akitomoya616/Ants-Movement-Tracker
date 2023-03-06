#ifndef BAR_H
#define BAR_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>


class Bar : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Bar(QColor color, const int x, const int y, int height);  // constructor

    int get_x() const { return x_; }  // inline member function

    int get_y() const { return y_; }  // inline member function

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; };

    void set_height(int height) { height_ = height; };

    int get_height() { return height_; };

    void update_condition(int x, int y, int height);


private:
  int x_;
  int y_;
  QColor color_;


  // all our Points will be 20 wide and tall
  static const int width_=20;
  int height_;

};  // class Point
#endif
