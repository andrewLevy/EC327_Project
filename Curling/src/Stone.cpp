#include "Stone.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

int Stone::NumofStones = 0;

float dot_product(sf::Vector2f v1, sf::Vector2f v2)
{
    return (v1.x*v2.x+v1.y*v2.y);
}

float norm_2(sf::Vector2f v)
{
    return v.x*v.x + v.y*v.y;
}

float randMToN(float M, float N)
{
    return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
}


Stone::Stone()
{
    this->setRadius(radius);
    this->setOrigin(radius,radius);
    this->setPosition(rand()%400+50,rand()%400+50);
    this->setOutlineColor(sf::Color(160,160,160));
    this->setOutlineThickness(2.0);
    if (NumofStones%2>0)
    {
        this->setFillColor(c[0]);
    }
    else
    {
        this->setFillColor(c[1]);
    }
    this->setInitialVelocity();
    NumofStones++;
}


Stone::~Stone()
{
    //dtor
}

float Stone::getDistance(Stone s_o)
{
    x_dist = this->getPosition().x - s_o.getPosition().x;
    y_dist = this->getPosition().y - s_o.getPosition().y;
    tot_dist = sqrt((x_dist*x_dist)+(y_dist*y_dist));

    return tot_dist;

}

bool Stone::positionTaken(Stone s_o)
{
    if(this->getGlobalBounds().intersects(s_o.getGlobalBounds()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Stone::isWithinWindow(sf::Vector2u ws)
{
    pos = this->getPosition();
    if ((pos.x+radius<0 || pos.x-radius > ws.x))
    {
        return false;
    }
    if ((pos.y+radius < 0 || pos.y-radius > ws.y))
    {
        return false;
    }
    return true;
}

void Stone::putInOpenSpot(Stone s_o[],int i, const int NUM_OF_STONES)
{
    int position_check = 0;
    while (position_check < NUM_OF_STONES)
    {
        for (int q=0; q<NUM_OF_STONES; q++)
        {
            if (i!=q && this->positionTaken(s_o[q]))
            {
                this->setPosition(rand()%400+50,rand()%400+50);
                position_check = 0;
            }
            else
            {
                position_check++;
            }
        }
    }
}

void Stone::setInitialVelocity()
{

    v.x=randMToN(-8.0,8.0);
    v.y=randMToN(-8.0,8.0);

    //v.x = randMToN(-10.0/6.0,10.0/6.0);
    //v.y = randMToN(-10.0/6.0,10.0/6.0);

}

void Stone::setInitialVelocity(float vx_i, float vy_i)
{
    v.x=vx_i;
    v.y=vy_i;

}

sf::Vector2f Stone::getVelocity()
{
    return v;
}

/*void Stone::makeMove()
{
    move(v.x,v.y);
}*/

void Stone::makeMove(float step)
{
    move(v.x/step,v.y/step);
}

bool Stone::checkWallCollision(sf::Vector2u ws)
{
    pos = this->getPosition();
    if (((pos.x-radius)<0 || (pos.x+radius)>ws.x) && !wall_lr)
    {
        wall_lr = true;
        return true;
    }
    if (((pos.y-radius)<0 || (pos.y+radius)>ws.y) && !wall_tb)
    {
        wall_tb = true;
        return true;
    }
    wall_lr = tooCloseWall_lr(ws);
    wall_tb = tooCloseWall_tb(ws);
    return false;
}

bool Stone::tooCloseWall_lr(sf::Vector2u ws)
{
    //pos = this->getPosition();
    if ((pos.x-radius)<0 || (pos.x+radius)>ws.x)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Stone::tooCloseWall_tb(sf::Vector2u ws)
{
    //pos = this->getPosition();
    if ((pos.y-radius)<0 || (pos.y+radius)>ws.y)
    {
        return true;
    }
    else
    {
        return false;
    }

}


void Stone::setVelocity_w(sf::Vector2u ws)
{
    pos = this->getPosition();
    if ((pos.x-radius)<0 || (pos.x+radius)>ws.x)
    {
        v.x*=-1;
    }
    if ((pos.y-radius)<0 || (pos.y+radius)>ws.y)
    {
        v.y*=-1;
    }
}

bool Stone::checkStoneCollision(Stone s_o)
{
    if(this->getDistance(s_o) < 2*radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Stone::setVelocity_s(sf::Vector2f scv, sf::Vector2f svp)
{
    v=v-(dot_product((v-scv),(this->getPosition()-svp))/(norm_2(this->getPosition()-svp)))*(this->getPosition()-svp);
}


