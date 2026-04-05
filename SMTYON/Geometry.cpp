// ---------------------------- Constants and Type Definitions ----------------------------
const int inf = 1e18;
const ld EPS = 1e-9;
const ld pi = acos(-1);
typedef ld T;
typedef complex<T> pt;
#define X real()
#define Y imag()

// ---------------------------- Input / Output ----------------------------
istream &operator>>(istream &in, pt &a)
{
  T real, imag;
  in >> real >> imag;
  a = pt(real, imag);
  return in;
}

// ---------------------------- Basic Operations ----------------------------
bool cmp(const pt &a, const pt &b) { return make_pair(a.X, a.Y) < make_pair(b.X, b.Y); }
T sq(pt p) { return p.X * p.X + p.Y * p.Y; }
T dot(pt v, pt w) { return v.X * w.X + v.Y * w.Y; }
T cross(pt v, pt w) { return v.X * w.Y - v.Y * w.X; }
ld dist(pt a, pt b) { return sqrtl(sq(a - b)); }
int sgn(T val) { return (val > EPS) - (val < -EPS); }
bool isPerp(pt v, pt w) { return fabs(dot(v, w)) < EPS; }
pt perp(pt p) { return {-p.Y, p.X}; }
ld toDeg(ld ang) { return ang * 180.0 / pi; }
ld toRad(ld ang) { return ang * pi / 180.0; }
bool same(pt a, pt b) { return !sgn(a.X - b.X) and !sgn(a.Y - b.Y); }

// ---------------------------- Angle Utilities ----------------------------

T orient(pt a, pt b, pt c) { return cross(b - a, c - a); }
T clamp(T val, T low, T high) { return max(low, min(val, high)); }
ld angle(pt v, pt w) { return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0)); }
// x,y adjacent sides | z opposite side
ld angle(T x, T y, T z) { return acos(clamp((x * x + y * y - z * z) / (2 * x * y), (T)-1.0, (T)1.0)); }
T orientedAngle(pt a, pt b, pt c)
{
  ld ampli = angle(b - a, c - a);
  if (orient(a, b, c) > 0)
    return ampli;
  else
    return 2 * pi - ampli;
}

T angleTravelled(pt a, pt b, pt c)
{
  ld ampli = angle(b - a, c - a);
  if (orient(a, b, c) > 0)
    return ampli;
  else
    return -ampli;
}

// check p in between angle(bac) counter clockwise
bool inAngle(pt a, pt b, pt c, pt p)
{
  T abp = orient(a, b, p), acp = orient(a, c, p), abc = orient(a, b, c);
  if (abc < 0)
    swap(abp, acp);
  return (abp >= 0 && acp <= 0) ^ (abc < 0);
}

// ---------------------------- Helper Functions ----------------------------

vector<pt> remove_colinear(vector<pt> pts)
{
  vector<pt> use;
  use.push_back(pts[0]);
  use.push_back(pts[1]);
  for (int i = 2; true; ++i)
  {
    pt a = use[(int)use.size() - 2];
    pt b = use[(int)use.size() - 1];
    if (sgn(cross(b - a, pts[i % (int)pts.size()] - a)) == 0)
      use.pop_back();
    if (i == (int)pts.size())
      break;
    use.push_back(pts[i]);
  }
  pt a = use[(int)use.size() - 1];
  pt b = use[0];
  pt c = use[1];
  if (sgn(cross(b - a, c - a)) == 0)
    use.erase(use.begin());
  return use;
}

void prepareConvexPolygon(vector<pt> &v)
{
  // 1. Find the point with the lowest y (break ties by x)
  int pivot = 0;
  for (int i = 1; i < v.size(); ++i)
    if (v[i].Y < v[pivot].Y || (v[i].Y == v[pivot].Y && v[i].X < v[pivot].X))
      pivot = i;

  // 2. Rotate the polygon so that pivot is at index 0
  rotate(v.begin(), v.begin() + pivot, v.end());

  // 3. Ensure polygon is CCW
  if (orient(v[0], v[1], v[2]) < 0)
    reverse(v.begin() + 1, v.end()); // Keep v[0], reverse rest
}

