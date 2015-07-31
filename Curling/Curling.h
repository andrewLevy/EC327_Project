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
    Curling(int newPlayType, int newPointsToWin, char newPointType, sf::Color newTeamAColor, sf::Color newTeamBColor, string newTeamAName, string newTeamBName);
     virtual ~Curling();

    void prepareStones(Stone stone_array[]);

    void drawRink(sf::CircleShape Targets[], sf::RectangleShape Lines[], sf::CircleShape resting_Spots[], Stone s_b[], int NUM_OF_STONES, sf::Text& gameTypeLabel, sf::Font& font);
    void drawHouseZoom(sf::CircleShape houseTargets[], sf::RectangleShape houseLines[]);
    void drawScoreboard(sf::RectangleShape sb[], sf::Text sb_Text[], sf::Vector2f sb_size, sf::Font font);
    void updateHouseZoom(const Stone stone_array[], Stone houseStones[], int numberOfStones, sf::CircleShape Target);
    void drawUserInputsZoom(sf::RectangleShape boundaryLines[], const Stone currentStone, Stone& zoomStone, const sf::Sprite arrow, sf::Sprite& arrowZoom, sf::RectangleShape zoomBoundary[]);

    float getDistance(sf::Vector2f vector1, sf::Vector2f vector2);
    bool inHouse(Stone stone1, sf::CircleShape target);
    int findClosestStone(Stone stone_array[], int NUM_OF_STONES);
    int getClostest_movingStone(Stone s[], sf::Vector2i mouse_loc, const int N);

    void markClosestStone(sf::CircleShape& triangle, int closestStoneIndex, Stone stone_array[]);
    int findPointsScored(int winner, Stone stone_array[], sf::CircleShape Target);
    int getPoints(Stone winning_team[], const float closest_opponent, sf::CircleShape target);
    int calculatePointsEarned(const int winner, Stone team_even[], Stone team_odd[], sf::CircleShape target);
    void updateScoreboard(int winner, int points, sf::Text& team_A_score, sf::Text& team_B_score);
    string Winner();

    bool checkPlay_Status(Stone stone_array[], int numberOfStones);
    bool inValid_Throw(Stone s);

    int getPlayType();
    int getPointsToWin();
    char getPointType();
    int getTeam_A_Points();
    int getTeam_B_Points();
    sf::Color getTeam_A_Color();
    sf::Color getTeam_B_Color();
    string getTeamAName();
    string getTeamBName();

    void updateTeam_A_Points(int points);
    void updateTeam_B_Points(int points);

private:
    int playType;
    int pointsToWin;
    char PointType;
    int team_A_points;
    int team_B_points;
    sf::Color team_A_color;
    sf::Color team_B_color;
    string team_A_name;
    string team_B_name;

};

#endif
