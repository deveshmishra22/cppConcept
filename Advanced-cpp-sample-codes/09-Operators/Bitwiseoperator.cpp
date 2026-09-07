#include<iostream>
using namespace std;

int main()
{
    int a = 10;      // Binary: 1010
    int b = 5;      // Binary: 0101

    // Bitwise AND
    int andResult = a & b;  // Binary: 0000, Decimal:
    cout << "Bitwise AND (a & b): " << andResult << endl;

    // Bitwise OR
    int orResult = a | b;   // Binary: 1111, Decimal: 15
    cout << "Bitwise OR (a | b): " << orResult << endl;

    // Bitwise XOR
    int xorResult = a ^ b;  // Binary: 1111, Decimal:
    cout << "Bitwise XOR (a ^ b): " << xorResult << endl;

    // Bitwise NOT
    int notResult = ~a;     // Binary: 0101 (in 4 bits), Decimal: -11 (two's complement)
    cout << "Bitwise NOT (~a): " << notResult << endl;

    // Left Shift
    int leftShiftResult = a << 1; // Binary: 10100, Decimal
    cout << "Left Shift (a << 1): " << leftShiftResult << endl;

    // Right Shift
    int rightShiftResult = a >> 1; // Binary: 0101, Decimal
    cout << "Right Shift (a >> 1): " << rightShiftResult << endl;

    return 0;
}