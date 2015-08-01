//     - The Curling class creates instances of customizable curling games and includes several important data fields that are specific to each Curling game
//       such as team names, current turn/end, and team points, among others.
//     - The Curling class includes various functions enable Curling objects to initialize stone positions/coloring, draw the scoreboard and rink,
//       and determine the winner of each end and game, among others.

#ifndef CURLING_H
#define CURLING_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Stone.h"
#include <string>

using namespace std;


class Curling
{
public:
    // Curling constructor
    // Inputs include (i) Play Type (Play or Train), (ii) Scoring Metrics, (iii) Team Colors, (iv) Team Names
    Curling(int newPlayType, int newNumberToWin, char newScoringType, sf::Color newTeamAColor, sf::Color newTeamBColor, string newTeamAName, string newTeamBName);

    // Curling destructor
     virtual ~Curling();

    // Sets intial positions and fill colors.  Positions and coloring depends on play type (play or train)
    void prepareStones(Stone stone_array[]);

    // Below group of function draws and updates rink, scoreboard, zoom windows, and delivery tips textbox
    void drawRink(sf::CircleShape Targets[], sf::RectangleShape Lines[], sf::CircleShape resting_Spots[], Stone s_b[], int NUM_OF_STONES, sf::Text& gameTypeLabel, sf::Font& font);
    void drawHouseZoom(sf::CircleShape houseTargets[], sf::RectangleShape houseLines[]);
    void updateHouseZoom(const Stone stone_array[], Stone houseStones[], int numberOfStones, sf::CircleShape Target);
    void drawUserInputsZoom(sf::RectangleShape boundaryLines[], const Stone currentStone, Stone& zoomStone, const sf::Sprite arrow, sf::Sprite& arrowZoom, sf::RectangleShape zoomBoundary[]);

    // Find distance between two position points
    float getDistance(sf::Vector2f vector1, sf::Vector2f vector2);

    // Boolean value that describes whether a stone is in the House
    bool inHouse(Stone stone1, sf::CircleShape target);

    // Function find stone within array of stones closest to "button"
    int findClosestStone(Stone stone_array[], int NUM_OF_STONES);

    // Function places a triangle marker over the closest stone to the "button"
    void markClosestStone(sf::CircleShape& triangle, int closestStoneIndex, Stone stone_array[]);

    // Function finds stone closest to click position to determine which stone to sweep
    int getClostest_movingStone(Stone s[], sf::Vector2i mouse_loc, const int N);

    // Belwo functions determine points scored after end is complete, update scoreboard, and determine winner of game
    int findPointsScored(int winner, Stone stone_array[], sf::CircleShape Target);
    int getPoints(Stone winning_team[], const float closest_opponent, sf::CircleShape target);
    int calculatePointsEarned(const int winner, Stone team_even[], Stone team_odd[], sf::CircleShape target);
    void updateScoreboard(int winner, int points, sf::Text& team_A_score, sf::Text& team_B_score,sf::Text& setNumber);
    bool isGameOver();

    // Function determines whether any stones in array of stones are still moving
    bool checkPlay_Status(Stone stone_array[], int numberOfStones);

    // Function determines whether a stone delivery was invalid
    bool inValid_Throw(Stone s);

    // Getters/Setters of data fields
    int getPlayType();
    int getNumberToWin();
    char getScoringType();
    int getTeam_A_Points();
    int getTeam_B_Points();
    sf::Color getTeam_A_Color();
    sf::Color getTeam_B_Color();
    string getTeamAName();
    string getTeamBName();
    int getTurnNumber();
    int getCurrentSet();

    void updateTeam_A_Points(int points);
    void updateTeam_B_Points(int points);
    void setTurnNumber(int newTurnNumber);
    void setCurrentSet(int newCurrentSet);

private:

    // Determines whether Curling game is training (playType = 0) or one-on-one (playType = 1)
    // playType is determined by game mode user chooses in introductory window
    int playType;

    // Represents either (i) the number of sets to be played or (ii) number of points required to win depending
    // "scoringType" data field.
    // "numberToWin" is selected by user in game options window
    int numberToWin;

    // Determines whether game ends after (i) a certain number of sets have been played or (ii) a team has won a certain number of points.
    // "scoringType" is chosen by user in game options window.
    char scoringType;

    // Data fields store points won by each team.  Team points are initized to zero when Curling instance is created.
    int team_A_points;
    int team_B_points;

    // Data fields store team colors.  Team colors can be selected in game options window.
    sf::Color team_A_color;
    sf::Color team_B_color;

    // Data fields store team names.  Team names can be entered in game options window.
    string team_A_name;
    string team_B_name;

    // Data field stores the current turn number/set of the game.  "turnNumber" is intialized to 0 when Curling instance is created.
    int turnNumber;

    // Data field stores the current set of the game.  "currentSet" is intialized to 1 when a Curling instance is created.
    int currentSet;

};

#endif
