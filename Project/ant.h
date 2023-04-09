#ifndef ANT_H
#define ANT_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>


class Ant : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Ant(QColor color, const int x, const int y, bool now, bool next);  // constructor

    int get_x() const { return x_; }  // inline member function
    int get_y() const { return y_; }  // inline member function

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; };

    int set_condition();

    int now_decision();

    void set_next(int condition); // 0 for left, 1 for right, 2 for no decision

    void update_condition();

    void set_no_decision();

    int get_right(){return stay_right_;};


private:
  int x_;
  int y_;
  QColor color_;
  int current_decision_;
  int next_decision_;
  int stay_right_=0;


  // all our ant will be 20 wide and tall
  static const int width_=20;

  // if you declare the operator as a friend, then it isn't
  // part of the object
  // friend will let this method access the private fields
  friend bool operator==(const Ant &first, const Ant &other);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};  // class Point




#endif
