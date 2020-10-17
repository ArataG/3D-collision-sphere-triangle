/*
  Enemy Class

*/

#pragma once

class Enemy{
private:
  //Sphere outsph;  //Bounding Sphere
  Box outbox; //Bounding Box
  Triangle tri;
  double len;
  double area;
public:
  Point center;
  //get
  Triangle GetTriangle() {return tri;}
  Box GetBox(){return outbox;}
  Point GetCenter(){return center;}
  Sphere GetSphere(){return Sphere(center,len);}
  
  //比較演算子
  bool operator < (const Enemy &e) const{
    //sort するために比較演算子の定義(centerを基準にsort)
    return center.x!=e.center.x ? center.x < e.center.x : 
    (center.y != e.center.y ? center.y < e.center.y : center.z < e.center.z);
  }


  bool operator == (const Enemy &e) const{
    return fabs(center.x-e.center.x) < EPS && fabs(center.y-e.center.y) < EPS && fabs(center.z-e.center.z) < EPS;
  }

  //show
  void Show(){
    cout << "=== Enemy Statas" << endl;
    cout << "== Center" << endl;
    center.Show();
    // cout << "==Triangle" << endl;
    // tri.a.Show(); tri.b.Show(); tri.c.Show();
    // // cout << "==Sphere" << endl;
    // // cout << "c: "; outsph.c.Show();cout << "r: " << outsph.r << endl;
    // cout << "==Box" << endl;
    // cout << "lt: "; outbox.lt.Show();cout << "rb: ";outbox.rb.Show();
    // cout << "=================" << endl;
  }

  //set
  void SetEnemy(Triangle &t){
    tri = t;
    outbox = ToBoundingBox(tri);
    center = (outbox.lt + outbox.rb) * 0.5;
    len = abs(outbox.lt - center) + EPS;
    area = abs(cross((t.b-t.a),(t.c-t.a)));
    //outsph = ToSphereSimple(tri);
  }

private:
  //三角形を囲む箱を作成
  Box ToBoundingBox(Triangle &t){
    Point lt = Point(min({t.a.x,t.b.x,t.c.x}),
                      min({t.a.y,t.b.y,t.c.y}),
                      min({t.a.z,t.b.z,t.c.z}));
    Point rb = Point(max({t.a.x,t.b.x,t.c.x}),
                      max({t.a.y,t.b.y,t.c.y}),
                      max({t.a.z,t.b.z,t.c.z}));
    return Box(lt,rb);
  }
};// End Enemy class