// rotate the polygon such that the (bottom, left)-most point is at the first position
void reorder_polygon(vector<pt> &p)
{
  int pos = 0;
  for (int i = 1; i < p.size(); i++)
  {
    if (p[i].Y < p[pos].Y || (sgn(p[i].Y - p[pos].Y) == 0 && p[i].X < p[pos].X))
      pos = i;
  }
  rotate(p.begin(), p.begin() + pos, p.end());
}

bool point_in_triangle(pt a, pt b, pt c, pt p, bool strictly_in = false)
{
  int sign1 = cross(b - a, p - a);
  int sign2 = cross(c - b, p - b);
  int sign3 = cross(a - c, p - c);
  if (strictly_in)
    return ((sign1 > 0 and sign2 > 0 and sign3 > 0) or (sign1 < 0 and sign2 < 0 and sign3 < 0));
  else
    return ((sign1 >= 0 and sign2 >= 0 and sign3 >= 0) or (sign1 <= 0 and sign2 <= 0 and sign3 <= 0));
}

bool half(pt p) // true if in blue half
{
  assert(p.X != 0 || p.Y != 0); // the argument of (0,0) is undefined
  return p.Y > 0 || (p.Y == 0 && p.X < 0);
}
void polarSort(vector<pt> &v)
{
  sort(v.begin(), v.end(), [](pt v, pt w)
       { return make_tuple(half(v), 0) < make_tuple(half(w), cross(v, w)); });
}

// ---------------------------- Transformations ----------------------------

pt translate(pt v, pt p) { return p + v; }
pt scale(pt c, T factor, pt p) { return c + (p - c) * factor; }
pt rot(pt p, pt c, ld a) { return c + pt(cos(a), sin(a)) * (p - c); }
pt normalize(pt a) { return a / abs(a); }
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) { return fp + (r - p) * (fq - fp) / (q - p); }

// ---------------------------- Line Structure ----------------------------

struct Line
{
  pt v;
  T c;

  Line(pt v, T c) : v(v), c(c) {}

  // from equation ax+by = c
  Line(T a, T b, T _c)
  {
    v = {b, -a};
    c = _c;
  }

  // line from two points
  Line(pt p, pt q)
  {
    v = q - p;
    c = cross(v, p);
  }

  T side(pt p) { return cross(v, p) - c; } // test point location wrt line
  ld dist(pt p) { return abs(side(p)) / abs(v); }
  double sqDist(pt p) { return side(p) * side(p) / (T)sq(v); }
  Line perpThrought(pt p) { return {p, p + perp(v)}; }
  bool cmpProj(pt p, pt q)
  {
    return dot(v, p) < dot(v, q);
  }
  Line translate(pt t) { return {v, c + cross(v, t)}; }
  Line shiftLeft(T dist) { return {v, c + dist * abs(v)}; } // shift right by negative dist
  pt proj(pt p) { return p - perp(v) * side(p) / sq(v); }
  pt refl(pt p) { return p - perp(v) * (T)2.0 * side(p) / sq(v); }
};

bool inter(Line l1, Line l2, pt &out) // out is the intersection point
{
  T d = cross(l1.v, l2.v);
  if (fabs(d) < EPS)
    return false;
  out = (l2.v * l1.c - l1.v * l2.c) / d; // requires floating-point coordinates
  return true;
}

Line bisector(Line l1, Line l2, bool interior)
{
  assert(cross(l1.v, l2.v) != 0); // l1 and l2 cannot be parallel!
  T sign = interior ? 1 : -1;
  return {l2.v / abs(l2.v) + l1.v / abs(l1.v) * sign, l2.c / abs(l2.v) + l1.c / abs(l1.v) * sign};
}

Line normalizeLine(pt a, pt b)
{
  int A = b.Y - a.Y;
  int B = b.X - a.X;
  int g = gcd(A, B);
  A /= g;
  B /= g;
  if (A < 0 or A == 0 and B < 0)
    A *= -1, B *= -1;
  int C = cross(pt(B, A), a);
  return Line(A, B, C);
}

// ---------------------------- Segment Utilities ----------------------------

bool inDisk(pt a, pt b, pt p)
{
  return dot(a - p, b - p) <= EPS;
}

bool onSegment(pt a, pt b, pt c) // check if c on ab segment
{
  return orient(a, b, c) == 0 && inDisk(a, b, c);
}

