#include "rrmathlib.h"
int add(int a, int b) {
    return a + b;
}
int subtract(int a, int b) {
    return a - b;
}   
int multiply(int a, int b) {
    return a * b;
}
double divide(int a, int b) {
    if (b == 0) {
        throw "Division by zero error!";
    }
    return static_cast<double>(a) / b;
}
