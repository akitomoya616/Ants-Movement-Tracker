#include "mainwindow.h"
#include "ant.h"
#include "bar.h"
#include "cell.h"
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
    QGraphicsView * board = ui->antMindView;

    // scene is a QGraphicsScene pointer field of the PlotWindow class
    // this makes our lives easier by letting us easily access it
    // from other functions in this class.
    antDecisionScene = new QGraphicsScene;
    board->setScene(antDecisionScene);
    // make the scene the same size as the view containing it
    board->setSceneRect(0,0,board->frameSize().width(),board->frameSize().height());

    //set up view and scene for the statistics board
    QGraphicsView * statisticsBoard = ui->statisticsView;
    staticScene = new QGraphicsScene;
    statisticsBoard->setScene(staticScene);
    statisticsBoard->setSceneRect(0,0,statisticsBoard->frameSize().width(),statisticsBoard->frameSize().height());

    // set up view and scene for the map board - STILL WIP
    QGraphicsView * mapBoard = ui->antArmyView;
    mapScene = new QGraphicsScene;
    mapBoard->setScene(mapScene);
    // make the scene the same size as the view containing it
    mapBoard->setSceneRect(0,0,mapBoard->frameSize().width(),mapBoard->frameSize().height());

    // set demo block view for user to understand
    QGraphicsView * demoView = ui->demoView;
    demoScene = new QGraphicsScene;
    demoView->setScene(demoScene);
    demoView->setSceneRect(0,0,demoView->frameSize().width(),demoView->frameSize().height());
    QColor red(255,0,0);
    QColor green(0,255,0);
    QColor blue(0,0,255);
    QColor yellow(255,255,0);
    QColor orange(255,165,0);
    QColor black(0,0,0);
    QColor white(255,255,255);

    Cell *green_icon=new Cell(green, 90, 32);
    Cell *orange_icon=new Cell(orange, 90, 62);
    Cell *blue_icon=new Cell(blue, 90, 92);

    Cell *white_icon=new Cell(white, 90, 152);
    Cell *black_icon=new Cell(black, 90, 182);
    Cell *red_icon=new Cell(red, 90, 212);
    Cell *yellow_icon=new Cell(yellow, 90, 242);

    demoScene->addItem(green_icon);
    demoScene->addItem(orange_icon);
    demoScene->addItem(blue_icon);
    demoScene->addItem(white_icon);
    demoScene->addItem(black_icon);
    demoScene->addItem(red_icon);
    demoScene->addItem(yellow_icon);

    QGraphicsTextItem *text1 = demoScene->addText("For Group Decision Table:");
    text1->setPos(0, 0);
    QGraphicsTextItem *text2 = demoScene->addText("No Decision");
    text2->setPos(0, 30);
    QGraphicsTextItem *text3 = demoScene->addText("Left");
    text3->setPos(0, 60);
    QGraphicsTextItem *text4 = demoScene->addText("Right");
    text4->setPos(0, 90);
    QGraphicsTextItem *text5 = demoScene->addText("For Ant Army Map:");
    text5->setPos(0, 120);
    QGraphicsTextItem *text6 = demoScene->addText("Empty");
    text6->setPos(0, 150);
    QGraphicsTextItem *text7 = demoScene->addText("Obstacle");
    text7->setPos(0, 180);
    QGraphicsTextItem *text8 = demoScene->addText("Ant Army");
    text8->setPos(0, 210);
    QGraphicsTextItem *text9 = demoScene->addText("Food");
    text9->setPos(0, 240);

    // assign buttons to later modification on clickable/unclickable
    step_button = ui->StepButton;
    play_button = ui->PlayButton;
    pause_button = ui->PauseButton;

    // we'll want to generate random numbers later so we're
    // going to seed our random number generator once
    srand(static_cast<unsigned>(QTime::currentTime().msec()));

    qDebug() << "Game starts";

    //creat a 2d vector of ants' decision map
    for(int i=0;i<rows_;i++){
        ant_army_decision_board.push_back(std::vector<Ant*>()); //insert a row into the board
        for(int j=0;j<columns_;j++){
            Ant *p1=new Ant(white,90+20*j,40+20*i,true,true); //set the new ant with x and y coordinates
            //for each ant's initial decision, set it to be either left or right
            p1->set_condition();
            ant_army_decision_board[i].push_back(p1); //add value to this new-inserted row vertically as column value on the same row
        }
    }

    //for initializing the statistics board
    for (int i=0;i<22;i++){
        Bar *p1=new Bar(QColor(255,255,255),0+i*20,ui->statisticsView->height(),0);
        bar_board.push_back(p1);
    }

    //for initializing the ant army's movement map
    for(int i=0;i<rows_;i++){
        ant_moving_map.push_back(std::vector<Cell*>()); //insert a row into the army map
        for(int j=0;j<columns_;j++){
            Cell *p1=new Cell(white,90+20*j,40+20*i); //set the new cell (either ant, empty, food, or obstacle with x and y coordinates
            //for each cell, first set it to be either empty or obstacle cell
            p1->set_condition();
            //then update their role to either ant army or food based on their coordinates
            if(i == ant_army_coordinates[0] && j == ant_army_coordinates[1]){
                p1->set_ant_army();
            }
            else if(i == food_coordinates[0] && j == food_coordinates[1]){
                p1->set_food();
            }
            ant_moving_map[i].push_back(p1); //add value to this new-inserted row vertically as column value on the same row
        }
    }

    //print the intialized board on the screen
    print_board();

    //initialize a timer, and connect the on_timer_fired slot with the timeout signal so everytime timeout emit, on_timer_fired will run once
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MainWindow::OnTimerFired);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//print the current condition of the board every time a turn is ended
void MainWindow::print_board() {
    //extra work for counting ants thta has survived for more than 2 turns
    int ant_alive=0;

    nx = 0;
    ny = 0;
    total_=0;
    //print ant decision map
    for (int i=0;i<ant_army_decision_board.size();i++){
        for (int j=0;j<ant_army_decision_board[i].size();j++){
            //for the first turn, put ants on UI
            if(turn_count==0 && !reset_){
                antDecisionScene->addItem(ant_army_decision_board[i][j]); //addItem() will draw the ant for us on UI, which will finally call Ant::paint()
            }

            //count the total number of all ants and ants go either left or right
            total_+=1;
            if(ant_army_decision_board[i][j]->now_decision() == 0){
                nx += 1;

                //extra work for counting ants thta has survived for more than 2 turns
                if(ant_army_decision_board[i][j]->get_right()>2){
                    ant_alive++;
                }
            }
            else if(ant_army_decision_board[i][j]->now_decision() == 1){
                ny += 1;
            }
        }
    }

    //print ant army map
    for (int i=0;i<ant_moving_map.size();i++){
        for (int j=0;j<ant_moving_map[i].size();j++){
            //for the first turn, put ants on UI
            if(turn_count==0 && !reset_){
                mapScene->addItem(ant_moving_map[i][j]); //addItem() will draw the ant army for us on UI, which will finally call Cell::paint()
            }
        }
    }

    //update turn count and population
    QString text="Turn: "+QString::number(turn_count);
    ui->TurnBar->setText(text);
    double percentage=nx*1.0/total_*100.0;
    QString text2="Left vs. Right: " + QString::number(nx) + " : " +
            QString::number(total_ - nx);
    ui->PercentageBar->setText(text2);

    //extra work for counting ants thta has survived for more than 2 turns
    double percentage_alive=ant_alive*1.0/total_*100.0;
    QString text3="Right > 2: "+QString::number(ant_alive)+" ("+QString::number(percentage_alive)+"%)";
    ui->AliveBar->setText(text3);

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

    //print static bar map
    for (int i=0;i<bar_board.size();i++){
        if(turn_count==0 && !reset_){
            staticScene->addItem(bar_board[i]); //addItem() will draw the static bar for us on UI, which will finally call Bar::paint()
        }
    }

    //disable all buttons except reset button when ant army reaches the food
    if (ant_army_coordinates[0] == food_coordinates[0] && ant_army_coordinates[1] == food_coordinates[1]){
        end_game();
    }
}

