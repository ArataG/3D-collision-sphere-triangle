# 3D-collision-sphere-triangle
3次元空間上における球と三角形の交差判定アルゴリズムです．
プログラミングコンテストで利用しました．

# 問題設定
球状の魔法オブジェクトと交差する敵オブジェクトの数を求める．
- n: 敵オブジェクトの数
- q: 魔法オブジェクトの数
- p_ax, p_ay, p_az, p_bx, p_by, p_bz, p_cx, p_cy, p_cz
:敵オブジェクト△abcの座標
- r v_x v_y v_z: 魔法オブジェクトの半径rと中心座標v_x v_y v_z

が与えられる．
## 入力
入力は以下の形式で標準入力から与えられる．

```
n q
p_ax p_ay p_az p_bx p_by p_bz p_cx p_cy p_cz
:
:
r v_x v_y v_z
:
:
```
## 制約
- 1 < n, q < 200000

## 出力
各魔法オブジェクト(球)と接している敵オブジェクト(三角形)の数を出力せよ．
```
a1
:
:
aq
```


# ソースコードの概要
|ファイル名|説明|
|:---|:---|
|myPoint3D.h  |3次元座標で定義された点，直線，線分のクラス |
|myObject3D.h |3次元座標で定義されたオブジェクト(球，平面，ボックス,三角形)のクラス|
|myCross3D.h |3次元座標で定義されたオブジェクト(球，平面，ボックス,三角形)の交差判定用クラス|
|Enemy.h | 三角形オブジェクトを継承した敵クラス|
|Magic.h | 球オブジェクトを継承した魔法クラス|
|main.cpp|入出力，時間管理(TMManager)|

## 三角形と球の衝突判定のアルゴリズム
1. 球の中心から角形を含む平面に下ろした垂線の距離が，球の半径を超えている場合，非交差
2. 球の中心から三角形の距離の最短距離の候補のすべてが球の半径よりも短い場合，非交差(内包)，球内，球外の双方に存在する場合交差と判定
    - 三角形の3頂点
    - 三角形の3頂点で構成される3つの線分
3. 2ですべての候補が球の外部と判定された場合，球の中心から3角形を含む平面に下ろした垂線の足が，三角形の内部に存在する場合，交差と判定

### 条件1 -> 球と平面の交差判定
平面は3D座標の3点を用いて表現します．
球の中心から平面に下ろした垂線の長さが，球の半径より短い場合交差と判定

```cpp
//4. 球と平面の交差判定
bool isCrossSpherePlane(Sphere s, Plane pla){
  double S = abs(cross((pla.p2-pla.p1),(pla.p3-pla.p1)));
  double V = abs( dot((pla.p1-s.c), cross((pla.p2-s.c), (pla.p3-s.c))) );
  double dist =V/S; //球の中心から，平面に下ろした垂線の長さ
  if(dist <= s.r) return 1;
  return 0; 
}
```

### 条件2 -> 球と線分の交差判定

 - 線分の端点 p1 または p2 が球に接している場合
 - または，p1, p2 のいずれかが球の内部にあり，他方が外部にある場合
 - p1,p2が球の外部にあり，線分の最短距離が球の内部にある場合．
 

 ```cpp
//3. 球と線分の交差判定

bool isCrossSphereSegment(Sphere sph, Segment seg){
  bool isIn = false;
  if(getDistance(sph.c,seg.p1) <= sph.r) isIn = true;
  if((getDistance(sph.c,seg.p2) <= sph.r) != isIn) return 1;
  if(isIn) return 0; // 非交差(線分の両端が球の内部に存在)
  //線分の両端が，球の外部に存在．
  if(dot(seg.p2 - seg.p1, sph.c - seg.p1) < 0) return 0; //線分の最短距離はseg.p2
  if(dot(seg.p1 - seg.p2, sph.c - seg.p2) < 0) return 0; //線分の最短距離はseg.p1
  //線分の最短距離は，線分上に存在
  if(isCrossSphereLine(sph, seg)) return 1;
  return 0;
}
 ```


### 条件3

まず，ある直線から三角形の３つの頂点からなる平面に下ろした垂線の足が，三角形の内部にあるかの判定は，下記のコードで行うことができます．

```cpp
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
```

球と三角形の交差判定は次のようになります．

```cpp
//5. 球オブジェクトと三角形の交差判定
//三角形の各点，各辺，三角形平面のいずれかが，球の内外に少なくとも一つ存在すれば交差している．
bool isCrossSphereTriangle(Sphere sph, Triangle tri){
  
  //5-1三角形を構成する平面が球と交差しない場合，非交差
  if(!isCrossSpherePlane(sph, Plane(tri.a,tri.b,tri.c))) return 0;
  
 //5-2. 球の中心と三角形の各頂点の距離を比較
  bool isIn = false;
  double r2 = sph.r * sph.r;
  double distpp1 = norm(sph.c - tri.a) - r2;
  if(fabs(distpp1) <=  EPS ) return 1;  //接触:distpp1 == 0, 誤差を考慮してEPS
  if (distpp1 < 0) isIn = true;

  double distpp2 = norm(sph.c-tri.b) - r2;
  if(fabs(distpp2) <=  EPS ) return 1;
  if(isIn != distpp2 < 0) return 1;

  double distpp3 = norm(sph.c - tri.c) - r2;
  if(fabs(distpp3) <=  EPS ) return 1;
  if(isIn != distpp3 < 0) return 1;

  //5-3. 球の中心のと三角形の各辺の距離を比較
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

  //5-4球と三角形内部平面の接触判定(全て球の外側に属していた場合のみ実行)
  if(isIn == false){
    if(isContainTriangle(sph.c, tri)) return 1;
  }
  return 0;
}
```


* 球と三角形の交差判定についての補足
  * 入力される3点が一直線上になる可能性がある場合は，例外処理が必要です．

## installing
git cloneしてください．

## Usage
```
$ g++ main.cpp
$ ./a.out < ../in/00_sample1.in
```

## Reference
1. [○×つくろーどっとコム 衝突判定編](http://marupeke296.com/COL_main.html)
2. [Atsushi's Homepage 衝突判定のアルゴリズム](https://www.antun.net/tips/algorithm/collision.html)
3. プログラミングコンテスト攻略のためのアルゴリズムとデータ構造, 株式会社マイナビ出版,渡辺有隆

- 衝突判定のアルゴリズムに関しては資料1,2を参考にしました．
- また，３次元空間座標系を扱うために，資料3-第16章「計算幾何学」で解説されている２次元座標空間をあつかうライブラリを拡張して使っています．

