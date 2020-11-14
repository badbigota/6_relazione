#include <vector>
#include<iostream>

using namespace std;

vector<double> func()
{
    vector<double> vec;
    vec.push_back(1);
    vec.push_back(3);
    return vec;
}
int main()
{
    auto top = func();
    for (auto c : top)
    {
        cout << c << endl;
    }
    return 0;
}
