#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <queue>
#include <algorithm>
#include <stack>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <bitset>
#include <random>
#include <chrono>
#include <cctype>
#include <sstream>
 
using namespace std;
 
struct Point {
    int x, y;
};
 
int orientation(Point a, Point b, Point c) {
    int val = (b.y - a.y)*(c.x - b.x) - (c.y - b.y)*(b.x - a.x);
    if(val == 0) return 0; // colinear
    return val > 0 ? 1 : 2; // clock or counterclock
}
 
int distSq(Point a, Point b) {
    return pow(b.x-a.x, 2) + pow(b.y-a.y, 2);
}
 
void convexHull(vector<Point> points) {
    if(points.empty()) return;
 
    int N = points.size();
    // find lowest y, break ties with lowest x (bottom left point)
    int start_pt_idx = 0;
    for(int i = 1; i < N; i++) {
        bool win_tie = points[i].y == points[start_pt_idx].y && points[i].x < points[start_pt_idx].x;
        if(win_tie || points[i].y < points[start_pt_idx].y) {
            start_pt_idx = i;
        }
    }
 
    // sort points based on polar angle with respect to start_pt
    swap(points[start_pt_idx], points[0]);
    Point start_pt = points[0];
    sort(points.begin() + 1, points.end(), [&start_pt](const Point& lhs, const Point rhs){
        int o = orientation(start_pt, lhs, rhs);
        if(o == 0) return distSq(start_pt, lhs) < distSq(start_pt, rhs); // same angle so put less distance first
        return o == 2; // if start_pt, lhs, rhs is ccw put lhs before rhs
    });
 
    // remove all collinear points with start_pt except the furthest one as it overshadows the closer ones
    int arr_len = 1;
    for(int i = 1; i < N; i++) {
        while(i < N - 1 && orientation(start_pt, points[i], points[i+1]) == 0) i++;
        points[arr_len] = points[i];
        arr_len++;
    }
 
    if(arr_len < 3) return; // no convex hull
    vector<Point> stk(points.begin(), points.begin() + 3); // first 3 elements are ccw and are part of the result initially
    for(int i = 3; i < arr_len; i++) {
        int stk_size = stk.size();
        // keep removing elements from the result while points next-to-top, top, and points[i] makes a non-left turn
        while(orientation(stk[stk_size - 2], stk[stk_size - 1], points[i]) != 2) {
            stk.pop_back();
            stk_size--;
        }
        stk.push_back(points[i]);
    }
 
    while(!stk.empty()) {
        Point p = stk.back();
        cout << "(" << p.x << ", " << p.y <<")" << endl;
        stk.pop_back();
    }
}
 
int main() {
    vector<Point> points{{0, 3}, {1, 1}, {2, 2}, {4, 4},
                      {0, 0}, {1, 2}, {3, 1}, {3, 3}};
    convexHull(points);
    return 0;
}