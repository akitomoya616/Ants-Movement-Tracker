#ifndef ANT_H
#define ANT_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>


class Ant : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Ant(QColor color, const int id, int x, int y, bool now, bool next);  // constructor

    int get_x() { return x_; }  // inline member function
    int get_y() { return y_; }  // inline member function
    int get_id() const { return id_; }

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
  int id_;
  int x_;
  int y_;
  QColor color_;
  bool current_alive_;
  bool next_alive_;
  int stay_alive_=0;


  // all our cell will be 20 wide and tall
  static const int width_=20;

  // if you declare the operator as a friend, then it isn't
  // part of the object
  // friend will let this method access the private fields
  friend bool operator==(const Ant &first, const Ant &other);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};  // class Point


#endif // ANT_H
