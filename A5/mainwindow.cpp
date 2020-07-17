#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <string>
#include <chrono>
#include <algorithm>

MainWindow::MainWindow(SimonModel& model, QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    connect(this, &MainWindow::newGame, this, &MainWindow::toStart);
    connect(&model, &SimonModel::computerPlay, this, &MainWindow::blink);
    connect(&model, &SimonModel::playerTurn, this, &MainWindow::toPlay);
    connect(&model, &SimonModel::computerTurn, this, &MainWindow::toView);
    connect(&model, &SimonModel::gameOver, this, &MainWindow::gameOver);
    connect(&model, &SimonModel::updateProgress, [this](double p) {
        this->_ui->progressBar->setValue(static_cast<int>(p * 100.0));
    });
    connect(_ui->redButton, &QPushButton::pressed, [&model]() { model.play(0); });
    connect(_ui->blueButton, &QPushButton::pressed, [&model]() { model.play(1); });
    connect(_ui->startButton, &QPushButton::pressed, &model, &SimonModel::startGame);
}

MainWindow::~MainWindow()
{
    delete _ui;
}


void MainWindow::blink(int button) {
    static auto lt = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    qDebug() << std::chrono::duration_cast<std::chrono::milliseconds>(now - lt).count();
    lt = now;
    switch(button) {
    case 0:
        _ui->redButton->setStyleSheet("background-color:pink;");
        QTimer::singleShot(150, [b=_ui->redButton]() {
            b->setStyleSheet("background-color:red;");
        });
        break;
    case 1:
        _ui->blueButton->setStyleSheet("background-color:lightblue;");
        QTimer::singleShot(150, [b=_ui->blueButton]() {
            b->setStyleSheet("background-color:blue;");
        });
        break;
    }
}

void MainWindow::toStart() {
    _ui->currentPlayerLabel->setText("");
    _ui->redButton->setEnabled(false);
    _ui->blueButton->setEnabled(false);
    _ui->startButton->setEnabled(true);
}

void MainWindow::toPlay() {
    _ui->currentPlayerLabel->setText("Player");
    _ui->startButton->setEnabled(false);
    _ui->redButton->setEnabled(true);
    _ui->blueButton->setEnabled(true);
}

void MainWindow::toView() {
    _ui->currentPlayerLabel->setText("Computer");
    _ui->redButton->setEnabled(false);
    _ui->blueButton->setEnabled(false);
    _ui->startButton->setEnabled(false);
}

void MainWindow::gameOver(int score) {
    auto it = std::lower_bound(std::begin(_scores), std::end(_scores), score, std::greater<int>());
    auto idx = it - std::begin(_scores);
    _scores.insert(it, score);
    qDebug() << idx;
    _ui->scoreList->insertItem(idx, std::to_string(score).c_str());
    QMessageBox msgBox{};
    msgBox.setWindowTitle("You lose!");
    msgBox.setText("You lose! Click OK to restart the game");
    msgBox.setBaseSize(800, 150);
    msgBox.setStandardButtons(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes){
        msgBox.close();
        emit newGame();
    } else {
        emit newGame();
    }
}

