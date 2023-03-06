#ifndef CELL_H
#define CELL_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>


class Cell : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Cell(QColor color, const int x, const int y, bool now, bool next);  // constructor

    int get_x() const { return x_; }  // inline member function
    int get_y() const { return y_; }  // inline member function

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; };

    int set_condition();

    bool now_alive();

    void set_next(bool condition);

    void update_condition();

    int get_alive(){return stay_alive_;};


private:
  int x_;
  int y_;
  QColor color_;
  bool current_alive_;
  bool next_alive_;
  int stay_alive_=0;


  // all our Cells will be 20 wide and tall
  static const int width_=20;

  // if you declare the operator as a friend, then it isn't
  // part of the object
  // friend will let this method access the private fields
  friend bool operator==(const Cell &first, const Cell &other);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};  // class Point




#endif
