#include "Stone.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define PI 3.14159265


using namespace std;

// Stone Constructor
// Stone Constructor initiliazes the radius, origin, colors, speed, and directon
Stone::Stone()
{
    setRadius(radius);
    setOrigin(radius,radius);
    setOutlineColor(sf::Color(160,160,160));
    setOutlineThickness(-2.0);

    speed = 0;
    direction = 0;
}


Stone::~Stone()
{
    //dtor
}


// Move the stone by its velocity
void Stone::makeMove(float step)
{
    move(v.x/step,v.y/step);
}


// Function that updates stone's status if it is in play (moving)
void Stone::changeStatus()
{
    if(inPlay == false)
        inPlay = true;
    else
        inPlay = false;
}


bool Stone::getStatus()
{
    return inPlay;
}


// Check if collision has occurred among specified array of stones
bool Stone::isCollision(Stone stone_array[], const int SIZE, int collisionStones[], sf::Vector2u windowSize)
{
    for(int i = 0; i < SIZE - 1; i++)
    {
        if(stone_array[i].getStatus())
        {
            for(int j = i + 1; j < SIZE; j++)
            {
                float distance = stone_array[i].getDistance(stone_array[j]);

                if(distance < (2 * stone_array[i].getRadius()))
                {
                    collisionStones[0] = i;
                    collisionStones[1] = j;
                    return true;
                }
            }
        }
    }
    return false;
}


// Determines if stone is "still" colliding with other stone
bool Stone::isCollision(Stone stone1, Stone stone2)
{
    float distance = stone1.getDistance(stone2);

    if(distance < (2 * stone1.getRadius()))
        return true;
    else
        return false;
}


// Below function finds new velocity for stone post collision
sf::Vector2f Stone::findVelocityPostCollision(Stone otherStone)
{
    sf::Vector2f newVelocity;
    sf::Vector2f velocityDifference = getVelocity() - otherStone.getVelocity();
    sf::Vector2f positionDifference = getPosition() - otherStone.getPosition();

    float dotProduct1 = velocityDifference.x * positionDifference.x + velocityDifference.y * positionDifference.y;
    float dotProduct2 = positionDifference.x * positionDifference.x + positionDifference.y * positionDifference.y;

    sf::Vector2f intermediateVector;
    intermediateVector.x = (dotProduct1 / dotProduct2) * positionDifference.x;
    intermediateVector.y = (dotProduct1 / dotProduct2) * positionDifference.y;

    newVelocity = (getVelocity() - intermediateVector);
    newVelocity.x * 0.9;
    newVelocity.y * 0.9;
    return newVelocity;
}


// Set new velocity of stone post collision
void Stone::updatePostCollision(sf::Vector2f newVelocity)
{
    speed = sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);

    // Determine direction for stones moving vertically (cannot use inverse tangent as horizontal component is 0)
    if(newVelocity.x == 0)
    {
        if(newVelocity.y > 0)
            direction = PI / 2;
        else
            direction = -PI / 2;
    }
    else
    {
        // Determine direction for other possibilities
        direction = atan(newVelocity.y / newVelocity.x);

        // Adjust direction as needed
        if(newVelocity.x < 0 && newVelocity.y < 0)
            direction = direction + PI;
        else if(newVelocity.x < 0 && newVelocity.y > 0)
            direction = direction + PI;
        else if(newVelocity.x < 0)
            direction = direction + PI;
    }
    spin = 0;
    setVelocity();
}


// Determines if stone collides with wall of rink
bool Stone::isWallCollision(Stone stone_array[], const int SIZE, int& collisionInfo, sf::Vector2u windowSize)
{
    for(int i = 0; i < SIZE; i++)
    {
        if(stone_array[i].getStatus() && !stone_array[i].onRink(windowSize))
        {
            collisionInfo = i;
            //cout << "Stone number: " << collisionInfo << endl;
            return true;
        }
    }
    return false;
}

// Determines if stone is on Rink
bool Stone::onRink(sf::Vector2u windowSize)
{
    float x_position = getPosition().x;
    float y_position = getPosition().y;

    if(x_position - radius <= 0 || x_position + radius >= windowSize.x)
    {
        return false;
    }
    else if (y_position - radius <= 0 || y_position + radius >= windowSize.y)
    {
        return false;
    }
    else
        return true;
}


void Stone::setInitialSpeed(float e)
{
    speed = e;
}
void Stone::setSpeed()
{
    if (speed - friction > 0)
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
void Stone::setFriction(float f)
{
    friction = f;
}


void Stone::setInitialDirection(float d)
{
    direction = d;
}
void Stone::setDirection()
{
    direction += spin;;
}
float Stone::getDirection()
{
    return direction;
}
void Stone::setSpin(float s)
{
    spin = s;
}
float Stone::getSpin()
{
    return spin;
}


// Function that returns distance to another stone (used for collision detection)
float Stone::getDistance(Stone s_o)
{
    x_dist = getPosition().x - s_o.getPosition().x;
    y_dist = getPosition().y - s_o.getPosition().y;
    tot_dist = sqrt((x_dist*x_dist)+(y_dist*y_dist));

    return tot_dist;
}


void Stone::setVelocity()
{
    v.x= speed * cos(direction);
    v.y= speed * sin(direction);
}
sf::Vector2f Stone::getVelocity()
{
    return v;
}









