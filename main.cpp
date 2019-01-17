#include <iostream>
#include "Find.hpp"

using namespace std;

int main()
{
    Database* data = new Database();
    ///read
    read(data);
    ///find
    find_circle(data);
    return 0;
}
