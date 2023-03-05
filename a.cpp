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
pair<double, double> get_vector(Vertex *, Vertex *);
bool is_greater_than_180(Vertex *, Vertex *, Vertex *);
    
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
    
    bool add_edge(Vertex *v1, Vertex *v2)
    {
        if (v1 == v2 || edge_exits(v1, v2))
            return false;
        Edge *new_edge = new Edge(v1, v2);
        Edge *new_edge_twin = new Edge(v2, v1);
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
    
        Edge *curr = v2->inc_edge;
        /* figure out how to add faces
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
        */
        return true;
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
    
    bool edge_exits(Vertex *v1, Vertex *v2)
    {
        for (int i = 0; i < edges.size(); i++)
        {
            if (edges[i]->start == v1 && edges[i]->end == v2)
                return true;
        }
        return false;
    }
    
    vector<Vertex *> find_notch_vertices(vector<Vertex *> vertices)
    {
        vector<Vertex *> ans;
        int n = vertices.size();
        if (n < 3)
            return ans;
        for (int i = 0; i < n; i++)
        {
            if (is_greater_than_180(vertices[(i - 1 + n) % n], vertices[i], vertices[(i + 1) % n]))
                ans.push_back(vertices[i]);
        }
        return ans;
    }
    
    void write_edges()
    {
        freopen("inputPy.txt", "w", stdout);
        for(auto i : edges)
            cout<<i->start->x*10<<" "<<i->start->y*10<<" "<<i->end->x*10<<" "<<i->end->y*10<<"\n";
    }
    
    void print()
    {
        cout << "Vertex : \n";
        for (Vertex *v : vertices)
            print_vertex(v);
        cout << "\n";
        cout << "Edge : " << edges.size() << "\n";
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
    
bool is_same_sign(double a, double b)
{
    return ((a > 0 && b > 0) || (a < 0 && b < 0));
}
    
pair<double, double> get_vector(Vertex *v1, Vertex *v2)
{
    return {v2->x - v1->x, v2->y - v1->y};
}
    
int get_cross_product(Vertex *v1, Vertex *v2, Vertex *v3)
{
    pair<double, double> p1 = get_vector(v2, v1);
    pair<double, double> p2 = get_vector(v2, v3);
    double cross_product = p1.first * p2.second - p1.second * p2.first;
    return cross_product; // greater than zero if angle greater than 180
}
    
bool is_greater_than_180(Vertex *v1, Vertex *v2, Vertex *v3)
{
    double cross_product = get_cross_product(v1, v2, v3);
    return cross_product < 0; // greater than zero if angle greater than 180
}
    
bool is_inside_rectange(vector<pair<double, double>> &rectangle, Vertex *v)
{
    return ((v->x >= rectangle[0].first && v->x <= rectangle[2].first) && (v->y >= rectangle[0].second && v->y <= rectangle[1].second));
}
    
bool is_inside_polygon(vector<Vertex *> &polygon_vertices, Vertex *v)
{
    int pos = 0, neg = 0;
    int n = polygon_vertices.size();
    for (int i = 0; i < n; i++)
    {
        pair<double, double> p1 = get_vector(v, polygon_vertices[i]);
        pair<double, double> p2 = get_vector(polygon_vertices[i], polygon_vertices[(i + 1) % n]);
        double cross_product = p1.first * p2.second - p1.second * p2.first;
        if (cross_product > 0)
            pos++;
        else
            neg++;
    }
    return ((pos == 0) || (neg == 0));
}
    
// function to find smallest rectangle such that all points are within it
// input = points(Vertices)
// output = 4 points in vector in the order {Xmin, Ymin}, {Xmin, Ymax}, {Xmax, Ymax}, {Xmax, Ymin}
vector<pair<double, double>> find_smallest_rectangle(vector<Vertex *> &vertices)
{
    double Xmin = 1e9, Xmax = -1e9, Ymin = 1e9, Ymax = -1e9;
    for (int i = 0; i < vertices.size(); i++)
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
    
void solve(vector<pair<double, double>> &points, DCEL *polygon)
{
    vector<vector<pair<double, double>>> L;
    vector<pair<double, double>> curr_l;
    curr_l.push_back(points[0]);
    L.push_back(curr_l);
    Vertex *v0 = polygon->find_vertex(points[0].first, points[0].second);
    
    int ptr = 0;
    while (points.size() > 3)
    {
        int n = points.size();
        curr_l.clear();
        pair<double, double> v1 = L[L.size()-1][L[L.size()-1].size()-1];
        for(int i = 0;i<n;i++)
        {
            if(v1 == points[i])
            {
                ptr = i+1;
                break;
            }
        }
        pair<double, double> v2 = points[(ptr)%n];
        int i = (ptr)%n;
    
        curr_l.push_back(v1);
        curr_l.push_back(v2);
    
        while (curr_l.size() < n)
        {
            // cout<<i<<endl;
            // polygon->print_vertex(polygon->find_vertex(points[(i)%n].first, points[(i)%n].second));
            // cout<<is_greater_than_180(polygon->find_vertex(points[i-1].first, points[i-1].second), polygon->find_vertex(points[i].first, points[i].second),polygon->find_vertex(points[(i+1)%n].first, points[(i+1)%n].second))<<" ";
            // cout<<is_greater_than_180(polygon->find_vertex(points[i].first, points[i].second),polygon->find_vertex(points[(i+1)%n].first, points[(i+1)%n].second),polygon->find_vertex(v1.first, v2.second))<<"\n";
            if (is_greater_than_180(polygon->find_vertex(points[(i - 1+n)%n].first, points[(i - 1+n)%n].second), polygon->find_vertex(points[i%n].first, points[i%n].second), polygon->find_vertex(points[(i + 1) % n].first, points[(i + 1) % n].second))
                    || is_greater_than_180(polygon->find_vertex(points[i%n].first, points[i%n].second), polygon->find_vertex(points[(i + 1) % n].first, points[(i + 1) % n].second), polygon->find_vertex(v1.first, v1.second))
                    || is_greater_than_180(polygon->find_vertex(points[(i + 1) % n].first, points[(i + 1) % n].second), polygon->find_vertex(v1.first, v1.second), polygon->find_vertex(v2.first, v2.second)))
                break;
            i++;
            curr_l.push_back(points[i%n]);
        }
        if(curr_l.size() == 2)
        {
            L.push_back(curr_l);
            continue;
        }
        //cout <<"i :  "<< i <<"n : "<<n<< "\n";
    
        if (curr_l.size() != points.size())
        {
            vector<Vertex *> curr_polygon;
            for (int i = 0; i < curr_l.size(); i++)
                curr_polygon.push_back(polygon->find_vertex(curr_l[i].first, curr_l[i].second));
    
            // very inefficient -> change it later
            vector<Vertex *> notches = polygon->find_notch_vertices(polygon->vertices);
            vector<Vertex *> LSPV;
            for (int i = 0; i < notches.size(); i++)
            {
                bool check = true;
                for (int j = 0; j < curr_l.size(); j++)
                    check &= (notches[i] != polygon->find_vertex(curr_l[j].first, curr_l[j].second));
                if (check)
                    LSPV.push_back(notches[i]);
            }
            /*
            cout<<LSPV.size()<<"L\n";
            for(auto i : LSPV)
                cout<<i->x<<" "<<i->y<<"\n";
            */
            while (LSPV.size() > 0)
            {
                //cout<<LSPV.size()<<"\n";
                vector<pair<double, double>> smallest_rectangle = find_smallest_rectangle(curr_polygon);
                bool backward = false;
                while (!backward && LSPV.size() > 0)
                {
                    //cout<<LSPV.size()<<"\n";
                    Vertex *v = LSPV[0];
                    //polygon->print_vertex(v);
                    //cout<<is_inside_rectange(smallest_rectangle, v)<<" "<<is_inside_polygon(curr_polygon, v)<<"D\n";
                    if (!is_inside_rectange(smallest_rectangle, v))
                    {
                        LSPV.erase(LSPV.begin());
                        continue;
                    }
    
                    if (!is_inside_polygon(curr_polygon, v))
                    {
                        LSPV.erase(LSPV.begin());
                        continue;
                    }
                    //cout<<LSPV.size()<<"\n";
                    double cross_product = get_cross_product(curr_polygon[0], v, curr_polygon[curr_polygon.size() - 1]);
    
                    for (int i = curr_l.size() - 1; i > 0; i--)
                    {
                        double curr_cross_product = get_cross_product(curr_polygon[0], v, curr_polygon[curr_polygon.size() - 1]);
                        if (is_same_sign(cross_product, curr_cross_product))
                        {
                            curr_l.pop_back();
                            curr_polygon.pop_back();
                        }
                        else
                            break;
                    }
                    LSPV.erase(LSPV.begin());
                    backward = true;
                }
            }
        }
        //cout<<points.size()<<" "<<curr_l.size()<<"\n";
        for (int j = 1; j < curr_l.size()-1; j++)
            points.erase(points.begin()+ptr);
        L.push_back(curr_l);
    }
    //cout << points.size() << "\n";
    /*
    if (points.size() > 0)
    {
        curr_l.clear();
        for (auto point : points)
            curr_l.push_back(point);
        curr_l.push_back({v0->x, v0->y});
    }
    L.push_back(curr_l);
    */
    for (auto l : L)
    {
        // cout<<"/*\n";
        polygon->add_edge(polygon->find_vertex(l[0].first, l[0].second), polygon->find_vertex(l[l.size() - 1].first, l[l.size() - 1].second));
        // cout<<"*/\n";
        for (auto p : l)
            cout << p.first << " " << p.second << "\n";
        cout << "----\n\n";
    }
}


    
int main()
{
    FASTIO;
    int n;
    cin >> n;
    vector<pair<double, double>> points(n);
    for (int i = 0; i < n; i++)
        cin >> points[i].first >> points[i].second;
    DCEL *polygon = new DCEL(points);
    // polygon->print();
    // cout << is_greater_than_180(new Vertex(0, 0), new Vertex(2, 0), new Vertex(1, 1));
    // polygon->add_edge(polygon->find_vertex(1.0,1.0),polygon->find_vertex(0.0,0.0));
    // polygon->print();
    //polygon->write_edges();
    solve(points, polygon);
    polygon->write_edges();
    
    /*
    vector<Vertex*> v = polygon->find_notch_vertices(polygon->vertices);
    for(auto i : v)
        cout<<i->x<<" "<<i->y<<"\n";
    */
    return 0;
}