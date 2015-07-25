#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <Stone.h>
#include <vector>

#define PI 3.14159265

using namespace std;

const int BOARD_WIDTH = 1250;
const int BOARD_HEIGHT = 165;
const int NUM_OF_STONES = 16;

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

float getDistance(sf::Vector2f vector1, sf::Vector2f vector2)
{
    float x_difference = vector2.x - vector1.x;
    float y_difference = vector2.y - vector1.y;

    return sqrt(x_difference * x_difference + y_difference * y_difference);
}

int findClosestStone(Stone stone_array[], const int NUM_OF_STONES)
{
    // Add closest stone(s) to vector
    sf::Vector2f buttonPosition(180,BOARD_HEIGHT / 2);
    float closest_distance = getDistance(stone_array[0].getPosition(), buttonPosition);
    vector<int> closestStones;
    if(stone_array[0].getStatus())
        closestStones.push_back(0);
    else
        closestStones.push_back(-1);

    for(int i = 1; i < NUM_OF_STONES; i++)
    {
        if(stone_array[i].getStatus())
        {
            float next_distance = getDistance(stone_array[i].getPosition(), buttonPosition);
            if(next_distance == closest_distance)
            {
                if(closestStones[0] == -1)
                {
                    closestStones.clear();
                    closestStones.push_back(i);
                }
                else
                {
                    closestStones.push_back(i);
                }
            }
            else if(next_distance < closest_distance)
            {
                closest_distance = next_distance;
                closestStones.clear();
                closestStones.push_back(i);
            }
        }
    }

    // If tie determine if stones come from same or both teams
    bool team_even = false;
    bool team_odd = false;
    if(closestStones.size() > 1)
    {
        for(int i = 0; i < closestStones.size(); i++)
        {
            if(closestStones[i] % 2 == 0)
                team_even = true;
            else
                team_odd = true;
        }

        if(team_even && team_odd)
        {
            closestStones.clear();
            closestStones.push_back(-1);
        }
    }

    // There was a tie game if -1 is returned
    return closestStones[0];
}

int getPoints(Stone winning_team[], const float closest_opponent)
{
    int points = 0;

    sf::Vector2f buttonPosition(180,BOARD_HEIGHT / 2);
    for(int i = 0; i < 8; i++)
    {
        if(getDistance(winning_team[i].getPosition(), buttonPosition) < closest_opponent)
            points++;
    }
    return points;
}

int calculatePointsEarned(const int winner, Stone team_even[], Stone team_odd[])
{
    int winning_team;
    if(winner % 2 == 0)
        winning_team = 0;
    else
        winning_team = 1;

    // Determine closest distance of losing team
    float closest_opponent;
    sf::Vector2f buttonPosition(180,BOARD_HEIGHT / 2);
    if(winning_team == 0)
        closest_opponent = getDistance(team_odd[findClosestStone(team_odd,8)].getPosition(),buttonPosition);
    else
        closest_opponent = getDistance(team_even[findClosestStone(team_even,8)].getPosition(),buttonPosition);

    // Calculate points earned by winning team
    if(winning_team == 0)
        return getPoints(team_even, closest_opponent);
    else
        return getPoints(team_odd, closest_opponent);
}

