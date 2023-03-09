#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "ant.h"
#include "bar.h"
#include <vector>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print_board();
    void play_once();
    int check_neighbor(int i, int j, int x_max, int y_max);
    void update_board();


private:
    Ui::MainWindow *ui;

    QGraphicsScene *antDecisionScene;

    QGraphicsScene *staticScene;

    QGraphicsScene *mapScene;

    std::vector<std::vector<Ant*>> ant_army_decision_board;

    std::vector<Bar*> bar_board;

    int rows_=10;

    int columns_=20;

    int turn_count=0;

    int bar_count=0;

    int population_;

    int total_;

    bool pause_=true;

    QTimer *timer_;

    bool reset_=false;

    double ms_=500.0;

private slots:
    void on_StepButton_pressed();
    void on_PlayButton_pressed();
    void OnTimerFired();
    void on_PauseButton_pressed();
    void on_horizontalSlider_sliderPressed();
    void on_ResetButton_pressed();
    void on_horizontalSlider_sliderReleased();
};
#endif // MAINWINDOW_H
