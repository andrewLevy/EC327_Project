// Submitted by Avi Klausner and Andrew Levy

// The following program simulates a Curling game.  The program has two modes: (i) a "one-on-one" two player game and (ii) a "training" single user game.
// The programs uses two custom classes: (a) Stone and (b) Curling
//      (a) Stone
//       - The Stone class creates "Stones" that extend the "sf::CircleShape" class to include speed, direction, spin, and friction among other data fields.
//       - The Stone class incorporates various functions including methods to search for collisions with walls and other Stones and then react appropriately.
//      (b) Curling
//       - The Curling class creates instances of curling games and includes several data fields that are specific to each curling game
//         such as game mode, team names, current turn, and team points, among others.
//       - The Curling class includes various functions including proving initial positioning/coloring to stones, drawing the scoreboard and rink,
//         and determining the winner of each end and game.

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "Curling.h"
#include <Stone.h>
#include <vector>
#include <string>

#define PI 3.14159265

using namespace std;

const int WINDOW_WIDTH = 1325;
const int WINDOW_HEIGHT = 600;

const int BOARD_WIDTH = 1250;
const int BOARD_HEIGHT = 165;
bool Program_on = true;

// Below functions launch an introductory animation and enable the user to specify various options for the curling game,
// including play type (game/training),team names, stone colors, and scoring type (# sets to play or # points to win).
// The "menu_launch" function returns an instance of the Curling game class that includes the options specified by the user.
Curling menu_launch();
void options_launch(sf::RenderWindow& menu, sf::Font font, int numberOfTeams, sf::CircleShape colorChoices[], Stone stoneColorPreviews[],sf::RectangleShape textEntryCells[], sf::Text userNames[], string newTeamAName, string newTeamBName, sf::CircleShape scoreButtons[], int pointsSelect,sf::RectangleShape scoreCells[], char& scoreType);
float getDistance(sf::Vector2f vector1, sf::Vector2f vector2);
bool isColorPressed(sf::Vector2f mouseClickPosition, sf::CircleShape colorChoices[], sf::Color& selectedColor);

