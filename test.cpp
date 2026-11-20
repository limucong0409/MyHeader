#include"mylib/mylib_all.h"
using namespace std;
using namespace MyLib;
#define N 10004
zip<int> z(N);
int main()
{
    z[1].push_back(1);
    z[2].push_back(3);
    z[1].push_back(5);
    z[4].push_back(1);
    z[1].push_back(2);
    for(auto v:z[1])
    {
        cout<<v<<endl;
    }
    return 0;
}