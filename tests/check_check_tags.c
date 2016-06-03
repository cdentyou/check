/*
 * Check: a unit test framework for C
 * Copyright (C) 2001, 2002 Arien Malec
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */
#include "../lib/libcompat.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <assert.h>
#include "check_check.h"

/* from check_check_master.c */
#define MAXSTR 300
char * escape_percent(const char *original, size_t original_size);


START_TEST(red_test1)
{
  ck_assert_msg(0, "Red fail");
}
END_TEST

START_TEST(blue_test1)
{
  ck_assert_msg(0, "Blue fail");
}
END_TEST

START_TEST(purple_test1)
{
  ck_assert_msg(0, "Purple fail");
}
END_TEST

START_TEST(yellow_test1)
{
  ck_assert_msg(0, "Yellow fail");
}
END_TEST

START_TEST(black_test1)
{
  ck_assert_msg(0, "Black fail");
}
END_TEST


static Suite *make_tagged_suite(void)
{
  Suite *s;

  TCase *red, *blue, *purple, *yellow, *black;

  s = suite_create("Check Tag Filtering");

  red = tcase_create_tagged("Red", "Red");
  suite_add_tcase (s, red);
  tcase_add_test(red, red_test1);

  blue = tcase_create_tagged("Blue", "Blue");
  suite_add_tcase (s, blue);
  tcase_add_test(blue, blue_test1);

  purple = tcase_create_tagged("Purple", "Red Blue");
  suite_add_tcase (s, purple);
  tcase_add_test(purple, purple_test1);

  yellow = tcase_create_tagged("Yellow", "Yellow");
  suite_add_tcase (s, yellow);
  tcase_add_test(yellow, yellow_test1);

  black = tcase_create("Black");
  suite_add_tcase (s, black);
  tcase_add_test(black, black_test1);

  return s;
}

static   SRunner *sr = NULL;

static void tag_test_setup(void)
{
  Suite *s;

  s = make_tagged_suite();;
  sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);

}

static void tag_test_teardown (void)
{
  srunner_free (sr);
}

/*
 * Show that with no filter we run all the tests
 */
START_TEST(no_filter)
{
  const char *expected_msgs[5] = { "Red fail",
				   "Blue fail",
				   "Purple fail",
				   "Yellow fail",
				   "Black fail"};
  TestResult **tr_fail_array;
  unsigned int ntests_failed;
  unsigned int ntests_run;
  size_t n_expected_msgs;
  const char *got_msg;
  unsigned int i;
  TestResult *tr;


  n_expected_msgs = sizeof(expected_msgs)/sizeof(expected_msgs[0]);

  srunner_run(sr, NULL, NULL, NULL, NULL, CK_VERBOSE);

  ntests_run = srunner_ntests_run(sr);
  ck_assert_msg(ntests_run == n_expected_msgs,
		"Did not run expected num tests expected %u but got %u",
		n_expected_msgs, ntests_run);

  ntests_failed = srunner_ntests_failed(sr);
  ck_assert_msg(ntests_failed == n_expected_msgs,
		"Expected to fail %u tests but actually failed %u",
		n_expected_msgs, ntests_failed);

  tr_fail_array = srunner_failures(sr);

  for (i=0; i < n_expected_msgs; i++) {
    tr = tr_fail_array[i];
    got_msg = tr_msg(tr);

    if (strcmp(got_msg, expected_msgs[i]) != 0) {
      char *emsg;
      char *tmp = (char *)malloc(MAXSTR);
      snprintf(tmp, MAXSTR,"Expected %s, got %s",
               expected_msgs[i], got_msg);

      /*
       * NOTE: ck_abort_msg() will take the passed string
       * and feed it to printf. We need to escape any
       * '%' found, else they will result in odd formatting
       * in ck_abort_msg().
       */
      emsg = escape_percent(tmp, MAXSTR);
      free(tmp);

      ck_abort_msg(emsg);
      free(emsg);
    }
  }
} END_TEST

/*
 * Show that we can select just "Yellow" tagged test cases
 */