int main()
{
    // The "Program_on" boolean determines whether the program should continue to run
    // If "Program_on" is true the menu window will continue to run regardless of whether a Curling game is ongoing.
    while (Program_on)
    {




        sf::Music olympics_music;
        if (!olympics_music.openFromFile("Olympics.ogg"))
        {
            return EXIT_FAILURE;
        }

        sf::Music winner_music;
        if (!winner_music.openFromFile("Ta_Da.ogg"))
        {
            return EXIT_FAILURE;
        }

        sf::Music collision_music;
        if (!collision_music.openFromFile("collide_short.ogg"))
        {
            return EXIT_FAILURE;
        }

        sf::Music sweeping_music;
        if (!sweeping_music.openFromFile("sweeping_cont.ogg"))
        {
            return EXIT_FAILURE;
        }


        // Play the music
        olympics_music.play();

        Curling game = menu_launch();

        // If the Curling instance return by the "menu_launch" function has a play type other than "0" (training mode)
        // or "1" (play mode), the Program_on boolean will become false and the program will end
        if (game.getPlayType() == 2)
        {
            continue;
        }
        else if (game.getPlayType() == 3)
        {
            break;
        }

        // The number of stones to be included in each Curling game depends on the play type of each Curling instance.
        // Play mode involves two teams so requires 16 stones.  Training mode involves a single user so only requires 8 stones.
        int NUM_OF_STONES;
        if(game.getPlayType() == 0)
            NUM_OF_STONES = 8;
        else
            NUM_OF_STONES = 16;

        // Create an array of either 8 or 16 depending on play type (training vs play)
        Stone s_b[NUM_OF_STONES];

        // "prepareStones" initializes stone position and coloring (based on user selection)
        game.prepareStones(s_b);

        sf::Time t1=sf::seconds(1.0/60.0);
        sf::Time t2;
        sf::Time t3;
        sf::Time t_sweep[2];
        sf::Clock myclock;


        // Below constants define the change in spin for every press of spin GUI
        const float DEGREE_PER_TURN = 15;
        const float TIME_PER_TURN = 10;
        const float FRAMES_PER_SECOND = 60;
        const float MIN_SPIN = DEGREE_PER_TURN * (PI / 180) / TIME_PER_TURN / FRAMES_PER_SECOND;

        bool Stone_inPlay = false;


        char Play_Mode = 'B';
        char last_Mode = 'B';
        bool Changed_Mode = false;

        // "lastThrow" boolean describes whether a throw was legible or not
        bool lastThrow = true;


        int setNum = 0;
        int clockCounter = 0;

        // Each Curling game instance will be displayed on the "app" window.
        sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML window");


        sf::Font font;
        if (!font.loadFromFile("sansation.ttf"))
        {
            return EXIT_FAILURE;
        }

        // Use the "drawRink" function within the Curling glass to create the House, boundary lines, initial placement circles,
        // and label identifying play type (practice or one-on-one)
        sf::CircleShape Targets[4];
        sf::RectangleShape lines[8];
        sf::CircleShape resting_Spots[NUM_OF_STONES];
        sf::Text gameTypeLabel;
        game.drawRink(Targets,lines, resting_Spots,s_b,NUM_OF_STONES,gameTypeLabel,font);

        // Use "drawHouseZoom" function from Curling class to draw zoomed in view of House.
        //  The zoom of the House helps users form better strategies by providing a larger view of the stones currently residing in the House.
        sf::CircleShape houseTargets[4];
        sf::RectangleShape houseLines[2];
        game.drawHouseZoom(houseTargets,houseLines);

        // Create House close-up label
        sf::Text houseZoomLabel;
        houseZoomLabel.setFont(font);
        houseZoomLabel.setString("House Close-Up");
        houseZoomLabel.setCharacterSize(15);
        houseZoomLabel.setColor(sf::Color::Black);
        houseZoomLabel.setOrigin(houseZoomLabel.getLocalBounds().width / 2,0);
        houseZoomLabel.setPosition(180,BOARD_HEIGHT + 13);

        // Invalid Throw Message
        sf::Text message("Invalid Delivery",font,15);
        message.setPosition(1000,145);
        message.setColor(sf::Color::Red);

        // Create sweeping image to be used when user sweeps stone
        sf::Texture sweeping_pic;
        if (!sweeping_pic.loadFromFile("sweeping.png"))
        {
            return EXIT_FAILURE;
        }
        sf::Sprite sweeping(sweeping_pic);
        sweeping.scale(0.2,0.2);
        float sweep_w = sweeping.getTextureRect().width;
        float sweep_h = sweeping.getTextureRect().height;
        sweeping.setOrigin(sweep_w,(1/3)*sweep_h);
        sweeping.setPosition(1000,130);
        float ms = 1;
        float pos_sw_x = 0.0;
        float pos_sw_y = 0.0;


        // Create arrow that serves as device for user to set initial speed and direction.
        // The arrow will be drawn on both game board and a separate zoomed in view of user's initial inputs below the game board.
        sf::Texture arrow_pic;
        if (!arrow_pic.loadFromFile("arrow.png"))
        {
            return EXIT_FAILURE;
        }
        sf::Sprite arrow(arrow_pic);
        sf::Sprite arrowZoom(arrow_pic);
        arrow.scale(0.1,0.1);
        arrowZoom.scale(0.1,0.1);
        float arrow_w = arrow.getTextureRect().width;
        float arrow_h = arrow.getTextureRect().height;
        arrow.setOrigin(arrow_w,arrow_h/2-5);
        arrowZoom.setOrigin(arrow_w,arrow_h/2-5);
        arrow.setPosition(1110,BOARD_HEIGHT/2);


        // Create spin icons that display the magnitude and direction of spin chosen by user.
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

        // The magnitude and direction of spin will be determined by a combination of the "spinCounter" and the "MIN_SPIN" constant described above.
        int spinCounter = 0;

        // Create the Scoreboard that includes textboxes that display team names, current number of points, and current set.
        sf::RectangleShape sb[6];
        sf::Text sb_Text[6];

        sb[0].setFillColor(game.getTeam_A_Color());
        sb[1].setFillColor(sf::Color::Black);
        if(game.getPlayType() == 1)
            sb[2].setFillColor(game.getTeam_B_Color());
        else
            sb[2].setFillColor(game.getTeam_A_Color());

        for(int i = 0; i < 6; i++)
        {
            sb_Text[i].setCharacterSize(30);
            sb_Text[i].setFont(font);
            sb_Text[i].setColor(sf::Color::Black);
        }
        sb_Text[0].setString(game.getTeamAName());
        sb_Text[1].setString("Set");
        sb_Text[2].setString(game.getTeamBName());
        if(game.getPlayType() == 0)
        {
            sb_Text[3].setColor(sf::Color::White);
            sb_Text[2].setColor(game.getTeam_A_Color());
            sb_Text[5].setColor(sf::Color::White);
        }



        // Determine longer team name and set size of scoreboard boxes to that size
        sf::Vector2f boxSize;
        if(sb_Text[0].getLocalBounds().width >= sb_Text[2].getLocalBounds().width)
            boxSize = sf::Vector2f(sb_Text[0].getLocalBounds().width + 15,50);
        else
            boxSize = sf::Vector2f(sb_Text[2].getLocalBounds().width + 15,50);

        sb[0].setSize(boxSize);
        sb[2].setSize(boxSize);
        sb[3].setSize(boxSize);
        sb[5].setSize(boxSize);
        sb[1].setSize(sf::Vector2f(100,50));
        sb[4].setSize(sf::Vector2f(100,50));

        // Set properties of boxes
        for (int b=0; b<6; b++)
        {
            sb[b].setOrigin(sb[b].getLocalBounds().width / 2,sb[b].getLocalBounds().height / 2);
            sb[b].setOutlineColor(sf::Color::Black);
            sb[b].setOutlineThickness(-2.0);
            sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getLocalBounds().height/2);

            if (b<3)
            {
                sb[b].setPosition(BOARD_WIDTH / 2 + (b - 1) * (sb[1].getLocalBounds().width / 2 + sb[b].getLocalBounds().width / 2),195);
                sb_Text[b].setPosition(sb[b].getPosition() + sf::Vector2f(0,-7));
            }
            else
            {
                sb[b].setPosition(BOARD_WIDTH / 2 + (b - 4) * (sb[1].getLocalBounds().width / 2 + sb[b].getLocalBounds().width / 2),245);
                sb_Text[b].setPosition(sb[b].getPosition() + sf::Vector2f(0,-7));
                sb_Text[b].setCharacterSize(20);
                sb_Text[b].setString("0");
            }
        }
        sb_Text[1].setColor(sf::Color::White);
        sb_Text[4].setString(to_string(game.getCurrentSet()));

        // Create Winning Message
        sf::Text winning_message("Winner",font,50);
        winning_message.setOrigin(winning_message.getLocalBounds().width / 2, winning_message.getLocalBounds().height / 2);
        winning_message.setPosition(BOARD_WIDTH / 2 - 25,450);
        winning_message.setColor(sf::Color::Black);

        // Create textbox and associated GUI link that provide instructions on how to adjust initial speed, direction, and spin as well as how to sweep
        bool hintsOn = false;

        sf::RectangleShape hintsBox;
        sf::Text hintsText;
        sf::RectangleShape hintsGUIBox;
        sf::Text hintsGUIText;

        // Set settings for hints text box
        sf::Vector2f hintsBoxLeftCorner (sb[0].getPosition().x - sb[0].getLocalBounds().width / 2, 290);
        hintsBox.setSize(sf::Vector2f(340,280));
        hintsBox.setPosition(hintsBoxLeftCorner);
        hintsBox.setOutlineColor(sf::Color::Black);
        hintsBox.setOutlineThickness(-1);

        string hints("Tips: \n\n - Adjust speed with left/right arrows \n\n - Adjust direction with up/down arrows\n\n - Adjust curvature down/up with \n'A'/'Z' keys \n\n - Launch Stone with Enter or Space \n\n - Sweep individual stone by clicking \nstone on top board");
        hintsText.setString(hints);
        hintsText.setFont(font);
        hintsText.setCharacterSize(18);
        hintsText.setColor(sf::Color::Black);
        hintsText.setPosition(hintsBox.getPosition().x + 10,hintsBox.getPosition().y + 10);

        // Set settings for hints GUI box
        hintsGUIBox.setSize(sf::Vector2f(75,35));
        hintsGUIBox.setPosition(hintsBox.getPosition().x, hintsBox.getPosition().y);
        hintsGUIBox.setOutlineColor(sf::Color::Black);
        hintsGUIBox.setOutlineThickness(-1);

        hintsGUIText.setString("  + Tips");
        hintsGUIText.setFont(font);
        hintsGUIText.setCharacterSize(20);
        hintsGUIText.setColor(sf::Color::Black);
        hintsGUIText.setPosition(hintsGUIBox.getPosition().x, hintsGUIBox.getPosition().y + 4);

        sf::Vector2u window_size;
        window_size.x = BOARD_WIDTH;
        window_size.y = BOARD_HEIGHT;


        int winnerDeclaredCounter = 0;

        // Start the game loop
        while (app.isOpen())
        {

            //Turn off menu music
            olympics_music.stop();
            // Process events
            sf::Event event;
            while (app.pollEvent(event))
            {
                // Close window : exit
                if (event.type == sf::Event::Closed)
                {
                    //Menu_open = true;
                    app.close();
                }
            }

            // Clear screen
            if (Play_Mode != 'P')
            {
                app.clear(sf::Color(255,255,255));
            }


            Stone_inPlay = game.checkPlay_Status(s_b, NUM_OF_STONES);
            sf::RectangleShape boundaryLines[4];

            // Initialize the stone to appear in the zoom of initial user input to be the current stone
            Stone zoomStone;
            if(game.getTurnNumber() < NUM_OF_STONES)
                zoomStone = s_b[game.getTurnNumber()];

            // Place next stone in position for delivery, set user inputs including Stone speed, direction, and spin to default settings,
            // and create zoomed in view of user settings if Stone is not in play
            if (!Stone_inPlay)
            {
                // Move current stone to delivery position
                s_b[game.getTurnNumber()].setPosition(1110,BOARD_HEIGHT/2);

                // Draw array with length and direction as specified by user

                // If up/ down arrows are pressed the intial direction arrow moves up/down
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (arrow.getRotation() > 315 || arrow.getRotation() <= 45))
                {
                    arrow.rotate(-.25);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (arrow.getRotation() >= 315 || arrow.getRotation() < 45))
                {
                    arrow.rotate(.25);
                }

                // If the left/right arrows are pressed, the scale of the arrow increases/decreases
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && arrow.getScale().x < .2)
                {
                    arrow.scale(1.01,1.0);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && arrow.getScale().x > .05)
                {
                    arrow.scale(0.99,1.0);
                }
                app.draw(arrow);

                // Draw and rotate rotation icon per magnitude of spin chosen by user
                // User sets magnitude and direction of spin by number of times "F" or "J" keys are pressed
                // "F" key produces a downward curvature and "J" produces and upward curvature
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    while(sf::Keyboard::isKeyPressed(sf::Keyboard::A));
                    if(spinCounter <= 3)
                    {
                        spinCounter++;
                        clockwiseArrow.setPosition(clockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 12);
                        counterClockwiseArrow.setPosition(counterClockwiseArrow.getPosition().x, BOARD_HEIGHT / 2 - spinCounter * 10);
                    }
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                {
                    while(sf::Keyboard::isKeyPressed(sf::Keyboard::Z));
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

                // Display zoomed in view of user inputs
                sf::RectangleShape zoomBoundary[4];
                if(game.getTurnNumber() < NUM_OF_STONES)
                {
                    game.drawUserInputsZoom(boundaryLines,s_b[game.getTurnNumber()], zoomStone, arrow, arrowZoom, zoomBoundary);

                    app.draw(arrowZoom);
                    for(int i = 0; i < 4; i++)
                    {
                        app.draw(boundaryLines[i]);
                        app.draw(zoomBoundary[i]);
                    }
                    app.draw(zoomStone);
                }

                // Create user zoom window label
                sf::Text inputZoomLabel;
                inputZoomLabel.setFont(font);
                inputZoomLabel.setString("Stone Delivery Aid");
                inputZoomLabel.setCharacterSize(15);
                inputZoomLabel.setColor(sf::Color::Black);
                inputZoomLabel.setOrigin(inputZoomLabel.getLocalBounds().width / 2,0);
                inputZoomLabel.setPosition(1065,BOARD_HEIGHT + 13);
                app.draw(inputZoomLabel);
            }

            // Draw House
            app.draw(houseZoomLabel);
            for (int t=0; t<4; t++)
            {
                app.draw(Targets[t]);
                app.draw(houseTargets[t]);
            }

            // Draw boundary lines on game board
            for (int l=0; l<8; l++)
            {
                app.draw(lines[l]);
            }

            // Draw lines in zoom of House
            for(int i = 0; i < 2; i++)
            {
                app.draw(houseLines[i]);
            }

            //Draw Scoreboard
            for (int b=0; b<6; b++)
            {
                app.draw(sb[b]);
                app.draw(sb_Text[b]);
            }

            // Draw play type label
            app.draw(gameTypeLabel);

            // Draw stone array
            for (int s=0; s<NUM_OF_STONES; s++)
            {
                app.draw(resting_Spots[s]);
                app.draw(s_b[s]);
            }

            // Draw hints textbox or hints GUI link depending on position of mouse click
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !game.checkPlay_Status(s_b, NUM_OF_STONES))
            {
                // Only read in one click
                while(sf::Mouse::isButtonPressed(sf::Mouse::Left));

                // Get mouse click position
                sf::Vector2i mouseClickPosition_int = sf::Mouse::getPosition(app);
                sf::Vector2f mouseClickPosition;
                mouseClickPosition.x = mouseClickPosition_int.x;
                mouseClickPosition.y = mouseClickPosition_int.y;

                // Make floating point rectangles for hints textbox and hints GUI textbox
                sf::FloatRect hintsGUIBoundaries = hintsGUIBox.getGlobalBounds();
                sf::FloatRect hintsBoundaries = hintsBox.getGlobalBounds();

                // Update boolean value that decides whether or not hints textbox is displayed if user clicks on GUI link
                if(!hintsOn && hintsGUIBoundaries.contains(mouseClickPosition))
                    hintsOn = true;
                else if (hintsOn && !hintsBoundaries.contains(mouseClickPosition))
                    hintsOn = false;
            }

            // Draw either GUI link or hints textbox
            if(!hintsOn)
            {
                app.draw(hintsGUIBox);
                app.draw(hintsGUIText);
            }
            else
            {
                app.draw(hintsBox);
                app.draw(hintsText);
            }

            // Game mode detection
            // If user presses "P", game pauses
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                Play_Mode = 'P';
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                app.close();
            }

            // Deliver stone with speed, direction, and spin as specified by user and check for collisions
            if (Play_Mode !='P')
            {
                // Check for invalid throws
                if (!lastThrow)
                {
                    app.draw(message);
                }

                // Check for invalid throws
                if (!game.checkPlay_Status(s_b,NUM_OF_STONES))
                {
                    if (game.inValid_Throw(s_b[game.getTurnNumber() - 1]))
                    {
                        lastThrow = false;
                        s_b[game.getTurnNumber() - 1].setPosition(-5,-5);
                    }
                }

                // If user presses Enter/Space, set speed, direction, and spin fields of current stone
                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))) && !game.checkPlay_Status(s_b,NUM_OF_STONES))
                {
                    lastThrow = true;
                    if (game.getTurnNumber() < NUM_OF_STONES)
                    {
                        // Begin turn by changing status to "in play" and setting the initial speed, direction, and speed to values specified by user
                        s_b[game.getTurnNumber()].changeStatus();
                        s_b[game.getTurnNumber()].setInitialSpeed(arrow.getScale().x * 45);
                        s_b[game.getTurnNumber()].setSpin(spinCounter * MIN_SPIN);


                        if(arrow.getRotation() <= 45)
                        {
                            s_b[game.getTurnNumber()].setInitialDirection((180 + arrow.getRotation())*PI/180);
                        }
                        else
                        {
                            s_b[game.getTurnNumber()].setInitialDirection((180 - (360 - arrow.getRotation()))*PI/180);
                        }
                        game.setTurnNumber(game.getTurnNumber() + 1);

                        // Return spin icons to default settings
                        spinCounter = 0;
                        clockwiseArrow.setPosition(1325 - clockwise_w / 2,BOARD_HEIGHT/2);
                        counterClockwiseArrow.setPosition(1393 - counterClockwise_w / 2,BOARD_HEIGHT/2);

                        // Return arrow to normal length and rotation
                        arrow.setScale(0.1,0.1);
                        arrow.setRotation(0);
                    }
                }

                Changed_Mode =false;
                for (int b=0; b<NUM_OF_STONES; b++)
                {
                    // Set friction depending on whether user chooses to sweep stone by clicking
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && game.checkPlay_Status(s_b,NUM_OF_STONES))
                    {
                        if (sweeping_music.getStatus() != sf::SoundSource::Status::Playing)
                        {
                            sweeping_music.play();
                        }
                        int x = game.getClostest_movingStone(s_b,sf::Mouse::getPosition(app),NUM_OF_STONES);
                        s_b[x].setFriction(0.8*5*9.81*.0168/60);
                        pos_sw_x = s_b[x].getPosition().x;
                        pos_sw_y = s_b[x].getPosition().y;

                        app.draw(sweeping);
                        if (t_sweep[0] == sf::seconds(0.0))
                        {
                            t_sweep[0] = myclock.getElapsedTime();
                        }
                        else
                        {
                            t_sweep[1] = myclock.getElapsedTime();
                        }

                        if(t_sweep[1]-t_sweep[0] > sf::seconds(0.15))
                        {
                            t_sweep[0] = t_sweep[1];
                            ms *= -1;
                        }

                        sweeping.setPosition(pos_sw_x-8.0,pos_sw_y-18.0+5.0*ms);
                    }
                    else
                    {
                        sweeping_music.stop();
                        t_sweep[0]=sf::seconds(0.0);
                        t_sweep[1]=sf::seconds(0.0);
                        sweeping.setPosition(0.0,0.0);
                        s_b[b].setFriction();
                    }

                    // Update speed, direction, velocity and then move Stone
                    s_b[b].setSpeed();
                    s_b[b].setDirection();
                    s_b[b].setVelocity();
                    s_b[b].makeMove();
                }

                // Check for stone collisions among stones
                int collisionStones[2];
                bool collide=false;
                while(Stone::isCollision(s_b,NUM_OF_STONES,collisionStones, window_size))
                {

                    if (!collide)
                    {
                        collision_music.play();
                        collide = true;
                    }
                    // Find and set new velocities post collision for stones that collided
                    sf::Vector2f newStone1Velocity = s_b[collisionStones[0]].findVelocityPostCollision(s_b[collisionStones[1]]);
                    sf::Vector2f newStone2Velocity = s_b[collisionStones[1]].findVelocityPostCollision(s_b[collisionStones[0]]);

                    // Update velocities for colliding stones
                    s_b[collisionStones[0]].updatePostCollision(newStone1Velocity);
                    s_b[collisionStones[1]].updatePostCollision(newStone2Velocity);

                    // Move stones with new post_collision velocities while same collision continues
                    while(Stone::isCollision(s_b[collisionStones[0]],s_b[collisionStones[1]]))
                    {
                        s_b[collisionStones[0]].makeMove();
                        s_b[collisionStones[1]].makeMove();
                    }
                }


                // Check for wall collisions
                int stoneNumber;
                while(Stone::isWallCollision(s_b,NUM_OF_STONES,stoneNumber, window_size))
                {
                    s_b[stoneNumber].setFillColor(sf::Color::White);
                    s_b[stoneNumber].setOutlineColor(sf::Color::White);
                    s_b[stoneNumber].setInitialSpeed(0);
                    s_b[stoneNumber].changeStatus();
                    s_b[stoneNumber].setPosition(-5,-5);
                }

                // Display stone in House zoom if applicable
                Stone houseStones[NUM_OF_STONES];
                for(int i = 0; i < NUM_OF_STONES; i++)
                    houseStones[i] = s_b[i];

                game.updateHouseZoom(s_b,houseStones,NUM_OF_STONES,Targets[0]);
                for(int i = 0; i < NUM_OF_STONES; i++)
                {
                    if(game.inHouse(s_b[i],Targets[0]))
                        app.draw(houseStones[i]);
                }

                // Add marker above closest stone if in House on both rink and house zoom
                if(!game.checkPlay_Status(s_b,NUM_OF_STONES))
                {
                    int closest = game.findClosestStone(s_b,NUM_OF_STONES);
                    if(closest != -1 && game.inHouse(s_b[closest], Targets[0]))
                    {
                        sf::CircleShape triangle(5,3);
                        game.markClosestStone(triangle,closest,s_b);
                        app.draw(triangle);

                        sf::CircleShape triangleHouse(2.5 * 5,3);
                        game.markClosestStone(triangleHouse,closest,houseStones);
                        app.draw(triangleHouse);
                    }
                }

                // Determine winner of end and number of points earned if end is complete
                if(!game.checkPlay_Status(s_b,NUM_OF_STONES) && game.getTurnNumber() == NUM_OF_STONES && !game.isGameOver() && winnerDeclaredCounter == 0)
                {
                    // Increment current set number in both play and training modes
                    game.setCurrentSet(game.getCurrentSet() + 1);

                    if(game.getPlayType() == 1)
                    {
                        winnerDeclaredCounter++;
                        // Determine winner of end
                        int winner = game.findClosestStone(s_b,NUM_OF_STONES);

                        // Determine number of points won if not a tie game
                        int points = 0;
                        if(winner != -1 && game.inHouse(s_b[winner], Targets[0]))
                            points = game.findPointsScored(winner,s_b,Targets[0]);

                        // Update scoreboard to reflect updated point totals and net set number
                        game.updateScoreboard(winner,points,sb_Text[3],sb_Text[5],sb_Text[4]);
                    }
                    // Update current set number in training mode if 8 stones have been played
                    else
                        sb_Text[4].setString(to_string(game.getCurrentSet()));
                }

                // Begin new end if game is not over
                if(game.getTurnNumber() == NUM_OF_STONES && !game.checkPlay_Status(s_b,NUM_OF_STONES) && !game.isGameOver())
                {
                    Stone newStoneSet[NUM_OF_STONES];
                    for(int i = 0; i < NUM_OF_STONES; i++)
                        s_b[i] = newStoneSet[i];
                    game.prepareStones(s_b);
                    game.setTurnNumber(0);
                    winnerDeclaredCounter = 0;
                }

                // If game is over, declare winner and then return to menu after pause
                if(game.isGameOver())
                {

                    hintsOn = false;
                    if(game.getTeam_A_Points() > game.getTeam_B_Points())
                        winning_message.setString(game.getTeamAName() + " Wins!");
                    else if (game.getTeam_B_Points() > game.getTeam_A_Points())
                        winning_message.setString(game.getTeamBName() + " Wins!");
                    else
                        winning_message.setString("Tie Game!");
                    app.draw(winning_message);

                    if(clockCounter == 0)
                    {
                        winner_music.play();
                        t3=myclock.restart();
                        clockCounter++;
                    }
                    if(myclock.getElapsedTime().asSeconds() > 4)
                        app.close();
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
    }

    return EXIT_SUCCESS;
}

