#include "mainwindow.h"
#include "ant.h"
#include "bar.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <math.h>
#include <QTableView>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // we need to set up the ui before we draw on our scene
    ui->setupUi(this);

    // the QGraphicsView is the UI element that contains the
    // scene that we will actually get to draw on.
    QGraphicsView * board = ui->boardView;

    // scene is a QGraphicsScene pointer field of the PlotWindow class
    // this makes our lives easier by letting us easily access it
    // from other functions in this class.
    scene = new QGraphicsScene;
    board->setScene(scene);
    // make the scene the same size as the view containing it
    board->setSceneRect(0,0,board->frameSize().width(),board->frameSize().height());

    //set up view and scene for the bar
    QGraphicsView * view2 = ui->statisticsView;
    scene2 = new QGraphicsScene;
    view2->setScene(scene2);
    view2->setSceneRect(0,0,view2->frameSize().width(),view2->frameSize().height());

    // we'll want to generate random numbers later so we're
    // going to seed our random number generator once
    srand(static_cast<unsigned>(QTime::currentTime().msec()));

    qDebug() << "Game starts";
    QColor color1(255,255,255);

    //creat a 2d vector of ant
    int current_alive=0;
    for(int i=0;i<rows_;i++){
        game_board.push_back(std::vector<Ant*>());
        for(int j=0;j<columns_;j++){
            Ant *p1=new Ant(color1,90+20*j,40+20*i,true,true);
            //for each ant, set it to be either dead or alive
            current_alive+=p1->set_condition();
            game_board[i].push_back(p1);
        }
    }
    //for initializing the bar_board
    for (int i=0;i<22;i++){
        Bar *p1=new Bar(QColor(255,255,255),0+i*20,ui->statisticsView->height(),0);
        bar_board.push_back(p1);
        scene2->addItem(bar_board[i]);
    }
    print_board();
    //initialize a timer, and connect the on_timer_fired slot with the timeout signal so everytime timeout emit, on_timer_fired will run once
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::OnTimerFired);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//print the current condition of the board
void MainWindow::print_board() {
    //extra work for counting ants thta has survived for more than 2 turns
    int ant_alive=0;

    population_=0;
    total_=0;
    for (int i=0;i<game_board.size();i++){
        for (int j=0;j<game_board[i].size();j++){
            //for the first turn, put ants on UI
            if(turn_count==0 && !reset_){
                scene->addItem(game_board[i][j]);
            }

            //count the total number of all ants and alived ants
            total_+=1;
            if(game_board[i][j]->now_alive()){
                population_+=1;

                //extra work for counting ants thta has survived for more than 2 turns
                if(game_board[i][j]->get_alive()>2){
                    ant_alive++;
                }
                //

            }
        }
    }
    //update turn count and population
    QString text="Turn: "+QString::number(turn_count);
    ui->TurnBar->setText(text);
    double percentage=population_*1.0/total_*100.0;
    QString text2="Population: "+QString::number(population_)+" ("+QString::number(percentage)+"%)";
    ui->PercentageBar->setText(text2);

    //extra work for counting ants thta has survived for more than 2 turns
    double percentage_alive=ant_alive*1.0/total_*100.0;
    QString text3="Alive > 2: "+QString::number(ant_alive)+" ("+QString::number(percentage_alive)+"%)";
    ui->AliveBar->setText(text3);
    //

    //now try to create the bar
    int total_height=ui->statisticsView->height();
    //the height for the bar depends on the percentage of the total height in this view
    double actual_height=total_height*percentage/100;
    //if the bar in the screen is not full, add bar to the next position
    if(bar_count<22){
        bar_board[bar_count]->update_condition(0+bar_count*20,total_height-actual_height,actual_height);
    }
    else{
        bar_count=21;
        //otherwise, move all the bar to the left by one, add create a new bar on the right side
        for(int i=1;i<22;i++){
            bar_board[i-1]->update_condition(0+(i-1)*20,bar_board[i]->get_y(),bar_board[i]->get_height());
        }
        bar_board[bar_count]->update_condition(0+bar_count*20,total_height-actual_height,actual_height);
    }
    bar_count++;
}

