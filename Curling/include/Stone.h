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
        sf::Vector2f getVelocity();
        char whichWallCollision(sf::Vector2u ws);
        //void makeMove();
        void makeMove(float step=1.0);
        void putInOpenSpot(Stone s_o[],int i, const int NUM_OF_STONES);
        void setInitialVelocity();
        void setInitialVelocity(float vx_i, float vy_i);
        void setVelocity_s(sf::Vector2f scv, sf::Vector2f svp);
        void setVelocity_w(sf::Vector2u ws);
        void friction();
        bool checkWallCollision(sf::Vector2u ws);
        bool checkStoneCollision(Stone s_o);
        //bool tooCloseStone(Stone s_o);
        bool tooCloseWall_tb(sf::Vector2u ws);
        bool wall_tb = false;
        bool tooCloseWall_lr(sf::Vector2u ws);
        bool wall_lr = false;
        bool positionTaken(Stone s_o);
        bool isWithinWindow(sf::Vector2u ws);
    protected:
    private:
    sf::Vector2f v;
    sf::Vector2f cen;
    sf::Vector2f pos;
    sf::Color c[2] = {sf::Color::Yellow, sf::Color::Green};
    static int NumofStones;
    float x_dist;
    float y_dist;
    float tot_dist;
    float radius = 10*(11.4/12)*.5;
    //float radius = 30;
};

#endif // STONE_H
