#include <iostream>

using namespace std;
 
int main() {
    float num, average, sum = 0.0;
    int i, n;
 
    cout << "Maximum number of inputs: ";
    cin >> n;
 
    for(i = 1; i <= n; ++i)
    {
        cout << "Enter n" << i << ": ";
        cin >> num;
        if(num < 0.0)
        {
           // Control of the program move to jump:
            goto jump;
        } 
        sum += num;
    }
jump:
    average = sum / (i - 1);
    cout << "\nAverage = " << average;
    cout << "\nsum = " << sum;

    cout << "\nDemonstration of continue and break statements:\n";

    for (int i = 1; i <= 5; i++) {
        // condition to continue
        if (i == 2) {
            continue;
        }
        // break condition     

        if (i == 4) {

            break;

        }

        cout << i << endl;

    }
 
return 0;

}

 