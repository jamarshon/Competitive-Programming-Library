#include <iostream>

using namespace std;

template<typename T>
struct Edge {
    T to;
    int weight;
    Edge(T _to, int _weight): to(_to), weight(_weight) {}
    bool operator < (const Edge<T>& rhs) const {
        return (weight == rhs.weight) ? (to < rhs.to) : (weight < rhs.weight);
    }
};

int main() {
    return 0;
}