#include "simonmodel.h"
#include <QTimer>
#include <QDebug>
#include <random>
#include <cmath>
#include <chrono>

SimonModel::SimonModel(unsigned long seed) :
    _rg(seed)
{

}

SimonModel::~SimonModel() {

}

void SimonModel::play(int buttonId) {
    if(buttonId != _buttonSequence[_currentPos++]) {
        _currentPos = 0;
        auto score = _buttonSequence.size() == _start_length ? 0 : _buttonSequence.size() - 1;
        _buttonSequence = std::vector<int>();
        emit gameOver(score);
    } else {
        emit updateProgress(static_cast<double>(_currentPos) / _buttonSequence.size());
        if (_currentPos == _buttonSequence.size()) {
            _currentPos = 0;
            _buttonSequence.push_back(_rg() & 1);          
            runRound();
        }
    }
}

void SimonModel::runRound() {
    emit computerTurn();
    auto interval = 220.0 + 500.0*std::pow(_buttonSequence.size() - 2, -0.7);
    for(auto i = size_t{0}; i < _buttonSequence.size(); i++) {
        QTimer::singleShot(interval*(i + 1),
                           Qt::PreciseTimer,
                           [i, this]() { emit computerPlay(_buttonSequence[i]); });
    }

    QTimer::singleShot(interval*(_buttonSequence.size() + 1),
                       Qt::PreciseTimer,
                       [this]() {
        emit updateProgress(0.0);
        emit playerTurn();
    });
}

void SimonModel::startGame() {
    _currentPos = 0;
    for(auto i = 0; i < _start_length; i++) {
        _buttonSequence.push_back(_rg() & 1);
    }
    runRound();
}
