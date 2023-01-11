/* Example file for using µnit.
 *
 * µnit is MIT-licensed, but for this file and this file alone:
 *
 * To the extent possible under law, the author(s) of this file have
 * waived all copyright and related or neighboring rights to this
 * work.  See <https://creativecommons.org/publicdomain/zero/1.0/> for
 * details.
 *********************************************************************/

#include "munit.h"
#include <stdio.h>
#include "last10lines.h"

/* This is just to disable an MSVC warning about conditional
 * expressions being constant, which you shouldn't have to do for your
 * code.  It's only here because we want to be able to do silly things
 * like assert that 0 != 1 for our demo. */
#if defined(_MSC_VER)
#pragma warning(disable: 4127)
#endif

/* Tests are functions that return void, and take a single void*
 * parameter.  We'll get to what that parameter is later. */
static MunitResult
test_compare(const MunitParameter params[], void* data) {
  /* We'll use these later */
  const unsigned char val_uchar = 'b';
  const short val_short = 1729;
  double pi = 3.141592654;
  char* stewardesses = "stewardesses";
  char* most_fun_word_to_type;

  /* These are just to silence compiler warnings about the parameters
   * being unused. */
  (void) params;
  (void) data;

  /* Let's start with the basics. */
  munit_assert(0 != 1);

  /* There is also the more verbose, though slightly more descriptive
     munit_assert_true/false: */
  munit_assert_false(0);

  /* You can also call munit_error and munit_errorf yourself.  We
   * won't do it is used to indicate a failure, but here is what it
   * would look like: */
  /* munit_error("FAIL"); */
  /* munit_errorf("Goodbye, cruel %s", "world"); */

  /* There are macros for comparing lots of types. */
  munit_assert_char('a', ==, 'a');

  /* Sure, you could just assert('a' == 'a'), but if you did that, a
   * failed assertion would just say something like "assertion failed:
   * val_uchar == 'b'".  µnit will tell you the actual values, so a
   * failure here would result in something like "assertion failed:
   * val_uchar == 'b' ('X' == 'b')." */
  munit_assert_uchar(val_uchar, ==, 'b');

  /* Obviously we can handle values larger than 'char' and 'uchar'.
   * There are versions for char, short, int, long, long long,
   * int8/16/32/64_t, as well as the unsigned versions of them all. */
  munit_assert_short(42, <, val_short);

  /* There is also support for size_t.
   *
   * The longest word in English without repeating any letters is
   * "uncopyrightables", which has uncopyrightable (and
   * dermatoglyphics, which is the study of fingerprints) beat by a
   * character */
  munit_assert_size(strlen("uncopyrightables"), >, strlen("dermatoglyphics"));

  /* Of course there is also support for doubles and floats. */
  munit_assert_double(pi, ==, 3.141592654);

  /* If you want to compare two doubles for equality, you might want
   * to consider using munit_assert_double_equal.  It compares two
   * doubles for equality within a precison of 1.0 x 10^-(precision).
   * Note that precision (the third argument to the macro) needs to be
   * fully evaluated to an integer by the preprocessor so µnit doesn't
   * have to depend pow, which is often in libm not libc. */
  munit_assert_double_equal(3.141592654, 3.141592653589793, 9);

  /* And if you want to check strings for equality (or inequality),
   * there is munit_assert_string_equal/not_equal.
   *
   * "stewardesses" is the longest word you can type on a QWERTY
   * keyboard with only one hand, which makes it loads of fun to type.
   * If I'm going to have to type a string repeatedly, let's make it a
   * good one! */
  munit_assert_string_equal(stewardesses, "stewardesses");

  /* A personal favorite macro which is fantastic if you're working
   * with binary data, is the one which naïvely checks two blobs of
   * memory for equality.  If this fails it will tell you the offset
   * of the first differing byte. */
  munit_assert_memory_equal(7, stewardesses, "steward");

  /* You can also make sure that two blobs differ *somewhere*: */
  munit_assert_memory_not_equal(8, stewardesses, "steward");

  /* There are equal/not_equal macros for pointers, too: */
  most_fun_word_to_type = stewardesses;
  munit_assert_ptr_equal(most_fun_word_to_type, stewardesses);

  /* And null/not_null */
  munit_assert_null(NULL);
  munit_assert_not_null(most_fun_word_to_type);

  /* Lets verify that the data parameter is what we expected.  We'll
   * see where this comes from in a bit.
   *
   * Note that the casting isn't usually required; if you give this
   * function a real pointer (instead of a number like 0xdeadbeef) it
   * would work as expected. */
  munit_assert_ptr_equal(data, (void*)(uintptr_t)0xdeadbeef);

  return MUNIT_OK;
}

char *example1[] = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
};

