#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <Stone.h>

#define PI 3.14159265

using namespace std;

bool checkPlay_Status(Stone s[], int n)
{
    for (int i=0; i<n; i++)
    {
        if (s[i].getSpeed() != 0)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    sf::Time t1=sf::seconds(1.0/60.0);
    sf::Time t2;
    sf::Clock myclock;

    // Create the main window
    const int BOARD_WIDTH = 1200;
    const int BOARD_HEIGHT = 165;
    const int NUM_OF_STONES = 16;
    const int CHECK = 20;
    // Change below constant to update minimum spin
    const float DEGREE_PER_TURN = 10;
    const float TIME_PER_TURN = 10;
    const float MIN_SPIN = DEGREE_PER_TURN * (PI / 180) / 8 / 60;

    bool Stone_inPlay = false;
    int Stone_turn = -1;

    char Play_Mode = 'B';
    char last_Mode = 'B';
    bool Changed_Mode = false;

    sf::RenderWindow app(sf::VideoMode(1400, 500), "SFML window");

    //Create board
    sf::CircleShape Targets[4];
    float radius[4] = {60,40,20,5};
    sf::Color T[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};
    sf::RectangleShape lines[7];
    sf::Vector2f Lpos[7] = {sf::Vector2f(0,BOARD_HEIGHT/2-.5),sf::Vector2f(390-2,0),sf::Vector2f(180-.5,0),sf::Vector2f(1110-2,0),sf::Vector2f(0,BOARD_HEIGHT),sf::Vector2f(240,BOARD_HEIGHT/2+15),sf::Vector2f(240,BOARD_HEIGHT/2-15)};
    sf::Vector2f Lsize[7] = {sf::Vector2f(1110,1),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(1,BOARD_HEIGHT),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(BOARD_WIDTH,4),sf::Vector2f(1110-240,.5),sf::Vector2f(1110-240,.5)};
    for (int i=0; i<4; i++)
    {
        Targets[i].setRadius(radius[i]);
        Targets[i].setOrigin(radius[i],radius[i]);
        Targets[i].setFillColor(T[i]);
        Targets[i].setPosition(180,BOARD_HEIGHT/2);
        Targets[i].setOutlineColor(sf::Color::Black);
        Targets[i].setOutlineThickness(2.0);
    }
    for (int i=0; i<7; i++)
    {
        lines[i].setSize(Lsize[i]);
        lines[i].setFillColor(sf::Color::Black);
        lines[i].setPosition(Lpos[i]);
    }

    //Create stone array
    Stone s_b[NUM_OF_STONES];

    //Sweeping sign
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf"))
    {
        return EXIT_FAILURE;
    }
    sf::Text sweep_message("Sweeping",font,15);
    sweep_message.setPosition(1000,145);
    sweep_message.setColor(sf::Color::Red);

    //launch arrow
    sf::Texture texture;
    if (!texture.loadFromFile("arrow.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Sprite arrow(texture);
    arrow.scale(0.1,0.1);
    float arrow_w = arrow.getTextureRect().width;
    float arrow_h = arrow.getTextureRect().height;
    arrow.setOrigin(arrow_w,arrow_h/2-5);
    arrow.setPosition(1110,BOARD_HEIGHT/2);


    // Launch rotation icons
    sf::Texture clockwiseTexture;
    if (!clockwiseTexture.loadFromFile("clockwise_v2.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Sprite clockwiseArrow(clockwiseTexture);
    clockwiseArrow.scale(0.2,0.2);
    float clockwise_w = clockwiseArrow.getTextureRect().width;
    float clockwise_h = clockwiseArrow.getTextureRect().height;
    clockwiseArrow.setOrigin(clockwise_w / 2,clockwise_h / 2);
    clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT/2);

    sf::Texture counterClockwiseTexture;
    if (!counterClockwiseTexture.loadFromFile("counterclockwise_v2.png"))
    {
        return EXIT_FAILURE;
    }
    sf::Sprite counterClockwiseArrow(counterClockwiseTexture);
    counterClockwiseArrow.scale(0.1,0.1);
    float counterClockwise_w = counterClockwiseArrow.getTextureRect().width;
    float counterClockwise_h = counterClockwiseArrow.getTextureRect().height;
    counterClockwiseArrow.setOrigin(counterClockwise_w / 2,counterClockwise_h / 2);
    counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT/2);
    int spinCounter = 0;




    //sf::Vector2u window_size = app.getSize();
    sf::Vector2u window_size;
    window_size.x = BOARD_WIDTH;
    window_size.y = BOARD_HEIGHT;

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
        Stone_inPlay = checkPlay_Status(s_b, NUM_OF_STONES);

        if (!Stone_inPlay)
        {
            // Draw array with length and direction as specified by user
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (arrow.getRotation() > 315 || arrow.getRotation() <= 45))
            {
                arrow.rotate(-1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (arrow.getRotation() >= 315 || arrow.getRotation() < 45))
            {
                arrow.rotate(1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && arrow.getScale().x < .2)
            {
                arrow.scale(1.05,1.0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && arrow.getScale().x > .05)
            {
                arrow.scale(.95,1.0);
            }
            app.draw(arrow);

            // Draw and rotate rotation icon per magnitude of spin chosen by user
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::J))
            {
                if(spinCounter <= 3)
                {
                    spinCounter++;
                    clockwiseArrow.setPosition(clockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 12);
                    counterClockwiseArrow.setPosition(counterClockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 10);
                }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                if(spinCounter >= -3)
                {
                    spinCounter--;
                    counterClockwiseArrow.setPosition(counterClockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 12);
                    clockwiseArrow.setPosition(clockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 10);
                }
            }

            if(spinCounter >= 0)
            {
                app.draw(clockwiseArrow);
                clockwiseArrow.rotate(spinCounter * 5);
            }
            else
            {
                app.draw(counterClockwiseArrow);
                counterClockwiseArrow.rotate(spinCounter * 5);
            }
        }

        // Draw House
        for (int t=0; t<4; t++)
        {
            app.draw(Targets[t]);
        }

        // Draw boundary lines
        for (int l=0; l<7; l++)
        {
            app.draw(lines[l]);
        }

        // Draw stone array
        for (int s=0; s<16; s++)
        {
            app.draw(s_b[s]);
        }


        // game mode detection
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            Play_Mode = 'B';
            if (last_Mode != Play_Mode)
            {
                last_Mode = Play_Mode;
                Changed_Mode = true;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !checkPlay_Status(s_b,16))
        {
            if (Stone_turn < 16)
            {
                // Begin turn by setting initial speed, direction, and speed to values specified by user
                Stone_turn++;
                //Stone_inPlay = true;
                s_b[Stone_turn].setPosition(1110,BOARD_HEIGHT/2);
                s_b[Stone_turn].setInitialSpeed(arrow.getScale().x*50);
                s_b[Stone_turn].setSpin(spinCounter * MIN_SPIN);

                //cout << arrow.getScale().x << endl;

                if(arrow.getRotation() <= 45)
                {
                    s_b[Stone_turn].setInitialDirection(-1*arrow.getRotation()*PI/180);
                }
                else
                {
                    s_b[Stone_turn].setInitialDirection((360 - arrow.getRotation())*PI/180);
                }
                arrow.setRotation(0);

                // Return spin icons to default settings
                spinCounter = 0;
                clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT/2);
                counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT/2);
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            Play_Mode = 'P';
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
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    s_b[b].setFriction(0.1*5*9.81*.0168/60);
                    app.draw(sweep_message);
                }
                else
                {
                    s_b[b].setFriction();
                }
                //s_b[b].setSpin();
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
                    for (int g=0; g<Stone_turn; g++)
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
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                {
                    Play_Mode ='B';
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
