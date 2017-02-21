# Hodor
Hodor - a language for fast and convenient numerical computations

Hodor was meant to replace Fortran as my go-to numerical computation language. 
At the time (2013) there was no compelling, safe alternative to fortran/C when it comes to performant computing.
Since discovering Julia (http://julialang.org) my interest in developing Hodor has waned.

Hodor transpiler is based on yacc (bison) and flex. 
It translates hodor scripts (*.hd) to C++. They are then linked with the Hodor standard library (written in C++) 

Sample script for O(h^4) FDA looks like this:
```
float[] computeFirstDerivative(float [] x, float [] u)
{
   int size = u.size[0]; /* size of the first dimension of u */
   float retVal[u]; /* this creates an empty array with dimension equal to u */
   float denominator = x[2]-x[1];

   retVal[3:size-2] = 1.0/12.0 * u[1:size-4] - 2.0/3.0 * u[2:size-3] + 2.0/3.0 * u[4:size-1] - 1.0/12.0 * u[5:size];

   retVal[1] = -25.0/12.0 * u[1] + 4.0 * u[2] - 3.0 * u[3] + 4.0/3.0 * u[4] - 1.0/4.0 * u[5];
   retVal[2] = -1.0/4.0 * u[1] - 5.0/6.0 * u[2] + 3.0/2.0 * u[3] - 1.0/2.0 * u[4] + 1.0/12.0 * u[5];

   retVal[size-1] = -1.0/12.0 * u[size-4] + 1.0/2.0 * u[size-3] - 3.0/2.0 * u[size-2] + 5.0/6.0 * u[size-1] + 1.0/4.0 * u[size];
   retVal[size] = 1.0/4.0 * u[size-4] - 3.0/3.0 * u[size-3] + 3.0 * u[size-2] - 4.0 * u[size-1] + 25.0/12.0 * u[size];

   retVal/=denominator;
   return retVal;
}
```
Runge-Kutta step looks like this:
```
void rk4step(float [] x, float [] u, float t, float h, function right)
{
   float k1[u];
   float k2[u];
   float k3[u];
   float k4[u];

   k1 = right(x,u,t);
   k2 = right(x,u + h/2*k1,t + h/2.0);
   k3 = right(x,u + h/2*k2,t + h/2.0);
   k4 = right(x,u + h*k3,t + h);
   u = u + h/6.0 * (k1+ 2*k2 + 2*k3 + k4);
   t = t + h;
}
```

# Installation

1. Get bison[https://www.gnu.org/software/bison/] and flex [https://github.com/westes/flex] (on OS X they come pre-installed)
2. Get make and g++ (on OS X they come with XCode)
2. go to compiler directory
3. make
4. go to scripts directory
5. run ./hodor.sh name_of_your_hodor_file.hd

The output is visible in test_range dir.
The current version relies on OS X's Accelerate framework.
