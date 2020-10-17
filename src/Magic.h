/*
  Magic Class

*/
#pragma once

class Magic{
protected:
  Sphere  sph; //結界の基本ベクト
  Box     outbox; //Boundig box
  Box     inbox;  //Inside box
  double margin = 50;//敵のサイズの最大値
public:
  //Get
  Sphere GetSphere(){return sph;};
  Box    GetOutsideBox(){return outbox;}
  Box    GetInsideBox(){return inbox;}
  
  //Show
  void Show(){
      cout << "== Magic Statas" << endl;
      cout << "= Sphere" << endl;
      cout << "c: "; sph.c.Show();cout << "r: " << sph.r << endl;
      cout << "= OutSideBox" << endl;
      cout << "lt: "; outbox.lt.Show();cout << "rb: ";outbox.rb.Show();
      cout << "= InSideBox" << endl;
      cout << "lt: "; inbox.lt.Show();cout << "rb: ";inbox.rb.Show();
      cout << "=================" << endl;
  }
  
  //set
  void SetMagic(Sphere &s){
    sph = s;
    outbox = ToBoundingBox(sph);
    inbox  = ToInsideBox(sph);
  }

private:
  //球の周囲を囲むボックスを作成
  Box ToBoundingBox(Sphere &sph){
    Point lt, rb;
    lt = sph.c - Point(sph.r + margin, sph.r + margin, sph.r + margin);
    rb = sph.c + Point(sph.r + margin, sph.r + margin, sph.r + margin);
    return Box(lt, rb);
  }
  //球の内側のボックスを作成
  Box ToInsideBox(Sphere &sph){
    Point lt,rb;
    double r = sph.r * ROOT2 * 0.5;
    lt = sph.c - Point(r - margin,r - margin,r - margin);
    rb = sph.c + Point(r - margin,r - margin,r - margin);
    return Box(lt,rb);
  } 

}; //End class Magic