//the detailed process for playing each turn
void MainWindow::play_once(){
    //update turn count information
    turn_count++;
    //get the boundary of the board size
    int x_max=ant_army_decision_board.size()-1;
    int y_max=ant_army_decision_board[0].size()-1;

    // first check if the ant army can move forward without running into any obstacle
    bool straight_forward = check_army_forward();

    // if no obstacle ahead, no need to make group decision and just move the ant army forward
    if(straight_forward){
        qDebug()<<"no need to generate group decision, moving forward";
        final_direction_ = 0; // set next army movement forward
        // set all ant decision to orange, marking that they are not making specific direction for this turn
        for (int i=0;i<=x_max;i++){
            for (int j=0;j<=y_max;j++){
                Ant *current_ant=ant_army_decision_board[i][j];
                current_ant->set_next(2); // it will set tx and ty value back to 0
            }
        }
    }

    // otherwise do group decision using te any_army_deision_board to genereate the moving direction
    else{
        int left_after_this_turn = 0;
        int right_after_this_turn = 0;
        qDebug()<<"cannot move forward directly, start generating gorup decision";
        //loop through the board and update their next condition based on the equation of decision for individual ant
        for (int i=0;i<=x_max;i++){
            for (int j=0;j<=y_max;j++){
                Ant *current_ant=ant_army_decision_board[i][j];
                double p_left=generate_probability(current_ant->get_tx(), current_ant->get_ty());
                //decide to go left or right
                int random=rand()%100 + 1;
                if(p_left<=random){
                    //set its next move to left and update its tx, ty value
                    current_ant->set_next(0);
                    left_after_this_turn += 1;
                }
                else{
                    //set its next move to right and update its tx, ty value
                    current_ant->set_next(1);
                    right_after_this_turn += 1;
                }
            }
        }
        if(left_after_this_turn < right_after_this_turn){ // ant army go right
            final_direction_ = 1;
        }
        else{ // ant army go left
            final_direction_ = -1;
        }
    }

    //update the board and turn/popluation/alive information after the current turn is over
    update_board();
    print_board();
}

