#pragma once

#include "myObject3D.h"

//衝突の判定を管理する．
namespace MYCROSS{

//1. 三角形を含む平面と球の中心との距離が球の半径を越えていたら非交差と判定
bool isCrossSpherePlane(Sphere s, Triangle t){ 
  double dist = 0; //三角形を含む平面と中心の距離
  dist=abs( dot(t.a-s.c, cross(t.b-s.c, t.c-s.c)))/abs(cross(t.b-t.a,t.c-t.a));
  if(dist > s.r) return 0; // 非交差
  return 1; //交差の可能性あり
}
  /*素朴な実装
  double S = abs(cross((b-a),(c-a)))/2;
  double V = abs( dot((a-p), cross((b-p), (c-p))) )/6;
  double dist = (3*V)/S;
  if(dist > s.r) return 0;
  return 1;
  */

 //2. 球と球の交差判定　(2球の中心距離)^2 > (球１の半径+球2の半径)^2
 //-> sqrt取らないので多少高速に判定できる．
inline bool isCrossSphereSphere(Sphere s1, Sphere s2){
  //if(getDistance(s1.c, s2.c) > s1.r + s2.r) return 0;
  if(norm(s1.c-s2.c) > (s1.r + s2.r)*(s1.r+s2.r)) return 0;
  return 1;
}

//クラメルの式で使う
double det(Point a, Point b, Point c){
  return a.x*b.y*c.z + a.z*b.x*c.y + a.y*b.z*c.x
          - a.z*b.y*c.x - a.y*b.x*c.z - a.x*b.z*c.y; 
}

//クラメルの式: 3つの連立方程式
Point cramer(Point a, Point b, Point c, Point d){
  double  dt = det(a,b,c);
  return Point(det(d,b,c)/dt, det(a,d,c)/dt, det(a,b,d)/dt);
}

//レイと平面の交点が三角形の内部にあるかどうかの判定
bool isContainTriangle(Point p0, Triangle tri){
  //法線ベクトル
  Point n = cross(tri.b - tri.a, tri.c - tri.a);
  n = n / abs(n); //単位ベクトル
  n = Point(0,0,0)-n;
  Point p = cramer(n,tri.b - tri.a, tri.c - tri.a, p0 - tri.a);
  if(p.y >= 0 && p.z >= 0 && p.y+p.z <= 1) return 1;
  return 0;
}


//3. 球オブジェクトと三角形の交差判定
//三角形の各点，各辺，三角形平面のいずれかが，球の内外に少なくとも一つ存在すれば交差している．
inline bool isCrossSphereTriangle(Sphere sph, Triangle tri){
  bool isIn = false;
  double r2 = sph.r * sph.r;

 //3-1. 球の中心と三角形の各点の距離を比較
 // double distpp1 = getDistance(sph.c, tri.a) - sph.r;
  double distpp1 = norm(sph.c - tri.a) - r2;
  if(fabs(distpp1) <=  EPS ) return 1;  //接触:distpp1 == 0, 誤差を考慮してEPS
  if (distpp1 < 0) isIn = true;

  //double distpp2 = getDistance(sph.c, tri.b) - sph.r;
  double distpp2 = norm(sph.c-tri.b) - r2;
  if(fabs(distpp2) <=  EPS ) return 1;
  if(isIn != distpp2 < 0) return 1;

  //double distpp3 = getDistance(sph.c, tri.c) - sph.r;
  double distpp3 = norm(sph.c - tri.c) - r2;
  if(fabs(distpp3) <=  EPS ) return 1;
  if(isIn != distpp3 < 0) return 1;

  //3-2. 球の中心のと三角形の各辺の距離を比較//三角形の各距離は事前に計算できる
  Segment l1 = Segment(tri.a, tri.b);
  double distsp1 = getDistanceSP(l1, sph.c) - sph.r;
  if(fabs(distsp1)<= EPS ) return 1;
  if(isIn != distsp1 < 0) return 1;
  
  Segment l2 = Segment(tri.b, tri.c);
  double distsp2 = getDistanceSP(l2, sph.c) - sph.r;
  if(fabs(distsp2)<= EPS ) return 1;
  if(isIn != distsp2 < 0) return 1;

  Segment l3 = Segment(tri.c, tri.a);
  double distsp3 = getDistanceSP(l3, sph.c) - sph.r;
  if(fabs(distsp3)<= EPS ) return 1;
  if(isIn != distsp3 < 0) return 1;

  //3-3球と三角形内部平面の接触判定(全て球の外側に属していた場合に実行)
  if(isIn == false){
    if(isContainTriangle(sph.c, tri)) return 1;
  }
  return 0;
}

} // end namespace MYCROSS
