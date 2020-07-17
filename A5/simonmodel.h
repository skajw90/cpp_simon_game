#ifndef SIMONMODEL_H
#define SIMONMODEL_H
#include <QObject>
#include <vector>
#include <random>

class SimonModel : public QObject
{
    Q_OBJECT

public:
    SimonModel(unsigned long seed);
    virtual ~SimonModel();

public slots:
    void play(int buttonId);
    void startGame();

signals:
    void computerPlay(int buttonId);
    void updateProgress(double p);
    void playerTurn();
    void computerTurn();
    void gameOver(int score);

private:
    void runRound();

    std::vector<int> _buttonSequence;
    std::vector<int> _scores;
    size_t _currentPos;
    std::mt19937 _rg;

    static const int _start_length = 3;
};

#endif // SIMONMODEL_H
