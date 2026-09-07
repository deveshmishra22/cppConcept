#include<iostream>
using namespace std;

int main()
{
    int testarr[2][3] = { {1, 2, 3}, 
                          {4, 5, 6} 
                        };
    
    // Printing 2D array
    // Using nested for loop
    cout << "The elements in the 2D array are: " << endl;
    // outer loop for rows
    for(int i = 0; i < 2; i++)
    {
        // inner loop for columns
        for(int j = 0; j < 3; j++)
        {
            cout << testarr[i][j] << " ";
        }
        cout << endl;
    }


    int row, col;
    cout << "Enter number of rows and columns";
    cout << "Rows: ";
    cin >> row;
    cout << "Columns: ";
    cin >> col;
    int arr[row][col];
    // Taking input for 2D array
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << "Enter element at position [" << i << "][" << j << "]: ";
            cin >> arr[i][j];
        }
    }
    // Printing 2D array
    cout << "The elements in the 2D array are: " << endl;
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}