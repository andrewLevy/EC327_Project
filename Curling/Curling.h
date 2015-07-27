#ifndef CURLING_H
#define CURLING_H

#include <SFML/Graphics.hpp>
#include <Stone.h>

class Curling
{
public:
    Curling(int newPlayType, int newPointsToWin);

    void prepareStones(Stone stone_array[]);
    void drawRink(sf::CircleShape Targets[], sf::RectangleShape Lines[], sf::CircleShape resting_Spots[], Stone s_b[], int NUM_OF_STONES, sf::Text& gameTypeLabel, sf::Font& font);
    void drawScoreboard(sf::RectangleShape sb[], sf::Text sb_Text[], sf::Vector2f sb_size, sf::Font font);
    void createHouseView(sf::RenderWindow& houseZoom, const sf::RenderWindow& app);
    float getDistance(sf::Vector2f vector1, sf::Vector2f vector2);
    int findClosestStone(Stone stone_array[], int NUM_OF_STONES);
    bool inHouse(Stone stone1, sf::CircleShape target);
    void markClosestStone(sf::CircleShape& triangle, int closestStoneIndex, Stone stone_array[]);
    int findPointsScored(int winner, Stone stone_array[], sf::CircleShape Target);
    int getPoints(Stone winning_team[], const float closest_opponent, sf::CircleShape target);
    int calculatePointsEarned(const int winner, Stone team_even[], Stone team_odd[], sf::CircleShape target);
    void updateScoreboard(int winner, int points, sf::Text& team_A_score, sf::Text& team_B_score);
    bool checkPlay_Status(Stone stone_array[], int numberOfStones);
    bool inValid_Throw(Stone s);

    int getPlayType();
    int getPointsToWin();
    int getTeam_A_Points();
    int getTeam_B_Points();

    void updateTeam_A_Points(int points);
    void updateTeam_B_Points(int points);

private:
    int playType;
    int pointsToWin;
    int team_A_points;
    int team_B_points;

};

#endif
