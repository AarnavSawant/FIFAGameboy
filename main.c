#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
 #include "images/background.h"

#include "images/brazil.h"
#include "images/usa.h"
#include "images/worldcup.h"
#include "images/messi.h"

#include "graphics.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
    START,
    TEAMS,
    PLAY,
    WIN,
    LOSE,
};

struct team setUpTeam(int teamNumber, int home) {
    struct player player1;
    struct player player2;
    struct player player3;
    struct player player4;;
    struct player player5;
    struct player goalkeeper;
    goalkeeper.number = "1";
    goalkeeper.width = 12;
    struct team team1;
    switch (teamNumber) {
        case 1 : {
            u16 jerseyColor = YELLOW;
            u16 textColor = GREEN;
            player1.jerseyColor = jerseyColor;
            player1.textColor = textColor;
            player1.number = "9";
            player1.width = 12;
            player2.jerseyColor = jerseyColor;
            player2.textColor = textColor;
            player2.number = "10";
            player2.width = 12;
            player3.jerseyColor = jerseyColor;
            player3.textColor = textColor;
            player3.number = "11";
            player3.width = 12;
            player4.jerseyColor = jerseyColor;
            player4.textColor = textColor;
            player4.number = "7";
            player4.width = 12;
            player5.jerseyColor = jerseyColor;
            player5.textColor = textColor;
            player5.number = "6";
            player5.width = 12;
            goalkeeper.jerseyColor = GREEN;
            goalkeeper.textColor = jerseyColor;
            team1.name = "Brazil";
            team1.abbreviation = "BRA";
            break;
        }
        case 2 : {
            u16 jerseyColor = WHITE;
            u16 textColor = BLUE;
            player1.jerseyColor = jerseyColor;
            player1.number = "9";
            player1.width = 12;
            player1.textColor = textColor;
            player2.jerseyColor = jerseyColor;
            player2.number = "10";
            player2.width = 12;
            player2.textColor = textColor;
            player3.jerseyColor = jerseyColor;
            player3.number = "11";
            player3.width = 12;
            player3.textColor = textColor;
            player4.jerseyColor = jerseyColor;
            player4.number = "7";
            player4.width = 12;
            player4.textColor = textColor;
            player5.jerseyColor = jerseyColor;
            player5.number = "6";
            player5.width = 12;
            player5.textColor = textColor;
            goalkeeper.jerseyColor = BLUE;
            goalkeeper.textColor = jerseyColor;
            team1.name = "USA";
            team1.abbreviation = "USA";
            break;
        }
    }
    if (home) {
        player1.row = HEIGHT / 2;
        player1.column = WIDTH/ 2;
        player2.row = HEIGHT / 2 + 15;
        player2.column = WIDTH/ 2;
        player3.row = HEIGHT / 2 + 30;
        player3.column = WIDTH/ 2;
        player4.row = HEIGHT / 2 + 45;
        player4.column = WIDTH/ 2;
        player5.row = HEIGHT / 2 + 60;
        player5.column = WIDTH/ 2;
    } else {
        player1.row = HEIGHT / 2 - 15;
        player1.column = WIDTH/ 2;
        player2.row = HEIGHT / 2  - 30;
        player2.column = WIDTH/ 2;
        player3.row = HEIGHT / 2 - 45;
        player3.column = WIDTH/ 2;
        player4.row = HEIGHT / 2 -60;
        player4.column = WIDTH/ 2;
        player5.row = HEIGHT / 2 -75;
        player5.column = WIDTH/ 2;

    }
    team1.shooters[1] = player1;
    team1.shooters[0] = player2;
    team1.shooters[2] = player3;
    team1.shooters[4] = player4;
    team1.shooters[3] = player5;
    team1.numShooters = 5;
    team1.goalkeeper = goalkeeper;
    team1.goals = 0;
    team1.shotsRemaining = 5;
    return team1;
}

int gameOver(struct team team1, struct team team2) {
    if (team1.shotsRemaining == 0 && team2.shotsRemaining == 0 && team1.goals != team2.goals) {
        return 0;
    }
    int goalDif = abs(team1.goals - team2.goals);
    if (team1.goals > team2.goals) {
        return goalDif <= team2.shotsRemaining;
    } else {
       return goalDif <=  team1.shotsRemaining;
    }
}

