#include "price_util.h"

double compute_price(int distance, CarClass car_class) {
    switch (car_class) {
        case BASIC:
            return 3.25 + 0.62 * distance;
        case GREEN:
            return 4.00 + 0.79 * distance;
        case PREMIUM:
            return 5.20 + 0.94 * distance;
        default:
            return 0;
    }
}