int main()
{
    sf::Time t1=sf::seconds(1.0/60.0);
    sf::Time t2;
    sf::Clock myclock;

    // Create the main window
    const int CHECK = 20;
    // Change below constant to update minimum spin
    const float DEGREE_PER_TURN = 10;
    const float TIME_PER_TURN = 10;
    const float MIN_SPIN = DEGREE_PER_TURN * (PI / 180) / 8 / 60;

    bool Stone_inPlay = false;
    int Stone_turn = 0;

    char Play_Mode = 'B';
    char last_Mode = 'B';
    bool Changed_Mode = false;

    sf::RenderWindow app(sf::VideoMode(1400, 600), "SFML window");
    sf::RenderWindow houseZoom(sf::VideoMode(350, 350), "House Zoom");
    const sf::Vector2i houseZoomLocation(app.getPosition().x + 500,app.getPosition().y + 170);
    houseZoom.setPosition(houseZoomLocation);

    //Create board
    // Create House
    sf::CircleShape Targets[4];
    float radius[4] = {60,40,20,5};
    sf::Color T[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};

    // Create Boundary Lines
    sf::RectangleShape lines[7];
    sf::Vector2f Lpos[7] = {sf::Vector2f(0,BOARD_HEIGHT/2 - 0.5),sf::Vector2f(390-2,0),sf::Vector2f(180 -.5,0),sf::Vector2f(1110-2,0),sf::Vector2f(0,BOARD_HEIGHT - 4),sf::Vector2f(240,BOARD_HEIGHT/2+15),sf::Vector2f(240,BOARD_HEIGHT/2-15)};
    sf::Vector2f Lsize[7] = {sf::Vector2f(1110,1),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(1,BOARD_HEIGHT),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(BOARD_WIDTH,4),sf::Vector2f(1110-240,.5),sf::Vector2f(1110-240,.5)};
    for (int i=0; i<4; i++)
    {
        Targets[i].setRadius(radius[i]);
        Targets[i].setOrigin(radius[i],radius[i]);
        Targets[i].setFillColor(T[i]);
        Targets[i].setPosition(180,BOARD_HEIGHT/2);
        Targets[i].setOutlineColor(sf::Color::Black);
        Targets[i].setOutlineThickness(-2.0);
    }
    for (int i=0; i<7; i++)
    {
        lines[i].setSize(Lsize[i]);
        lines[i].setFillColor(sf::Color::Black);
        lines[i].setPosition(Lpos[i]);
    }

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
    clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT / 2);

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
    counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT / 2);

    int spinCounter = 0;


    //Create stone array
    Stone s_b[NUM_OF_STONES];


    //sf::Vector2u window_size = app.getSize();
    sf::Vector2u window_size;
    window_size.x = BOARD_WIDTH;
    window_size.y = BOARD_HEIGHT;

    // Initialize Scoreboard Data
    int winnerDeclaredCounter = 0;
    int points_to_win = 4;
    int team_A_points = 0;
    int team_B_points = 0;

	// Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                app.close();
                houseZoom.close();
            }
        }

        // Clear screen
        if (Play_Mode != 'P')
        {
            app.clear(sf::Color(255,255,255));
            houseZoom.clear(sf::Color::White);
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
                arrow.scale(0.95,1.0);
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
            houseZoom.draw(Targets[t]);
        }

        // Draw boundary lines
        for (int l=0; l<7; l++)
        {
            app.draw(lines[l]);
            houseZoom.draw(lines[l]);
        }

        // Draw stone array
        for (int s=0; s<16; s++)
        {
            app.draw(s_b[s]);
            houseZoom.draw(s_b[s]);
        }

        // Create a View
        const sf::Vector2f buttonLocation(180,BOARD_HEIGHT / 2);
        const sf::Vector2f viewSize(120,120);
        sf::View view(buttonLocation,viewSize);
        view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
        houseZoom.setView(view);

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
            if (Stone_turn < NUM_OF_STONES)
            {
                // Begin turn by setting initial speed, direction, and speed to values specified by user
                s_b[Stone_turn].changeStatus();

                //Stone_inPlay = true;
                s_b[Stone_turn].setPosition(1110,BOARD_HEIGHT/2);
                s_b[Stone_turn].setInitialSpeed(arrow.getScale().x*50);
                s_b[Stone_turn].setSpin(spinCounter * MIN_SPIN);

                //cout << arrow.getScale().x << endl;

                if(arrow.getRotation() <= 45)
                {
                    s_b[Stone_turn].setInitialDirection((180 + arrow.getRotation())*PI/180);
                }
                else
                {
                    s_b[Stone_turn].setInitialDirection((180 - (360 - arrow.getRotation()))*PI/180);
                }
                Stone_turn++;
                arrow.setRotation(0);

                // Return spin icons to default settings
                spinCounter = 0;
                clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT/2);
                counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT/2);

                // Return arrow to normal length
                arrow.setScale(0.1,0.1);
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


        // Deliver stone with speed, direction, and spin as specified by user and check for collisions
        if (Play_Mode=='B')
        {
            Changed_Mode =false;
            for (int b=0; b<NUM_OF_STONES; b++)
            {
                //app.draw(s_b[b]);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    s_b[b].setFriction(0.1*5*9.81*.0168/60);
                    app.draw(sweep_message);
                }
                else
                {
                    s_b[b].setFriction();
                }
                s_b[b].setSpeed();
                s_b[b].setDirection();
                s_b[b].setVelocity();
                s_b[b].makeMove();
            }


            //while(s_b[b].checkWallCollision(window_size))


            /*for(int i = 0; i < 16; i++)
            {
                cout << "Stone Number: " << i << " ";
                cout << "Postion: " << s_b[i].getPosition().x << " " << s_b[i].getPosition().y << endl;
            }*/
            // Check for stone collisions
            int collisionStones[2];
            while(Stone::isCollision(s_b,NUM_OF_STONES,collisionStones, window_size))
            {
                //cout << "Collison!" << endl;
                //cout << collisionStones[0] << " " << collisionStones[1] << endl;

                // Find and set new velocities post collision for stones that collided
                sf::Vector2f newStone1Velocity = s_b[collisionStones[0]].findVelocityPostCollision(s_b[collisionStones[1]]);
                sf::Vector2f newStone2Velocity = s_b[collisionStones[1]].findVelocityPostCollision(s_b[collisionStones[0]]);

                // Update velocities for colliding stones
                s_b[collisionStones[0]].updatePostCollision(newStone1Velocity);
                s_b[collisionStones[1]].updatePostCollision(newStone2Velocity);

                while(Stone::isCollision(s_b[collisionStones[0]],s_b[collisionStones[1]]))
                {
                    s_b[collisionStones[0]].makeMove();
                    s_b[collisionStones[1]].makeMove();
                }
            }

            int stoneNumber;
            while(Stone::isWallCollision(s_b,NUM_OF_STONES,stoneNumber, window_size))
            {
                s_b[stoneNumber].setFillColor(sf::Color::White);
                s_b[stoneNumber].setOutlineColor(sf::Color::White);
                s_b[stoneNumber].setInitialSpeed(0);
                s_b[stoneNumber].changeStatus();
                s_b[stoneNumber].setPosition(-5,-5);
            }

            // Add sign above closest stone
            if(!checkPlay_Status(s_b,NUM_OF_STONES))
            {
                int closest = findClosestStone(s_b,NUM_OF_STONES);
                if(closest != -1)
                {
                    sf::CircleShape triangle(5,3);
                    triangle.setOrigin(triangle.getRadius(), triangle.getRadius());
                    triangle.rotate(180);
                    if(closest % 2 == 0)
                        triangle.setFillColor(sf::Color::Green);
                    else
                        triangle.setFillColor(sf::Color::Yellow);
                    triangle.setPosition(s_b[closest].getPosition().x, s_b[closest].getPosition().y - s_b[closest].getRadius() - triangle.getRadius() - 2);
                    app.draw(triangle);
                    houseZoom.draw(triangle);
                }
            }

            // Determine winner of end and number of points earned if finished end
            if(Stone_turn == NUM_OF_STONES && !checkPlay_Status(s_b,NUM_OF_STONES) && winnerDeclaredCounter == 0)
            {
                winnerDeclaredCounter++;
                // Determine winner
                int winner = findClosestStone(s_b,NUM_OF_STONES);

                // Determine number of points won if not a tie game

                int points;
                if(winner != -1)
                {
                    // Split up teams to find number of points earned
                    Stone team_even[8];
                    Stone team_odd[8];
                    int evenCount = 0;
                    int oddCount = 0;
                    for(int i = 0; i < 16; i++)
                    {
                        if(i % 2 == 0)
                        {
                            team_even[evenCount] = s_b[i];
                            evenCount++;
                        }
                        else
                        {
                            team_odd[oddCount] = s_b[i];
                            oddCount++;
                        }
                    }

                    points = calculatePointsEarned(winner,team_even, team_odd);
                }


                if(winner == -1)
                    cout << "Tie game! No points earned." << endl;
                else if(winner % 2 == 0)
                {
                    team_A_points += points;
                    cout << "Team A wins " << points << " points!" << endl;
                }
                else
                {
                    team_B_points += points;
                    cout << "Team B wins " << points << " points!" << endl;
                }
                // Display current score
                cout << "Current Score: " << "Team A - " << team_A_points << " " << "Team B - " << team_B_points << endl;
            }

            // Begin new end if neither player has reached required number of points
            if(Stone_turn == NUM_OF_STONES && !checkPlay_Status(s_b,NUM_OF_STONES) && team_A_points < points_to_win && team_B_points < points_to_win)
            {
                Stone::resetNumberofStones();
                Stone newStoneSet[NUM_OF_STONES];
                for(int i = 0; i < 16; i++)
                    s_b[i] = newStoneSet[i];
                Stone_turn = 0;
                winnerDeclaredCounter = 0;
                //s_b[0].setFillColor(sf::Color::Green);
                //s_b[1].setFillColor(sf::Color::Yellow);
                //for(int i = 0; i < 16; i++)
                    //cout << s_b[i].getPosition().x << " " << s_b[i].getPosition().y << " " << s_b[i].getFillColor().r.toInteger() << " " << s_b[i].getFillColor().g << " " << s_b[i].getFillColor().b << endl;
            }

                /*for (int bCheck=0; bCheck<=CHECK; bCheck++)
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
                }*/



        /*
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
        }*/

        }
        // Update the window
        app.display();
        houseZoom.display();
        sf::sleep(t1);
        t2=myclock.getElapsedTime();
        // std::cout << t2.asSeconds() << std::endl;

    }

    return EXIT_SUCCESS;
}