// Function to check if the ball is touching the goalkeeper
int goalkeeperSavedShot(struct player keeper, struct ball ball) {
    // Calculate the coordinates of the edges of the goalkeeper
    int keeperLeft = keeper.column;
    int keeperRight = keeper.column + keeper.width;
    int keeperTop = keeper.row;
    int keeperBottom = keeper.row + keeper.width;

    // Calculate the coordinates of the edges of the ball
    int ballLeft = ball.col - ball.size / 2;
    int ballRight = ball.col + ball.size / 2;
    int ballTop = ball.row - ball.size / 2;
    int ballBottom = ball.row + ball.size / 2;

    // Check if the ball is touching the goalkeeper
    int isTouching = (ballLeft <= keeperRight && ballRight >= keeperLeft &&
                       ballTop <= keeperBottom && ballBottom >= keeperTop);

    return isTouching;
}

int main(void) {
    /* TODO: */
    // Manipulate REG_DISPCNT here to set Mode 3. //
    REG_DISPCNT = MODE3 | BG2_ENABLE;

    // Save current and previous state of button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;
    struct team team1 = setUpTeam(1, 1);
    struct team team2 = setUpTeam(2, 0);
    struct game game;
    game.teams[1] = team1;
    game.teams[0] = team2;
    struct ball ball;
    // Load initial application state
    enum gba_state state = START;
    struct trajectory traj;
    int trajectoryDyChanger = 1;

    enum play_state playState = PLAYER_WALKING_UP;
    int currentLevel = 15;
    int levelChangerSign = 1;
    int levelChangeMagnitude = 10;
    int scoredGoal = 0;
    int currentTeam = 0;
    int currentPlayer = 0;
    int currentGoalkeeperChangerSign = 1;
    int currentGoalkeeperChangeMagnitude = 1;
    int userGoalkeeperChangeMagnitude = 2;
    int max = 5;
    int min = 0;
    int random_number1 = rand() % (max - min + 1) + min;
    int max2 = 10;
    int min2 = 0;
    int random_number2 = rand() % (max - min + 1) + min;
    int aiTeamIndex = 1;
    char score1[20] = "0";
    char score2[20] = "0";

    int initialPositionRow = -1;
    int initialPositionColumn = -1;
    struct player currentGoalkeeper = currentTeam == 0 ? game.teams[1].goalkeeper : game.teams[0].goalkeeper;
    struct player otherGoalkeeper = currentTeam == 0 ? game.teams[0].goalkeeper : game.teams[1].goalkeeper;
    game.teams[0].goals = 0;
    game.teams[1].goals = 0;
    game.teams[0].shotsRemaining = 5;
    game.teams[1].shotsRemaining = 5;
    int shouldEraseAim = 1;
    int shouldDrawStart = 1;
    int shouldDrawWin = 1;
    int shouldDrawAllPlayers = 1;
    while (1) {
        currentButtons = BUTTONS; // Load the current state of the buttons
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
            state = START;
            shouldDrawStart = 1;
        }

        /* TODO: */
        // Manipulate the state machine below as needed //
        // NOTE: Call waitForVBlank() before you draw
        waitForVBlank();
        switch (state) {
            case START: {
                if (shouldDrawStart) {
                    drawFullScreenImageDMA(messi);
                    char myString[] = "FIFA GAMEBOY";
                    drawString(HEIGHT / 2, WIDTH / 2 - 35, myString, MAGENTA);
                    char myString2[] = "PRESS ENTER TO START";
                    drawString(HEIGHT / 2 + 20, WIDTH / 2 - 55, myString2, BLACK);
                    shouldDrawStart = 0;
                    team1 = setUpTeam(1, 1);
                    team2 = setUpTeam(2, 0);
                    game.teams[1] = team1;
                    game.teams[0] = team2;
                    sprintf(score1, "%d", game.teams[0].goals);
                    sprintf(score2, "%d", game.teams[1].goals);
                    // Load initial application state
                    trajectoryDyChanger = 1;
                    currentLevel = 15;
                    levelChangerSign = 1;
                    levelChangeMagnitude = 10;
                    scoredGoal = 0;
                    currentTeam = 0;
                    currentPlayer = 0;
                    playState = PLAYER_WALKING_UP;
                    currentGoalkeeperChangerSign = 1;
                    currentGoalkeeperChangeMagnitude = 1;
                    userGoalkeeperChangeMagnitude = 2;
                    max = 5;
                    min = 0;
                    random_number1 = rand() % (max - min + 1) + min;
                    max2 = 10;
                    min2 = 0;
                    random_number2 = rand() % (max - min + 1) + min;
                    aiTeamIndex = 1;
                    initialPositionRow = -1;
                    initialPositionColumn = -1;
                    currentGoalkeeper = currentTeam == 0 ? game.teams[1].goalkeeper : game.teams[0].goalkeeper;
                    otherGoalkeeper = currentTeam == 0 ? game.teams[0].goalkeeper : game.teams[1].goalkeeper;
                    game.teams[0].goals = 0;
                    game.teams[1].goals = 0;
                    game.teams[0].shotsRemaining = 5;
                    game.teams[1].shotsRemaining = 5;
                    shouldEraseAim = 1;
                    shouldDrawWin = 1;
                    shouldDrawAllPlayers = 1;
                }
                if (KEY_DOWN(BUTTON_START, currentButtons)) {
                    fillScreenDMA(BLACK);
                    state = TEAMS;
                    shouldDrawStart = 1;
                }
                break;

            }
            case TEAMS: {
                if (shouldDrawStart) {
//                    drawFullScreenImageDMA(brazil2);
                    drawImageDMA(20, 0, BRAZIL2_WIDTH,BRAZIL2_HEIGHT, brazil2);
                    drawImageDMA(20, WIDTH - USA_WIDTH, USA_WIDTH,USA_HEIGHT, usa);
                    drawImageDMA(20, BRAZIL2_WIDTH, WORLDCUP_WIDTH,WORLDCUP_HEIGHT, worldcup);
                    char myString[] = "FINAL";
                    drawString(120, WIDTH / 2 - 12.5, myString, MAGENTA);
                    char myString2[] = "PRESS ENTER TO HEAD TO PENALTIES!";
                    drawString(150, WIDTH / 2 - 90, myString2, WHITE);
                    shouldDrawStart = 0;
                }
                if (KEY_DOWN(BUTTON_START, currentButtons) && !(KEY_DOWN(BUTTON_START, previousButtons))) {
                    state = PLAY;
                    drawFullScreenImageDMA(background2);
                    drawString(SCOREROW, SCORE1COL + 30, game.teams[1].abbreviation, game.teams[1].shooters[0].jerseyColor);
                    drawString(SCOREROW, SCORE1COL - 20, game.teams[0].abbreviation, game.teams[0].shooters[0].jerseyColor);
                }

                // state = ?
                break;
            }

            case PLAY:
                if (playState == PLAYER_WALKING_UP || playState == WALK_BACK || playState == RUN_UP) {
                    undrawImageDMA(game.teams[currentTeam].shooters[currentPlayer].row, game.teams[currentTeam].shooters[currentPlayer].column, game.teams[currentTeam].shooters[currentPlayer].width, game.teams[currentTeam].shooters[currentPlayer].width, background2);
                }

                if (playState == AIM || playState == POWER || playState == RUN_UP || playState == SHOT_GRAPHIC) {
                    undrawImageDMA(currentGoalkeeper.row, currentGoalkeeper.column, currentGoalkeeper.width, currentGoalkeeper.width, background2);
                }

                if (playState == SHOT_GRAPHIC) {
                    undrawImageDMA(SCOREROW, SCORE1COL, 30, 70, background2);
                }

                if (playState == PLAYER_WALKING_UP) {
                    undrawImageDMA(GOALKEEPER_START_SPOT - 20, 0, currentGoalkeeper.width, 70, background2);
                }

                undrawImageDMA(ball.row, ball.col, ball.size, ball.size, background2);

                undrawTrajectory(traj, background2);




                //////////////
                if (gameOver(game.teams[0], game.teams[1])) {
                    switch (playState) {
                        case PLAYER_WALKING_UP: {
                            ball.row = PENALTY_SPOT_ROW;
                            ball.col = PENALTY_SPOT_COLUMN;
                            ball.size = BALL_SIZE;
                            traj.row = ball.row + ball.size / 2;
                            traj.column = ball.col + ball.size / 2;
                            traj.dx = -5;
                            traj.dy = 0;
                            traj.numPoints = 4;
                            trajectoryDyChanger = 1;
                            levelChangeMagnitude = 10;
                            currentGoalkeeper = currentTeam == 0 ? game.teams[1].goalkeeper : game.teams[0].goalkeeper;
                            otherGoalkeeper = currentTeam == 0 ? game.teams[0].goalkeeper : game.teams[1].goalkeeper;
                            otherGoalkeeper.row = GOALKEEPER_SIDE_SPOT;
                            otherGoalkeeper.column = 0;
                            currentGoalkeeper.row = GOALKEEPER_START_SPOT;
                            currentGoalkeeper.column = 0;
                            if (initialPositionColumn == -1 && initialPositionRow == -1) {
                                initialPositionColumn  = game.teams[currentTeam].shooters[currentPlayer].column;;
                                initialPositionRow = game.teams[currentTeam].shooters[currentPlayer].row;
                            }
                            if (animatePlayer(&game.teams[currentTeam].shooters[currentPlayer], 1, SHOOTER_SPOT_ROW, SHOOTER_SPOT_COLUMN)) {
                                playState = AIM;
                            }
                            break;
                        }
                        case AIM : {
                            if (traj.dy < -5 || traj.dy > 5) {
                                trajectoryDyChanger *= -1;
                            }
                            traj.dy += trajectoryDyChanger;
                            if (currentTeam != aiTeamIndex) {
                                int aimPressed = KEY_DOWN(BUTTON_L, currentButtons);

                                if (aimPressed) {
                                    trajectoryDyChanger = 0;
                                    random_number1 = rand() % (max - min + 1) + min;
                                    random_number2 = rand() % (max2 - min2 + 1) + min;
                                    currentGoalkeeperChangeMagnitude = random_number1;
                                    if (random_number2 > 5) {
                                        currentGoalkeeperChangerSign = 1;
                                    } else {
                                        currentGoalkeeperChangerSign = -1;
                                    }

                                    playState = POWER;
                                }
                            } else {
                                int min3 = 0;
                                int max3 = 30;
                                int random_number3 = rand() % (max3 - min3 + 1) + min3;
                                int aimPressed = random_number3 == 12;
                                if (aimPressed) {
                                    trajectoryDyChanger = 0;
                                    playState = POWER;
                                }
                            }
                            break;
                        }
                        case RUN_UP : {
                            if (animatePlayer(&game.teams[currentTeam].shooters[currentPlayer], 1, SHOOTER_SPOT_ROW, SHOOTER_SPOT_COLUMN - 3)) {
                                playState = SHOT_GRAPHIC;
                            }
                            break;

                        }
                        case SHOT_GRAPHIC : {
                            if (ball.col > GOALLINE) {
                                animateShot(&ball,1 +  currentLevel / 100, traj);
                            } else if (goalkeeperSavedShot(currentGoalkeeper, ball)) {
                                scoredGoal = 0;
                                game.teams[currentTeam].shotsRemaining--;
                                playState = WALK_BACK;
                            } else {
                                if (ball.row > 55 && ball.row < 100) {
                                    game.teams[currentTeam].goals++;
                                    if (currentTeam == 0) {
                                        sprintf(score1, "%d", game.teams[0].goals);
                                    } else {
                                        sprintf(score2, "%d", game.teams[1].goals);
                                    }
                                    scoredGoal = 1;
                                } else {
                                    scoredGoal = 0;
                                }
                                game.teams[currentTeam].shotsRemaining--;
                                playState = WALK_BACK;
                            }
                            break;
                        }
                        case POWER : {
                            if (currentLevel > 100 || currentLevel < 15) {
                                levelChangerSign *= -1;
                            }
                            currentLevel += levelChangerSign * levelChangeMagnitude;
                            if (currentTeam != aiTeamIndex) {
                                int powerPressed = KEY_DOWN(BUTTON_R, currentButtons);
                                if (powerPressed) {
                                    levelChangeMagnitude = 0;
                                    playState = RUN_UP;
                                }
                            } else {
                                int min3 = 0;
                                int max3 = 50;
                                int random_number3 = rand() % (max3 - min3 + 1) + min3;
                                int powerPressed = random_number3 == 12;
                                if (powerPressed) {
                                    levelChangeMagnitude = 0;
                                    playState = RUN_UP;
                                }
                            }
                            break;
                        }
                        case WALK_BACK : {
                            if (animatePlayer(&game.teams[currentTeam].shooters[currentPlayer], 1, initialPositionRow, initialPositionColumn)) {
                                if (currentTeam == 1) {
                                    currentTeam = 0;
                                    if (currentPlayer == 4) {
                                        currentPlayer = 0;
                                    } else {
                                        currentPlayer++;
                                    }
                                    if (game.teams[0].shotsRemaining == 0 && game.teams[1].shotsRemaining == 0 &&game.teams[0].goals == game.teams[1].goals) {
                                        game.teams[0].shotsRemaining = 1;
                                        game.teams[1].shotsRemaining = 1;
                                    }
                                } else {
                                    currentTeam = 1;
                                }
                                playState = PLAYER_WALKING_UP;
                                initialPositionRow = -1;
                                initialPositionColumn = -1;
                            }
                            break;
                        }
                    }

                    if (currentTeam == aiTeamIndex) {
                        if (playState == AIM || playState == RUN_UP || playState == POWER || playState == SHOT_GRAPHIC) {
                            int moveKeeperUp = KEY_DOWN(BUTTON_DOWN, currentButtons);
                            int moveKeeperDown = KEY_DOWN(BUTTON_UP, currentButtons);
                            if (moveKeeperUp) {
                                if (currentGoalkeeper.row + userGoalkeeperChangeMagnitude > 85) {
                                    currentGoalkeeper.row = 85;
                                } else {
                                    currentGoalkeeper.row += userGoalkeeperChangeMagnitude;
                                }

                            }
                            if (moveKeeperDown){
                                if (currentGoalkeeper.row  - userGoalkeeperChangeMagnitude< 60)  {
                                    currentGoalkeeper.row = 60;
                                } else {
                                    currentGoalkeeper.row -= userGoalkeeperChangeMagnitude;
                                }
                            }

                        }
                    } else {
                        if (playState == RUN_UP || playState == SHOT_GRAPHIC) {
                            if (currentGoalkeeper.row > 85 || currentGoalkeeper.row < 60) {
                                currentGoalkeeperChangerSign *= -1;
                            }
                            currentGoalkeeper.row += currentGoalkeeperChangerSign * currentGoalkeeperChangeMagnitude;
                        }
                    }
                } else if (game.teams[0].goals < game.teams[1].goals) {
                    state = LOSE;
                } else {
                    state = WIN;
                }


                //////////////
                //Drawing Power Bar
                drawPowerBar(currentLevel);

                drawString(SCOREROW, SCORE1COL, score1, game.teams[0].shooters[0].jerseyColor);
                drawString(SCOREROW, SCORE2COL, score2, game.teams[1].shooters[0].jerseyColor);



                //Drawing Trajectory
                if (playState != SHOT_GRAPHIC && currentTeam != aiTeamIndex) {
                    drawTrajectory(traj, MAGENTA);
                }


                //Disappear Soccer Ball
                if (ball.col > GOALLINE) {
                    drawSoccerBall(ball);
                }

                //Draw all players on the teams
                if (playState == PLAYER_WALKING_UP) {
                    if (shouldDrawAllPlayers) {
                        for (int i = 0; i < 2; i++) {
                            for (int j = 0; j < game.teams[i].numShooters; j++) {
                                drawPlayer(game.teams[i].shooters[j]);
                            }
                            drawPlayer(currentGoalkeeper);
                            drawPlayer(otherGoalkeeper);
                        }
                        shouldDrawAllPlayers = 0;
                    } else {
                        drawPlayer(currentGoalkeeper);
                        drawPlayer(otherGoalkeeper);
                        drawPlayer(game.teams[currentTeam].shooters[currentPlayer]);

                    }
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < game.teams[i].numShooters; j++) {
                            drawPlayer(game.teams[i].shooters[j]);
                        }
                        drawPlayer(currentGoalkeeper);
                        drawPlayer(otherGoalkeeper);
                    }
                } else {
                    drawPlayer(currentGoalkeeper);
                    drawPlayer(game.teams[currentTeam].shooters[currentPlayer]);

                }


                //Write Goal Message
                if (playState == WALK_BACK) {
                    if (scoredGoal) {
                        drawString(GOALMESSAGEROW, GOALMESSAGECOLUMN, "GOAL!!!!!!!!", GREEN);
                    } else {
                        drawString(GOALMESSAGEROW, GOALMESSAGECOLUMN, "MISS!!!!!!!!", RED);
                    }
                } else if (playState == AIM) {
                    char str3[20];
                    sprintf(str3, "%d", game.teams[currentTeam].shotsRemaining);
                    drawString(GOALMESSAGEROW, GOALMESSAGECOLUMN, "SHOTS LEFT", game.teams[currentTeam].shooters[0].jerseyColor);
                    drawString(GOALMESSAGEROW, GOALMESSAGECOLUMN + 70, str3, game.teams[currentTeam].shooters[0].jerseyColor);
                    if (currentTeam != aiTeamIndex) {
                        drawString(TUTORIALROW, TUTORIALCOL, "PRESS A TO AIM", game.teams[currentTeam].shooters[0].jerseyColor);
                    } else {
                        drawString(TUTORIALROW, TUTORIALCOL, "USE UP AND DOWN", game.teams[currentTeam].shooters[0].jerseyColor);
                        drawString(TUTORIALROW + 15, TUTORIALCOL, "ARROW FOR KEEPER", game.teams[currentTeam].shooters[0].jerseyColor);
                    }
                } else if (playState == POWER) {
                    char str3[20];
                    sprintf(str3, "%d", game.teams[currentTeam].shotsRemaining);
                    if (shouldEraseAim) {
                        undrawImageDMA(TUTORIALROW, TUTORIALCOL, GOALMESSAGEERASERWIDTH, GOALMESSAGEERASERHEIGHT, background2);
                        shouldEraseAim = 0;
                    }
                    if (currentTeam != aiTeamIndex) {
                        drawString(TUTORIALROW, TUTORIALCOL, "PRESS S TO SHOOT", game.teams[currentTeam].shooters[0].jerseyColor);
                    } else {
                        drawString(TUTORIALROW, TUTORIALCOL, "USE UP AND DOWN", game.teams[currentTeam].shooters[0].jerseyColor);
                        drawString(TUTORIALROW + 15, TUTORIALCOL, "ARROW FOR KEEPER", game.teams[currentTeam].shooters[0].jerseyColor);
                    }
                } else {
                    undrawImageDMA(GOALMESSAGEROW, GOALMESSAGECOLUMN, GOALMESSAGEERASERWIDTH, GOALMESSAGEERASERHEIGHT, background2);
                    undrawImageDMA(TUTORIALROW, TUTORIALCOL, TUTORIALWIDTH, TUTORIALHEIGHT, background2);
                    shouldEraseAim = 1;
                }
                // state = ?
                break;
            case WIN:
                if (shouldDrawWin) {
                    fillScreenDMA(BLACK);
                    drawImageDMA(20, WIDTH - USA_WIDTH, USA_WIDTH,USA_HEIGHT, usa);
                    drawString(40, 95, "WORLD CUP", game.teams[currentTeam].shooters[1].jerseyColor);
                    drawString(60, 95, "CHAMPIONS", game.teams[currentTeam].shooters[1].jerseyColor);
                    drawImageDMA(20, 0, WORLDCUP_WIDTH,WORLDCUP_HEIGHT, worldcup);
                    shouldDrawWin = 0;
                }
                // state = ?
                break;
            case LOSE:
                if (shouldDrawWin) {
                    fillScreenDMA(BLACK);
                    drawString(40, 95, "WORLD CUP", game.teams[currentTeam].shooters[1].jerseyColor);
                    drawString(60, 95, "CHAMPIONS", game.teams[currentTeam].shooters[1].jerseyColor);
                    drawImageDMA(20, 0, BRAZIL2_WIDTH,BRAZIL2_HEIGHT, brazil2);
                    drawImageDMA(20, WIDTH - USA_WIDTH, WORLDCUP_WIDTH,WORLDCUP_HEIGHT, worldcup);
                    shouldDrawWin = 0;
                }
                // state = ?
                break;
        }

        previousButtons = currentButtons; // Store the current state of the buttons
    }

    UNUSED(previousButtons); // You can remove this once previousButtons is used

    return 0;
}