// "menu_launch" launches the introductory animation and provides GUIs that enable the user to adjust options for the upcoming Curling game including team names, colors, and scoring type.
// The function returns an instance of the Curling game class that includes the settings chosen by the user.
Curling menu_launch()
{
    // The variable menu_mode will determine what appears on the "menu" window.
    //  - menu_mode = 'M': window will show introductory animation
    //  - menu_mode = 'I': window will show options for "Play" mode
    //  - menu_mode = 'T': window will show options for "Training" mode
    char menu_mode = 'M';
    int UI_pt_win = 1;

    sf::Time m1=sf::seconds(1.0/60.0);
    sf::Time m2;
    sf::Clock menu_clock;

    // The introductory animation and Curling game options will be displayed on the "menu" window
    sf::RenderWindow menu(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Menu");

    sf::Font font;
    font.loadFromFile("sansation.ttf");

    // Load Curling image to be displayed in introductory animation
    sf::Texture introImage;
    introImage.loadFromFile("introImage_v2.png");
    sf::Sprite introImageSprite(introImage);
    introImageSprite.setScale(1.75,1.75);
    introImageSprite.setOrigin(introImageSprite.getLocalBounds().width / 2, introImageSprite.getLocalBounds().height / 2);
    introImageSprite.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 70);

    // Create introductory game options
    sf::Text menu_list[3];
    menu_list[0].setString("Play");
    menu_list[1].setString("Train");
    menu_list[2].setString("Quit");

    for (int t=0; t<3; t++)
    {
        menu_list[t].setFont(font);
        menu_list[t].setCharacterSize(30);
        menu_list[t].setColor(sf::Color::Black);
        menu_list[t].setOrigin(menu_list[t].getLocalBounds().width / 2, menu_list[t].getLocalBounds().height / 2);
    }

    menu_list[1].setPosition(WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 + 200);
    menu_list[0].setPosition(menu_list[1].getPosition().x - menu_list[1].getLocalBounds().width / 2 - menu_list[0].getLocalBounds().width /2 - 100,menu_list[1].getPosition().y);
    menu_list[2].setPosition(menu_list[1].getPosition().x + menu_list[1].getLocalBounds().width / 2 + menu_list[2].getLocalBounds().width /2 + 100,menu_list[1].getPosition().y);

    // Set boolean to determine whether initial colors have changed
    bool userSettingsChanged = false;

    // Initialize objects required for user name entry
    sf::RectangleShape textEntryCells[2];
    sf::Text userNames[2];
    int selectedTextBox;

    string teamAName("Enter Team A Name");
    string teamBName("Enter Team B Name");
    string C_instruct("Select Color for Team A");

    // Initialize objects needed to set stone color options
    Stone stoneColorPreviews[2];
    int mouseClickCounter = 0;
    sf::Vector2i mouseClickPosition_int;
    sf::Vector2f mouseClickPosition;
    sf::Color selectedColor;
    sf::CircleShape colorChoices[8];
    sf::RectangleShape scoreCells[2];
    sf::CircleShape scoreButtons[8];
    int pointsSelect = 0;
    char scoreType;

    // Create "Play Now" button.  Button will be used to begin Curling game with specified options.
    sf::Text playNowText;
    playNowText.setString("Play");
    playNowText.setFont(font);
    playNowText.setCharacterSize(30);
    playNowText.setColor(sf::Color::Black);
    playNowText.setOrigin(playNowText.getLocalBounds().width / 2, playNowText.getLocalBounds().height / 2);
    playNowText.setPosition(WINDOW_WIDTH - playNowText.getLocalBounds().width / 2 - 110, WINDOW_HEIGHT - playNowText.getLocalBounds().height / 2 - 100);

    // Create "Back" button.  Button will be used to return to main menu from options window.
    sf::Text backButton;
    backButton.setString("Back");
    backButton.setFont(font);
    backButton.setCharacterSize(30);
    backButton.setColor(sf::Color::Black);
    backButton.setOrigin(backButton.getLocalBounds().width / 2, backButton.getLocalBounds().height / 2);
    backButton.setPosition(playNowText.getPosition().x - playNowText.getLocalBounds().width / 2 - 175,playNowText.getPosition().y);

    sf::Clock introClock;
    sf::Time introTime;
    int timeCounter = 0;


    while (menu.isOpen())
    {
        // Process events
        sf::Event event;
        while (menu.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                Program_on = false;
                menu.close();
            }
        }
        menu.clear(sf::Color(255,255,255));

        // If "menu_mode" = "M", draw introductory animation and play type options
        if (menu_mode == 'M')
        {

            // Draw curling image
            menu.draw(introImageSprite);

            // Set default scoring type to number of ends played
            scoreType = 'E';

            // Create animation
            if(introClock.getElapsedTime().asSeconds() > 0.5)
            {
                if(introClock.getElapsedTime().asSeconds() > 0.5)
                    menu.draw(menu_list[0]);
                if(introClock.getElapsedTime().asSeconds() > 1.0)
                    menu.draw(menu_list[1]);
                if(introClock.getElapsedTime().asSeconds() > 1.5)
                    menu.draw(menu_list[2]);
            }

            // If cursor scrolls over game option type, highlight option
            sf::Vector2i currentMousePosition_int(sf::Mouse::getPosition(menu));
            sf::Vector2f currentMousePosition;
            currentMousePosition.x = currentMousePosition_int.x;
            currentMousePosition.y = currentMousePosition_int.y;

            sf::FloatRect playBox = menu_list[0].getGlobalBounds();
            sf::FloatRect trainingBox = menu_list[1].getGlobalBounds();
            sf::FloatRect quitBox = menu_list[2].getGlobalBounds();

            if(playBox.contains(currentMousePosition))
                menu_list[0].setColor(sf::Color(255,140,0));
            else
                menu_list[0].setColor(sf::Color::Black);

            if(trainingBox.contains(currentMousePosition))
                menu_list[1].setColor(sf::Color(255,140,0));
            else
                menu_list[1].setColor(sf::Color::Black);

            if(quitBox.contains(currentMousePosition))
                menu_list[2].setColor(sf::Color(255,140,0));
            else
                menu_list[2].setColor(sf::Color::Black);


            // If mouse clicked and clicked GUI take appropriate action
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                // Only read in one click
                while(sf::Mouse::isButtonPressed(sf::Mouse::Left));

                // Find where mouse clikced
                sf::Vector2i currentMouseClickPosition_int(sf::Mouse::getPosition(menu));
                sf::Vector2f currentMouseClickPosition;
                currentMouseClickPosition.x = currentMouseClickPosition_int.x;
                currentMouseClickPosition.y = currentMouseClickPosition_int.y;

                // If "Play", "Train", or "Quit" selected change "menu_mode" accordingly
                if(playBox.contains(currentMouseClickPosition))
                    menu_mode = 'I';
                else if(trainingBox.contains(currentMouseClickPosition))
                    menu_mode = 'T';
                else if(quitBox.contains(currentMouseClickPosition))
                {
                    Program_on = false;
                    menu.close();
                    scoreType = 'Q';
                    return Curling(2,UI_pt_win,scoreType,sf::Color::Green,sf::Color::Green, "Team A", "Team B");
                }

            }
            // User can also quit program my pressing "Q"
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                Program_on = false;
                menu.close();
                scoreType = 'Q';
                return Curling(2,UI_pt_win,scoreType,sf::Color::Green,sf::Color::Green, "Team A", "Team B");
            }
            menu.display();

        }
        // Show game options if user clicks on "Play" or "Train" mode
        else if (menu_mode == 'I' || menu_mode == 'T')
        {
            int numberOfTeams = (menu_mode == 'I' ? 2 : 1);
            if(numberOfTeams == 1)
                playNowText.setString("Train");

            // "options_launch" function sets labels, color choices, stone previews, and textboxes on options window
            options_launch(menu,font,numberOfTeams,colorChoices,stoneColorPreviews,textEntryCells,userNames,teamAName,teamBName,scoreButtons,pointsSelect,scoreCells,scoreType);
            UI_pt_win = pointsSelect+1;

            // Create flag if "tab" button pressed
            // User will be able to move from Team A textbox to Team B textbox by pressing "tab" but not from Team B to Team A
            bool tabFlag;

            // Get the global bounds for the two username textboxes to later determine if user clicks into
            sf::FloatRect nameExtryBox1 = textEntryCells[0].getGlobalBounds();
            sf::FloatRect nameEntryBox2 = textEntryCells[1].getGlobalBounds();

            // Set color of preview stones to gray and orange
            if(!userSettingsChanged)
            {
                stoneColorPreviews[0].setFillColor(sf::Color(230,230,230));
                stoneColorPreviews[1].setFillColor(sf::Color(255,140,0));
            }

            // Create text notifying user the team color that can be adjusted
            // The adjustable team color will alternate every time a color options is clicked
            sf::Text selectColorInstruction;
            selectColorInstruction.setString(C_instruct);
            selectColorInstruction.setFont(font);
            selectColorInstruction.setCharacterSize(15);
            selectColorInstruction.setColor(sf::Color::Black);
            selectColorInstruction.setPosition(1000,230);
            menu.draw(selectColorInstruction);

            // Draw "Play" and "Back" buttons
            menu.draw(playNowText);
            menu.draw(backButton);

            // Draw stone color previews
            for(int i = 0; i < 2; i++)
                menu.draw(stoneColorPreviews[i]);
            for(int i = 0; i < 8; i++)
                menu.draw(colorChoices[i]);
            menu.display();

            // If mouse cursor is above "Play" or "Back" buttons highlight the button
            sf::Vector2i currentMousePosition_int_2(sf::Mouse::getPosition(menu));
            sf::Vector2f currentMousePosition_2;
            currentMousePosition_2.x = currentMousePosition_int_2.x;
            currentMousePosition_2.y = currentMousePosition_int_2.y;

            if(playNowText.getGlobalBounds().contains(currentMousePosition_2))
                playNowText.setColor(sf::Color::Green);
            else
                playNowText.setColor(sf::Color::Black);

            if(backButton.getGlobalBounds().contains(currentMousePosition_2))
                backButton.setColor(sf::Color::Red);
            else
                backButton.setColor(sf::Color::Black);

            // If mouse is clicked on GUI take appropriate action
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                // Only read in one click
                while(sf::Mouse::isButtonPressed(sf::Mouse::Left));

                // Get mouse click position and convert to floating point form
                mouseClickPosition_int = sf::Mouse::getPosition(menu);
                mouseClickPosition.x = mouseClickPosition_int.x;
                mouseClickPosition.y = mouseClickPosition_int.y;

                // If color option clicked on, set the current adjustable team color to the chosen color
                if(isColorPressed(mouseClickPosition,colorChoices,selectedColor))
                {
                    userSettingsChanged = true;
                    if(numberOfTeams == 1)
                        stoneColorPreviews[0].setFillColor(selectedColor);
                    else
                    {
                        stoneColorPreviews[mouseClickCounter % 2].setFillColor(selectedColor);
                        if ((mouseClickCounter % 2) > 0)
                            C_instruct = "Select Color for Team A";
                        else
                            C_instruct = "Select Color for Team B";
                        mouseClickCounter++;
                    }
                }
                // Update score type if necessary
                else if(scoreCells[0].getGlobalBounds().contains(mouseClickPosition))
                    scoreType = 'E';
                else if(scoreCells[1].getGlobalBounds().contains(mouseClickPosition))
                    scoreType = 'P';

                // Check if user selected a number of points/ends to play
                for(int i = 0; i < 8; i++)
                {
                    if(getDistance(mouseClickPosition,scoreButtons[i].getPosition()) <= scoreButtons[i].getRadius())
                    {
                        pointsSelect = i;
                    }
                }

                // Boolean flag to enable user to tab from first textbox to second textbox.  Flag resets every click.
                tabFlag = false;
            }

            // Update username if user clicks and types in text box
            if(nameExtryBox1.contains(mouseClickPosition) || nameEntryBox2.contains(mouseClickPosition))
            {
                if(nameExtryBox1.contains(mouseClickPosition) && !tabFlag)
                {
                    selectedTextBox = 0;
                    // Remove pre-filled insructions if necessary
                    if (teamAName == "Enter Team A Name")
                    {
                        teamAName = "";
                    }
                }
                else
                {
                    selectedTextBox = 1;
                    if (teamBName == "Enter Team B Name")
                    {
                        teamBName = "";
                    }
                }

                // If user has clicked on team name textboxes, look for text entered
                while (menu.pollEvent(event))
                {
                    if(event.type == sf::Event::TextEntered)
                    {
                        if(event.text.unicode < 128 && event.text.unicode != 13)
                        {
                            // If user presses tab in Team A textbox, move cursor to Team B textbox
                            if(event.text.unicode == 9 && selectedTextBox == 0 && numberOfTeams == 2)
                                tabFlag = true;

                            if(selectedTextBox == 0)
                            {
                                // Take appropriate action for delete, tab, enter, and other keys
                                if(event.text.unicode == 8 && teamAName.size() > 0)
                                    teamAName.erase(teamAName.size() - 1,1);
                                else if(event.text.unicode != 8 && event.text.unicode != 9 && teamAName.size() < 10)
                                    teamAName.push_back(static_cast<char>(event.text.unicode));
                            }
                            else
                            {
                                if(event.text.unicode == 8 && teamBName.size() > 0)
                                    teamBName.erase(teamBName.size() - 1,1);
                                else if(event.text.unicode != 8 && event.text.unicode != 9 && teamBName.size() < 10)
                                    teamBName.push_back(static_cast<char>(event.text.unicode));
                            }
                        }
                    }
                }
            }

            // If user clicks "Play" return Curling instance with user specified option and begin curling game
            if (playNowText.getGlobalBounds().contains(mouseClickPosition))
            {
                menu.close();
                if(numberOfTeams == 2)
                    return Curling(1,UI_pt_win,scoreType,stoneColorPreviews[0].getFillColor(),stoneColorPreviews[1].getFillColor(),teamAName,teamBName);
                else
                    return Curling(0,UI_pt_win,scoreType,stoneColorPreviews[0].getFillColor(),stoneColorPreviews[1].getFillColor(),teamAName,teamBName);
            }

            // If user clicks "Back" return to main menu
            if (backButton.getGlobalBounds().contains(mouseClickPosition))
            {
                menu.close();
                return Curling(2,UI_pt_win,'M',sf::Color::Green,sf::Color::Green,"Team A", "Team B");
            }
        }

        sf::sleep(m1);
        m2=menu_clock.getElapsedTime();
    }
    return Curling(3,UI_pt_win,'Q',sf::Color::Green,sf::Color::Green,"Team A", "Team B");
}

