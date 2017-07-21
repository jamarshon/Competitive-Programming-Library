void TestGetShortestPath1() {
    Graph<char> g;
    vector<char> v{'A', 'B', 'C', 'D', 'E', 'F'};
    for(auto c: v) g.AddVertex(c);
    g.AddEdge('A', 'B', 5);
    g.AddEdge('B', 'C', 2);
    g.AddEdge('C', 'D', 3);
    g.AddEdge('A', 'D', 9);
    g.AddEdge('A', 'E', 2);
    g.AddEdge('D', 'F', 2);
    g.AddEdge('E', 'F', 3);

    pair<unordered_map<char, int>, unordered_map<char, char>> output;
    output = g.GetShortestPath('A');

    unordered_map<char, int> distance;
    unordered_map<char, char> parent;
    tie(distance, parent) = output;

    unordered_map<char, int> expected_distance{
        {'A',0},
        {'B',5},
        {'C',7},
        {'D',7},
        {'E',2},
        {'F',5},
    };

    unordered_map<char, char> expected_parent{
        {'C','B'},
        {'F','E'},
        {'E','A'},
        {'D','F'},
        {'B','A'},
        {'A','A'},
    };

    cout << "Run distance test" << endl;
    assert(expected_distance.size() == distance.size());
    for(auto d: distance) {
        assert(expected_distance[d.first] == d.second);
    }
    cout << "Pass distance test" << endl;

    cout << "Run parent test" << endl;
    assert(expected_parent.size() == expected_parent.size());
    for(auto p: parent) {
        assert(expected_parent[p.first] == p.second);
    }
    cout << "Pass parent test" << endl;
}

void TestGetShortestPath2() {
    Graph<int> g;
    vector<int> v{0,1,2,3,4,5,6,7,8};
    for(auto d: v) g.AddVertex(d);
    g.AddEdge(0, 1, 4);
    g.AddEdge(0, 7, 8);
    g.AddEdge(1, 2, 8);
    g.AddEdge(1, 7, 11);
    g.AddEdge(2, 3, 7);
    g.AddEdge(2, 8, 2);
    g.AddEdge(2, 5, 4);
    g.AddEdge(3, 4, 9);
    g.AddEdge(3, 5, 14);
    g.AddEdge(4, 5, 10);
    g.AddEdge(5, 6, 2);
    g.AddEdge(6, 7, 1);
    g.AddEdge(6, 8, 6);
    g.AddEdge(7, 8, 7);

    pair<unordered_map<int, int>, unordered_map<int, int>> output;
    output = g.GetShortestPath(0);

    unordered_map<int, int> distance;
    unordered_map<int, int> parent;
    tie(distance, parent) = output;

    unordered_map<int, int> expected_distance{
        {0,0},
        {1,4},
        {2,12},
        {3,19},
        {4,21},
        {5,11},
        {6,9},
        {7,8},
        {8,14},
    };

    unordered_map<int, int> expected_parent{
        {4,5},
        {3,2},
        {5,6},
        {8,2},
        {6,7},
        {2,1},
        {7,0},
        {1,0},
        {0,0},
    };

    cout << "Run distance test" << endl;
    assert(expected_distance.size() == distance.size());
    for(auto d: distance) {
        assert(expected_distance[d.first] == d.second);
    }
    cout << "Pass distance test" << endl;

    cout << "Run parent test" << endl;
    assert(expected_parent.size() == expected_parent.size());
    for(auto p: parent) {
        cout << p.first << ' ' << p.second << endl;
        assert(expected_parent[p.first] == p.second);
    }
    cout << "Pass parent test" << endl;
}

void TestTopologicalSort() {
    Graph<char> g(true);
    vector<char> v{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    for(auto c: v) g.AddVertex(c);

    g.AddEdge('A', 'C', 0);
    g.AddEdge('B', 'C', 0);
    g.AddEdge('B', 'D', 0);
    g.AddEdge('C', 'E', 0);
    g.AddEdge('D', 'F', 0);
    g.AddEdge('E', 'F', 0);
    g.AddEdge('F', 'G', 0);

    vector<char> output;
    output = g.TopologicalSort();

    int output_len = output.size();

    cout << "Run topological sort test" << endl;
    assert(v.size() == output_len);
    for(int i = 0; i < output_len; i++) {
        assert(v[i] == output[i]);
    }
    cout << "Pass topological sort test" << endl;
}

