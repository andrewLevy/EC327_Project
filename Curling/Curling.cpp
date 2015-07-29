#include "Curling.h"
#include <SFML/Graphics.hpp>
#include "Stone.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int BOARD_WIDTH = 1250;
const int BOARD_HEIGHT = 165;
const sf::Vector2f zoomButtonPosition(180,400);
const float zoomRatio = 2.5;

Curling::Curling(int newPlayType, int newPointsToWin)
{
    // Data field to specity game type includes practice (playType = 0) or One-on-One (playType = 1)
    playType = newPlayType;
    pointsToWin = newPointsToWin;
    team_A_points = 0;
    team_B_points = 0;
}

void Curling::prepareStones(Stone stone_array[])
{
    if(playType == 0)
    {
        // Specify positioning and fill color for stone array
        for(int i = 0; i < 8; i++)
        {
            stone_array[i].setFillColor(sf::Color::Green);
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
                stone_array[i].setFillColor(sf::Color::Green);
                stone_array[i].setPosition(1125 + i / 2 * (2 * stone_array[i].getRadius() + 1),15);
            }
            else
            {
                stone_array[i].setFillColor(sf::Color::Yellow);
                stone_array[i].setPosition(1125 + i / 2 * (2 * stone_array[i].getRadius() + 1),150);
            }
        }

    }
    return;
}

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
        gameTypeLabel.setString("Practice");
    else
        gameTypeLabel.setString("One-on-One");
    gameTypeLabel.setCharacterSize(30);
    //gameTypeLabel.setOrigin(gameTypeLabel.getLocalBounds().width,gameTypeLabel.getLocalBounds().height);
    gameTypeLabel.setPosition(1330 - gameTypeLabel.getLocalBounds().width,580 - gameTypeLabel.getLocalBounds().height);
}

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

void Curling::drawScoreboard(sf::RectangleShape sb[], sf::Text sb_Text[], sf::Vector2f sb_size, sf::Font font)
{
    sb[0].setFillColor(sf::Color::Green);
    sb[1].setFillColor(sf::Color::Black);
    sb[2].setFillColor(sf::Color::Yellow);
    sb_Text[0].setString("Score");
    sb_Text[1].setString("Set");
    sb_Text[2].setString("Score");

    for (int b=0; b<6; b++)
    {
        if (b<3)
        {
            sb[b].setSize(sb_size);
            sb[b].setOrigin(sb_size.x/2,sb_size.y/2);
            sb[b].setOutlineColor(sf::Color::Black);
            sb[b].setOutlineThickness(-2.0);
            sb[b].setPosition(BOARD_WIDTH/2+(b-1)*sb_size.x,195);
            sb_Text[b].setFont(font);
            sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getLocalBounds().height/2);
            sb_Text[b].setCharacterSize(30);
            sb_Text[b].setPosition(sb[b].getPosition());
            sb_Text[b].setColor(sf::Color::Black);
        }
        else
        {
            sb[b].setSize(sb_size);
            sb[b].setOrigin(sb_size.x/2,sb_size.y/2);
            sb[b].setOutlineColor(sf::Color::Black);
            sb[b].setOutlineThickness(-2.0);
            sb[b].setPosition(BOARD_WIDTH/2+(b-4)*sb_size.x,245);
            sb_Text[b].setFont(font);
            sb_Text[b].setOrigin(sb_Text[b].getLocalBounds().width/2,sb_Text[b].getCharacterSize()/2);
            sb_Text[b].setCharacterSize(20);
            sb_Text[b].setPosition(sb[b].getPosition());
            sb_Text[b].setString("0");
            sb_Text[b].setColor(sf::Color::Black);
        }
    }
    sb_Text[1].setColor(sf::Color::White);
}

void Curling::createHouseView(sf::RenderWindow& houseZoom, const sf::RenderWindow& app)
{
    // Set location of window
    const sf::Vector2i houseZoomLocation(app.getPosition().x,app.getPosition().y + 170);
    houseZoom.setPosition(houseZoomLocation);

    // Set view of original game window and placement of view in House window
    const sf::Vector2f buttonLocation(180,BOARD_HEIGHT / 2);
    const sf::Vector2f viewSize(120,120);
    sf::View view(buttonLocation,viewSize);
    view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    houseZoom.setView(view);
}

float Curling::getDistance(sf::Vector2f vector1, sf::Vector2f vector2)
{
    float x_difference = vector2.x - vector1.x;
    float y_difference = vector2.y - vector1.y;

    return sqrt(x_difference * x_difference + y_difference * y_difference);
}

int Curling::findClosestStone(Stone stone_array[], int NUM_OF_STONES)
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