bool properInter(pt a, pt b, pt c, pt d, pt &out)
{
  T oa = orient(c, d, a),
    ob = orient(c, d, b),
    oc = orient(a, b, c),
    od = orient(a, b, d);
  // Proper intersection exists iff opposite signs
  if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
  {
    out = (a * ob - b * oa) / (ob - oa);
    return true;
  }
  return false;
}

set<pair<ld, ld>> inters(pt a, pt b, pt c, pt d)
{
  set<pair<ld, ld>> s;
  pt out;
  if (a == c || a == d)
  {
    s.insert(make_pair(a.X, a.Y));
  }
  if (b == c || b == d)
  {
    s.insert(make_pair(b.X, b.Y));
  }
  if (s.size())
    return s;

  if (properInter(a, b, c, d, out))
    return {make_pair(out.X, out.Y)};
  if (onSegment(c, d, a))
    s.insert(make_pair(a.X, a.Y));
  if (onSegment(c, d, b))
    s.insert(make_pair(b.X, b.Y));
  if (onSegment(a, b, c))
    s.insert(make_pair(c.X, c.Y));
  if (onSegment(a, b, d))
    s.insert(make_pair(d.X, d.Y));

  return s;
}

ld segPoint(pt a, pt b, pt p, pt &out)
{
  if (a != b)
  {
    Line l(a, b);
    if (l.cmpProj(a, p) and l.cmpProj(p, b)) // if closest to projection (in between a & b)
    {
      out = l.proj(p);
      return l.dist(p); // output distance to line
    }
  }
  if (abs(p - a) - abs(p - b) > EPS)
    out = b;
  else
    out = a;
  return min(abs(p - a), abs(p - b)); // otherwise distance to A or B
}

ld segSeg(pt a, pt b, pt c, pt d)
{
  pt dummy;
  if (properInter(a, b, c, d, dummy))
    return 0;
  return min({segPoint(a, b, c, dummy), segPoint(a, b, d, dummy),
              segPoint(c, d, a, dummy), segPoint(c, d, b, dummy)});
}

// ---------------------------- Polygon Utilities ----------------------------

ld areaTriangle(pt a, pt b, pt c)
{
  return abs(cross(b - a, c - a)) / 2.0;
}

T areaPolygon(vector<pt> &p)
{
  T area = 0;
  for (int i = 0, n = p.size(); i < n; i++)
  {
    area += cross(p[i], p[(i + 1) % n]); // wrap back to 0 if i == n - 1
  }
  return abs(area) / 2.0;
}

bool above(pt a, pt p)
{
  return p.Y >= a.Y;
}
// check if [PQ] crosses ray from A
bool crossesRay(pt a, pt p, pt q)
{
  return (above(a, q) - above(a, p)) * orient(a, p, q) > 0;
}

bool inPolygon(vector<pt> p, pt a, bool &onBoundary, bool strict = true) // strict false will count border points also
{
  int numCrossings = 0;
  for (int i = 0, n = p.size(); i < n; i++)
  {
    if (onSegment(p[i], p[(i + 1) % n], a))
    {
      onBoundary = true;
      return !strict;
    }
    numCrossings += crossesRay(a, p[i], p[(i + 1) % n]);
  }
  return (numCrossings & 1); // inside if odd number of crossings
}

// checks if a polygon is convex or not
bool isConvex(vector<pt> p)
{
  bool hasPos = false, hasNeg = false;
  int n = p.size();
  for (int i = 0; i < n; ++i)
  {
    int sign = orient(p[i], p[(i + 1) % n], p[(i + 2) % n]);
    hasPos |= (sign > 0);
    hasNeg |= (sign < 0);
  }
  return !(hasPos & hasNeg);
}

