//     - The Stone class creates instances of customizable individual Stone and includes several important data fields that are specific to each Curling Stone
//       such as speed, direction, friction, and rotation, among others.
//     - The Stone class includes various functions that enable Curling stone to move, spin, and collide. The class additionally initializes stone positions, coloring and size,

#ifndef STONE_H
#define STONE_H
#include <SFML/Graphics.hpp>
#include <string.h>


class Stone:public sf::CircleShape
{
public:
    Stone();
    virtual ~Stone();

    //Make Stone move and status updaters
    void makeMove(float step=1.0);
    void changeStatus();
    bool getStatus();

    //functions that deal with colliding with other stones and the wall
    static bool isCollision(Stone stone_array[], int SIZE, int collisionStones[], sf::Vector2u windowSize);
    static bool isCollision(Stone stone1, Stone stone2);

    //function that calculate and update stone velocties afteer a collision
    sf::Vector2f findVelocityPostCollision(Stone otherStone);
    void updatePostCollision(sf::Vector2f newVelocity);

    //function that determine if stone went off the rink
    static bool isWallCollision(Stone stone_array[], int SIZE, int& collisionInfo, sf::Vector2u windowSize);
    bool onRink(sf::Vector2u ws);


    // Getters/Setters of data fields
    void setInitialSpeed(float e);
    void setSpeed();
    float getSpeed();
    void setFriction(float f=5*9.81*.0168/60);
    void setInitialDirection(float d);
    void setDirection();
    float getDirection();
    void setSpin(float s=0);
    float getSpin();
    float getDistance(Stone s_o);
    void setVelocity();
    sf::Vector2f getVelocity();

protected:
private:

    //speed value representative of magnitude of Stone velocity down the ice
    float speed;

    // direction value is an angle (radians) where 0 is completely straight
    float direction;

    //spin value allows stone to curl down ice
    float spin;

    //friction value set to initial value and cahnged as sweeping occurs
    float friction;

    //status of stone
    bool inPlay = false;

    //vector that holds position of stone on rink
    sf::Vector2f pos;
    //vector that holds velocity componants of stone on rinl
    sf::Vector2f v;

    //constants used to check if a collision is occuring
    float x_dist;
    float y_dist;
    float tot_dist;

    //constant radius for all the stones
    float radius = 10*(11.4/12)*.5 + 2;
};

#endif // STONE_H
