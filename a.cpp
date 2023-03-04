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
        for (int i = 0; i < n; i += 2)
        {
            edges[i]->next = edges[(i + 2) % n];
            edges[(i + 2) % n]->prev = edges[i];

            edges[(i + 1) % n]->prev = edges[(i + 3) % n];
            edges[(i + 3) % n]->next = edges[(i + 1) % n];

            edges[i]->face = faces[0];
            edges[(i + 1) % n]->face = faces[1];
        }
    }

    void add_edge(Vertex* v1, Vertex* v2)
    {
        Edge* new_edge = new Edge(v1, v2);
        Edge* new_edge_twin = new Edge(v2, v1);
        new_edge->twin = new_edge_twin;
        new_edge_twin->twin = new_edge;

        new_edge->next = v2->inc_edge;
        new_edge->prev = v1->inc_edge->prev;
        new_edge_twin->next = v1->inc_edge;
        new_edge_twin->prev = v2->inc_edge->prev;

        v2->inc_edge->prev->next = new_edge_twin;
        v2->inc_edge->prev = new_edge;

        v1->inc_edge->prev->next = new_edge;
        v1->inc_edge->prev = new_edge_twin;

        edges.push_back(new_edge);
        edges.push_back(new_edge_twin);

        Edge* curr = v2->inc_edge;
        faces.push_back(new Face());
        faces.push_back(new Face());
        faces[faces.size()-2]->inc_edge = new_edge;
        faces[faces.size()-1]->inc_edge = new_edge_twin;
        while(curr != new_edge)
        {
            curr->face = faces[faces.size()-2];
            curr = curr->next;
        }
        curr->face = faces[faces.size()-2];

        curr = v1->inc_edge;
        while(curr != new_edge_twin)
        {
            curr->face = faces[faces.size()-1];
            curr = curr->next;
        }
        curr->face = faces[faces.size()-1];
    }
    /*
    HELPER FUNCTIONS
    */
    void print_vertex(Vertex *v)
    {
        cout << v->x << " " << v->y << " " << find_edge_index(v->inc_edge) << "\n";
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

    int find_edge_index(Edge *e1)
    {
        for (int i = 0; i < edges.size(); i++)
        {
            if (e1 == edges[i])
                return i;
        }
        return -1;
    }

    int find_face_index(Face *f)
    {
        for (int i = 0; i < faces.size(); i++)
        {
            if (f == faces[i])
                return i;
        }
        return -1;
    }
    void print()
    {
        cout << "Vertex : \n";
        for (Vertex *v : vertices)
            print_vertex(v);
        cout << "\n";
        cout << "Edge : \n";
        for (Edge *e : edges)
        {
            cout << "Start : ";
            print_vertex(e->start);
            cout << "End : ";
            print_vertex(e->end);
            cout << "Twin : " << find_edge_index(e->twin) << "\n";
            cout << "Prev : " << find_edge_index(e->prev) << "\n";
            cout << "Next : " << find_edge_index(e->next) << "\n";
            cout << "Face : " << find_face_index(e->face) << "\n";
            cout << "\n";
        }
    }
    /*
     */
};

pair<double, double> get_vector(Vertex *v1, Vertex *v2)
{
    return {v2->x - v1->x, v2->y - v1->y};
}

bool is_greater_than_180(Vertex *v1, Vertex *v2, Vertex *v3)
{
    pair<double, double> p1 = get_vector(v2, v1);
    pair<double, double> p2 = get_vector(v2, v3);
    double cross_product = p1.first * p2.second - p1.second * p2.first;
    return cross_product < 0; // greater than zero if angle greater than 180

}

//function to find smallest rectangle such that all points are within it
//input = points(Vertices)
//output = 4 points in vector in the order {Xmin, Ymin}, {Xmin, Ymax}, {Xmax, Ymax}, {Xmax, Ymin}
vector<pair<double, double>> find_smallest_rectangle(vector<Vertex*> &vertices)
{
    double Xmin = 1e9, Xmax = -1e9, Ymin = 1e9, Ymax = -1e9;
    for(int i = 0; i<vertices.size(); i++)
    {
        Xmin = min(Xmin, vertices[i]->x);
        Xmax = max(Xmax, vertices[i]->x);
        Ymin = min(Ymin, vertices[i]->y);
        Ymax = max(Ymax, vertices[i]->y);
    }
    vector<pair<double, double>> ans;
    ans.push_back({Xmin, Ymin});
    ans.push_back({Xmin, Ymax});
    ans.push_back({Xmax, Ymax});
    ans.push_back({Xmax, Ymin});
    return ans;
}

void solve(vector<pair<double, double>> &points, DCEL* polygon)
{
    vector<vector<pair<double, double>>> L;
    vector<pair<double, double>> curr_l;
    curr_l.push_back(points[0]);
    L.push_back(curr_l);
    while(points.size() > 3)
    {
        curr_l.clear();
        pair<double, double> v1 = points[0];
        pair<double, double> v2 = points[1];

        curr_l.push_back(v1);
        curr_l.push_back(v2);

        int i = 1;
        int n = points.size();
        while(i < n)
        {
            cout<<n<<endl;
            polygon->print_vertex(polygon->find_vertex(points[(i)%n].first, points[(i)%n].second));
            //cout<<is_greater_than_180(polygon->find_vertex(points[i-1].first, points[i-1].second), polygon->find_vertex(points[i].first, points[i].second),polygon->find_vertex(points[(i+1)%n].first, points[(i+1)%n].second))<<" ";
            //cout<<is_greater_than_180(polygon->find_vertex(points[i].first, points[i].second),polygon->find_vertex(points[(i+1)%n].first, points[(i+1)%n].second),polygon->find_vertex(v1.first, v2.second))<<"\n";
            if(is_greater_than_180(polygon->find_vertex(points[i-1].first, points[i-1].second), polygon->find_vertex(points[i].first, points[i].second),polygon->find_vertex(points[(i+1)%n].first, points[(i+1)%n].second))
                    || is_greater_than_180(polygon->find_vertex(points[i].first, points[i].second),polygon->find_vertex(points[(i+1)%n].first, points[(i+1)%n].second),polygon->find_vertex(v1.first, v1.second)) )
                break;
            i++;
        }
        //cout<<i<<"\n";
        for(int j = 1; j<i; j++)
            curr_l.push_back(points[j]);

        for(int j = 0; j<i; j++)
            points.erase(points.begin());

        L.push_back(curr_l);
    }

    for(auto l : L)
    {
        for(auto p : l)
            cout<<p.first<<" "<<p.second<<"\n";
        cout<<"----\n\n";
    }
}

bool isClockwise(vector<pair<double,double>> points)
{
    int end = points.size() - 1;
    double area = (points[end].first)*points[0].second - (points[0].first)*points[end].second;
    for(int i=0; i<end; ++i) {
        int j=i+1;
        area += points[i].first*points[j].second - points[j].first*points[i].second;
    }
    return area<0;
    
}

int main()
{
    FASTIO;
    vector<pair<double, double>> points = {{0.0, 0.0}, {0.0, 2.0},{1.0, 1.0}, {2.0, 2.0},{2.0, 0.0}, {1.0,-1.0}};
    if(!isClockwise(points)) reverse(all(points));
    DCEL *polygon = new DCEL(points);
    //polygon->print();
    cout << is_greater_than_180(new Vertex(0, 0), new Vertex(0, 2), new Vertex(1, 1));
    //polygon->add_edge(polygon->find_vertex(1.0,1.0),polygon->find_vertex(0.0,0.0));
    //polygon->print();
    solve(points, polygon);
    return 0;
}
