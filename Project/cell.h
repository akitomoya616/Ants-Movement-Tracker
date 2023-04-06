#ifndef MAP_H
#define MAP_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>


class Cell : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Cell(QColor color, const int x, const int y);  // constructor

    int get_x() const { return x_; }  // inline member function
    int get_y() const { return y_; }  // inline member function

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; };

    int set_condition();

    void set_ant_army();

    void set_food();

    void set_empty();

    int get_role();

    void set_next(int role);

    void update_condition();

    std::vector<std::string*> get_possible_direction(){return direction_;};


private:
    int x_;
    int y_;
    QColor color_;
    int current_role_; // 0 for empty, 1 for obstacle, 2 for food, 3 for ant army
    int next_role_;

    std::vector<std::string*> direction_; // set of directions that show where the ant army can go for this turn
    std::vector<int*> ant_army; // coordinates of the location of the current any army
    static const std::vector<int*> food; // coordinates of the location of the food in the map

    // all our map cell will be 20 wide and tall
    static const int width_=20;

    // if you declare the operator as a friend, then it isn't
    // part of the object
    // friend will let this method access the private fields
    friend bool operator==(const Cell &first, const Cell &other);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};  // class Point




#endif