// 1 -> point out of polygon
// 0 -> point on border of polygon
// -1 -> point inside polygon
// call prepareConvexPolygon(v) before it
int inPolygonlg(pt pp, vector<pt> &v) // O(log(N))
{
  int n = v.size();
  int o1 = orient(v[0], v[1], pp);
  int o2 = orient(v[0], v[n - 1], pp);
  if (o1 == 0 && onSegment(v[0], v[1], pp))
    return 0;
  else if (o2 == 0 && onSegment(v[0], v[n - 1], pp))
    return 0;
  else if (o1 < 0 || o2 > 0)
    return 1;
  int l = 0, r = n - 1;
  int ans = 1;
  while (l <= r)
  {
    int mid = l + (r - l) / 2;
    int o = orient(v[0], v[mid], pp);
    if (o > 0)
    {
      ans = mid;
      l = mid + 1;
    }
    else
    {
      r = mid - 1;
    }
  }
  int oo = orient(v[ans], v[(ans + 1) % n], pp);
  if (oo > 0)
    return -1;
  else if (oo == 0 && onSegment(v[ans], v[(ans + 1) % n], pp))
    return 0;
  return 1;
}

// minimum distance between 2 polygons
ld polyPoly(vector<pt> p1, vector<pt> p2)
{
  ld ret = 1e18;
  auto lt = [&](pt a, pt b) -> bool
  { return make_pair(a.X, a.Y) < make_pair(b.X, b.Y); };
  auto gt = [&](pt a, pt b) -> bool
  { return make_pair(a.X, a.Y) > make_pair(b.X, b.Y); };
  auto nxt = [&](int a, int sz) -> int
  { return (a + 1) % sz; };
  auto prv = [&](int a, int sz) -> int
  { return (a - 1 + sz) % sz; };
  for (int rep = 0; rep < 2; ++rep)
  {
    swap(p1, p2);
    int ptr1 = 0, ptr2 = 0, n = p1.size(), m = p2.size();
    for (int i = 1; i < n; ++i)
    {
      if (lt(p1[i], p1[ptr1]))
        ptr1 = i;
    }
    for (int i = 1; i < m; ++i)
    {
      if (gt(p2[i], p2[ptr2]))
        ptr2 = i;
    }
    int cnt = 0;
    for (; ptr1 < n; ptr1 = (ptr1 + 1) % n)
    {
      if (cnt == n)
        break;
      cnt++;
      pt base = p1[nxt(ptr1, n)] - p1[ptr1];
      Line l(p1[nxt(ptr1, n)], p1[ptr1]);
      while (sgn(cross(base, p2[nxt(ptr2, m)] - p2[ptr2])) == sgn(cross(base, p2[ptr2] - p2[prv(ptr2, m)])))
      {
        ptr2 = nxt(ptr2, m);
      }
      ret = min(ret, segSeg(p1[nxt(ptr1, n)], p1[ptr1], p2[nxt(ptr2, m)], p2[ptr2]));
      ret = min(ret, segSeg(p1[nxt(ptr1, n)], p1[ptr1], p2[ptr2], p2[prv(ptr2, m)]));
    }
  }
  return ret;
}

// ---------------------------- Ray Utilities ----------------------------

int pointRay(pt p, pt a, pt b) // a is ray's origin, b defines direction
{
  pt dir = b - a; // ray direction vector
  pt v = p - a;   // vector from origin to point

  T u = dot(v, dir);

  if (u <= 0) // Point is behind the ray origin
  {
    return abs(v); // distance from p to a
  }
  else // Point is in front of the ray
  {
    // Compute closest point on the ray: a + (u / ||dir||²) * dir
    pt closest = a + (u / norm(dir)) * dir;
    return abs(p - closest); // distance from p to closest point
  }
}

ld rayRay(pt a, pt b, pt c, pt d)
{
  T inf = 1e12;
  b = a + (b - a) * inf;
  d = c + (d - c) * inf;
  if (inters(a, b, c, d).size())
    return 0;
  pt dummy;
  return min({segPoint(a, b, c, dummy), segPoint(a, b, d, dummy), segPoint(c, d, a, dummy), segPoint(c, d, b, dummy)});
}



// ---------------------------- Lattice Points ----------------------------

int latticeAB(pt A, pt B)
{
  return gcd((int)abs(A.X - B.X), (int)abs(A.Y - B.Y)) + 1;
}

// Pick's Theorem -> Area = (interior lattice points) + (boundary lattice points / 2) - 1
// make T int
pair<int, int> picks(vector<pt> &v) // {inside,boundary}
{
  int n = v.size();
  ld area = 0;
  int boundary = 0;
  for (int i = 0; i < n; ++i)
  {
    area += cross(v[i], v[(i + 1) % n]);
    pt vec = v[(i + 1) % n] - v[i];
    boundary += gcd((int)vec.X, (int)vec.Y); // T has to be int
  }
  int in = (abs(area) - boundary + 2) / 2;
  return make_pair(in, boundary);
}