char *example2[] = {
    "555555555555555555555555555555555555555555555555555555555555555555555", 
    "666666666666666666666666666666666666666666666666666666666666666666666", 
    "777777777777777777777777777777777777777777777777777777777777777777777", 
    "888888888888888888888888888888888888888888888888888888888888888888888", 
    "999999999999999999999999999999999999999999999999999999999999999999999", 
    "---------------------------------------------------------------------", 
};

char *example3[] = {
    "1111111111111111111111111111111", 
    "22222222222222222222222222222222222222", 
    "333", 
    "44444", 
    "55555555", 
    "66666666666666666", 
    "777777777777777777777", 
    "888888888888888", 
    "999999999999999999999999999999999999999999999999999999999999999999999", 
    "--------------", 
};

struct LastLines {
    char *filename;
    int last_lines_num;
    char **lines;
} last_lines[] = {
    {
        "example1.txt",
        sizeof(example1) / sizeof(example1[0]),
        example1,
    },
    {
        "example2.txt",
        sizeof(example2) / sizeof(example2[0]),
        example2,
    },
    {
        "example3.txt",
        sizeof(example3) / sizeof(example3[0]),
        example3,
    },
    {
        NULL,
        0
    },
};

static MunitResult
test_simple(const MunitParameter params[], void* user_data) {

    (void) params;
    (void) user_data;

    FILE *source = fopen("example4.txt", "r");
    int n = 12;
    char **lines = read_last_n_lines(source, n);

    puts(""); puts(""); puts(""); puts("");

    for (int q = 0; q < n; ++q) {
        printf("line '%s'\n", lines[q]);
    }
    fclose(source);

    return MUNIT_OK;
}

static MunitResult
test_files(const MunitParameter params[], void* user_data) {

    (void) params;
    (void) user_data;

    int j = 0;
    while (last_lines[j].filename) {
        FILE *source = fopen(last_lines[j].filename, "r");
        char **lines = read_last_n_lines(source, last_lines[j].last_lines_num);
        munit_assert_ptr_not_null(lines);
        fclose(source);
        for (int k = 0; k < last_lines[j].last_lines_num; ++k) {
            char *line1 = last_lines[j].lines[k];
            char *line2 = lines[k];
            printf("line1 '%s'\n", line1);
            printf("line2 '%s'\n", line2);
            munit_assert_string_equal(line1, line2);
        }
        for (int z = 0; z < last_lines[j].last_lines_num; ++z) {
            free(lines[z]);
        }
        free(lines);
        j++;
    }

    return MUNIT_OK;
}

/* This test case shows how to accept parameters.  We'll see how to
 * specify them soon.
 *
 * By default, every possible variation of a parameterized test is
 * run, but you can specify parameters manually if you want to only
 * run specific test(s), or you can pass the --single argument to the
 * CLI to have the harness simply choose one variation at random
 * instead of running them all. */
static MunitResult
test_parameters(const MunitParameter params[], void* user_data) {
  const char* foo;
  const char* bar;

  (void) user_data;

  /* The "foo" parameter is specified as one of the following values:
   * "one", "two", or "three". */
  foo = munit_parameters_get(params, "foo");
  /* Similarly, "bar" is one of "four", "five", or "six". */
  bar = munit_parameters_get(params, "bar");
  /* "baz" is a bit more complicated.  We don't actually specify a
   * list of valid values, so by default NULL is passed.  However, the
   * CLI will accept any value.  This is a good way to have a value
   * that is usually selected randomly by the test, but can be
   * overridden on the command line if desired. */
  /* const char* baz = munit_parameters_get(params, "baz"); */

  /* Notice that we're returning MUNIT_FAIL instead of writing an
   * error message.  Error messages are generally preferable, since
   * they make it easier to diagnose the issue, but this is an
   * option.
   *
   * Possible values are:
   *  - MUNIT_OK: Sucess
   *  - MUNIT_FAIL: Failure
   *  - MUNIT_SKIP: The test was skipped; usually this happens when a
   *    particular feature isn't in use.  For example, if you're
   *    writing a test which uses a Wayland-only feature, but your
   *    application is running on X11.
   *  - MUNIT_ERROR: The test failed, but not because of anything you
   *    wanted to test.  For example, maybe your test downloads a
   *    remote resource and tries to parse it, but the network was
   *    down.
   */

  if (strcmp(foo, "one") != 0 &&
      strcmp(foo, "two") != 0 &&
      strcmp(foo, "three") != 0)
    return MUNIT_FAIL;

  if (strcmp(bar, "red") != 0 &&
      strcmp(bar, "green") != 0 &&
      strcmp(bar, "blue") != 0)
    return MUNIT_FAIL;

  return MUNIT_OK;
}

