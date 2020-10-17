/*

  myPoint3D.h

*/

#pragma once

//#include <bits/stdc++.h>
#include<algorithm>
#include<cmath>
using namespace std;

#define EPS (1e-10)
#define equals(a,b) (fabs((a)-(b)) < EPS)
class Point{
  public:
  double x,y,z;
  Point(double x = 0, double y = 0,double z = 0) : x(x), y(y), z(z){}

  Point operator + (Point p) {return Point(x+p.x,y+p.y,z+p.z);}
  Point operator - (Point p) {return Point(x-p.x,y-p.y,z-p.z);}
  Point operator * (double k)  {return Point(x*k,y*k,z*k);}
  Point operator / (double k) {return Point(x/k,y/k,z/k);}
  double norm(){return x*x + y*y + z*z;}
  double abs(){return sqrt(norm());}

  bool operator < (const Point &p) const{
    return x!=p.x ? x < p.x : y < p.y;
  }
  bool operator == (const Point &p) const{
    return fabs(x-p.x) < EPS && fabs(y-p.y) < EPS && fabs(z-p.z) < EPS;
  }
  void Show(){cout<<"point " << "x: " << x << " y: " << y <<" z: " << z << endl;};
};

typedef Point Vector;

double norm(Vector a){
  return a.x*a.x + a.y*a.y + a.z*a.z;
}
double abs(Vector a){
  return sqrt(norm(a));
}
double dot(Vector a,Vector b){
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point cross(Vector a,Vector b){
  return Point(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
