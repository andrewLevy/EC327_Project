#ifndef STONE_H
#define STONE_H
#include <SFML/Graphics.hpp>
//#include <Color.hpp>
#include <string.h>


class Stone:public sf::CircleShape
{
    public:
        Stone();
        virtual ~Stone();
        int getNumofStones();
        float getDistance(Stone s_o);
        float getSpeed();
        sf::Vector2f getVelocity();

        void makeMove(float step=1.0);

        void setFriction(float f=5*9.81*.0168/60);
        void setSpin(float s=0);
        void setInitialSpeed(float e);
        void setSpeed();
        void setInitialDirection(float d);
        void setDirection();
        void changeStatus();
        void updatePostCollision(sf::Vector2f newVelocity);
        static void resetNumberofStones();

        float getDirection();
        float getSpin();
        bool getStatus();
        static int getNumberofStones();


        void setVelocity();

        void setVelocity_s(sf::Vector2f scv, sf::Vector2f svp);

        static bool isCollision(Stone stone_array[], const int SIZE, int collisionStones[], sf::Vector2u windowSize);
        static bool isCollision(Stone stone1, Stone stone2);
        static bool isWallCollision(Stone stone_array[], const int SIZE, int& collisionInfo, sf::Vector2u windowSize);
        sf::Vector2f findVelocityPostCollision(Stone otherStone);

        bool checkWallCollision(sf::Vector2u ws);
        bool checkStoneCollision(Stone s_o);

        bool isWithinWindow(sf::Vector2u ws);
        bool onRink(sf::Vector2u ws);
    protected:
    private:

    float speed;
    float direction;
    float spin;
    float friction;
    bool inPlay = false;

    sf::Vector2f v;
    sf::Vector2f cen;
    sf::Vector2f pos;
    sf::Color c[2] = {sf::Color::Yellow, sf::Color::Green};
    static int NumofStones;
    float x_dist;
    float y_dist;
    float tot_dist;
    float radius = 10*(11.4/12)*.5 + 2;
    //float radius = 30;
};

#endif // STONE_H