/* The setup function, if you provide one, for a test will be run
 * before the test, and the return value will be passed as the sole
 * parameter to the test function. */
static void*
test_compare_setup(const MunitParameter params[], void* user_data) {
  (void) params;

  munit_assert_string_equal(user_data, "µnit");
  return (void*) (uintptr_t) 0xdeadbeef;
}

/* To clean up after a test, you can use a tear down function.  The
 * fixture argument is the value returned by the setup function
 * above. */
static void
test_compare_tear_down(void* fixture) {
  munit_assert_ptr_equal(fixture, (void*)(uintptr_t)0xdeadbeef);
}

static char* foo_params[] = {
  (char*) "one", (char*) "two", (char*) "three", NULL
};

static char* bar_params[] = {
  (char*) "red", (char*) "green", (char*) "blue", NULL
};

static MunitParameterEnum test_params[] = {
  { (char*) "foo", foo_params },
  { (char*) "bar", bar_params },
  { (char*) "baz", NULL },
  { NULL, NULL },
};

/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest last10lines_tests[] = {
  //{ (char*) "/last10lines/files", test_files, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { (char*) "/last10lines/simple", test_simple, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
};


/* Creating a test suite is pretty simple.  First, you'll need an
 * array of tests: */
static MunitTest test_suite_tests[] = {
  {
    /* The name is just a unique human-readable way to identify the
     * test. You can use it to run a specific test if you want, but
     * usually it's mostly decorative. */
    (char*) "/example/compare",
    /* You probably won't be surprised to learn that the tests are
     * functions. */
    test_compare,
    /* If you want, you can supply a function to set up a fixture.  If
     * you supply NULL, the user_data parameter from munit_suite_main
     * will be used directly.  If, however, you provide a callback
     * here the user_data parameter will be passed to this callback,
     * and the return value from this callback will be passed to the
     * test function.
     *
     * For our example we don't really need a fixture, but lets
     * provide one anyways. */
    test_compare_setup,
    /* If you passed a callback for the fixture setup function, you
     * may want to pass a corresponding callback here to reverse the
     * operation. */
    test_compare_tear_down,
    /* Finally, there is a bitmask for options you can pass here.  You
     * can provide either MUNIT_TEST_OPTION_NONE or 0 here to use the
     * defaults. */
    MUNIT_TEST_OPTION_NONE,
    NULL
  },
  /* Usually this is written in a much more compact format; all these
   * comments kind of ruin that, though.  Here is how you'll usually
   * see entries written: */
  /*{ (char*) "/example/rand", test_rand, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },*/
  /* To tell the test runner when the array is over, just add a NULL
   * entry at the end. */
  { (char*) "/example/parameters", test_parameters, NULL, NULL, MUNIT_TEST_OPTION_NONE, test_params },
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

/* If you wanted to have your test suite run other test suites you
 * could declare an array of them.  Of course each sub-suite can
 * contain more suites, etc. */
/* static const MunitSuite other_suites[] = { */
/*   { "/second", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE }, */
/*   { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE } */
/* }; */

/* Now we'll actually declare the test suite.  You could do this in
 * the main function, or on the heap, or whatever you want. */
static const MunitSuite test_suite = {
  /* This string will be prepended to all test names in this suite;
   * for example, "/example/rand" will become "/µnit/example/rand".
   * Note that, while it doesn't really matter for the top-level
   * suite, NULL signal the end of an array of tests; you should use
   * an empty string ("") instead. */
  (char*) "last10lines",
  /* The first parameter is the array of test suites. */
  last10lines_tests,
  /* In addition to containing test cases, suites can contain other
   * test suites.  This isn't necessary in this example, but it can be
   * a great help to projects with lots of tests by making it easier
   * to spread the tests across many files.  This is where you would
   * put "other_suites" (which is commented out above). */
  NULL,
  /* An interesting feature of µnit is that it supports automatically
   * running multiple iterations of the tests.  This is usually only
   * interesting if you make use of the PRNG to randomize your tests
   * cases a bit, or if you are doing performance testing and want to
   * average multiple runs.  0 is an alias for 1. */
  1,
  /* Just like MUNIT_TEST_OPTION_NONE, you can provide
   * MUNIT_SUITE_OPTION_NONE or 0 to use the default settings. */
  MUNIT_SUITE_OPTION_NONE
};

/* This is only necessary for EXIT_SUCCESS and EXIT_FAILURE, which you
 * *should* be using but probably aren't (no, zero and non-zero don't
 * always mean success and failure).  I guess my point is that nothing
 * about µnit requires it. */
#include <stdlib.h>

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  /* Finally, we'll actually run our test suite!  That second argument
   * is the user_data parameter which will be passed either to the
   * test or (if provided) the fixture setup function. */
  return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}
