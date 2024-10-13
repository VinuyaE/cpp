#include<iostream>
using namespace std;
int main() {

    double num1, num2, result;
    int code;

    cout << "Enter first number :";
    cin >> num1;
    cout << "Enter second number :";
    cin >> num2;

    cout << "Enter the operation code :";
    cin >> code;

    switch (code) {
        case 1:
            result = num1 + num2;
            cout << "the sum is :" << result << endl;
            break;
        case 2:
            result = num1 - num2;
            cout << "the difference is :" << result << endl;
            break;
        case 3:
            result = num1 / num2;
            cout << "The product is :" << result << endl;
            break;
        case 4:
            if (num2 != 0) {
                result = num1 / num2;
                cout << "The quotient is :" << result << endl;
            } else {
                cout << "Invalid  operation code please enter the correctvcode !" << endl;
            }

            break;
        default:
            cout << "error invalid code !" << endl;


            return 0;
    }
}