START_TEST(inc_yellow)
{
  const char *expected_msgs[1] = { "Yellow fail" };
  TestResult **tr_fail_array;
  unsigned int ntests_failed;
  unsigned int ntests_run;
  size_t n_expected_msgs;
  const char *got_msg;
  unsigned int i;
  TestResult *tr;

  n_expected_msgs = sizeof(expected_msgs)/sizeof(expected_msgs[0]);

  srunner_run(sr, NULL, NULL, "Yellow", NULL, CK_VERBOSE);

  ntests_run = srunner_ntests_run(sr);
  ck_assert_msg(ntests_run == n_expected_msgs,
		"Did not run expected num tests expected %u but got %u",
		n_expected_msgs, ntests_run);

  ntests_failed = srunner_ntests_failed(sr);
  ck_assert_msg(ntests_failed == n_expected_msgs,
		"Expected to fail %u tests but actually failed %u",
		n_expected_msgs, ntests_failed);

  tr_fail_array = srunner_failures(sr);


  for (i=0; i < n_expected_msgs; i++) {
    tr = tr_fail_array[i];
    got_msg = tr_msg(tr);

    if (strcmp(got_msg, expected_msgs[i]) != 0) {
      char *emsg;
      char *tmp = (char *)malloc(MAXSTR);
      snprintf(tmp, MAXSTR,"Expected %s, got %s",
               expected_msgs[i], got_msg);

      /*
       * NOTE: ck_abort_msg() will take the passed string
       * and feed it to printf. We need to escape any
       * '%' found, else they will result in odd formatting
       * in ck_abort_msg().
       */
      emsg = escape_percent(tmp, MAXSTR);
      free(tmp);

      ck_abort_msg(emsg);
      free(emsg);
    }
  }
} END_TEST

/*
 * Show that selecting "Red" tagged test cases gives us red and purple
 */
START_TEST(inc_red)
{
  const char *expected_msgs[2] = { "Red fail", "Purple fail" };
  TestResult **tr_fail_array;
  unsigned int ntests_failed;
  unsigned int ntests_run;
  size_t n_expected_msgs;
  const char *got_msg;
  unsigned int i;
  TestResult *tr;


  n_expected_msgs = sizeof(expected_msgs)/sizeof(expected_msgs[0]);

  srunner_run(sr, NULL, NULL, "Red", NULL, CK_VERBOSE);
  ntests_run = srunner_ntests_run(sr);

  ck_assert_msg(ntests_run == n_expected_msgs,
		"Expected to run %u tests but actually ran %u",
		n_expected_msgs, ntests_run);

  ntests_failed = srunner_ntests_failed(sr);

  ck_assert_msg(ntests_failed == n_expected_msgs,
		"Expected to fail %u tests but actually failed %u",
		n_expected_msgs, ntests_failed);

  tr_fail_array = srunner_failures(sr);


  for (i=0; i < n_expected_msgs; i++) {
    tr = tr_fail_array[i];
    got_msg = tr_msg(tr);

    if (strcmp(got_msg, expected_msgs[i]) != 0) {
      char *emsg;
      char *tmp = (char *)malloc(MAXSTR);

      snprintf(tmp, MAXSTR,"Expected %s, got %s",
               expected_msgs[i], got_msg);

      /*
       * NOTE: ck_abort_msg() will take the passed string
       * and feed it to printf. We need to escape any
       * '%' found, else they will result in odd formatting
       * in ck_abort_msg().
       */
      emsg = escape_percent(tmp, MAXSTR);
      free(tmp);

      ck_abort_msg(emsg);
      free(emsg);
    }
  }

} END_TEST

Suite *make_tag_suite(void)
{
  TCase *no_filters, *inc_filters, *exc_filters, *inc_exc_filters;
  Suite *s;

  s = suite_create("Check Tag Filtering");

  no_filters = tcase_create("no tag filters");
  suite_add_tcase (s, no_filters);
  tcase_add_test(no_filters, no_filter);
  tcase_add_unchecked_fixture (no_filters,
                                 tag_test_setup,
                                 tag_test_teardown);

  inc_filters = tcase_create("include tags");
  suite_add_tcase (s, inc_filters);
  tcase_add_test(inc_filters, inc_yellow);
  tcase_add_test(inc_filters, inc_red);

  tcase_add_unchecked_fixture (inc_filters,
                               tag_test_setup,
                               tag_test_teardown);
  return s;
}
