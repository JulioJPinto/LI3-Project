#include "struct_util.h"

const static Date reference_date = {9, 10, 2022};

int get_age(Date date_of_birth) {
    if (reference_date.month > date_of_birth.month ||
        (reference_date.month == date_of_birth.month && reference_date.day >= date_of_birth.day))
        return reference_date.year - date_of_birth.year;
    else
        return reference_date.year - date_of_birth.year - 1;
}

int date_compare(Date date_1, Date date_2) {
    if (date_1.year != date_2.year)
        return date_1.year - date_2.year;
    else if (date_1.month != date_2.month)
        return date_1.month - date_2.month;
    else if (date_1.day != date_2.day)
        return date_1.day - date_2.day;
    else
        return 0;
}
