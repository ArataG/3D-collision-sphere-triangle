#pragma once
#include "myPoint3D.h"
static const double ROOT2 = 1.41421356237;

//線分オブジェクト
class Segment {
  public:
  Point p1, p2;
  Segment(Point p1 = Point(), Point p2 = Point()): p1(p1), p2(p2){}
};
typedef Segment Line;

//球体オブジェクト
class Sphere{
    public:
    Point c;
    double r;
    Sphere(Point c = Point(), double r = 0.0): c(c),r(r){}
};

//ボックス型オブジェクト
class Box{
  public:
  Point lt, rb; //LeftTop, RightBottom
  Box(Point lt = Point(), Point rb = Point()):lt(lt),rb(rb){}
};

//三角形オブジェクト
class Triangle {
  public:
  Point a, b, c;
  Triangle(Point a = Point(), Point b = Point(), Point c = Point()):a(a),b(b),c(c){}
};

//2点間の距離
double getDistance(Point a, Point b) {
  return abs(a - b);
}
//点と直線の距離
double getDistanceLP(Line l,Point p){
  return abs(cross(l.p2-l.p1, p-l.p1)) / abs(l.p2-l.p1);//ここの計算式怪しい
}
//点と線分の距離
double getDistanceSP(Segment s,Point p){
  if(dot(s.p2-s.p1, p-s.p1) < 0.0 ) return abs(p-s.p1);
  if(dot(s.p1-s.p2, p-s.p2) < 0.0 ) return abs(p-s.p2);
  return getDistanceLP(s,p);
}




