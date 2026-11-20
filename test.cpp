#include"myheader/mylib_all.h"
using namespace std;
using namespace MyLib;
#define N 102
#define A 700005
int n;
int a[N];
bitset<A> b;
int maxa,ans;
int main()
{
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    for(int i=1,ct;i<=n;i++)
    {
        b.reset();
        b[0]=1;
        for(int j=1;j<=n;j++)
        {
            if(i==j)continue;
            b._ior_shifted(a[j]);
        }
        ct=b.count();
        if(ct>maxa)ans=i,maxa=ct;
    }
    cout<<ans;
    return 0;
}