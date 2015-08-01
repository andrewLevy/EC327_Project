#include "Curling.h"
#include <SFML/Graphics.hpp>
#include "Stone.h"
#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <cmath>

using namespace std;

const int BOARD_WIDTH = 1250;
const int BOARD_HEIGHT = 165;
const sf::Vector2f zoomButtonPosition(180,375);
const float zoomRatio = 2.5;

// Curling constructor
// Inputs include (i) Play Type (Play or Train), (ii) Scoring Metrics, (iii) Team Colors, (iv) Team Names
Curling::Curling(int newPlayType, int newNumberToWin, char newScoringType, sf::Color newTeamAColor, sf::Color newTeamBColor, string newTeamAName, string newTeamBName)
{
    playType = newPlayType;
    numberToWin = newNumberToWin;
    scoringType = newScoringType;
    team_A_points = 0;
    team_B_points = 0;
    team_A_color = newTeamAColor;
    team_B_color = newTeamBColor;
    turnNumber = 0;
    currentSet = 1;
    if (newTeamAName.empty() || newTeamAName == "Enter Team A Name")
        team_A_name = "Team A";
    else
        team_A_name = newTeamAName;

    if (newTeamBName.empty() || newTeamBName == "Enter Team B Name")
        team_B_name = "Team B";
    else
        team_B_name = newTeamBName;
}

Curling::~Curling()
{
    //dtor
}

// Sets intial positions and fill colors.  Positions and coloring depends on play type (play or train)
// If a one-on-one game, even-numbered stones belong to Team A while odd-numbered stone belong to Team B
void Curling::prepareStones(Stone stone_array[])
{
    if(playType == 0)
    {
        // Specify positioning and fill color for stone array
        for(int i = 0; i < 8; i++)
        {
            stone_array[i].setFillColor(getTeam_A_Color());
            stone_array[i].setPosition(1125 + i * (2 * stone_array[i].getRadius() + 1),15);
        }
    }
    else
    {
        // Specify positioning and fill color for stone array
        for(int i = 0; i < 16; i++)
        {
            if(i % 2 == 0)
            {
                stone_array[i].setFillColor(getTeam_A_Color());
                stone_array[i].setPosition(1125 + i / 2 * (2 * stone_array[i].getRadius() + 1),15);
            }
            else
            {
                stone_array[i].setFillColor(getTeam_B_Color());
                stone_array[i].setPosition(1125 + i / 2 * (2 * stone_array[i].getRadius() + 1),150);
            }
        }

    }
    return;
}

// Function draws the rink including the House, boundary lines, initial stone placement outlines, play type label,
void Curling::drawRink(sf::CircleShape Targets[], sf::RectangleShape lines[], sf::CircleShape resting_Spots[], Stone s_b[], int NUM_OF_STONES, sf::Text& gameTypeLabel, sf::Font& font)
{
    // Create House
    float radius[4] = {60,40,20,5};
    sf::Color T[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};
    for (int i=0; i<4; i++)
    {
        Targets[i].setRadius(radius[i]);
        Targets[i].setOrigin(radius[i],radius[i]);
        Targets[i].setFillColor(T[i]);
        Targets[i].setPosition(180,BOARD_HEIGHT/2);
        Targets[i].setOutlineColor(sf::Color::Black);
        Targets[i].setOutlineThickness(-2.0);
    }

    // Create Boundary Lines
    sf::Vector2f Lpos[8] = {sf::Vector2f(0,BOARD_HEIGHT/2 - 0.5),sf::Vector2f(390-2,0),sf::Vector2f(180 -.5,0),sf::Vector2f(1110-2,0),sf::Vector2f(0,BOARD_HEIGHT - 4),sf::Vector2f(240,BOARD_HEIGHT/2+15),sf::Vector2f(240,BOARD_HEIGHT/2-15),sf::Vector2f(120-.5,0)};
    sf::Vector2f Lsize[8] = {sf::Vector2f(1110,1),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(1,BOARD_HEIGHT),sf::Vector2f(4,BOARD_HEIGHT),sf::Vector2f(BOARD_WIDTH,4),sf::Vector2f(1110-240,.5),sf::Vector2f(1110-240,.5),sf::Vector2f(1,BOARD_HEIGHT)};
    for (int i=0; i<8; i++)
    {
        lines[i].setSize(Lsize[i]);
        lines[i].setFillColor(sf::Color::Black);
        lines[i].setPosition(Lpos[i]);
    }

    // Draw resting spots
    for (int rs=0; rs<NUM_OF_STONES; rs++)
    {
        resting_Spots[rs].setRadius(s_b[rs].getRadius());
        resting_Spots[rs].setFillColor(sf::Color::White);
        resting_Spots[rs].setOutlineColor(sf::Color::Black);
        resting_Spots[rs].setOutlineThickness(-1.0);
        resting_Spots[rs].setOrigin(s_b[rs].getRadius(),s_b[rs].getRadius());
        resting_Spots[rs].setPosition(s_b[rs].getPosition());
    }

    // Create label identifying game type
    gameTypeLabel.setFont(font);
    gameTypeLabel.setColor(sf::Color::Black);
    if(playType == 0)
        gameTypeLabel.setString("Training");
    else
        gameTypeLabel.setString("One-on-One");
    gameTypeLabel.setCharacterSize(20);
    gameTypeLabel.setPosition(1275 / 2.0 - gameTypeLabel.getLocalBounds().width / 2,5);
}

