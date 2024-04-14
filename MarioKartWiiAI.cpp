#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const int TRACK_WIDTH = 1000;
const int TRACK_HEIGHT = 1000;
const double PI = 3.14159265;

struct Position {
    double x;
    double y;
};

double distance(Position p1, Position p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

double angle(Position p1, Position p2) {
    return atan2(p2.y - p1.y, p2.x - p1.x);
}

class MarioKartAI {
private:
    Position currentPosition;
    Position targetPosition;
    vector<Position> waypoints;
    int currentWaypointIndex;
    double speed;
    double maxSpeed;
    double acceleration;
    double steeringAngle;
    bool hasBoost;

public:
    MarioKartAI() {
        currentPosition.x = 0;
        currentPosition.y = 0;
        currentWaypointIndex = 0;
        speed = 0.0;
        maxSpeed = 5.0;
        acceleration = 0.1;
        steeringAngle = 0.0;
        hasBoost = false;

        waypoints.push_back({500, 200});
        waypoints.push_back({800, 600});
        waypoints.push_back({200, 800});

        targetPosition = waypoints[currentWaypointIndex];
    }

    void updatePosition(Position newPosition, vector<Position>& obstacles) {
        currentPosition = newPosition;

        if (distance(currentPosition, targetPosition) < 10) {
            currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
            targetPosition = waypoints[currentWaypointIndex];
        }

        double targetAngle = angle(currentPosition, targetPosition);
        double angleDiff = targetAngle - steeringAngle;
        while (angleDiff > PI) angleDiff -= 2 * PI;
        while (angleDiff < -PI) angleDiff += 2 * PI;
        steeringAngle += angleDiff * 0.1;

        double distToTarget = distance(currentPosition, targetPosition);
        if (distToTarget < 100) {
            speed += acceleration;
        } else {
            speed -= acceleration;
        }
        speed = max(0.0, min(speed, maxSpeed));

        avoidObstacles(obstacles);

        if (hasBoost) {
            speed += 2.0;
            hasBoost = false;
        }

        currentPosition.x += speed * cos(steeringAngle);
        currentPosition.y += speed * sin(steeringAngle);
    }

    void avoidObstacles(vector<Position>& obstacles) {
        for (const auto& obstacle : obstacles) {
            double distToObstacle = distance(currentPosition, obstacle);
            if (distToObstacle < 50) {
                double obstacleAngle = angle(currentPosition, obstacle);
                double angleDiff = obstacleAngle - steeringAngle;
                while (angleDiff > PI) angleDiff -= 2 * PI;
                while (angleDiff < -PI) angleDiff += 2 * PI;
                if (angleDiff > 0) {
                    steeringAngle -= 0.1;
                } else {
                    steeringAngle += 0.1;
                }
            }
        }
    }

    void activateSpeedBoost() {
        hasBoost = true;
    }
};

int main() {
    srand(time(NULL));
    MarioKartAI ai;

    for (int i = 0; i < 100; ++i) {
        Position currentPosition;
        currentPosition.x = rand() % TRACK_WIDTH;
        currentPosition.y = rand() % TRACK_HEIGHT;

        vector<Position> obstacles;
        for (int i = 0; i < 5; ++i) {
            Position obstacle;
            obstacle.x = rand() % TRACK_WIDTH;
            obstacle.y = rand() % TRACK_HEIGHT;
            obstacles.push_back(obstacle);
        }

        ai.updatePosition(currentPosition, obstacles);

        if (rand() % 100 < 5) {
            ai.activateSpeedBoost();
        }
    }

    return 0;
}
