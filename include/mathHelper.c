
#include "helper.h"

float degToRad(int a)
{ 
    return ((a * M_PI) / 180.0);
}

int FixAng(int a)
{ 
    if(a>359)
        a -= 360;
    if(a<0)
        a += 360;
    return a;
}

float distance(float ax, float ay, float bx, float by, float ang)
{ 
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void sort(int array[], int n) 
{ 
    for (int step = 1; step < n; step++) {
    int key = array[step];
    int j = step - 1;

    // Compare key with each element on the left of it until an element smaller than
    // it is found.
    // For descending order, change key<array[j] to key>array[j].
    while (key < array[j] && j >= 0) {
      array[j + 1] = array[j];
      --j;
    }
    array[j + 1] = key;
  }
} 