// Function sets the labels, color options, team name textboxes, stone previews in option windows
void options_launch(sf::RenderWindow& menu, sf::Font font, int numberOfTeams, sf::CircleShape colorChoices[], Stone stoneColorPreviews[],sf::RectangleShape textEntryCells[], sf::Text userNames[], string newTeamAName, string newTeamBName, sf::CircleShape scoreButtons[], int pointsSelect, sf::RectangleShape scoreCells[], char& scoreType)
{
    // Add text labels to page
    sf::Text optionsLabels[5];
    sf::Vector2f labelPositions[5] = {sf::Vector2f(WINDOW_WIDTH / 2 - 75,5),sf::Vector2f(20,15), sf::Vector2f(20,65), sf::Vector2f(20,240), sf::Vector2f(20,415)};
    string labelStrings[5] = {"Game Options", "Enter:", "1) Names", "2) Stone Colors", "3) Scoring Type"};

    int numberOfLabels = (numberOfTeams == 2 ? 5 : 4);
    for(int i = 0; i < numberOfLabels; i++)
    {
        optionsLabels[i].setFont(font);
        optionsLabels[i].setPosition(labelPositions[i]);
        optionsLabels[i].setString(labelStrings[i]);
        optionsLabels[i].setColor(sf::Color::Black);
        optionsLabels[i].setCharacterSize(30);
    }
    if(numberOfTeams == 1)
    {
        optionsLabels[2].setString("1) Name");
        optionsLabels[3].setString("2) Stone Color");
    }
    for(int i = 0; i < numberOfLabels; i++)
        menu.draw(optionsLabels[i]);

    // Display name preview labels
    string namePreviewStrings[2] = {"Team A", "Team B"};
    sf::Text namePreviews[2];
    for(int i = 0; i < numberOfTeams; i++)
    {
        namePreviews[i].setString(namePreviewStrings[i]);
        namePreviews[i].setFont(font);
        namePreviews[i].setCharacterSize(15);
        namePreviews[i].setColor(sf::Color::Black);
        namePreviews[i].setOrigin(namePreviews[i].getLocalBounds().width / 2,0);
        namePreviews[i].setPosition(350 + 250 * i,60);
        menu.draw(namePreviews[i]);
    }

    sf::Vector2f cellSize(200,50);

    userNames[0].setString(newTeamAName);
    userNames[1].setString(newTeamBName);

    for(int i = 0; i < numberOfTeams; i++)
    {
        textEntryCells[i].setSize(cellSize);
        textEntryCells[i].setOrigin(textEntryCells[i].getSize().x / 2,textEntryCells[i].getSize().y / 2);
        textEntryCells[i].setPosition(350 + 250 * i,115);
        textEntryCells[i].setFillColor(sf::Color(160,160,160));
        textEntryCells[i].setOutlineThickness(2);
        textEntryCells[i].setOutlineColor(sf::Color::Black);
        menu.draw(textEntryCells[i]);

        userNames[i].setColor(sf::Color::White);
        userNames[i].setFont(font);
        userNames[i].setCharacterSize(20);
        userNames[i].setOrigin(userNames[i].getLocalBounds().width / 2,userNames[i].getLocalBounds().height / 2);
        userNames[i].setPosition(textEntryCells[i].getPosition());
        menu.draw(userNames[i]);
    }


    // Display color choices
    const int NUMBER_OF_COLORS = 8;
    sf::Color colorOptions[NUMBER_OF_COLORS] = {sf::Color(230,230,230),sf::Color(255,140,0),sf::Color(230,0,0),sf::Color::Green,sf::Color(0,0,230),sf::Color::Yellow,sf::Color::Magenta,sf::Color::Cyan};
    float colorRadius = 25;
    for(int i = 0; i < NUMBER_OF_COLORS; i++)
    {
        colorChoices[i].setFillColor(colorOptions[i]);
        colorChoices[i].setRadius(colorRadius);
        colorChoices[i].setOrigin(colorChoices[i].getRadius(), colorChoices[i].getRadius());
        colorChoices[i].setOutlineColor(sf::Color::Black);
        colorChoices[i].setOutlineThickness(-1);
        colorChoices[i].setPosition(1000 + i % 4 * 2 * (colorChoices[i].getRadius() + 5),300 + i / 4 * 2 * (colorChoices[i].getRadius() + 5));
    }


    // Create labels above preview of stone with user selected color
    string stonePreviewStrings[2] = {"Team A", "Team B"};
    sf::Text stonePreviews[2];
    for(int i = 0; i < numberOfTeams; i++)
    {
        stonePreviews[i].setString(stonePreviewStrings[i]);
        stonePreviews[i].setFont(font);
        stonePreviews[i].setCharacterSize(15);
        stonePreviews[i].setColor(sf::Color::Black);
        stonePreviews[i].setOrigin(stonePreviews[i].getLocalBounds().width / 2,0);
        stonePreviews[i].setPosition(350 + 250 * i,230);
        menu.draw(stonePreviews[i]);
    }

    // Set color preview stones
    sf::Vector2f stonePositions[2] = {sf::Vector2f(350,310),sf::Vector2f(600,310)};
    float stonePreviewRadius = 40;
    for(int i = 0; i < numberOfTeams; i++)
    {
        stoneColorPreviews[i].setRadius(stonePreviewRadius);
        stoneColorPreviews[i].setOutlineThickness(40 / (10*(11.4/12)*.5 + 2) * -2.0);
        stoneColorPreviews[i].setOrigin(stoneColorPreviews[i].getRadius(),stoneColorPreviews[i].getRadius());
        stoneColorPreviews[i].setPosition(stonePositions[i]);
    }

    // Set Scoring options and labels
    if(numberOfTeams == 2)
    {
        sf::Text scoreTextLabels[2];
        scoreTextLabels[0].setString("Ends");
        scoreTextLabels[1].setString("Points");
        sf::Vector2f scoreCellSize(100,40);
        sf::Vector2f scoreCellSize_t(300,40);
        for (int s=0; s<2; s++)
        {
            scoreCells[s].setSize(scoreCellSize);
            scoreCells[s].setOrigin(scoreCellSize.x/2,scoreCellSize.y/2);
            scoreCells[s].setFillColor(sf::Color(160,160,160));
            scoreCells[s].setPosition(450+(s*100),475);
            scoreCells[s].setOutlineThickness(-2);
            if (scoreType == 'P')
                scoreCells[1].setFillColor(sf::Color::Black);
            else
                scoreCells[0].setFillColor(sf::Color::Black);
            scoreCells[s].setOutlineColor(sf::Color::Black);
            scoreTextLabels[s].setFont(font);
            scoreTextLabels[s].setCharacterSize(20);
            scoreTextLabels[s].setOrigin(scoreTextLabels[s].getLocalBounds().width/2,scoreTextLabels[s].getLocalBounds().height/2);
            scoreTextLabels[s].setPosition(scoreCells[s].getPosition().x,scoreCells[s].getPosition().y-5);
            menu.draw(scoreCells[s]);
            menu.draw(scoreTextLabels[s]);
        }

        sf::Text scoreButtonValues[8];
        float buttonRadius = 25.0;
        int valueSelect = pointsSelect;
        for (int b=0; b<8; b++)
        {
            scoreButtons[b].setFillColor(sf::Color(255,255,255));
            scoreButtons[b].setRadius(buttonRadius);
            scoreButtons[b].setOrigin(buttonRadius,buttonRadius);
            scoreButtons[b].setOutlineThickness(-10);
            scoreButtons[b].setOutlineColor(sf::Color(160,160,160));
            if (b==valueSelect)
                scoreButtons[b].setOutlineColor(sf::Color(0,0,0));
            scoreButtons[b].setPosition(300+b*55,560);

            scoreButtonValues[b].setString(to_string(b+1));
            scoreButtonValues[b].setCharacterSize(20);
            scoreButtonValues[b].setFont(font);
            scoreButtonValues[b].setColor(sf::Color::Black);
            scoreButtonValues[b].setOrigin(scoreButtonValues[b].getLocalBounds().width/2,scoreButtons[b].getLocalBounds().height/2);
            scoreButtonValues[b].setPosition(300+b*55,572);
            menu.draw(scoreButtons[b]);
            menu.draw(scoreButtonValues[b]);
        }
        sf::Text scoringInstruction[2];
        scoringInstruction[0].setString("Select a game score type");
        scoringInstruction[1].setString("Select a value to play until");
        for (int si=0; si<2; si++)
        {
            scoringInstruction[si].setColor(sf::Color::Black);
            scoringInstruction[si].setFont(font);
            scoringInstruction[si].setCharacterSize(15);
            scoringInstruction[si].setOrigin(scoringInstruction[si].getLocalBounds().width/2,scoringInstruction[si].getLocalBounds().height/2);
            scoringInstruction[si].setPosition(500,440+si*80);
            menu.draw(scoringInstruction[si]);
        }
    }

}

// Finds distance between two position vectors
float getDistance(sf::Vector2f vector1, sf::Vector2f vector2)
{
    return sqrt((vector2.x - vector1.x) * (vector2.x - vector1.x) + (vector2.y - vector1.y) * (vector2.y - vector1.y));
}

// Check whether the user clikced on a color option by calculating the distance between the click position and the color options
// If color option clicked, assigns color to the "selectedColor" variable that was passed by reference.
bool isColorPressed(sf::Vector2f mouseClickPosition, sf::CircleShape colorChoices[], sf::Color& selectedColor)
{
    for(int i = 0; i < 8; i++)
    {
        if(getDistance(mouseClickPosition,colorChoices[i].getPosition()) <= colorChoices[i].getRadius())
        {
            selectedColor = colorChoices[i].getFillColor();
            return true;
        }
    }
    return false;
}


