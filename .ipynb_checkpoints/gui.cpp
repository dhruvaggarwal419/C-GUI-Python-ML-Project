#include <iostream>
#include <cstdlib>  

using namespace std;

int main() {
    cout << "=== Expression Validator ===" << endl;
    cout << "Expressions are read from input.csv" << endl;
    cout << "Click Run to evaluate..." << endl;
    system("python evaluate_csv.py");

    return 0;
}