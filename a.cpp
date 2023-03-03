#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define pb push_back
#define FASTIO ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)
#define all(x) (x).begin(), (x).end()
#define ss second
#define ff first
class Vertex;
class Edge;
class Face;
class DCEL;

class Vertex
{
public:
    double x, y;
    Edge *inc_edge;
    Vertex(double x, double y)
    {
        this->x = x;
        this->y = y;
        inc_edge = NULL;
    }
};

class Edge
{
public:
    Vertex *start;
    Vertex *end;
    Edge *twin;
    Face *left;
    Face *right;
    Edge *prev;
    Edge *next;
    Edge(Vertex *start, Vertex *end)
    {
        this->start = start;
        this->end = end;
        twin = NULL;
        prev = NULL;
        next = NULL;
        left = NULL;
        right = NULL;
    }
};

class Face
{
public:
    Edge *inc_edge;
};

class DCEL
{
public:
    vector<Vertex *> vertices;
    vector<Edge *> edges;
    vector<Face *> faces;
    DCEL(vector<pair<double, double>> &points)
    {
        for (pair<double, double> point : points)
        {
            Vertex *curr = new Vertex(point.first, point.second);
            vertices.push_back(curr);
        }

        for (int i = 0; i < points.size(); i++)
        {
            Vertex *v1 = find_vertex(points[i].first, points[i].second);
            Vertex *v2 = find_vertex(points[(i + 1) % points.size()].first, points[(i + 1) % points.size()].second);

            Edge *e1 = new Edge(v1, v2);
            Edge *e2 = new Edge(v2, v1);

            e1->twin = e2;
            e2->twin = e1;

            v1->inc_edge = e1;
            v2->inc_edge = e2;

            edges.push_back(e1);
            edges.push_back(e2);
        }
    }

    Vertex *find_vertex(double x, double y)
    {
        for (Vertex *v : vertices)
        {
            if (v->x == x && v->y == y)
                return v;
        }
        return NULL;
    }

    Edge *find_edge(Vertex *v1, Vertex *v2)
    {
        for (Edge *edge : edges)
        {
            if ((v1->x == edge->start->x && v1->y == edge->start->y) && (v2->x == edge->end->x && v2->y == edge->end->y))
                return edge;
        }
        return NULL;
    }
};

int main()
{
    FASTIO;
    vector<pair<double, double>> points = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
    DCEL *polygon = new DCEL(points);
    return 0;
}
