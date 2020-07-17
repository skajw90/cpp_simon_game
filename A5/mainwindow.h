#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "simonmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(SimonModel& sg, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void blink(int button);
    void toStart();
    void toView();
    void toPlay();
    void gameOver(int score);

signals:
    void newGame();

private:
    Ui::MainWindow *_ui;

    std::vector<int> _scores;
};

#endif // MAINWINDOW_H
