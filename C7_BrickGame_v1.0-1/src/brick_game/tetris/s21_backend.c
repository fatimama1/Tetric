#include "s21_tetris.h"

/**
 * @brief Преобразуем пользовательский ввод в структуру
 *
 * @param file Файл, в котором хранится рекордный результат
 * @param figures Массив шаблонов фигур
 *
 * @return Возвращается созданный объект игры
 */

void createPlayfield(int (*playfield)[10]) {
  for (int i = 0; i < ROWS; i++) {  //инициализируем поле
    for (int j = 0; j < COLUMNS; j++) {
      playfield[i][j] = 0;
    }
  }
}

void getState(int (*playfield)[10], int (*view_playfield)[10], Pieces piece) {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLUMNS; x++) {
      view_playfield[y][x] = playfield[y][x];
    }
  }

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (piece.block[y][x]) view_playfield[piece.y + y][piece.x + x] = 1;
    }
  }
}

void initGame(GameInfo_t* game) {
  game->level = 1;
  createPlayfield(game->playfield);
  game->score = 0;
  FILE* file = fopen("record.txt", "r+");  // Открываем файл для чтения и записи
  if (file == NULL) {  // Если файл не существует, создаем его
    file = fopen("record.txt", "w+");
    if (file == NULL) {
      exit(EXIT_FAILURE);
    }
  }
  int temp = 0;
  if (fscanf(file, "%d", &temp) != 1) {  // Считываем число из файла
    game->high_score = 0;
  } else
    game->high_score = temp;
  fclose(file);
  initPatterns(game->next_pattern, getRandomPattern());
  game->speed = 0;
  game->game_over = FALSE;
  game->exit_game = FALSE;
}

void initPiece(Pieces* piece, int pattern[4][4]) {
  piece->x = 3;
  piece->y = 0;
  copyBlock(pattern, piece->block);
}

void initPatterns(
    int block[4][4],
    Pattern pattern) {  // не забудь поменять паттерны при переходе на 4х4
  switch (pattern) {
    case PATTERN1:
      int temp1[4][4] = PATTERN_1;
      copyBlock(temp1, block);
      break;
    case PATTERN2:
      int temp2[4][4] = PATTERN_2;
      copyBlock(temp2, block);
      break;
    case PATTERN3:
      int temp3[4][4] = PATTERN_3;
      copyBlock(temp3, block);
      break;
    case PATTERN4:
      int temp4[4][4] = PATTERN_4;
      copyBlock(temp4, block);
      break;
    case PATTERN5:
      int temp5[4][4] = PATTERN_5;
      copyBlock(temp5, block);
      break;
    case PATTERN6:
      int temp6[4][4] = PATTERN_6;
      copyBlock(temp6, block);
      break;
    case PATTERN7:
      int temp7[4][4] = PATTERN_7;
      copyBlock(temp7, block);
      break;
  }
}

void moving(int (*playfield)[10], Pieces* piece, UserAction_t action) {
  if (action == Left)
    movePieceLeft(playfield, piece);
  else if (action == Right)
    movePieceRight(playfield, piece);
  else if (action == Down)
    movePieceDown(playfield, piece);
  else if (action == Up)
    rotatePiece(playfield, piece);
}

void movePieceLeft(int (*playfield)[10], Pieces* piece) {
  piece->x--;
  if (isOutOfBounds(playfield, *piece)) piece->x++;
}

void movePieceRight(int (*playfield)[10], Pieces* piece) {
  piece->x++;
  if (isOutOfBounds(playfield, *piece)) piece->x--;
}

void movePieceDown(int (*playfield)[10], Pieces* piece) {
  piece->y++;
  if (isOutOfBounds(playfield, *piece)) piece->y--;
}

void rotatePiece(int (*playfield)[10], Pieces* piece) {
  int new_block[4][4];
  int temp[4][4];

  copyBlock(piece->block, new_block);
  copyBlock(piece->block, temp);
  transposeBlock(new_block);
  copyBlock(new_block, piece->block);

  if (isOutOfBounds(playfield, *piece)) copyBlock(temp, piece->block);
}

int isOutOfBounds(int (*playfield)[10], Pieces piece) {
  int out = FALSE;
  for (int y = 0; y < 4 && out == FALSE; y++) {
    for (int x = 0; x < 4 && out == FALSE; x++) {
      if (piece.block[y][x] == 1 &&
          (piece.x + x > 9 || piece.x + x < 0 || piece.y + y > 19 ||
           piece.y + y < 0 || playfield[piece.y + y][piece.x + x] == 1))
        out = TRUE;
    }
  }

  return out;
}

void lockPiece(int (*playfield)[10],
               Pieces piece) {  // перед ней всегда проверка
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (piece.block[y][x] == 1) playfield[piece.y + y][piece.x + x] = 1;
    }
  }
}

void clearLines(GameInfo_t* game) {
  int lines[ROWS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int count_lines = 0;
  for (int i = ROWS - 1; i > -1; i--) {
    int count_block = 0;
    for (int j = 0; j < COLUMNS; j++) {
      count_block += game->playfield[i][j];
    }
    if (count_block == COLUMNS) {
      lines[i] = 1;
      count_lines++;
    }
  }
  for (int i = 0; i < ROWS; i++) {
    if (lines[i]) {
      for (int y = i; y > 0; y--) {
        for (int x = 0; x < COLUMNS; x++) {
          game->playfield[y][x] = game->playfield[y - 1][x];
        }
      }
      for (int j = 0; j < COLUMNS; j++) {
        game->playfield[0][j] = 0;
      }
    }
  }
  if (count_lines) updateScore(game, count_lines);
}

void updateScore(GameInfo_t* game, int count_lines) {
  if (count_lines == 2)
    game->score += 300;
  else if (count_lines == 3)
    game->score += 700;
  else if (count_lines == 4)
    game->score += 1500;
  else
    game->score += 100;

  game->level = game->score / 500;
  if (game->level > 10) game->level = 10;
  game->speed = SPEED - (game->level - 1) * 5;
}

void transposeBlock(int block[4][4]) {
  for (int i = 0; i < 4; ++i) {
    for (int j = i + 1; j < 4; ++j) {
      int temp = block[i][j];
      block[i][j] = block[j][i];
      block[j][i] = temp;
    }
  }

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 2; ++j) {
      int temp = block[i][j];
      block[i][j] = block[i][3 - j];
      block[i][3 - j] = temp;
    }
  }
}

void copyBlock(int origin[4][4], int new[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      new[i][j] = origin[i][j];
    }
  }
}

UserAction_t userInput(int user_input, bool hold) {
  UserAction_t action = No_enter;
  if (hold) {
    if (user_input == KEY_UP)
      action = Up;
    else if (user_input == KEY_DOWN)
      action = Down;
    else if (user_input == KEY_LEFT)
      action = Left;
    else if (user_input == KEY_RIGHT)
      action = Right;
    else if (user_input == ' ')
      action = Pause;
    else if (user_input == 10 || user_input == 13)
      action = Start;
    else if (user_input == 'q' || user_input == 'Q')
      action = Exit;
  }
  return action;
}

Pattern getRandomPattern() {
  time_t now;
  struct tm* work_time;

  now = time(0);
  if ((work_time = localtime(&now)) == NULL) {
    return 1;
  }
  return work_time->tm_sec % 7;
}