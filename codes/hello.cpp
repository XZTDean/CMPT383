#include <iostream>
#include <ctime>
using namespace std;

int main() {
    time_t t = time(nullptr);
    string str = ctime(&t);
    cout << "Current time is " << str;
    cout << "Hello, World!" << endl;
    return 0;
}
