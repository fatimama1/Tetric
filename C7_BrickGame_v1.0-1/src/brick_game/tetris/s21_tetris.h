#ifndef TETRIS
#define TETRIS

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SPEED 60

#define ROWS 20
#define COLUMNS 10

#define BLOCK_HIGH_SCORE_Y 0
#define BLOCK_HIGH_SCORE_X 16

#define HIGH_SCORE_Y 1
#define HIGH_SCORE_X 17

#define BLOCK_CURR_SCORE_Y 18
#define BLOCK_CURR_SCORE_X 16

#define CURR_SCORE_Y 19
#define CURR_SCORE_X 17

#define BLOCK_NEXT_PATTERN_Y 5
#define BLOCK_NEXT_PATTERN_X 16

#define BLOCK_LEVEL_Y 14
#define BLOCK_LEVEL_X 16

#define GAME_STATUS_Y 22
#define GAME_STATUS_X 1

#define TRUE 1
#define FALSE 0

#define PATTERN_1                                            \
  {                                                          \
    {0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, { 0, 0, 0, 0 } \
  }
#define PATTERN_2                                            \
  {                                                          \
    {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, { 0, 0, 0, 0 } \
  }
#define PATTERN_3                                            \
  {                                                          \
    {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, { 0, 0, 0, 0 } \
  }
#define PATTERN_4                                            \
  {                                                          \
    {0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, { 0, 0, 0, 0 } \
  }
#define PATTERN_5                                            \
  {                                                          \
    {0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 1}, { 0, 0, 0, 0 } \
  }
#define PATTERN_6                                            \
  {                                                          \
    {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, { 0, 0, 0, 0 } \
  }
#define PATTERN_7                                            \
  {                                                          \
    {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, { 0, 0, 0, 0 } \
  }

#define SIDE_OF_BLOCK 4

typedef enum {
  PATTERN1,
  PATTERN2,
  PATTERN3,
  PATTERN4,
  PATTERN5,
  PATTERN6,
  PATTERN7
} Pattern;

typedef struct {
  int playfield[20][10];
  int next_pattern[4][4];
  int score;
  int high_score;
  int level;
  int speed;
  int game_over;
  int exit_game;
} GameInfo_t;

typedef enum {
  Start,   // 0, enter
  Pause,   // 1, keyspace
  Exit,    // 2 q
  Left,    // 3 key_left
  Right,   // 4 key_right
  Up,      // 5 key_up
  Down,    // 6 key_down
  Action,  // 7 key_up - rotate
  No_enter
} UserAction_t;

typedef struct pieces {
  int block[4][4];
  int x;
  int y;
} Pieces;

void movePieceLeft(int (*playfield)[10], Pieces* piece);
void movePieceRight(int (*playfield)[10], Pieces* piece);
void movePieceDown(int (*playfield)[10], Pieces* piece);
int isOutOfBounds(int (*playfield)[10], Pieces piece);
void rotatePiece(int (*playfield)[10], Pieces* piece);
void initGame(GameInfo_t* game);
void updateScore(GameInfo_t* game, int lines);
void lockPiece(int (*playfield)[10], Pieces piece);
void transposeBlock(int block[4][4]);
void createPlayfield(int (*playfield)[10]);
void initPiece(Pieces* piece, int pattern[4][4]);
void initPatterns(int block[4][4], Pattern pattern);
void copyBlock(int origin[4][4], int new[4][4]);

void clearLines(GameInfo_t* game);

void getState(int (*playfield)[10], int (*view_playfield)[10], Pieces piece);

UserAction_t userInput(int user_input, bool hold);

Pattern getRandomPattern();
void moving(int (*playfield)[10], Pieces* piece, UserAction_t action);

//
//
//

void finish();
void init();

void updateCurrentState(GameInfo_t* game, UserAction_t user_action,
                        int view_playfield[ROWS][COLUMNS]);

void holdPause(GameInfo_t* game);
void renderStart(GameInfo_t game, int view_playfield[ROWS][COLUMNS],
                 char* status);
void renderGame(GameInfo_t game, int view_playfield[ROWS][COLUMNS],
                char* status);

void renderPlayfield(const int (*playfield)[10]);
void renderHighScore(const int score);
void renderCurrScore(const int score);
void renderNextPattern(int block[4][4]);
void renderLevel(const int level);
void renderGameStatus(char* status);
void gaming(GameInfo_t* game, Pieces* piece, UserAction_t action, int* counter,
            int (*view_playfield)[10]);

#endif