void MainWindow::update_board(){
    // for ant army decision map
    for (int i=0;i<ant_army_decision_board.size();i++){
        for (int j=0;j<ant_army_decision_board[i].size();j++){
            ant_army_decision_board[i][j]->update_condition();
        }
    }

    // for ant army moving map
    int current_x = ant_army_coordinates[0];
    int current_y = ant_army_coordinates[1];
    ant_moving_map[current_x][current_y]->set_empty(); // first set the current ant army's location to empty

    bool moved = move_ant_army(current_x, current_y); // move the ant army and check if the move is successfull or not

    if(!moved){
        qDebug() << "reaching the boundary or runing into obstacle, ant army won't move for this turn.";
    }

    int next_x = ant_army_coordinates[0];
    int next_y = ant_army_coordinates[1];
    ant_moving_map[next_x][next_y]->set_ant_army(); // set the cell that ant army is currently landing at as ant army
}

bool MainWindow::check_army_forward(){
    bool straight_forward = false;
    int current_x = ant_army_coordinates[0];
    int current_y = ant_army_coordinates[1];
    if(current_x != 0){ // if not at the top of the board, which means moving forward still doable
        if (ant_moving_map[current_x - 1][current_y]->get_role() != 1){ // if not reaching top row and the cell above is not obstacle
            straight_forward = true;
        }
    }
    return straight_forward;
}

