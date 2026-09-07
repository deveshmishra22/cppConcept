#include<iostream>
using namespace std;
int main()
{
    char operatorChoice;
    float num1, num2, result;
    cout << "Enter operator (+, -, *, /): ";
    cin >> operatorChoice;
    cout << "Enter two operands: ";
    cin >> num1 >> num2;

    switch(operatorChoice)
    {
        case '+':
            result = num1 + num2;
            cout << num1 << " + " << num2 << " = " << result;
            break;
        case '-':
            result = num1 - num2;
            cout << num1 << " - " << num2 << " = " << result;
            break;
        case '*':
            result = num1 * num2;
            cout << num1 << " * " << num2 << " = " << result;
            break;
        case '/':
            if(num2 != 0)
            {
                result = num1 / num2;
                cout << num1 << " / " << num2 << " = " << result;
            }
            else
            {
                cout << "Error! Division by zero.";
            }
            break;
        default:
            cout << "Error! Operator is not correct";
            break;
    }
    return 0;
}