// Function draws zoomed in window of House including the House and boundary lines
void Curling::drawHouseZoom(sf::CircleShape houseTargets[], sf::RectangleShape houseLines[])
{
    // Create House
    float radius[4] = {60,40,20,5};
    for(int i = 0; i < 4; i++)
        radius[i] *= zoomRatio;

    sf::Color T[4] = {sf::Color::Blue, sf::Color::White, sf::Color::Red, sf::Color::White};
    for (int i=0; i<4; i++)
    {
        houseTargets[i].setRadius(radius[i]);
        houseTargets[i].setOrigin(radius[i],radius[i]);
        houseTargets[i].setFillColor(T[i]);
        houseTargets[i].setPosition(zoomButtonPosition);
        houseTargets[i].setOutlineColor(sf::Color::Black);
        houseTargets[i].setOutlineThickness(-4.0);
    }

    // Draw House Lines
    sf::Vector2f lineSize[2] = {sf::Vector2f(radius[0] * 2,zoomRatio),sf::Vector2f(zoomRatio,radius[0] * 2)};
    sf::Vector2f linePosition[2] = {sf::Vector2f(zoomButtonPosition.x - radius[0], zoomButtonPosition.y - zoomRatio / 2),sf::Vector2f(zoomButtonPosition.x - zoomRatio / 2, zoomButtonPosition.y - radius[0])};
    for(int i = 0; i < 2; i++)
    {
        houseLines[i].setPosition(linePosition[i]);
        houseLines[i].setSize(lineSize[i]);
        houseLines[i].setFillColor(sf::Color::Black);
    }
}

// Function creates a zoomed in view of the initial user inputs
void Curling::drawUserInputsZoom(sf::RectangleShape boundaryLines[], const Stone currentStone, Stone& zoomStone, const sf::Sprite arrow, sf::Sprite& arrowZoom, sf::RectangleShape zoomBoundary[])
{
    // Magnification multiple
    float userInputsZoom = 2.0;

    // Determine positioning for boundary lines in zoom of user inputs
    sf::Vector2f Lpos[4] = {sf::Vector2f(1110 - 200,375),sf::Vector2f(1110 - 2 * userInputsZoom,375 - BOARD_HEIGHT / 2 * userInputsZoom),sf::Vector2f(1110 - 200,375 + 15 * userInputsZoom),sf::Vector2f(1110 - 200,375 - 15 * userInputsZoom)};
    sf::Vector2f Lsize[4] = {sf::Vector2f(200,1 * userInputsZoom),sf::Vector2f(4 * userInputsZoom,BOARD_HEIGHT * userInputsZoom),sf::Vector2f(200,.5 * userInputsZoom),sf::Vector2f(200,.5 * userInputsZoom)};
    for(int i = 0; i < 4; i++)
    {
        boundaryLines[i].setPosition(Lpos[i]);
        boundaryLines[i].setSize(Lsize[i]);
        boundaryLines[i].setFillColor(sf::Color::Black);
    }

    // Add current stone to zoom of user inputs
    zoomStone.setRadius(currentStone.getRadius() * userInputsZoom);
    zoomStone.setOrigin(zoomStone.getRadius(), zoomStone.getRadius());
    zoomStone.setOutlineThickness(currentStone.getOutlineThickness() * userInputsZoom);
    zoomStone.setPosition(1110,375);

    // Add zoomed in arrow to zoomed in view of user inputs
    float new_scale_x = arrow.getScale().x * userInputsZoom;
    float new_scale_y = arrow.getScale().y * userInputsZoom;
    arrowZoom.setScale(new_scale_x,new_scale_y);
    arrowZoom.setRotation(arrow.getRotation());
    arrowZoom.setPosition(1110 - boundaryLines[1].getSize().x / 2,375);

    // Add boundary rectangle to zoomed in window
    sf::Vector2f boundaryPosition[4] = {sf::Vector2f(1110 - 200, 375 - BOARD_HEIGHT / 2 * userInputsZoom), sf::Vector2f(1110 - 200 + 300, 375 - BOARD_HEIGHT / 2 * userInputsZoom), sf::Vector2f(1110 - 200 + 300, 376 + BOARD_HEIGHT / 2 * userInputsZoom), sf::Vector2f(1110 - 200, 376 + BOARD_HEIGHT / 2 * userInputsZoom)};
    sf::Vector2f boundarySize[4] = {sf::Vector2f(300,-1),sf::Vector2f(-1, BOARD_HEIGHT * userInputsZoom), sf::Vector2f(-300, 1), sf::Vector2f(1, -BOARD_HEIGHT * userInputsZoom)};
    for(int i = 0; i < 4; i++)
    {
        zoomBoundary[i].setPosition(boundaryPosition[i]);
        zoomBoundary[i].setSize(boundarySize[i]);
        zoomBoundary[i].setFillColor(sf::Color::Black);
    }

}

