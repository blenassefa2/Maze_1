
#include "helper.h"

/**
 * @brief A function for changing degree to radian
 * 
 * @param a: The degree to be converted 
 * @return float 
 */
float degToRad(int a)
{ 
    return ((a * M_PI) / 180.0);
}

/**
 * @brief A functions for keeping a dgree within
 *        the range of [0, 2*PI] 
 * 
 * @param a: The degree to keep within range
 * @return float 
 */
float FixAng(float a)
{ 
    if(a>359)
        a -= 360;
    if(a<0)
        a += 360;
    return a;
}