bool MainWindow::move_ant_army(int current_x, int current_y){
    bool moved = false;

    if(final_direction_ == 0){ // move forward
        if (current_x - 1 >= 0){ // boundary check
            if(ant_moving_map[current_x - 1][current_y]->get_role() != 1){ // obstacle check
                ant_army_coordinates[0] -= 1;
                moved = true; // STILL NOT FOOD CHECK TO END THE GAME
            }
        }
    }
    else if(final_direction_ == 1){ // move right
        if (current_y + 1 < columns_){ // boundary check
            if(ant_moving_map[current_x][current_y + 1]->get_role() != 1){ // obstacle check
                ant_army_coordinates[1] += 1;
                moved = true;
            }
        }
    }
    else if(final_direction_ == -1){ // move left
        if (current_y - 1 >= 0){ // boundary check
            if(ant_moving_map[current_x][current_y - 1]->get_role() != 1){ // obstacle check
                ant_army_coordinates[1] -= 1;
                moved = true;
            }
        }
    }
    return moved;
}

//return the value of neighbors that are still alive
double MainWindow::generate_probability(int tx, int ty){
    //save the neighbor rows and columns the original ant is going to check
    double decay_x = exp(-pow(tx, 2) / (pow(lm, 2)));
    double decay_y = exp(-pow(ty, 2) / (pow(lm, 2)));
    double dx = 1 + a * powf(s, -(nx * decay_x - k * ny));
    double dy = 1 + a * powf(s, -(ny * decay_y - k * nx));
    double px = 1 / (1 + (dx / dy));
    double px_in_percentage = px * 100;
    qDebug()<<"current px_in_percentage: ";
    qDebug()<<px_in_percentage;
    return px_in_percentage;
}

//set all buttons except the reset one as unclickable buttons
void MainWindow::end_game(){
    step_button->setEnabled(false);
    play_button->setEnabled(false);
    pause_button->setEnabled(false);

    // set timer to stop so even autp play will be paused
    timer_->stop();
    pause_=true;
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
    timer_->start(ms_);
}

//once the pause button been pressed, stop the timer
void MainWindow::on_PauseButton_pressed(){
    timer_->stop();
    pause_=true;
}

void MainWindow::on_ResetButton_pressed(){
    qDebug() << "Game restarts";
    //re-initialize the play board
    for (int i=0;i<ant_army_decision_board.size();i++){
        for (int j=0;j<ant_army_decision_board[i].size();j++){
            ant_army_decision_board[i][j]->set_condition();
            ant_army_decision_board[i][j]->update();
        }
    }
    //re-initialize the bar board
    for (int i=0;i<22;i++){
        bar_board[i]->update_condition(0+i*20,ui->statisticsView->height(),0);
        bar_board[i]->update();
    }

    //re-locate ant army and food
    ant_army_coordinates = {rows_ - 1, columns_/2 - 1};
    food_coordinates = {0, columns_/2 - 1};

    //re-initialize the ant army map board
    for (int i=0;i<ant_moving_map.size();i++){
        for (int j=0;j<ant_moving_map[i].size();j++){
            ant_moving_map[i][j]->set_condition();
            if(i == ant_army_coordinates[0] && j == ant_army_coordinates[1]){
                ant_moving_map[i][j]->set_ant_army();
            }
            else if(i == food_coordinates[0] && j == food_coordinates[1]){
                ant_moving_map[i][j]->set_food();
            }
            ant_moving_map[i][j]->update();
        }
    }

    //re-start the counters
    bar_count=0;
    turn_count=0;
    reset_=true;

    //pause the timer so it will auto run the game only by pressing play button again
    timer_->stop();
    pause_=true;

    print_board();

    //re-able other buttons
    step_button->setEnabled(true);
    play_button->setEnabled(true);
    pause_button->setEnabled(true);
}

//adjust the playing speed
void MainWindow::on_horizontalSlider_sliderReleased(){
    qDebug()<<"speed adjusted";
    //get the adjusted speed based on the return value of sliderPosition, it will be a value in [0,99]
    //transfer it into percentage type
    double speed_rate=ui->horizontalSlider->sliderPosition()*1.0/100;
    //and adjust ms value based on this rate
    ms_=500-500*speed_rate;
    //update the ms value on screen as well
    QString text="Current speed: "+QString::number(ms_)+" ms/turn";
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