// ---------------------------- Convex Hull ----------------------------

vector<pt> lower_hull(vector<pt> &points)
{
  // make sure points are sorted with cmp
  vector<pt> lower;
  for (auto &p : points)
  {
    while (lower.size() > 1 && orient(lower[lower.size() - 2], lower.back(), p) <= 0) // < to include collinear
    {
      lower.pop_back();
    }
    lower.push_back(p);
  }
  return lower;
}

vector<pt> upper_hull(vector<pt> &points)
{
  // make sure points are sorted with cmp
  vector<pt> upper;
  for (auto &p : points)
  {
    while (upper.size() > 1 && orient(upper[upper.size() - 2], upper.back(), p) >= 0) // > to include collinear
    {
      upper.pop_back();
    }
    upper.push_back(p);
  }
  return upper;
}

vector<pt> convex_hull(vector<pt> &p)
{
  if (p.size() <= 1)
    return p;

  vector<pt> sorted = p;
  sort(all(sorted), cmp);

  vector<pt> lower = lower_hull(sorted);
  vector<pt> upper = upper_hull(sorted);

  // Remove duplicate endpoints
  lower.pop_back();
  reverse(upper.begin(), upper.end());
  upper.pop_back();

  // Combine
  lower.insert(lower.end(), upper.begin(), upper.end());

  if (lower.size() == 2 && lower[0] == lower[1])
    lower.pop_back();

  return lower;
}

// ---------------------------- Circles ----------------------------

struct Circle
{
  pt o;
  T r;

  Circle() {}
  Circle(pt o, T r) : o(o), r(r) {}

  Circle(pt a, pt b, pt c)
  {
    b -= a, c -= a;
    assert(cross(b, c) != 0);
    pt center = a + perp(b * sq(c) - c * sq(b)) / cross(b, c) / (T)2;
    T radius = abs(center - a);
    r = radius;
    o = center;
  }

  // Line intersection: returns number of intersection points, stores them in 'out'
  int circleLine(Line l, pair<pt, pt> &out) const
  {
    T h2 = r * r - l.sqDist(o);
    if (h2 >= 0)
    {
      pt p = l.proj(o);
      pt h = l.v * (T)(sqrt(h2) / abs(l.v));
      out = {p - h, p + h};
    }
    return 1 + sgn(h2);
  }

  // Circle-circle intersection: returns number of intersection points, stores them in 'out'
  int circleCircle(pt o1, T r1, pt o2, T r2, pair<pt, pt> &out)
  {
    pt d = o2 - o1;
    T d2 = sq(d);
    if (d2 == 0)
    {
      assert(r1 != r2);
      return 0;
    } // concentric circles
    T pd = (d2 + r1 * r1 - r2 * r2) / 2; // = |O_1P| * d
    T h2 = r1 * r1 - pd * pd / d2;       // = hˆ2
    if (h2 >= 0)
    {
      pt p = o1 + d * pd / d2, h = perp(d) * (T)sqrt(h2 / d2);
      out = {p - h, p + h};
    }
    return 1 + sgn(h2);
  }

  // Tangents from this to another circle: returns number of tangent lines, fills 'out'
  int tangents(const Circle &c2, bool inner, vector<pair<pt, pt>> &out) const
  {
    T r2 = inner ? -c2.r : c2.r;
    pt d = c2.o - o;
    T dr = r - r2, d2 = sq(d), h2 = d2 - dr * dr;
    if (d2 == 0 || h2 < 0)
    {
      assert(h2 != 0);
      return 0;
    }
    for (T sign : {-1, 1})
    {
      pt v = (d * dr + perp(d) * (T)sqrt(h2) * sign) / d2;
      out.push_back({o + v * r, c2.o + v * r2});
    }
    return 1 + (h2 > 0);
  }

