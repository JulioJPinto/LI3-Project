#pragma once
#ifndef LI3_PRICE_UTIL_H
#define LI3_PRICE_UTIL_H

#include "struct_util.h"

/**
 * Calculates the price of a ride given the distance and car class.
 * The price is calculated as follows:
 * - If the car class is "Basic", the price is 3.25 + 0.62 * distance
 * - If the car class is "Green", the price is 4.00 + 0.79 * distance
 * - If the car class is "Premium", the price is 5.20 + 0.94 * distance
 */
double compute_price(int distance, CarClass car_class);

#endif //LI3_PRICE_UTIL_H
