#include "Stone.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define PI 3.14159265


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
    this->setOutlineColor(sf::Color(160,160,160));
    this->setOutlineThickness(2.0);
    if (NumofStones<8)
    {
        this->setFillColor(sf::Color::Blue);
        this->setPosition(1125+NumofStones*15,15);
        this->setInitialSpeed(0);
        this->setInitialDirection(0);
    }
    else
    {
        this->setFillColor(sf::Color::Yellow);
        this->setPosition(1125+(NumofStones-8)*15,150);
        this->setInitialSpeed(0);
        this->setInitialDirection(0);
    }
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
void Stone::setFriction(float f)
{
    friction = f;
}

void Stone::setSpin(float s)
{
    spin = s;
}

void Stone::setInitialSpeed(float e)
{
    speed = e;
}

void Stone::setSpeed()
{
    if (speed > 0)
    {
        speed -= friction;
    }
    else
    {
        speed = 0;
    }
}

float Stone::getSpeed()
{
    return speed;
}

void Stone::setInitialDirection(float d)
{
    direction = d;
}

void Stone::setDirection()
{
    direction -= spin;
}


void Stone::setVelocity()
{

    v.x=-1*speed*cos(direction);
    v.y=speed*sin(direction);

    if (v.x>0)
    {
        v.x -= friction;
    }
    else if (v.x<0)
    {
        v.x += friction;
    }
    if (v.y>0)
    {
        v.y -= friction;
    }
    else if (v.y<0)
    {
        v.y += friction;
    }


}

sf::Vector2f Stone::getVelocity()
{
    return v;
}


void Stone::makeMove(float step)
{
    move(v.x/step,v.y/step);
}

bool Stone::checkWallCollision(sf::Vector2u ws)
{
    pos = this->getPosition();
    if (((pos.x+radius)<0 || (pos.x-radius)>ws.x))
    {
        return true;
    }
    if (((pos.y+radius)<0 || (pos.y-radius)>ws.y))
    {
        return true;
    }
    return false;
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


/*void Stone::friction()
{
    float friction_coef = 9.8*.0168/60;
    if (v.x>0)
    {
        v.x = v.x - friction_coef;
    }
    else if (v.x<0)
    {
        v.x = v.x + friction_coef;
    }
    if (v.y>0)
    {
        v.y = v.y - friction_coef;
    }
    else if (v.y<0)
    {
        v.y = v.y + friction_coef;
    }
}*/