  // Area of intersection with another circle
  ld intersection_area(const Circle &c) const
  {
    ld dx = o.X - c.o.X;
    ld dy = o.Y - c.o.Y;
    ld d = sqrtl(dx * dx + dy * dy);

    if (d >= r + c.r)
      return 0.0L;
    if (d + c.r <= r)
      return pi * c.r * c.r;
    if (d + r <= c.r)
      return pi * r * r;

    // angle and segment area for c
    ld ct = (c.r * c.r + d * d - r * r) / (2.0L * c.r * d);
    ld theta = acosl(ct);
    ld area1 = c.r * c.r * (theta - sinl(theta) * cosl(theta));

    // angle and segment area for this circle
    ct = (r * r + d * d - c.r * c.r) / (2.0L * r * d);
    theta = acosl(ct);
    ld area2 = r * r * (theta - sinl(theta) * cosl(theta));

    return area1 + area2;
  }
  bool operator==(const Circle &c) const { return c.o == o and sgn(c.r - r) == 0 and sgn(r) > 0; }
};

// ---------------------------- Rotating Calipers ----------------------------

// Computes max distance between any two points (diameter) of a convex polygon
ld farthest_pair(vector<pt> &p)
{
  vector<pt> hull = convex_hull(p);
  int m = hull.size();
  int j = 1;
  ld d = 0;
  // rotating calipers
  for (int i = 0; i < m; ++i)
  {
    while (norm(hull[i] - hull[(j + 1) % m]) > norm(hull[i] - hull[j]))
      j = (j + 1) % m;
    d = max((T)d, norm(hull[i] - hull[j]));
  }
  return sqrtl(d);
}

// Computes min width of a parallel strip that can enclose all points
ld min_width(vector<pt> p)
{
  vector<pt> hull = convex_hull(p);
  int m = hull.size();
  if (m <= 2)
    return 0;
  ld ret = 1e18;
  // rotating calipers
  for (int i = 0, j = 1; i < m; ++i)
  {
    pt u = hull[i];
    pt v = hull[(i + 1) % m];
    while (areaTriangle(u, v, hull[(j + 1) % m]) > areaTriangle(u, v, hull[j]))
      j = (j + 1) % m;
    ld height = 2 * areaTriangle(u, v, hull[j]) / dist(u, v);
    ret = min(ret, height);
  }
  return ret;
}

// area and perimeter of minimum enclosing rectangle
pair<ld, ld> minimum_enclosing_rectangle(vector<pt> &p)
{
  int n = p.size();
  if (n <= 2)
  {
    ld d = 0;
    for (int i = 0; i < n; ++i)
      d += dist(p[i], p[(i + 1) % n]);
    return {0, d};
  }
  int mndot = 0;
  double tmp = dot(p[1] - p[0], p[0]);
  for (int i = 1; i < n; i++)
  {
    if (dot(p[1] - p[0], p[i]) <= tmp)
    {
      tmp = dot(p[1] - p[0], p[i]);
      mndot = i;
    }
  }
  ld perim = 1e18, area = 1e18;
  int i = 0, j = 1, mxdot = 1;
  while (i < n)
  {
    pt cur = p[(i + 1) % n] - p[i];
    while (cross(cur, p[(j + 1) % n] - p[j]) >= 0)
      j = (j + 1) % n;
    while (dot(p[(mxdot + 1) % n], cur) >= dot(p[mxdot], cur))
      mxdot = (mxdot + 1) % n;
    while (dot(p[(mndot + 1) % n], cur) <= dot(p[mndot], cur))
      mndot = (mndot + 1) % n;
    Line l(p[i], p[(i + 1) % n]);
    ld w = (dot(p[mxdot], cur) / abs(cur) - dot(p[mndot], cur) / abs(cur));
    ld h = l.dist(p[j]);
    perim = min(perim, 2.0 * (w + h));
    area = min(area, w * h);
    i++;
  }
  return {area, perim};
}

// ---------------------------- Half-Plane Intersection ----------------------------

struct Halfplane
{

  // 'p' is a passing point of the line and 'pq' is the direction vector of the line.
  pt p, pq;
  long double angle;

  Halfplane() {}
  Halfplane(const pt &a, const pt &b) : p(a), pq(b - a)
  {
    angle = atan2l(pq.Y, pq.X);
  }

  // Check if point 'r' is outside this half-plane.
  // Every half-plane allows the region to the LEFT of its line.
  bool out(const pt &r)
  {
    return cross(pq, r - p) < -EPS;
  }

