#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <Stone.h>

using namespace std;


int main()
{
    sf::Time t1=sf::seconds(1.0/120.0);
    sf::Time t2;
    sf::Clock myclock;

    // Create the main window
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 165;
    const int NUM_OF_STONES = 2;
    const int CHECK = 20;

    //nt position_check = 0;
    char Play_Mode = 'B';
    char last_Mode = 'B';
    bool Changed_Mode = false;

    sf::RenderWindow app(sf::VideoMode(1400, 500), "SFML window");

    //Create board
    sf::CircleShape circle[4];
    float radius[4] = {60,40,20,5};
    sf::Color c[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};
    sf::RectangleShape lines[5];
    sf::Vector2f Lpos[5] = {sf::Vector2f(0,WINDOW_HEIGHT/2-.5),sf::Vector2f(390-2,0),sf::Vector2f(180-.5,0),sf::Vector2f(1110-2,0),sf::Vector2f(0,WINDOW_HEIGHT)};
    sf::Vector2f Lsize[5] = {sf::Vector2f(WINDOW_WIDTH,1),sf::Vector2f(4,WINDOW_HEIGHT),sf::Vector2f(1,WINDOW_HEIGHT),sf::Vector2f(4,WINDOW_HEIGHT),sf::Vector2f(WINDOW_WIDTH,4)};
    for (int i=0; i<4; i++)
    {
        circle[i].setRadius(radius[i]);
        circle[i].setOrigin(radius[i],radius[i]);
        circle[i].setFillColor(c[i]);
        circle[i].setPosition(180,WINDOW_HEIGHT/2);
        circle[i].setOutlineColor(sf::Color::Black);
        circle[i].setOutlineThickness(2.0);
    }
    for (int i=0; i<5; i++)
    {
        lines[i].setSize(Lsize[i]);
        lines[i].setFillColor(sf::Color::Black);
        lines[i].setPosition(Lpos[i]);
    }

    //Create Stone bounce stones
    Stone s_b[NUM_OF_STONES];

    //Create Stone test stones
    Stone s_t[2];

    sf::Vector2u window_size = app.getSize();

	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        if (Play_Mode != 'P')
        {
            app.clear(sf::Color(255,255,255));
        }

        // Draw the board
        for (int c=0; c<4; c++)
        {
            app.draw(circle[c]);
        }
        for (int l=0; l<5; l++)
        {
            app.draw(lines[l]);
        }

        // game mode detection
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            Play_Mode = 'R';
            if (last_Mode != Play_Mode)
            {
                last_Mode = Play_Mode;
                Changed_Mode = true;
            }
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            Play_Mode = 'P';
            /*if (last_Mode != Play_Mode)
            {
                last_Mode = Play_Mode;
                Changed_Mode = true;
            }*/
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            app.close();
        }

        //move stone in Bouce Mode
        if (Play_Mode=='B')
        {
            Changed_Mode =false;
            for (int b=0; b<NUM_OF_STONES; b++)
            {
                app.draw(s_b[b]);
                s_b[b].setFriction();
                s_b[b].setSpin();
                s_b[b].setSpeed();
                s_b[b].setDirection();
                s_b[b].setVelocity();
                s_b[b].makeMove();

                if(s_b[b].checkWallCollision(window_size))
                {
                    s_b[b].setInitialSpeed(0.0);
                }

                for (int bCheck=0; bCheck<=CHECK; bCheck++)
                {
                    for (int g=0; g<NUM_OF_STONES; g++)
                    {
                        if (g!=b)
                        {
                            if (s_b[b].checkStoneCollision(s_b[g]))
                            {
                                sf::Vector2f b_v=s_b[b].getVelocity();
                                s_b[b].setVelocity_s(s_b[g].getVelocity(), s_b[g].getPosition());
                                s_b[g].setVelocity_s(b_v, s_b[b].getPosition());

                                s_b[b].setInitialSpeed(sqrt(s_b[b].getVelocity().x*s_b[b].getVelocity().x+s_b[b].getVelocity().y*s_b[b].getVelocity().y));
                                s_b[g].setInitialSpeed(sqrt(s_b[g].getVelocity().x*s_b[g].getVelocity().x+s_b[g].getVelocity().y*s_b[g].getVelocity().y));

                                s_b[b].setInitialDirection(atan(s_b[b].getVelocity().y/s_b[b].getVelocity().x));
                                s_b[g].setInitialDirection(sqrt(s_b[g].getVelocity().y/s_b[g].getVelocity().x));
                            }
                            while (s_b[b].checkStoneCollision(s_b[g]))
                            {
                                s_b[b].makeMove(CHECK);
                                s_b[g].makeMove(CHECK);
                            }
                        }
                    }
                }
            }
        }

        // Pause Game
        else if (Play_Mode == 'P')
        {
            while (Play_Mode =='P')
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    Play_Mode ='R';
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                {
                    Play_Mode ='B';
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
                {
                    Play_Mode = 'T';
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                {
                    Play_Mode = 'Q';
                }
            }
        }

        // Update the window
        app.display();
        sf::sleep(t1);
        t2=myclock.getElapsedTime();
        // std::cout << t2.asSeconds() << std::endl;
    }

    return EXIT_SUCCESS;
}
