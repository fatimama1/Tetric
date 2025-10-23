#include "../../brick_game/tetris/s21_tetris.h"

/**
 * @brief Запускаем игру ТЕТРИС в консоли
 */
int main() {
  init();
  int view_playfield[ROWS][COLUMNS];
  createPlayfield(view_playfield);
  GameInfo_t game;
  initGame(&game);
  Pieces piece = {};
  initPiece(&piece, game.next_pattern);
  initPatterns(game.next_pattern, getRandomPattern());
  UserAction_t action;
  int user_input;
  int counter = 0;
  renderStart(game, view_playfield, "START    ");
  while (game.exit_game != 1) {
    user_input = getch();
    action = userInput(user_input, TRUE);
    if (action != Exit && game.speed != 0) {
      gaming(&game, &piece, action, &counter, view_playfield);
    }
    updateCurrentState(&game, action, view_playfield);
    napms(17);  // 60 кд/с
  }
  finish();
  if (game.high_score < game.score) {
    FILE* file = fopen("record.txt", "w");
    fprintf(file, "%d", game.score);
    fclose(file);
  }
}

void finish() {
  curs_set(1);
  // clear();
  refresh();
  resetty();
  endwin();
}

void init() {
  initscr();
  savetty();
  nonl();
  cbreak();
  noecho();
  timeout(0);
  keypad(stdscr, TRUE);
  leaveok(stdscr, TRUE);
  curs_set(0);
}

void updateCurrentState(GameInfo_t* game, UserAction_t user_action,
                        int view_playfield[ROWS][COLUMNS]) {
  // if(game.exit_game) return;

  if (user_action == Start && game->speed == 0) {
    // renderStart(*game, view_playfield);
    game->game_over = 0;
    game->speed = SPEED;
  } else if (user_action == Exit) {
    game->exit_game = 1;
  } else if (user_action == Pause) {
    renderGame(*game, view_playfield, "PAUSE       ");
    holdPause(game);
  } else if (game->game_over == 0 && game->speed != 0) {
    renderGame(*game, view_playfield, "GAME        ");
  }
}

void renderStart(GameInfo_t game, int view_playfield[ROWS][COLUMNS],
                 char* status) {
  int temp[4][4];
  renderPlayfield(view_playfield);
  renderHighScore(game.high_score);
  renderNextPattern(temp);
  renderLevel(game.level);
  renderCurrScore(game.score);
  renderGameStatus(status);
  refresh();
}

void renderGame(GameInfo_t game, int view_playfield[ROWS][COLUMNS],
                char* status) {
  renderPlayfield(view_playfield);
  renderHighScore(game.high_score);
  renderNextPattern(game.next_pattern);
  renderLevel(game.level);
  renderCurrScore(game.score);
  renderGameStatus(status);
  refresh();
}

void renderPlayfield(const int (*playfield)[10]) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < COLUMNS + 2; j++) {
      move((ROWS + 1) * i, j);
      addch('=');
    }
  }

  for (int i = 0; i < ROWS + 2; i++) {
    for (int j = 0; j < 2; j++) {
      move(i, (COLUMNS + 1) * j);
      addch('|');
    }
  }

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      move(i + 1, j + 1);
      if (playfield[i][j])
        addch('o');
      else
        addch(' ');
    }
  }
}

void renderGameStatus(char* status) {
  move(GAME_STATUS_Y, GAME_STATUS_X);
  printw("%s", status);
}

void renderHighScore(const int score) {
  move(BLOCK_HIGH_SCORE_Y, BLOCK_HIGH_SCORE_X);
  for (int i = 0; i < 2; i++) {
    for (int j = BLOCK_HIGH_SCORE_X; j < BLOCK_HIGH_SCORE_X + 12; j++) {
      move((BLOCK_HIGH_SCORE_Y + 2) * i, j);
      addch('=');
    }
  }

  for (int j = 0; j < 2; j++) {
    move(BLOCK_HIGH_SCORE_Y + 1, (BLOCK_HIGH_SCORE_X) + 11 * j);
    addch('|');
  }

  move(HIGH_SCORE_Y, HIGH_SCORE_X);
  printw("BEST SCORE");
  move(HIGH_SCORE_Y + 2, HIGH_SCORE_X);
  printw("%d", score);
}

