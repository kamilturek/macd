#include "check_array.h"
#include <check.h>
#include <stdlib.h>

int main(void)
{
    int no_failed = 0;
    SRunner *runner;

    runner = srunner_create(array_suite());

    srunner_run_all(runner, CK_NORMAL);
    no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