//the detailed process for playing each turn
void MainWindow::play_once(){
    //update turn count information
    turn_count++;
    //get the boundary of the board size
    int x_max=game_board.size()-1;
    int y_max=game_board[0].size()-1;
    //loop through the board and update their next condition based on the number of alive neighbors the Ant holds
    for (int i=0;i<=x_max;i++){
        for (int j=0;j<=y_max;j++){
            int alive=check_neighbor(i,j,x_max,y_max);
            //if the current ant is alive
            Ant *current_ant=game_board[i][j];
            if(current_ant->now_alive()){
                if(alive<2){
                    //live ant dies
                    current_ant->set_next(false);
                }
                else if(alive==2||alive==3){
                    //live ant remains alive
                    current_ant->set_next(true);
                }
                else if(alive>3){
                    //live ant dies
                    current_ant->set_next(false);
                }
            }
            //if the current ant is dead
            else{
                if(alive==3){
                    //dead ant becomes alive
                    current_ant->set_next(true);
                }
                else{
                    //dead ant is still dead
                    current_ant->set_next(false);
                }
            }
        }
    }
    //update the board and turn/popluation/alive information
    update_board();
    print_board();
}

void MainWindow::update_board(){
    for (int i=0;i<game_board.size();i++){
        for (int j=0;j<game_board[i].size();j++){
            game_board[i][j]->update_condition();
        }
    }
}

//return the value of neighbors that are still alive
int MainWindow::check_neighbor(int i, int j, int x_max, int y_max){
    //save the neighbor rows and columns the original ant is going to check
    int count=0;
    std::vector<int> rows;
    std::vector<int> columns;
    rows.push_back(i);
    columns.push_back(j);
    if(i==0){
        //upper bound
        rows.push_back(x_max);
        rows.push_back(i+1);
    }
    else if(i==x_max){
        //lower bound
        rows.push_back(i-1);
        rows.push_back(0);
    }
    else{
        //general situation
        rows.push_back(i-1);
        rows.push_back(i+1);
    }
    if(j==0){
        //left side
        columns.push_back(y_max);
        columns.push_back(j+1);
    }
    else if(j==y_max){
        //right side
        columns.push_back(j-1);
        columns.push_back(0);
    }
    else{
        //general situation
        columns.push_back(j-1);
        columns.push_back(j+1);
    }
    for(int a=0;a<3;a++){
        for(int b=0;b<3;b++){
            //if we are not at point (i,j) itself. the same thing as !(if a==i && b==j)
            if(rows[a]!=i||columns[b]!=j){
                //every member covered in this for loop is a neighbor ant of the original ant
                //count all neighbors alive
                if(game_board[rows[a]][columns[b]]->now_alive()){
                    count++;
                }
            }
        }
    }
    return count;
}

//once the step button has been pressed, run one turn
void MainWindow::on_StepButton_pressed(){
    play_once();
}

void MainWindow::OnTimerFired(){
    //when the timer signal emit, run the turn once
    play_once();
}

void MainWindow::on_PlayButton_pressed(){
    pause_=false;
    //timer returns a signal every 500ms, or to say 0.5s
    //int ms=500;
    timer_->start(ms);
}

//once the pause button been pressed, stop the timer
void MainWindow::on_PauseButton_pressed(){
    timer_->stop();
    pause_=true;
}

void MainWindow::on_ResetButton_pressed(){
    qDebug() << "Game restarts";
    //re-initialize the play board
    for (int i=0;i<game_board.size();i++){
        for (int j=0;j<game_board[i].size();j++){
            game_board[i][j]->set_condition();
            game_board[i][j]->update();
        }
    }
    //re-initialize the bar board
    for (int i=0;i<22;i++){
        bar_board[i]->update_condition(0+i*20,ui->statisticsView->height(),0);
    }
    //re-start the counters
    bar_count=0;
    turn_count=0;
    reset_=true;
    print_board();
}

//adjust the playing speed
void MainWindow::on_horizontalSlider_sliderReleased(){
    qDebug()<<"speed adjusted";
    //get the adjusted speed based on the return value of sliderPosition, it will be a value in [0,99]
    //transfer it into percentage type
    double speed_rate=ui->horizontalSlider->sliderPosition()*1.0/100;
    //and adjust ms value based on this rate
    ms=500-500*speed_rate;
    //update the ms value on screen as well
    QString text="Current speed: "+QString::number(ms)+" ms/turn";
    ui->CurrentSpeed->setText(text);
    //if the game is playing at present, re-activate the playbutton_pressed slot with the updated ms value
    if(!pause_){
        on_PlayButton_pressed();
    }
    //otherwise just keep in the current situation
}

void MainWindow::on_horizontalSlider_sliderPressed(){
    qDebug()<<"adjusting speed";
}
