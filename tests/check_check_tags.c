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

enum {
    RED,
    BLUE,
    PURPLE,
    YELLOW,
    BLACK,
    MAX_TESTS
} test_ids;

unsigned int test_executed[MAX_TESTS];

static void reset_executed(void)
{
    unsigned int i;

    for (i = 0; i < MAX_TESTS; i++)
    {
	test_executed[i] = 0;
    }
}

START_TEST(red_test1)
{
    test_executed[RED] = 1;
}
END_TEST

START_TEST(blue_test1)
{
    test_executed[BLUE] = 1;
}
END_TEST

START_TEST(purple_test1)
{
    test_executed[PURPLE] = 1;
}
END_TEST

START_TEST(yellow_test1)
{
    test_executed[YELLOW] = 1;
}
END_TEST

START_TEST(black_test1)
{
    test_executed[BLACK] = 1;
}
END_TEST


static Suite *make_tagged_suite(void)
{
    Suite *s;

    TCase *red, *blue, *purple, *yellow, *black;

    s = suite_create("Check Tag Filtering");

    red = tcase_create("Red");
    tcase_set_tags(red, "Red");

    suite_add_tcase (s, red);
    tcase_add_test(red, red_test1);

    blue = tcase_create("Blue");
    tcase_set_tags(blue, "Blue");
    suite_add_tcase (s, blue);
    tcase_add_test(blue, blue_test1);

    purple = tcase_create("Purple");
    tcase_set_tags(purple, "Red Blue");
    suite_add_tcase (s, purple);
    tcase_add_test(purple, purple_test1);

    yellow = tcase_create("Yellow");
    tcase_set_tags(yellow, "Yellow");
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

    s = make_tagged_suite();
    sr = srunner_create(s);
    srunner_set_fork_status(sr, CK_NOFORK);
}

static void tag_test_teardown (void)
{
    srunner_free (sr);
}

START_TEST(set_get_basic)
{
    char ret_str[100];
    const char *expected_str = "Red Blue Green Yellow";
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    tcase_set_tags(tc, expected_str);
    ret_val = tcase_get_tags(tc, ret_str, sizeof(ret_str));

    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(expected_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(expected_str));
    
    ck_assert_msg(strcmp(ret_str, expected_str) ==  0,
	      "Got '%s' instead of '%s'",
	      ret_str, expected_str);
} END_TEST;

START_TEST(set_get_empty)
{
    char ret_str[100];
    const char *expected_str = "";
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    tcase_set_tags(tc, expected_str);
    ret_val = tcase_get_tags(tc, ret_str, sizeof(ret_str));

    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(expected_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(expected_str));
    
    ck_assert_msg(strcmp(ret_str, expected_str) ==  0,
	      "Got '%s' instead of '%s'",
	      ret_str, expected_str);
} END_TEST;

START_TEST(set_get_null)
{
    char ret_str[100];
    const char *expected_str = "";
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    tcase_set_tags(tc, NULL);
    ret_val = tcase_get_tags(tc, ret_str, sizeof(ret_str));

    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(expected_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(expected_str));
    
    ck_assert_msg(strcmp(ret_str, expected_str) ==  0,
	      "Got '%s' instead of '%s'",
	      ret_str, expected_str);
} END_TEST;

START_TEST(init_get)
{
    char ret_str[100];
    const char *expected_str = "";
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    ret_val = tcase_get_tags(tc, ret_str, sizeof(ret_str));

    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(expected_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(expected_str));
    
    ck_assert_msg(strcmp(ret_str, expected_str) ==  0,
	      "Got '%s' instead of '%s'",
	      ret_str, expected_str);
} END_TEST;

START_TEST(set_get_truncated)
{
    char ret_str[10];
    const char *actual_str = "Red Blue Green Yellow";
    char *expected_str;
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    tcase_set_tags(tc, actual_str);
    ret_val = tcase_get_tags(tc, ret_str, sizeof(ret_str));
    
    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(actual_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(actual_str));
    
    expected_str = strdup(actual_str);
    expected_str[sizeof(ret_str) - 1] = '\0';

    ck_assert_msg(strcmp(ret_str, expected_str) ==  0,
	      "Got '%s' instead of '%s'",
	      ret_str, expected_str);
    free(expected_str);
} END_TEST;

START_TEST(set_get_zero_len)
{
    const char *expected_str = "Red Blue Green Yellow";
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    tcase_set_tags(tc, expected_str);
    ret_val = tcase_get_tags(tc, NULL, 0);
    
    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(expected_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(expected_str));
    
} END_TEST;

START_TEST(set_get_two_space)
{
    char ret_str[100];
    const char *actual_str = "Red   Blue Green   Yellow";
    const char *expected_str = "Red Blue Green Yellow";
    int ret_val;
    TCase *tc;

    tc = tcase_create("TC");
    tcase_set_tags(tc, actual_str);
    ret_val = tcase_get_tags(tc, ret_str, sizeof(ret_str));
    
    ck_assert_msg(ret_val >=  0,
		  "Got illegal negative return value");

    ck_assert_msg((unsigned int)ret_val == strlen(expected_str),
	      "Got %d instead of %d for returned tag length",
	      ret_val, strlen(expected_str));
    
    ck_assert_msg(strcmp(ret_str, expected_str) ==  0,
	      "Got '%s' instead of '%s'",
	      ret_str, expected_str);
} END_TEST;