void renderNextPattern(int block[SIDE_OF_BLOCK][SIDE_OF_BLOCK]) {
  move(BLOCK_NEXT_PATTERN_Y, BLOCK_NEXT_PATTERN_X);
  for (int i = 0; i < 2; i++) {
    for (int j = BLOCK_NEXT_PATTERN_X; j < BLOCK_NEXT_PATTERN_X + 12; j++) {
      move((BLOCK_NEXT_PATTERN_Y) + 2 * i, j);
      addch('=');
    }
  }
  for (int j = 0; j < 2; j++) {
    move(BLOCK_NEXT_PATTERN_Y + 1, (BLOCK_NEXT_PATTERN_X) + 11 * j);
    addch('|');
  }
  move(BLOCK_NEXT_PATTERN_Y + 1, BLOCK_NEXT_PATTERN_X + 1);
  printw("NEXT PIECE");
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 2; j++) {
      move(BLOCK_NEXT_PATTERN_Y + 3 + i, (BLOCK_NEXT_PATTERN_X) + 11 * j);
      addch('|');
    }
  }
  for (int j = BLOCK_NEXT_PATTERN_X; j < BLOCK_NEXT_PATTERN_X + 12; j++) {
    move(BLOCK_NEXT_PATTERN_Y + 8, j);
    addch('=');
  }
  for (int i = 0; i < SIDE_OF_BLOCK; i++) {
    for (int j = 0; j < SIDE_OF_BLOCK; j++) {
      if (block[i][j]) {
        move(BLOCK_NEXT_PATTERN_Y + 4 + i, BLOCK_NEXT_PATTERN_X + 5 + j);
        addch('o');
      } else {
        move(BLOCK_NEXT_PATTERN_Y + 4 + i, BLOCK_NEXT_PATTERN_X + 5 + j);
        addch(' ');
      }
    }
  }
}

void renderLevel(const int level) {
  move(BLOCK_LEVEL_Y, BLOCK_LEVEL_X);
  for (int i = 0; i < 2; i++) {
    for (int j = BLOCK_LEVEL_X; j < BLOCK_LEVEL_X + 12; j++) {
      move((BLOCK_LEVEL_Y) + 2 * i, j);
      addch('=');
    }
  }
  for (int j = 0; j < 2; j++) {
    move(BLOCK_LEVEL_Y + 1, (BLOCK_LEVEL_X) + 11 * j);
    addch('|');
  }
  move(BLOCK_LEVEL_Y + 1, BLOCK_LEVEL_X + 1);
  printw("   LEVEL");
  move(BLOCK_LEVEL_Y + 3, BLOCK_LEVEL_X + 1);
  printw("     %d", level);
}

void holdPause(GameInfo_t* game) {
  char c;
  while ((c = getch()) != ' ' && c != 'q' && c != 'Q')
    ;
  if (c == 'q' || c == 'Q') game->exit_game = 1;
}

void renderCurrScore(const int score) {
  move(BLOCK_CURR_SCORE_Y, BLOCK_CURR_SCORE_X);
  for (int i = 0; i < 2; i++) {
    for (int j = BLOCK_CURR_SCORE_X; j < BLOCK_CURR_SCORE_X + 12; j++) {
      move((BLOCK_CURR_SCORE_Y + 2) * i, j);
      addch('=');
    }
  }

  for (int j = 0; j < 2; j++) {
    move(BLOCK_CURR_SCORE_Y + 1, (BLOCK_CURR_SCORE_X) + 11 * j);
    addch('|');
  }

  move(CURR_SCORE_Y, CURR_SCORE_X);
  printw("CURR SCORE");
  move(CURR_SCORE_Y + 2, CURR_SCORE_X);
  printw("%d", score);
}

void gaming(GameInfo_t* game, Pieces* piece, UserAction_t action, int* counter,
            int (*view_playfield)[10]) {
  if (isOutOfBounds(game->playfield, *piece)) {
    renderGame(*game, view_playfield, "GAME OVER    ");
    initGame(game);
    createPlayfield(view_playfield);
    game->game_over = 1;
  } else if (game->game_over == 0) {
    moving(game->playfield, piece, action);
    getState(game->playfield, view_playfield, *piece);
    if (*counter == game->speed) {
      int temp_y = piece->y;
      movePieceDown(game->playfield, piece);
      if (temp_y == piece->y) {
        lockPiece(game->playfield, *piece);
        initPiece(piece, game->next_pattern);
        initPatterns(game->next_pattern, getRandomPattern());
      }
      *counter = 0;
    }
    clearLines(game);
    (*counter)++;
  }
}