int Curling::getClostest_movingStone(Stone s[], sf::Vector2i mouse_loc, const int N)
{
    int dist = 50000;
    int dist_i;
    int temp_dist;
    for (int f=0; f<N; f++)
    {
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

/*int Curling::findClosestStone(const vector<Stone>& houseStones)
{
    // Add closest stone(s) to vector
    sf::Vector2f buttonPosition(180,BOARD_HEIGHT / 2);
    float closest_distance = getDistance(houseStones[0].getPosition(), buttonPosition);
    vector<int> closestStones;
    if(houseStones[0].getStatus())
        closestStones.push_back(0);
    else
        closestStones.push_back(-1);

    for(int i = 1; i < houseStones.size(); i++)
    {
        if(houseStones[i].getStatus())
        {
            float next_distance = getDistance(houseStones[i].getPosition(), buttonPosition);
            if(next_distance == closest_distance)
            {
                if(houseStones[0] == -1)
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
}*/

bool Curling::inHouse(Stone stone1, sf::CircleShape target)
{
    if(getDistance(stone1.getPosition(), target.getPosition()) - stone1.getRadius() <= target.getRadius())
        return true;
    else
        return false;
}

void Curling::updateHouseZoom(Stone stone_array[], vector<Stone>& houseStones, int numberOfStones, sf::CircleShape Target)
{
    houseStones.clear();
    sf::Vector2f positionFromButton;
    for(int i = 0; i < numberOfStones; i++)
    {
        if(inHouse(stone_array[i], Target))
        {
            // Find position relative to button
            positionFromButton.x = stone_array[i].getPosition().x - Target.getPosition().x;
            positionFromButton.y = stone_array[i].getPosition().y - Target.getPosition().y;

            // Add house stone to house stone vector
            Stone houseStone;
            houseStone.setRadius(zoomRatio * houseStone.getRadius());
            houseStone.setOrigin(houseStone.getRadius(), houseStone.getRadius());
            houseStone.setOutlineThickness(houseStone.getOutlineThickness() * zoomRatio);
            if(getPlayType() == 0)
                houseStone.setFillColor(sf::Color::Green);
            else if(i % 2 == 0)
                houseStone.setFillColor(sf::Color::Green);
            else
                houseStone.setFillColor(sf::Color::Yellow);

            float x_newPosition = positionFromButton.x * zoomRatio;
            float y_newPosition = positionFromButton.y * zoomRatio;
            houseStone.setPosition(zoomButtonPosition.x + x_newPosition, zoomButtonPosition.y + y_newPosition);

            houseStones.push_back(houseStone);
        }

    }
}

void Curling::markClosestStone(sf::CircleShape& triangle, int closestStoneIndex, Stone stone_array[])
{
    triangle.setOrigin(triangle.getRadius(), triangle.getRadius());
    triangle.rotate(180);

    // Set color of market to team color of closest stone
    if(closestStoneIndex % 2 == 0)
        triangle.setFillColor(sf::Color::Green);
    else
        triangle.setFillColor(sf::Color::Yellow);

    // Set position of marker above closest stone
    triangle.setPosition(stone_array[closestStoneIndex].getPosition().x, stone_array[closestStoneIndex].getPosition().y - stone_array[closestStoneIndex].getRadius() - triangle.getRadius() - 2);
}

int Curling::findPointsScored(int winner, Stone stone_array[], sf::CircleShape Target)
{
    // Split up teams to find number of points earned
    Stone team_even[8];
    Stone team_odd[8];
    //int evenCount = 0;
    //int oddCount = 0;
    for(int i = 0; i < 16; i++)
    {
        if(i % 2 == 0)
        {
            team_even[i / 2] = stone_array[i];
            //evenCount++;
        }
        else
        {
            team_odd[i / 2] = stone_array[i];
            //oddCount++;
        }
    }

    return calculatePointsEarned(winner,team_even, team_odd, Target);
}

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

void Curling::updateScoreboard(int winner, int points, sf::Text& team_A_score, sf::Text& team_B_score)
{
    if(winner % 2 == 0)
    {
        updateTeam_A_Points(points);
        cout << "Team A wins " << points << " points!" << endl;
        team_A_score.setString(to_string(getTeam_A_Points()));
    }
    else
    {
        updateTeam_B_Points(points);
        cout << "Team B wins " << points << " points!" << endl;
        team_B_score.setString(to_string(getTeam_B_Points()));
    }
}

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

int Curling::getPointsToWin()
{
    return pointsToWin;
}

int Curling::getTeam_A_Points()
{
    return team_A_points;
}

int Curling::getTeam_B_Points()
{
    return team_B_points;
}

void Curling::updateTeam_A_Points(int points)
{
    team_A_points += points;
}

void Curling::updateTeam_B_Points(int points)
{
    team_B_points += points;
}
