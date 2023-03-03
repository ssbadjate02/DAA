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
    Face *face;
    Edge *prev;
    Edge *next;
    Edge(Vertex *start, Vertex *end)
    {
        this->start = start;
        this->end = end;
        twin = NULL;
        prev = NULL;
        next = NULL;
        face = NULL;
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

        int n = points.size();
        for (int i = 0; i < n; i++)
        {
            Vertex *v1 = find_vertex(points[i].first, points[i].second);
            Vertex *v2 = find_vertex(points[(i + 1) % n].first, points[(i + 1) % n].second);

            Edge *e1 = new Edge(v1, v2);
            Edge *e2 = new Edge(v2, v1);

            e1->twin = e2;
            e2->twin = e1;

            v1->inc_edge = e1;

            edges.push_back(e1);
            edges.push_back(e2);
        }

        n = edges.size();
        faces.push_back(new Face());
        faces.push_back(new Face());

        faces[0]->inc_edge = edges[0];
        faces[1]->inc_edge = edges[1];
        for(int i = 0; i<n; i += 2)
        {
            edges[i]->next = edges[(i+2)%n];
            edges[(i+2)%n]->prev = edges[i];

            edges[(i+1)%n]->prev = edges[(i+3)%n];
            edges[(i+3)%n]->next = edges[(i+1)%n];

            edges[i]->face = faces[0];
            edges[(i+1)%n]->face = faces[1];
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
    /*
    HELPER FUNCTIONS
    */

    void print_vertex(Vertex* v)
    {
        cout<<v->x<<" "<<v->y<<" "<<find_edge_index(v->inc_edge)<<"\n";
    }

    int find_edge_index(Edge* e1)
    {
        for(int i = 0; i<edges.size(); i++)
        {
            if(e1 == edges[i])
                return i;
        }
        return -1;
    }

    int find_face_index(Face* f)
    {
        for(int i = 0;i<faces.size();i++)
        {
            if(f == faces[i])
                return i;
        }
        return -1;
    }
    void print()
    {
        cout<<"Vertex : \n";
        for(Vertex* v : vertices)
            print_vertex(v);
        cout<<"\n";
        cout<<"Edge : \n";
        for(Edge* e : edges)
        {
            cout<<"Start : ";
            print_vertex(e->start);
            cout<<"End : ";
            print_vertex(e->end);
            cout<<"Twin : "<<find_edge_index(e->twin)<<"\n";
            cout<<"Prev : "<<find_edge_index(e->prev)<<"\n";
            cout<<"Next : "<<find_edge_index(e->next)<<"\n";
            cout<<"Face : "<<find_face_index(e->face)<<"\n";
            cout<<"\n";
        }
    }
    /*
    */
};

int main()
{
    FASTIO;
    vector<pair<double, double>> points = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
    DCEL *polygon = new DCEL(points);
    polygon->print();
    return 0;
}
