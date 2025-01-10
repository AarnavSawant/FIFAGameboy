#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#include "string.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

struct player {
    char* name;
    char* number;
    int row;
    int column;
    int width;
    u16 jerseyColor;
    u16 textColor;
};

struct ball {
    int row;
    int col;
    int size;
};

#define PENALTY_SPOT_ROW  77
#define PENALTY_SPOT_COLUMN 32
#define BALL_SIZE  3
#define SHOOTER_SPOT_ROW 72
#define SHOOTER_SPOT_COLUMN 40
#define GOALKEEPER_SIDE_SPOT 20
#define GOALKEEPER_START_SPOT 70
#define SCORE1COL 170
#define SCORE2COL 190
#define SCOREROW 10
#define GOALLINE 3
#define GOALMESSAGEROW 130
#define GOALMESSAGECOLUMN 140
#define GOALMESSAGEERASERWIDTH 90
#define GOALMESSAGEERASERHEIGHT 15
#define TUTORIALROW 130
#define TUTORIALCOL 5
#define TUTORIALWIDTH 100
#define TUTORIALHEIGHT 30



struct team {
    char *name;
    char *abbreviation;
    struct player shooters[5];
    struct player goalkeeper;
    int numShooters;
    int goals;
    int shotsRemaining;
};

struct trajectory {
    int row;
    int column;
    int dx;
    int dy;
    int numPoints;
};

enum play_state {
    PLAYER_WALKING_UP,
    AIM,
    POWER,
    SHOT_GRAPHIC,
    RUN_UP,
    WALK_BACK
};

struct game {
    struct team teams[2];
    int awayScore;
    int homeScore;
};


#endif
