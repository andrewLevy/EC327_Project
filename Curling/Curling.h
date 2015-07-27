#ifndef CURLING_H
#define CURLING_H

#include <SFML/Graphics.hpp>
#include <Stone.h>

class Curling
{
public:
    Curling(int newPlayType, int newPointsToWin);

    void prepareStones(Stone stone_array[]);
    void drawRink(sf::CircleShape Targets[], sf::RectangleShape Lines[], sf::CircleShape resting_Spots[], Stone s_b[], int NUM_OF_STONES);

    int getPlayType();
    int getPointsToWin();

private:
    int playType;
    int pointsToWin;

};

#endif
