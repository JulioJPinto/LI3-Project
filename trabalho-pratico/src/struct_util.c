#include "struct_util.h"

const static Date reference_date = {9, 10, 2022};

int get_age(Date date_of_birth) {
    if(reference_date.month > date_of_birth.month ||
        (reference_date.month == date_of_birth.month && reference_date.day >= date_of_birth.day))
        return reference_date.year - date_of_birth.year;
    else
        return reference_date.year - date_of_birth.year - 1;
}