/*
 * Show that with no filter we run all the tests
 */
START_TEST(null_filter)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, NULL, NULL, CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(test_executed[YELLOW]);
    ck_assert(test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_nothing)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "", NULL, CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(exc_nothing)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, NULL, "", CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(test_executed[YELLOW]);
    ck_assert(test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_nothing_exc_nothing)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "", "", CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_yellow)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Yellow", NULL, CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST


START_TEST(inc_red)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Red", NULL, CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_red_blue)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Red Blue", NULL, CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_red_blue_yellow)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Red Blue Yellow", NULL, CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(exc_yellow)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, NULL, "Yellow", CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(exc_red)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, NULL, "Red", CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(test_executed[YELLOW]);
    ck_assert(test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(exc_red_blue)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, NULL, "Red Blue", CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(test_executed[YELLOW]);
    ck_assert(test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(exc_red_blue_yellow)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, NULL, "Red Blue Yellow", CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(test_executed[BLACK]);

    reset_executed();

} END_TEST


START_TEST(inc_red_exc_red)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Red", "Red", CK_VERBOSE);

    ck_assert(!test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_red_exc_blue)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Red", "Blue", CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(!test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_red_inc_red)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "Red Red Red", NULL, CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(!test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

START_TEST(inc_w_spaces)
{
    reset_executed();

    srunner_run(sr, NULL, NULL, "  Red    Blue ", NULL, CK_VERBOSE);

    ck_assert(test_executed[RED]);
    ck_assert(test_executed[BLUE]);
    ck_assert(test_executed[PURPLE]);
    ck_assert(!test_executed[YELLOW]);
    ck_assert(!test_executed[BLACK]);

    reset_executed();

} END_TEST

Suite *make_tag_suite(void)
{
    TCase *set_get_tags, *no_filters, *inc_filters, *exc_filters;
    TCase **inc_exc_filters, *strange_filters;
    Suite *s;

    s = suite_create("Check Tag Filtering");

    set_get_tags = tcase_create("set/get tag filters");
    suite_add_tcase (s, set_get_tags);
    tcase_add_test(set_get_tags, init_get);
    tcase_add_test(set_get_tags, set_get_basic);
    tcase_add_test(set_get_tags, set_get_empty);
    tcase_add_test(set_get_tags, set_get_null);
    tcase_add_test(set_get_tags, set_get_truncated);
    tcase_add_test(set_get_tags, set_get_zero_len);  tcase_add_test(inc_filters, inc_red);
    tcase_add_test(set_get_tags, set_get_two_space);

    no_filters = tcase_create("no tag filters");
    suite_add_tcase (s, no_filters);
    tcase_add_test(no_filters, null_filter);
    tcase_add_test(no_filters, inc_nothing);
    tcase_add_test(no_filters, exc_nothing);
    tcase_add_unchecked_fixture (no_filters,
				 tag_test_setup,
				 tag_test_teardown);

    inc_filters = tcase_create("include tags");
    suite_add_tcase (s, inc_filters);
    tcase_add_test(inc_filters, inc_yellow);
    tcase_add_test(inc_filters, inc_red);
    tcase_add_test(inc_filters, inc_red_blue);
    tcase_add_test(inc_filters, inc_red_blue_yellow);
    tcase_add_unchecked_fixture (inc_filters,
				 tag_test_setup,
				 tag_test_teardown);

    exc_filters = tcase_create("exclude tags");
    suite_add_tcase (s, exc_filters);
    tcase_add_test(exc_filters, exc_yellow);
    tcase_add_test(exc_filters, exc_red);
    tcase_add_test(exc_filters, exc_red_blue);
    tcase_add_test(exc_filters, exc_red_blue_yellow);
    tcase_add_unchecked_fixture (exc_filters,
				 tag_test_setup,
				 tag_test_teardown);

    inc_exc_filters = tcase_create("include and exclude tags");
    suite_add_tcase (s, inc_exc_filters);
    tcase_add_test(inc_exc_filters, inc_nothing_exc_nothing);
    tcase_add_test(inc_exc_filters, inc_red_exc_blue);
    tcase_add_test(inc_exc_filters, inc_red_exc_red);
    tcase_add_unchecked_fixture (inc_exc_filters,
				 tag_test_setup,
				 tag_test_teardown);

    strange_filters = tcase_create("strange tag filters");
    suite_add_tcase (s, strange_filters);
    tcase_add_test(strange_filters, inc_red_inc_red);
    tcase_add_test(strange_filters, inc_w_spaces);
    tcase_add_unchecked_fixture (strange_filters,
				 tag_test_setup,
				 tag_test_teardown);
    return s;
}
