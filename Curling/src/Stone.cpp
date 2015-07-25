#include "Stone.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define PI 3.14159265


using namespace std;

int Stone::NumofStones = 0;
int evenNumberStones = 0;
int oddNumberStones = 0;

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
    setRadius(radius);
    setOrigin(radius,radius);
    setOutlineColor(sf::Color(160,160,160));
    setOutlineThickness(-2.0);

    speed = 0;
    direction = 0;

    if(NumofStones % 2 == 0)
    {
        setFillColor(sf::Color::Green);
        setPosition(1125+evenNumberStones*2*radius + evenNumberStones * 1,15);
        evenNumberStones++;
    }
    else
    {
        setFillColor(sf::Color::Yellow);
        setPosition(1125+oddNumberStones*2*radius + oddNumberStones * 1,150);
        oddNumberStones++;
    }
    NumofStones++;
}


Stone::~Stone()
{
    //dtor
}

float Stone::getDistance(Stone s_o)
{
    x_dist = getPosition().x - s_o.getPosition().x;
    y_dist = getPosition().y - s_o.getPosition().y;
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

float Stone::getSpin()
{
    return spin;
}

void Stone::setVelocity()
{

    v.x= speed * cos(direction);
    v.y= speed * sin(direction);

    /*
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
    }*/


}

void Stone::changeStatus()
{
    if(inPlay == false)
        inPlay = true;
    else
        inPlay = false;
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
    pos = getPosition();
    if (((pos.x - radius) <= 0 || (pos.x + radius) >= ws.x))
    {
        return true;
    }
    if (((pos.y - radius) <= 0 || (pos.y + radius) >= ws.y - 4))
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
bool Stone::getStatus()
{
    return inPlay;
}

bool Stone::onRink(sf::Vector2u windowSize)
{
    float x_position = getPosition().x;
    float y_position = getPosition().y;

    if(x_position - radius <= 0 || x_position + radius >= windowSize.x)
    {
        //cout << "x position" << " " << x_position << endl;
        return false;
    }
    else if (y_position - radius <= 0 || y_position + radius >= windowSize.y)
    {
        //cout << "y position" << " " << y_position << endl;
        return false;
    }
    else
        return true;
}

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

    newVelocity = getVelocity() - intermediateVector;
    return newVelocity;
}

void Stone::updatePostCollision(sf::Vector2f newVelocity)
{
    speed = sqrt(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y);

    // Determine new direction

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
