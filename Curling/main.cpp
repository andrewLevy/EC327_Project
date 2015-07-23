#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <Stone.h>

using namespace std;

double randMToN(double M, double N)
{
    return M + (rand() / ( RAND_MAX / (N-M) ) ) ;
}

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

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");

    //Create board
    sf::CircleShape circle[4];
    float radius[4] = {60,40,20,5};
    sf::Color c[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};
    sf::RectangleShape lines[4];
    sf::Vector2f Lpos[4] = {sf::Vector2f(0,WINDOW_HEIGHT/2-.5),sf::Vector2f(390-2,0),sf::Vector2f(180-.5,0),sf::Vector2f(1110-2,0)};
    sf::Vector2f Lsize[4] = {sf::Vector2f(WINDOW_WIDTH,1),sf::Vector2f(4,WINDOW_HEIGHT),sf::Vector2f(1,WINDOW_HEIGHT),sf::Vector2f(4,WINDOW_HEIGHT)};
    for (int i=0; i<4; i++)
    {
        circle[i].setRadius(radius[i]);
        circle[i].setOrigin(radius[i],radius[i]);
        circle[i].setFillColor(c[i]);
        circle[i].setPosition(180,WINDOW_HEIGHT/2);
        circle[i].setOutlineColor(sf::Color::Black);
        circle[i].setOutlineThickness(2.0);
        lines[i].setSize(Lsize[i]);
        lines[i].setFillColor(sf::Color::Black);
        lines[i].setPosition(Lpos[i]);
    }

    //Create Stone bounce stones
    Stone s_b[NUM_OF_STONES];
    for (int p=0; p<NUM_OF_STONES; p++)
    {
        s_b[p].putInOpenSpot(s_b,p,NUM_OF_STONES);
    }
    //Create Stone random stones
    Stone s_r[10];
    for (int p=0; p<NUM_OF_STONES; p++)
    {
        s_r[p].putInOpenSpot(s_r,p,NUM_OF_STONES);
    }
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
        for (int l=0; l<4; l++)
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
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            Play_Mode = 'T';
            if (last_Mode != Play_Mode)
            {
                last_Mode = Play_Mode;
                Changed_Mode = true;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            Play_Mode = 'B';
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

        //move stone in Random Mode
        if (Play_Mode=='R')
        {
            Changed_Mode =false;
            for (int r=0; r<NUM_OF_STONES; r++)
            {
                app.draw(s_r[r]);
                s_r[r].makeMove();
                for (int rCheck=0; rCheck<=CHECK; rCheck++)
                {
                    if(!s_r[r].isWithinWindow(window_size))
                    {
                        s_r[r].setPosition(rand()%400+50,rand()%400+50);
                        s_r[r].putInOpenSpot(s_r,r,NUM_OF_STONES);
                    }
                    for (int g=0; g<NUM_OF_STONES; g++)
                    {
                        if (g!=r)
                        {
                            if (s_r[r].checkStoneCollision(s_r[g]))
                            {
                                sf::Vector2f r_v=s_r[r].getVelocity();
                                s_r[r].setVelocity_s(s_r[g].getVelocity(), s_r[g].getPosition());
                                s_r[g].setVelocity_s(r_v, s_r[r].getPosition());

                            }
                            while (s_r[r].checkStoneCollision(s_r[g]))
                            {
                                s_r[r].makeMove(CHECK);
                                s_r[g].makeMove(CHECK);
                            }
                        }
                    }
                }
            }
        }
        //move stone in Bouce Mode
        else if (Play_Mode=='B')
        {
            Changed_Mode =false;
            for (int b=0; b<NUM_OF_STONES; b++)
            {
                app.draw(s_b[b]);
                s_b[b].makeMove();
                s_b[b].friction();
                for (int bCheck=0; bCheck<=CHECK; bCheck++)
                {
                    if(s_b[b].checkWallCollision(window_size))
                    {
                        s_b[b].setVelocity_w(window_size);
                    }
                    while (s_b[b].checkWallCollision(window_size))
                    {
                        s_b[b].makeMove(CHECK);
                    }
                    for (int g=0; g<NUM_OF_STONES; g++)
                    {
                        if (g!=b)
                        {
                            if (s_b[b].checkStoneCollision(s_b[g]))
                            {
                                sf::Vector2f b_v=s_b[b].getVelocity();
                                s_b[b].setVelocity_s(s_b[g].getVelocity(), s_b[g].getPosition());
                                s_b[g].setVelocity_s(b_v, s_b[b].getPosition());
                            }
                            while (s_b[b].checkStoneCollision(s_b[g]))
                            {
                                s_b[b].makeMove(CHECK);
                                s_b[g].makeMove(CHECK);
                                //cout << "stone" << endl;
                                //cout << s_b[b].getPosition().x << "," << s_b[b].getPosition().y << endl;
                            }
                        }
                    }
                }
            }
        }
        //move stone in Test Mode
        else if (Play_Mode =='T')
        {
            if (Changed_Mode)
            {
                s_t[0].setPosition(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
                s_t[1].setPosition(WINDOW_WIDTH/2,37);
                s_t[0].setInitialVelocity(0.f,0.f);
                s_t[1].setInitialVelocity(0+randMToN(-.2,.2),(10.0/6.0)+randMToN(-.2,.2));
                Changed_Mode = false;
            }

            for (int t=0; t<2; t++)
            {
                app.draw(s_t[t]);
                s_t[t].makeMove();
            //}
                for (int tcheck=0; tcheck<CHECK; tcheck++)
                {
                    if(s_t[t].checkWallCollision(window_size))
                    {
                        s_t[t].setVelocity_w(window_size);
                    }
                    while (s_t[t].checkWallCollision(window_size))
                    {
                        s_t[t].makeMove(CHECK);
                    }
                    for (int g=0; g<2; g++)
                    {
                        if (t!=g)
                        {

                            if (s_t[t].checkStoneCollision(s_t[g]))
                            {
                                sf::Vector2f t_v=s_t[t].getVelocity();
                                s_t[t].setVelocity_s(s_t[g].getVelocity(), s_t[g].getPosition());
                                s_t[g].setVelocity_s(t_v, s_t[t].getPosition());
                            }
                            while (s_t[t].checkStoneCollision(s_t[g]))
                            {
                                s_t[t].makeMove(CHECK);
                                s_t[g].makeMove(CHECK);
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
