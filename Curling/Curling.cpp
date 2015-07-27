#include "Curling.h"
#include <SFML/Graphics.hpp>
#include "Stone.h"
#include <iostream>

using namespace std;

Curling::Curling(int newPlayType, int newPointsToWin)
{
    // Data field to specity game type includes practice (playType = 0) or One-on-One (playType = 1)
    playType = newPlayType;
    pointsToWin = newPointsToWin;
}

void Curling::prepareStones(Stone stone_array[])
{
    if(playType == 0)
    {
        // Specify positioning and fill color for stone array
        for(int i = 0; i < 8; i++)
        {
            stone_array[i].setFillColor(sf::Color::Green);
            stone_array[i].setPosition(1125 + i * (2 * stone_array[i].getRadius() + 1),15);
        }
    }
    else
    {
        // Specify positioning and fill color for stone array
        for(int i = 0; i < 16; i++)
        {
            if(i % 2 == 0)
            {
                stone_array[i].setFillColor(sf::Color::Green);
                stone_array[i].setPosition(1125 + i / 2 * (2 * stone_array[i].getRadius() + 1),15);
            }
            else
            {
                stone_array[i].setFillColor(sf::Color::Yellow);
                stone_array[i].setPosition(1125 + i / 2 * (2 * stone_array[i].getRadius() + 1),150);
            }
        }

    }
    return;
}

int Curling::getPlayType()
{
    return playType;
}

int Curling::getPointsToWin()
{
    return pointsToWin;
}