// Function return distance between two position vectors
float Curling::getDistance(sf::Vector2f vector1, sf::Vector2f vector2)
{
    float x_difference = vector2.x - vector1.x;
    float y_difference = vector2.y - vector1.y;

    return sqrt(x_difference * x_difference + y_difference * y_difference);
}

// Function return stone in array closest to "button"
int Curling::findClosestStone(Stone stone_array[], int NUM_OF_STONES)
{
    // Add closest stone(s) to vector
    sf::Vector2f buttonPosition(180,BOARD_HEIGHT / 2);
    float closest_distance = getDistance(stone_array[0].getPosition(), buttonPosition);

    vector<int> closestStones;
    if(stone_array[0].getStatus())
        closestStones.push_back(0);
    // Enter "-1" in Stone is not on rink because has collided with wall or was invalid
    else
        closestStones.push_back(-1);

    // Loop through stone array and find stone(s) closest to "button"
    for(int i = 1; i < NUM_OF_STONES; i++)
    {
        // Only include stones that are in play (i.e. stones have been played and are on rink)
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

// Function determines which stone to sweep by finding closest moving stone to mouse click location
int Curling::getClostest_movingStone(Stone s[], sf::Vector2i mouse_loc, const int N)
{
    int dist = 50000;
    int dist_i;
    int temp_dist;
    for (int f=0; f<N; f++)
    {
        // Only includes stones that are still moving
        if (s[f].getSpeed() != 0)
        {
            temp_dist = sqrt((s[f].getPosition().x-mouse_loc.x)*(s[f].getPosition().x-mouse_loc.x) + (s[f].getPosition().y-mouse_loc.y)*(s[f].getPosition().y-mouse_loc.y));
            if (temp_dist < dist)
            {
                dist = temp_dist;
                dist_i = f;
            }
        }
    }
    return dist_i;
}

// Functons determines whether a stone lies in the House (i.e. distance from the button < radius of largest circle in House)
bool Curling::inHouse(Stone stone1, sf::CircleShape target)
{
    if(getDistance(stone1.getPosition(), target.getPosition()) - stone1.getRadius() <= target.getRadius())
        return true;
    else
        return false;
}

// Function adds stones in House to zoom view of House
void Curling::updateHouseZoom(const Stone stone_array[], Stone houseStones[], int numberOfStones, sf::CircleShape Target)
{
    sf::Vector2f positionFromButton;
    for(int i = 0; i < numberOfStones; i++)
    {
        if(inHouse(stone_array[i], Target))
        {
            // Find position relative to button
            positionFromButton.x = stone_array[i].getPosition().x - Target.getPosition().x;
            positionFromButton.y = stone_array[i].getPosition().y - Target.getPosition().y;

            // Update properties of stone to account for magnification multiple
            houseStones[i].setRadius(zoomRatio * stone_array[i].getRadius());
            houseStones[i].setOrigin(houseStones[i].getRadius(), houseStones[i].getRadius());
            houseStones[i].setOutlineThickness(houseStones[i].getOutlineThickness() * zoomRatio);
            if(getPlayType() == 0)
                houseStones[i].setFillColor(getTeam_A_Color());
            else if(i % 2 == 0)
                houseStones[i].setFillColor(getTeam_A_Color());
            else
                houseStones[i].setFillColor(getTeam_B_Color());

            float x_newPosition = positionFromButton.x * zoomRatio;
            float y_newPosition = positionFromButton.y * zoomRatio;
            houseStones[i].setPosition(zoomButtonPosition.x + x_newPosition, zoomButtonPosition.y + y_newPosition);
        }

    }
}

// Function places a triangle marker over the closest stone to the "button"
void Curling::markClosestStone(sf::CircleShape& triangle, int closestStoneIndex, Stone stone_array[])
{
    triangle.setOrigin(triangle.getRadius(), triangle.getRadius());
    triangle.rotate(180);

    // Set color of marker to team color of closest stone
    if(getPlayType() == 0)
        triangle.setFillColor(getTeam_A_Color());
    // Team A has even-numbered stones and Team B has odd-numbered stones
    else if(closestStoneIndex % 2 == 0)
        triangle.setFillColor(getTeam_A_Color());
    else
        triangle.setFillColor(getTeam_B_Color());

    // Set position of marker above closest stone
    triangle.setPosition(stone_array[closestStoneIndex].getPosition().x, stone_array[closestStoneIndex].getPosition().y - stone_array[closestStoneIndex].getRadius() - triangle.getRadius() - triangle.getRadius() / 2.0);
}

// Function is one of several to help determine number of points scored in an end after the end is complete.  Function divides stone array into two teams.
int Curling::findPointsScored(int winner, Stone stone_array[], sf::CircleShape Target)
{
    // Split up teams to find number of points earned
    Stone team_even[8];
    Stone team_odd[8];

    for(int i = 0; i < 16; i++)
    {
        if(i % 2 == 0)
        {
            team_even[i / 2] = stone_array[i];
        }
        else
        {
            team_odd[i / 2] = stone_array[i];
        }
    }

    return calculatePointsEarned(winner,team_even, team_odd, Target);
}

// Function is one of several to help calculate the number of points won in an end.  Function find the number of stones from the winning team
// that are in the House and lie closer to the "button" than the the losing team's closest stone.
int Curling::getPoints(Stone winning_team[], const float closest_opponent, sf::CircleShape target)
{
    int points = 0;

    sf::Vector2f buttonPosition(180,BOARD_HEIGHT / 2);
    for(int i = 0; i < 8; i++)
    {
        if(inHouse(winning_team[i], target) && getDistance(winning_team[i].getPosition(), buttonPosition) < closest_opponent)
            points++;
    }
    return points;
}

// Function is one of several to help calcuate the number of points won in an end.  Function finds closest stone to the "button" from the losing team
int Curling::calculatePointsEarned(const int winner, Stone team_even[], Stone team_odd[], sf::CircleShape target)
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
        return getPoints(team_even, closest_opponent, target);
    else
        return getPoints(team_odd, closest_opponent, target);
}

// Function updates the scoreboard by updating point totals for the winning team of the previous set and increases the current set number.
void Curling::updateScoreboard(int winner, int points, sf::Text& team_A_score, sf::Text& team_B_score,sf::Text& setNumber)
{
    if(winner % 2 == 0)
    {
        updateTeam_A_Points(points);
        team_A_score.setString(to_string(getTeam_A_Points()));
    }
    else if(winner != -1)
    {
        updateTeam_B_Points(points);
        team_B_score.setString(to_string(getTeam_B_Points()));
    }
    setNumber.setString(to_string(currentSet));
}

// Function checks whether a stone is currently is plan by checking stone speeds
bool Curling::checkPlay_Status(Stone stone_array[], int numberOfStones)
{
    for (int i = 0; i < numberOfStones; i++)
    {
        if (stone_array[i].getSpeed() != 0)
        {
            return true;
        }
    }
    return false;
}

// Function checks whether a stone delivery was invalid
bool Curling::inValid_Throw(Stone s)
{
    if (s.getPosition().x > 390 && s.getPosition().x < 1110)
    {
        return true;
    }
    return false;
}

int Curling::getPlayType()
{
    return playType;
}

int Curling::getNumberToWin()
{
    return numberToWin;
}

char Curling::getScoringType()
{
    return scoringType;
}

int Curling::getTeam_A_Points()
{
    return team_A_points;
}

int Curling::getTeam_B_Points()
{
    return team_B_points;
}

sf::Color Curling::getTeam_A_Color()
{
    return team_A_color;
}

sf::Color Curling::getTeam_B_Color()
{
    return team_B_color;
}

string Curling::getTeamAName()
{
    return team_A_name;
}

string Curling::getTeamBName()
{
    return team_B_name;
}

int Curling::getTurnNumber()
{
    return turnNumber;
}

int Curling::getCurrentSet()
{
    return currentSet;
}

void Curling::updateTeam_A_Points(int points)
{
    team_A_points += points;
}

void Curling::updateTeam_B_Points(int points)
{
    team_B_points += points;
}

void Curling::setTurnNumber(int newTurnNumber)
{
    turnNumber = newTurnNumber;
}

void Curling::setCurrentSet(int newCurrentSet)
{
    currentSet = newCurrentSet;
}

// Function determines whether the game is over (i.e., a team's point totals are >= the number of points to
// win or the current set number is > the number of sets to be played)
bool Curling::isGameOver()
{
    if(playType == 1 && scoringType == 'E' && currentSet > numberToWin)
        return true;
    else if(playType == 1 && scoringType == 'P' && (team_A_points >= numberToWin || team_B_points >= numberToWin) && turnNumber == 16)
        return true;
    else
        return false;
}
