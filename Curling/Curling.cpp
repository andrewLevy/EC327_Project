#include "Curling.h"
#include <SFML/Graphics.hpp>
#include "Stone.h"
#include <iostream>

using namespace std;

const int BOARD_WIDTH = 1250;
const int BOARD_HEIGHT = 165;

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

void Curling::drawRink(sf::CircleShape Targets[], sf::RectangleShape lines[], sf::CircleShape resting_Spots[], Stone s_b[], int NUM_OF_STONES)
{
    // Create House
    float radius[4] = {60,40,20,5};
    sf::Color T[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};
    for (int i=0; i<4; i++)
    {
        Targets[i].setRadius(radius[i]);
        Targets[i].setOrigin(radius[i],radius[i]);
        Targets[i].setFillColor(T[i]);
        Targets[i].setPosition(180,BOARD_HEIGHT/2);
        Targets[i].setOutlineColor(sf::Color::Black);
        Targets[i].setOutlineThickness(-2.0);
    }

    // Create Boundary Lines
    sf::Vector2f Lpos[8] = {sf::Vector2f(0,BOARD_HEIGHT/2 - 0.5),sf::Vector2f(390-2,0),sf::Vector2f(180 -.5,0),sf::Vector2f(1110-2,0),sf::Vector2f(0,BOARD_HEIGHT - 4),sf::Vector2f(240,BOARD_HEIGHT/2+15),sf::Vector2f(240,BOARD_HEIGHT/2-15),sf::Vector2f(120-.5,0)};
    sf::Vector2f Lsize[8] = {sf::Vector2f(1110,1),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(1,BOARD_HEIGHT),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(BOARD_WIDTH,4),sf::Vector2f(1110-240,.5),sf::Vector2f(1110-240,.5),sf::Vector2f(1,BOARD_HEIGHT)};
    for (int i=0; i<8; i++)
    {
        lines[i].setSize(Lsize[i]);
        lines[i].setFillColor(sf::Color::Black);
        lines[i].setPosition(Lpos[i]);
    }

    // Draw resting spots
    for (int rs=0; rs<NUM_OF_STONES; rs++)
    {
        resting_Spots[rs].setRadius(s_b[rs].getRadius());
        resting_Spots[rs].setFillColor(sf::Color::White);
        resting_Spots[rs].setOutlineColor(sf::Color::Black);
        resting_Spots[rs].setOutlineThickness(-1.0);
        resting_Spots[rs].setOrigin(s_b[rs].getRadius(),s_b[rs].getRadius());
        resting_Spots[rs].setPosition(s_b[rs].getPosition());
    }

    /*
    // Create Scoreboard
    sb[0].setFillColor(sf::Color::Green);
    sb[1].setFillColor(sf::Color::Black);
    sb[2].setFillColor(sf::Color::Yellow);
    sb_Text[0].setString("Score");
    sb_Text[1].setString("Set");
    sb_Text[2].setString("Score");

    for (int b=0; b<6; b++)
    {
        if (b<3)
        {
            sb[b].setSize(sb_size);
            sb[b].setOrigin(sb_size.x/2,sb_size.y/2);
            sb[b].setOutlineColor(sf::Color::Black);
            sb[b].setOutlineThickness(-2.0);
            sb[b].setPosition(BOARD_WIDTH/2+(b-1)*sb_size.x,195);
            sb_Text[b].setFont(font);
            sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getLocalBounds().height/2);
            sb_Text[b].setCharacterSize(30);
            sb_Text[b].setPosition(sb[b].getPosition());
            sb_Text[b].setColor(sf::Color::Black);
        }
        else
        {
            sb[b].setSize(sb_size);
            sb[b].setOrigin(sb_size.x/2,sb_size.y/2);
            sb[b].setOutlineColor(sf::Color::Black);
            sb[b].setOutlineThickness(-2.0);
            sb[b].setPosition(BOARD_WIDTH/2+(b-4)*sb_size.x,245);
            sb_Text[b].setFont(font);
            sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getCharacterSize()/2);
            sb_Text[b].setCharacterSize(20);
            sb_Text[b].setPosition(sb[b].getPosition());
            sb_Text[b].setString("0");
            sb_Text[b].setColor(sf::Color::Black);
        }
    }
    sb_Text[1].setColor(sf::Color::White);*/
}

int Curling::getPlayType()
{
    return playType;
}

int Curling::getPointsToWin()
{
    return pointsToWin;
}
