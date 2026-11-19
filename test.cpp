#include<iostream>
#include<cmath>
#include"myheader/myheader.h"
using namespace std;
using namespace MyHead;
#define cin FastIO::cin
#define cout FastIO::cout
#define endl '\n'
zip<int> test(10);
int main()
{
    test.pushback(1,2);
    test.pushback(2,4);
    test.pushback(1,3);
    test[1].pushback(5);
    for(auto it=test[1].begin();it!=test[1].end();++it)
    {
        cout<<*it<<' ';
    }
    cout<<endl;
    test[1].clear();
    test.pushback(1,0);
    test[1].pushback(3);
    for(auto v:test[1])
    {
        cout<<v<<' ';
    }
    cout<<endl<<test[3].empty();
    cout<<endl<<cout.keeplen;
    cout<<endl<<keep(10)<<PI;
    return 0;
}