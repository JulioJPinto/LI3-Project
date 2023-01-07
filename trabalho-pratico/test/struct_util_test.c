#include "struct_util.h"

#include <glib.h>

void test_date(char *date, int expected_day, int expected_month, int expected_year) {
    char *date_copy = g_strdup(date);
    Date parsed_date = parse_date(date_copy);
    if (date_get_day(parsed_date) != expected_day) {
        g_test_fail_printf("Day should've been %d for date '%s' but is %d\n", expected_day, date, date_get_day(parsed_date));
    }
    if (date_get_month(parsed_date) != expected_month) {
        g_test_fail_printf("Month should've been %d for date '%s' but is %d\n", expected_month, date, date_get_month(parsed_date));
    }
    if (date_get_year(parsed_date) != expected_year) {
        g_test_fail_printf("Year should've been %d for date '%s' but is %d\n", expected_year, date, date_get_year(parsed_date));
    }
    free(date_copy);
}

void test_date_parse_and_encoding(void) {
    test_date("01/01/2022", 1, 1, 2022);
    test_date("31/12/2000", 31, 12, 2000);

    char date3[] = "38/01/2001";
    if (is_date_valid(parse_date(date3))) {
        g_test_fail_printf("Date should've been invalid for date '%s'\n", date3);
    }
}

void test_date_compare(void) {
    Date date1 = create_date(1, 1, 2000);
    Date date3 = create_date(1, 1, 2001);
    Date date4 = create_date(1, 2, 2000);
    Date date5 = create_date(2, 1, 2000);
    Date date6 = create_date(8, 5, 1999);

    if (date_compare(date1, date1) != 0) {
        g_test_fail_printf("Date1 (01/01/2000) should've been equal to itself");
    }
    if (date_compare(date1, date3) >= 0) {
        g_test_fail_printf("Date1 (01/01/2000) should've been before Date3 (01/01/2001)");
    }
    if (date_compare(date1, date4) >= 0) {
        g_test_fail_printf("Date1 (01/01/2000) should've been before Date4 (01/02/2000)");
    }
    if (date_compare(date5, date1) <= 0) {
        g_test_fail_printf("Date5 (02/01/2000) should've been after Date1 (01/01/2000)");
    }
    if (date_compare(date6, date1) >= 0) {
        g_test_fail_printf("Date6 (08/05/1999) should've been before Date1 (01/01/2000)");
    }
}


// 9/10/2022
void test_date_age(void) {
    Date date1 = create_date(1, 1, 2000);
    Date date2 = create_date(7, 2, 2005);
    Date date3 = create_date(9, 10, 2022);
    Date date4 = create_date(9, 10, 2021);

    if (get_age(date1) != 22) {
        g_test_fail_printf("Age should've been 22 for date1 (01/01/2000) but is %d", get_age(date1));
    }
    if (get_age(date2) != 17) {
        g_test_fail_printf("Age should've been 17 for date2 (07/02/2005) but is %d", get_age(date2));
    }
    if (get_age(date3) != 0) {
        g_test_fail_printf("Age should've been 0 for date3 (09/10/2022) but is %d", get_age(date3));
    }
    if (get_age(date4) != 1) {
        g_test_fail_printf("Age should've been 1 for date4 (09/10/2021) but is %d", get_age(date4));
    }
}
