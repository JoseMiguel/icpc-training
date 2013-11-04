#include<iostream>
#include<queue>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<set>
#include<string>
#include<cstring>
#include<map>
#include<numeric>
#include<sstream>
#include<cmath>
using namespace std;
#define all(v) (v).begin(),(v).end()
#define rall(v) (v).end,(v).begin
#define pb push_back
#define f(i,x,y) for(int i=x;i<y;i++)
#define FOR(it,A) for(typeof A.begin() it = A.begin();it!=A.end();it++)
#define sqr(x) (x)*(x)
#define mp make_pair
#define clr(x,y) memset(x,y,sizeof x)
typedef pair<int,int> pii;
typedef long long ll;
typedef long double ld;
map< pair<ll,ll>, ll > memo[103][103];
ll a[103],b[103];
ll A[103],B[104];
int n,m;
ll go(int aa,int bb,ll AA,ll BB){
       //cout<<aa<<" "<<bb<<" "<<AA<<" "<<BB<<endl;
       if(aa>=n or bb>=m) return 0;
       if(memo[aa][bb].count(mp(AA,BB)))return memo[aa][bb][mp(AA,BB)];
       ll res=0;
       //mismo tipo
       if(aa<n && bb<m && A[aa]==B[bb]){
               if(AA!=0 && AA<=BB)
                       res=max(res,AA+go(aa+1,bb,a[aa+1],BB-AA));
               if(BB!=0 && BB<=AA )
                       res=max(res,BB+go(aa,bb+1,AA-BB,b[bb+1]));
       }        
       
       if(aa<n)
               res=max(res,go(aa+1,bb,a[aa+1],BB));
       if(bb<m)
               res=max(res,go(aa,bb+1,AA,b[bb+1]));
       
       return memo[aa][bb][mp(AA,BB)]=res;
}
int cases;
int main(){
       int cases;
       cin>>cases;
       f(t,1,cases+1){
               cin>>n>>m;
               f(i,0,n)cin>>a[i]>>A[i];
               f(i,0,m)cin>>b[i]>>B[i];
               f(i,0,n)f(j,0,m)memo[i][j].clear();
               ll res=go(0,0,a[0],b[0]);
               cout<<"Case #"<<t<<": ";
               cout<<res<<endl;
       }
  return 0;
}
