//
// Created by Aarnav Sawant on 3/30/24.
//

#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

void drawSoccerBall(struct ball ball) {
    drawRectDMA(ball.row, ball.col, ball.size, ball.size, WHITE);
}

void drawPlayer(struct player player1) {
    drawRectDMA(player1.row, player1.column, player1.width, player1.width, player1.jerseyColor);
    if (strlen(player1.number) == 2) {
        drawString(player1.row + 3, player1.column - 1, player1.number, player1.textColor);
    } else {
        drawString(player1.row + 3, player1.column + 2, player1.number, player1.textColor);
    }
}

int animatePlayer(struct player *player1, int speed, int targetPointRow, int targetPointColumn) {
    int dx = targetPointColumn - (*player1).column;
    int dy = targetPointRow - (*player1).row;

    // Calculate the squared distance to the target point
    int distanceSquared = dx * dx + dy * dy;
//    int speedSquared = speed * speed;

    // Check if the player is within one frame of movement from the target point
    if (distanceSquared <= 16) {
        (*player1).row = targetPointRow;
        (*player1).column = targetPointColumn;
        return 1; // Player has reached the target point
    }

    // Calculate direction components without using sqrt
    double directionX = 0;
    double directionY = 0;
    if (dx != 0) {
        directionX = dx > 0 ? 1 : -1;
    }
    if (dy != 0) {
        directionY = dy > 0 ? 1 : -1;
    }

    // Update player position based on the direction vector and speed
    (*player1).row += (int)(directionY * speed);
    (*player1).column += (int)(directionX * speed);

    return 0; // Player has not reached the target point yet
}

void drawPowerBar(int level) {
    drawRectDMA(3, 3, 100, 5, BLUE);
    if (level > 100) {
        drawRectDMA(3, 3, 100, 5, GREEN);
    } else {
        drawRectDMA(3, 3, level, 5, GREEN);
    }
}

void drawTrajectory(struct trajectory traj, u16 color) {
    int currentRow = traj.row;
    int currentColumn = traj.column;
    for (int i = 0; i < traj.numPoints; i++) {
        setPixel(currentRow, currentColumn, color);
        currentRow = currentRow + traj.dy;
        currentColumn = currentColumn + traj.dx;
    }
}

void undrawTrajectory(struct trajectory traj, const u16 *image) {
    int currentRow = traj.row;
    int currentColumn = traj.column;
    for (int i = 0; i < traj.numPoints; i++) {
        undrawImageDMA(currentRow, currentColumn, 1, 1, image);
        currentRow = currentRow + traj.dy;
        currentColumn = currentColumn + traj.dx;
    }
}

void undrawPowerBar(const u16* image) {
    drawImageDMA(3, 3, 100, 5, image);
}

void animateShot(struct ball *ball, int speedFactor, struct trajectory traj) {
    (*ball).row += (int) (speedFactor * traj.dy);
    (*ball).col += (int) (speedFactor * traj.dx);
}

