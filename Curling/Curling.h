#ifndef CURLING_H
#define CURLING_H

#include <SFML/Graphics.hpp>
#include <Stone.h>

class Curling
{
public:
    Curling(int newPlayType, int newPointsToWin);

    void drawRink(sf::CircleShape Targets[], sf::RectangleShape Lines[]);
    void prepareStones(Stone stone_array[]);

    int getPlayType();
    int getPointsToWin();

private:
    int playType;
    int pointsToWin;

};

#endif
