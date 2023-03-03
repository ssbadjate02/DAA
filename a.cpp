#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define pb push_back
#define FASTIO ios_base::sync_with_stdio(false),cin.tie(NULL),cout.tie(NULL)
#define all(x) (x).begin(),(x).end()
#define ss second
#define ff first

class Vertex{
public:
    int x,y;
    Edge *inc_edge;
};

class Edge{
public:
    Vertex *start;
    Vertex *end;
    Edge *twin;
    Face *left;
    Face *right;
    Edge *prev;
    Edge *next;
};

class Face{
public:
    Edge *inc_edge;
};


int main()
{
    FASTIO;
     
}
