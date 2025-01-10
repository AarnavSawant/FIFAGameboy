//
// Created by Aarnav Sawant on 3/30/24.
//

#ifndef HW08_MACOS_GRAPHICS_H
#define HW08_MACOS_GRAPHICS_H
#include "main.h"

void drawSoccerBall(struct ball ball);

void drawPlayer(struct player player1);

int animatePlayer(struct player *player1, int speed, int targetPointRow, int targetPointColumn);

void drawTrajectory(struct trajectory traj, u16 color);

void undrawTrajectory(struct trajectory traj, const u16* image);

void drawPowerBar(int level);

void undrawPowerBar(const u16* image);

void undrawBall(struct ball *ball);

void animateShot(struct ball *ball, int speedFactor, struct trajectory traj);


#endif //HW08_MACOS_GRAPHICS_H


