#ifndef RK4_H
#define RK4_H
float *var2_rk4_fixed(float f(float, float),  float x0, float y0, float h, int n);
float *var3_rk4_fixed(float f(float, float, float), float g(float, float, float), float x0, float y0, float z0, float h, int n);
#endif // RK4_H