  // Comparator for sorting.
  bool operator<(const Halfplane &e) const
  {
    return angle < e.angle;
  }

  // Intersection point of the lines of two half-planes. It is assumed they're never parallel.
  friend pt inter(const Halfplane &s, const Halfplane &t)
  {
    long double alpha = cross((t.p - s.p), t.pq) / cross(s.pq, t.pq);
    return s.p + (s.pq * (T)alpha);
  }
};

// O(Nlog(N))
// NO COLLINEAR POINTS (Call remove_collinear first)
// points have to be in CCW order
vector<pt> hp_intersect(vector<Halfplane> &H, int sz = inf)
{
  pt box[4] = {// Bounding box in CCW order
               pt(sz, sz),
               pt(-sz, sz),
               pt(-sz, -sz),
               pt(sz, -sz)};

  for (int i = 0; i < 4; i++)
  { // Add bounding box half-planes.
    Halfplane aux(box[i], box[(i + 1) % 4]);
    H.push_back(aux);
  }

  // Sort by angle and start algorithm
  sort(H.begin(), H.end());
  deque<Halfplane> dq;
  int len = 0;
  for (int i = 0; i < H.size(); i++)
  {

    // Remove from the back of the deque while last half-plane is redundant
    while (len > 1 && H[i].out(inter(dq[len - 1], dq[len - 2])))
    {
      dq.pop_back();
      --len;
    }

    // Remove from the front of the deque while first half-plane is redundant
    while (len > 1 && H[i].out(inter(dq[0], dq[1])))
    {
      dq.pop_front();
      --len;
    }

    // Special case check: Parallel half-planes
    if (len > 0 && fabsl(cross(H[i].pq, dq[len - 1].pq)) < EPS)
    {
      // Opposite parallel half-planes that ended up checked against each other.
      if (dot(H[i].pq, dq[len - 1].pq) < 0.0)
        return vector<pt>();

      // Same direction half-plane: keep only the leftmost half-plane.
      if (H[i].out(dq[len - 1].p))
      {
        dq.pop_back();
        --len;
      }
      else
        continue;
    }

    // Add new half-plane
    dq.push_back(H[i]);
    ++len;
  }

  // Final cleanup: Check half-planes at the front against the back and vice-versa
  while (len > 2 && dq[0].out(inter(dq[len - 1], dq[len - 2])))
  {
    dq.pop_back();
    --len;
  }

  while (len > 2 && dq[len - 1].out(inter(dq[0], dq[1])))
  {
    dq.pop_front();
    --len;
  }

  // Report empty intersection if necessary
  if (len < 3)
    return vector<pt>();

  // Reconstruct the convex polygon from the remaining half-planes.
  vector<pt> ret(len);
  for (int i = 0; i + 1 < len; i++)
  {
    ret[i] = inter(dq[i], dq[i + 1]);
  }
  ret.back() = inter(dq[len - 1], dq[0]);
  return ret;
}

// ---------------------------- Minkowski Sum ----------------------------

// a and b are convex polygons
// returns a convex hull of their minkowski sum
// min(a.size(), b.size()) >= 2
vector<pt> minkowski_sum(vector<pt> a, vector<pt> b)
{
  reorder_polygon(a);
  reorder_polygon(b);
  int n = a.size(), m = b.size();
  int i = 0, j = 0;
  a.push_back(a[0]);
  a.push_back(a[1]);
  b.push_back(b[0]);
  b.push_back(b[1]);
  vector<pt> c;
  while (i < n || j < m)
  {
    c.push_back(a[i] + b[j]);
    double p = cross(a[i + 1] - a[i], b[j + 1] - b[j]);
    if (sgn(p) >= 0)
      ++i;
    if (sgn(p) <= 0)
      ++j;
  }
  return c;
}

// ---------------------------- Winding Number ----------------------------

