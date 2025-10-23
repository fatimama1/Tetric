#ifndef TEST
#define TEST

#include <check.h>
#include <limits.h>
#include <stdio.h>

#include "../brick_game/tetris/s21_tetris.h"

Suite *tetris_suite(void);
void run_testcase(Suite *testcase);

#endif