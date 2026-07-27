#include <deque>
#include <iostream>
#include <vector>
using namespace std;

/*
Finding the minimum value

Note: If you need to find the maximum instead of the minimum, 
you can simply add your lines as (-m, -c) 
and then negate the result of the query
*/

// Represents a line y = m*x + c
struct Line {
  long long m, c;

  // Evaluates the line equation at a given x
  long long eval(long long x) { return m * x + c; }
};

struct ConvexHullTrick {
  deque<Line> dq;

  // Helper to determine if line l3 makes line l2 redundant.
  // Given lines l1, l2, l3 in decreasing order of slopes:
  // l2 becomes useless if the intersection of l1 and l3 is
  // to the left of the intersection of l1 and l2.
  // Mathematical condition: (c3 - c1) / (m1 - m3) <= (c2 - c1) / (m1 - m2)
  // To avoid floating point division issues, we cross-multiply.
  bool useless(Line l1, Line l2, Line l3) {
    // (c3 - c1) * (m1 - m2) <= (c2 - c1) * (m1 - m3)
    // Note: double can be used if coordinates are extremely large,
    // but long long is safer for precision if it fits without overflow.
    return (double)(l3.c - l1.c) * (l1.m - l2.m) <=
           (double)(l2.c - l1.c) * (l1.m - l3.m);
  }

  // Add a new line y = m*x + c
  // REQUIRED: Slopes 'm' MUST BE ADDED IN MONOTONICALLY DECREASING ORDER.
  void add(long long m, long long c) {
    Line l3 = {m, c};

    // As long as there are at least two lines in the deque, check if the
    // new line makes the last line in the envelope redundant.
    while (dq.size() >= 2 && useless(dq[dq.size() - 2], dq.back(), l3)) {
      dq.pop_back(); // Remove redundant line
    }
    dq.push_back(l3);
  }

  // Find the minimum value among all lines at a given x
  // REQUIRED: Query values 'x' MUST BE MONOTONICALLY INCREASING.
  long long query(long long x) {
    // As x increases, the optimal line might shift to the right.
    // We evaluate the first two lines. If the second line is better (gives a
    // lower y), the first line will never be optimal again (due to increasing
    // x), so we drop it.
    while (dq.size() >= 2 && dq[0].eval(x) >= dq[1].eval(x)) {
      dq.pop_front();
    }

    // The first line in the deque is now the optimal line for this x.
    return dq.front().eval(x);
  }
};

int main() {
  ConvexHullTrick cht;

  // Example Usage:
  // Adding lines y = mx + c (must be in decreasing order of m)
  cht.add(10, 5);
  cht.add(8, 12);
  cht.add(5, 20);

  // Querying minimum at x (must be in increasing order of x)
  cout << "Min at x=2: " << cht.query(2) << endl;
  cout << "Min at x=5: " << cht.query(5) << endl;

  return 0;
}