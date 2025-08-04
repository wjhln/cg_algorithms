#include <cmath>
#include <complex>
#include <iostream>
#include <ostream>
#include <vector>
using namespace std;

/*
  点/向量：
    1.模长
    2.两点之间距离
    3.向量加减
    4.向量点乘/数乘
    5.向量叉乘
  直线/线段：
    1.点和直线的关系
    2.求投影点
i   3.求相对直线的对称点
    4.点到直线距离
    5.点到线段距离
    6.两直线是否平行

*/

class Point {
  // 向量和点公用类，表示从0点到该点构成的向量
public:
  Point() = default;
  Point(double x_, double y_) : x(x_), y(y_) {};

  double norm() const { return hypot(x, y); };
  double distanceTo(const Point &p) const { return hypot(x - p.x, y - p.y); };
  Point operator+(const Point &p) const { return {x + p.x, y + p.x}; };
  // 算向量用，末端点减起始端点
  Point operator-(const Point &p) const { return {x - p.x, y - p.y}; };
  // 点乘，有投影用
  double operator*(const Point &p) const { return (x * p.x) + (y * p.y); };
  Point operator*(double k) const { return Point(k * x, k * y); }
  // 叉乘，有面积或者向量旋转用
  double operator^(const Point &p) const { return (x * p.y) - (p.x * y); };

public:
  double x, y;
};

class Line {
public:
  Line() = default;
  Line(const Point &s_, const Point &e_) : s(s_), e(e_) {};

  // 点和直线的关系:使用叉乘判断，右手螺旋
  double relation(const Point &p) const {
    Point vector1 = p - s;
    Point vector2 = e - s;
    double cross = vector1 ^ vector2;
    return cross;
  }

  // 投影点:向量点乘是投影点向量放大被投影的模长
  Point projection(const Point &p) const {
    Point p1 = e - s;
    Point p2 = p - s;
    return p1 * ((p2 * p1) / (p1.norm() * p1.norm()));
  }

  // 求相对直线的对称点: P` = 2H - P
  Point symmetricalPoint(const Point &p) {
    Point h = projection(p);
    return h * 2 - p;
  }

  // 点到直线距离： 叉乘的几何意义是平行四边形的面积
  double distancePoint2Line(const Point &p) {
    return fabs(((p - s) ^ (e - s))) / (e - s).norm();
  }

  // 点到线段距离：先用那一边的点当向量起点可以形成钝角
  // 哪边能形成钝角，点到线段的距离就是构成钝角的两点的距离
  // 如果都不行，用点到直线的距离求垂线
  double distancePoint2Seg(const Point &p) {
    if ((p - s) * (e - s) < 0)
      return (p - s).norm();
    else if ((p - e) * (s - e) < 0)
      return (p - e).norm();
    else
      return distancePoint2Line(p);
  }

  bool parallel(const Line &l) { return (((e - s) ^ (l.e - l.s)) != 0); }

public:
  Point s, e;
};

std::ostream &operator<<(std::ostream &os, const Point &p) {
  os << "(" << p.x << ", " << p.y << ")";
  return os;
}

int main() {
  Point p1(0, 1);
  Point p2(3, 3);
  Point p3(5, 5);
  Point p4(1, 0);
  Point p5(0, 5);
  Point p6(8, 0);
  Line l1(Point(0, 0), Point(4, 4));

  double norm = p2.norm();
  double distance = p1.distanceTo(p2);
  double relation = l1.relation(p4);
  Point projection = l1.projection(p4);
  Point symmetrical = l1.symmetricalPoint(p1);
  double distance_point2line = l1.distancePoint2Line(p6);
  double distance_point2seg = l1.distancePoint2Seg(p6);

  cout << distance_point2seg << endl;

  return 0;
}