/*
the winding number of a closed curve in the plane around a given point
is an integer representing the total number of times that the curve travels counterclockwise around the point
*/
// inf -> a is on edge of p
// 0 -> a is outside p
// otherwise -> a is inside p
int winding_number(vector<pt> &p, pt a)
{
  int ret = 0;
  for (int i = 0, n = p.size(); i < n; ++i)
  {
    pt cur = p[i];
    pt nxt = p[(i + 1) % n];
    if (onSegment(cur, nxt, a))
      return inf;
    if (cur.Y <= a.Y and nxt.Y > a.Y and cross(nxt - cur, a - cur) > 0)
      ret++;
    else if (cur.Y > a.Y and nxt.Y <= a.Y and cross(nxt - cur, a - cur) < 0)
      ret--;
  }
  return ret;
}

// ---------------------------- Special Shapes ----------------------------

struct Star
{
  int n;    // number of sides of the star
  double r; // radius of the circumcircle
  Star(int _n, double _r)
  {
    n = _n;
    r = _r;
  }

  double area()
  {
    double theta = pi / n;
    double s = 2 * r * sin(theta);
    double R = 0.5 * s / tan(theta);
    double a = 0.5 * n * s * R;
    double a2 = 0.25 * s * s / tan(1.5 * theta);
    return a - n * a2;
  }
};

// ---------------------------- Miscellaneous ----------------------------

// find a circle of radius r that contains as many points as possible
// O(n^2 log n)
// Small EPS (1e-18) !
pair<int, pt> maximum_circle_cover(vector<pt> p, ld r)
{
  int n = p.size();
  int ans = 0;
  int id = 0;
  ld th = 0;
  for (int i = 0; i < n; ++i)
  {
    // maximum circle cover when the circle goes through this point
    vector<pair<ld, int>> events = {{-pi, +1}, {pi, -1}};
    for (int j = 0; j < n; ++j)
    {
      if (j == i)
        continue;
      ld d = abs(p[i] - p[j]);
      if (d > r * 2)
        continue;
      ld dir = arg(p[j] - p[i]);
      ld ang = acos(d / 2 / r);
      ld st = dir - ang, ed = dir + ang;
      if (st > pi)
        st -= pi * 2;
      if (st <= -pi)
        st += pi * 2;
      if (ed > pi)
        ed -= pi * 2;
      if (ed <= -pi)
        ed += pi * 2;
      events.push_back({st - EPS, +1}); // take care of precisions!
      events.push_back({ed, -1});
      if (st > ed)
      {
        events.push_back({-pi, +1});
        events.push_back({+pi, -1});
      }
    }
    sort(events.begin(), events.end());
    int cnt = 0;
    for (auto &&e : events)
    {
      cnt += e.second;
      if (cnt > ans)
      {
        ans = cnt;
        id = i;
        th = e.first;
      }
    }
  }
  pt w = pt(p[id].X + r * cos(th), p[id].Y + r * sin(th));
  return {ans, w}; // points covered and center of the circle
}

// radius of the maximum inscribed circle in a convex polygon
double maximum_inscribed_circle(vector<pt> p)
{
  int n = p.size();
  if (n <= 2)
    return 0;
  double l = 0, r = 20000;
  while (r - l > EPS)
  {
    double mid = (l + r) * 0.5;
    vector<Halfplane> h;
    const int L = 1e9;
    h.push_back(Halfplane(pt(-L, -L), pt(L, -L)));
    h.push_back(Halfplane(pt(L, -L), pt(L, L)));
    h.push_back(Halfplane(pt(L, L), pt(-L, L)));
    h.push_back(Halfplane(pt(-L, L), pt(-L, -L)));
    for (int i = 0; i < n; i++)
    {
      pt z = perp(p[(i + 1) % n] - p[i]);
      z = normalize(z);
      z *= mid;
      pt y = p[i] + z, q = p[(i + 1) % n] + z;
      h.push_back(Halfplane(p[i] + z, p[(i + 1) % n] + z));
    }
    vector<pt> nw = hp_intersect(h);
    if (!nw.empty())
      l = mid;
    else
      r = mid;
  }
  return l;
}

Circle inCircle(pt A, pt B, pt C) // triangle ABC
{
  T a = abs(B - C);
  T b = abs(C - A);
  T c = abs(A - B);
  pt I = (a * A + b * B + c * C) / (a + b + c); // incenter

  T s = (a + b + c) / 2;
  T area = sqrt(s * (s - a) * (s - b) * (s - c));
  T r = area / s; // inradius

  return Circle(I, r); // center and radius
}