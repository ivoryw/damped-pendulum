#include "rk4.h"
#include <cstdlib>

/* A 4th-order fixed-length Runge-Kutta scheme for numerical integration of function f(x,y)=dx/dy
Returns a 2d array with x and y values. x values from [0] to [n-1], y from [n] to [2n-1].
Takes float function f defined in the main program.
Memory used for the array should be freed using the free function later in the program */
float *var2_rk4_fixed(float f(float, float),  float x0, float y0, float h, int n){
    float k1, k2, k3, k4;
    float *x, *y;
    float* array = new float[2*n*sizeof(float)];

    x = array;
    y = &array[n];

    *x = x0;
    *y = y0;

    for(int i=0; i < n; i++) {
        k1 = h * f(*x,*y);
        k2 = h * f(*x+h/2, *y+k1/2);
        k3 = h * f(*x+h/2, *y+k2/2);
        k4 = h * f(*x+h, *y+k3);

        *(y+1) = *y + (2*(k2 + k3) + k1 + k4)/6;
        *(x+1) = *x + h;
        ++y;
        ++x;
    }
    return array;
}

/* A 4th-order fixed-length Runge-Kutta scheme for numerical integration of coupled functions
f(x,y,z) = dy/dx and g(x,y,z) = dz/dx
Returns a 2d array with x, y and z values. x values from [0] to [n-1], y from [n] to [2n-1] and z from [2n] to [3n-1].
Takes float functions f and g defined in the program.
Memory used for the array should be freed using the free function later in the program */

float *var3_rk4_fixed(float f(float, float, float), float g(float, float, float), float x0, float y0,
    float z0, float h, int n){
    float k1, k2, k3, k4, l1, l2, l3, l4;
    float *x, *y, *z;
    float* array = new float[3*n*sizeof(float)];

    x = array;
    y = &array[n];
    z = &array[2*n];

    *x = x0;
    *y = y0;
    *z = z0;

    for(int i=0; i < n; i++) {
        k1 = h * f(*x, *y, *z);
        l1 = h * g(*x, *y, *z);
        k2 = h * f(*x+h/2, *y+k1/2 ,*z+l1/2);
        l2 = h * g(*x+h/2, *y+k1/2, *z+l1/2);
        k3 = h * f(*x+h/2, *y+k2/2, *z+l2/2);
        l3 = h * g(*x+h/2, *y+k2/2, *z+l2/2);
        k4 = h * f(*x+h, *y+k3, *z+l3);
        l4 = h * g(*x+h, *y+k3, *z+l3);

        *(y+1) = *y + (2*(k2+k3) + k1+k4)/6;
        *(z+1) = *z + (2*(l2+l3) + l1+l4)/6;
        *(x+1) = *x + h;
        ++x;
        ++y;
        ++z;
    }
    return array;
}


