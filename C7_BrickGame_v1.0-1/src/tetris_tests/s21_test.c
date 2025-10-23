#include "s21_test.h"

START_TEST(test_get_random_shape) {
  int result = getRandomPattern();
  ck_assert_int_le(result, 6);
  ck_assert_int_ge(result, 0);
}
END_TEST

START_TEST(test_userInput_up) { ck_assert_int_eq(userInput(KEY_UP, true), Up); }
END_TEST

START_TEST(test_userInput_down) {
  ck_assert_int_eq(userInput(KEY_DOWN, true), Down);
}
END_TEST

START_TEST(test_userInput_left) {
  ck_assert_int_eq(userInput(KEY_LEFT, true), Left);
}
END_TEST

START_TEST(test_userInput_right) {
  ck_assert_int_eq(userInput(KEY_RIGHT, true), Right);
}
END_TEST

START_TEST(test_userInput_space) {
  ck_assert_int_eq(userInput(' ', true), Pause);
}
END_TEST

START_TEST(test_userInput_start) {
  ck_assert_int_eq(userInput(10, true), Start);
}
END_TEST

START_TEST(test_userInput_start_1) {
  ck_assert_int_eq(userInput(13, true), Start);
}
END_TEST

START_TEST(test_userInput_exit) {
  ck_assert_int_eq(userInput('q', true), Exit);
}
END_TEST

START_TEST(test_userInput_exit_1) {
  ck_assert_int_eq(userInput('Q', true), Exit);
}
END_TEST

START_TEST(test_create_matrix) {
  int matrix[20][10];
  createPlayfield(matrix);

  ck_assert_ptr_nonnull(matrix);

  for (int i = 0; i < 20; i++) {
    ck_assert_ptr_nonnull(matrix[i]);
  }
}
END_TEST

START_TEST(test_create_piece) {
  Pieces piece;
  int temp[4][4] = PATTERN_1;
  initPiece(&piece, temp);

  ck_assert_int_eq(piece.x, 3);
  ck_assert_int_eq(piece.y, 0);

  ck_assert_ptr_nonnull(piece.block);

  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(piece.block[i]);
  }
}
END_TEST

START_TEST(test_init_game) {
  GameInfo_t game;
  initGame(&game);

  ck_assert_int_eq(game.level, 1);
  ck_assert_int_eq(game.speed, 0);
  ck_assert_int_eq(game.game_over, 0);
  ck_assert_int_eq(game.exit_game, 0);
  ck_assert_int_eq(game.score, 0);
}
END_TEST

START_TEST(test_get_state) {
  int view_playfield[20][10];
  int playfield[20][10] = {0};
  int temp[4][4] = PATTERN_2;
  Pieces piece;
  initPiece(&piece, temp);
  getState(playfield, view_playfield, piece);

  ck_assert_ptr_nonnull(view_playfield);

  for (int i = 0; i < 20; i++) {
    ck_assert_ptr_nonnull(view_playfield[i]);
  }
}
END_TEST

START_TEST(test_patterns) {
  int temp[4][4];
  Pattern pattern = PATTERN1;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }

  pattern = PATTERN2;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }

  pattern = PATTERN3;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }

  pattern = PATTERN4;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }

  pattern = PATTERN5;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }

  pattern = PATTERN6;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }

  pattern = PATTERN7;
  initPatterns(temp, pattern);

  ck_assert_ptr_nonnull(temp);
  for (int i = 0; i < 4; i++) {
    ck_assert_ptr_nonnull(temp[i]);
  }
}
END_TEST

START_TEST(test_moving) {
  int playfield[20][10] = {0};
  UserAction_t action = Left;
  int temp[4][4] = PATTERN_2;
  Pieces piece;
  initPiece(&piece, temp);

  moving(playfield, &piece, action);
  ck_assert_int_eq(piece.x, 2);
  ck_assert_int_eq(piece.y, 0);

  moving(playfield, &piece, Right);
  ck_assert_int_eq(piece.x, 3);
  ck_assert_int_eq(piece.y, 0);

  moving(playfield, &piece, Down);
  ck_assert_int_eq(piece.x, 3);
  ck_assert_int_eq(piece.y, 1);

  moving(playfield, &piece, Up);
  ck_assert_int_eq(piece.x, 3);
  ck_assert_int_eq(piece.y, 1);
}
END_TEST

START_TEST(test_lock_piece) {
  int playfield[20][10] = {0};
  int temp[4][4] = PATTERN_2;
  Pieces piece;
  initPiece(&piece, temp);

  lockPiece(playfield, piece);

  ck_assert_ptr_nonnull(playfield);
  for (int i = 0; i < 20; i++) {
    ck_assert_ptr_nonnull(playfield[i]);
  }
}
END_TEST

START_TEST(test_clear_lines) {
  GameInfo_t game;
  initGame(&game);

  for (int j = 0; j < 10; j++) {
    game.playfield[15][j] = 1;
  }
  clearLines(&game);

  ck_assert_ptr_nonnull(game.playfield);
  for (int i = 0; i < 20; i++) {
    ck_assert_ptr_nonnull(game.playfield[i]);
  }
}
END_TEST

Suite *tetris_suite(void) {
  Suite *suite;
  suite = suite_create("Functions checking");
  TCase *tc = tcase_create("s21_tetris_tc");

  tcase_add_test(tc, test_get_random_shape);
  tcase_add_test(tc, test_userInput_up);
  tcase_add_test(tc, test_userInput_down);
  tcase_add_test(tc, test_userInput_left);
  tcase_add_test(tc, test_userInput_right);
  tcase_add_test(tc, test_userInput_space);
  tcase_add_test(tc, test_userInput_start);
  tcase_add_test(tc, test_userInput_start_1);
  tcase_add_test(tc, test_userInput_exit);
  tcase_add_test(tc, test_userInput_exit_1);
  tcase_add_test(tc, test_create_matrix);
  tcase_add_test(tc, test_create_piece);
  tcase_add_test(tc, test_init_game);
  tcase_add_test(tc, test_get_state);
  tcase_add_test(tc, test_patterns);
  tcase_add_test(tc, test_moving);
  tcase_add_test(tc, test_lock_piece);
  tcase_add_test(tc, test_clear_lines);

  suite_add_tcase(suite, tc);

  return suite;
}

void run_testcase(Suite *testcase) {
  static int counter_testcase = 1;

  if (counter_testcase > 0) putchar('\n');
  counter_testcase++;
  SRunner *sr = srunner_create(testcase);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
}

int main(void) {
  run_testcase(tetris_suite());
  return 0;
}