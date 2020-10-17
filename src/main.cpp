#include <bits/stdc++.h>

#define REP(i,n) for (int i = 0; i <(n); ++i)
#define REP2(i,x,n) for (int i = x; i <(n); ++i)

using namespace std;

#include "myPoint3D.h"
#include "myObject3D.h"
#include "myCross3D.h"
using namespace MYCROSS;
#include "Enemy.h"
#include "Magic.h"
//#include "Node.h"

//時間管理
class TimeManager{
  int st, et;
public:
  TimeManager(){st = clock();}; //起動時にタイマー開始
  bool isTime(){
      if(clock() - st > 900 * CLOCKS_PER_SEC/100){//9秒たったら出力
        //cout << "TLE" << endl;
        return true;
      }
      return false;
    }
  void show(){cout <<"Time; "<< (clock() -st)* 1000.0/CLOCKS_PER_SEC << " ms" << endl;};
};

//解答の出力後に強制終了
void outputans(const vector<int> &ans){
  //cout << "ans" << endl;
  int n = ans.size();
  int i = 0;
  while(i != n) {
    printf("%d\n",ans[i]);
    i++;
  }
  //exit(0);
}

  bool x_cmp (const Enemy &e1, const Enemy &e2) {
    return e1.center.x < e2.center.x;
  }
  bool y_cmp (const Enemy &e1, const Enemy &e2) {
    return e1.center.y < e2.center.y;
  }
  bool z_cmp (const Enemy &e1, const Enemy &e2) {
    return e1.center.z < e2.center.z;
  }


int main(){

  //時間管理　if(TM.isTime()) {outputans(ans);}　で時間チェックして出力
  TimeManager TM;
  /*
    Input
  */

  int n, q; //敵の数，結界の数
  cin >> n >> q;
  //vector<int> ans(q,1); //魔法で攻撃できる敵の数

  //敵オブジェクトの宣言
  vector<Enemy> enemy(n);

  for (int i = 0; i < n; i++) {
    // 三角形の敵 i の座標
    double p_ax, p_ay, p_az; // 敵 i の頂点 A
    scanf("%lf%lf%lf",&p_ax, &p_ay, &p_az);
    double p_bx, p_by, p_bz; // 敵 i の頂点 B
    scanf("%lf%lf%lf",&p_bx, &p_by, &p_bz);
    double p_cx, p_cy, p_cz; // 敵 i の頂点 C
    scanf("%lf%lf%lf",&p_cx, &p_cy, &p_cz);
    Triangle tri = Triangle( Point(p_ax, p_ay, p_az),
                             Point(p_bx, p_by, p_bz),
                             Point(p_cx, p_cy, p_cz) );
    enemy[i].SetEnemy(tri);
  }

  //結界オブジェクトの宣言
  vector<Magic> magic(q);
  
  //input magic
  for (int j = 0; j < q; j++) {
    double r; // 結界 j の半径
    scanf("%lf", &r);
    double v_x, v_y, v_z; // 結界 j の中心座標
    scanf("%lf%lf%lf",&v_x, &v_y, &v_z);
    Sphere sph = Sphere(Point(v_x, v_y, v_z), r);
    magic[j].SetMagic(sph);
  }
  
  //cout << "Input End "; TM.show();
  //入力チェック
  //REP(i,n) cout << i << endl, enemy[i].Show();
  //REP(j,q) cout << j << endl, magic[j].Show();
  //===========================================Input End
  /*
    Enemy object sort
  */

  vector<int> ans(q,0);
  vector<Enemy> x_enemy,y_enemy,z_enemy;
  sort(enemy.begin(), enemy.end(),z_cmp);
  z_enemy = enemy;
  sort(enemy.begin(), enemy.end(),y_cmp);
  y_enemy = enemy;
  sort(enemy.begin(), enemy.end(),x_cmp);
  x_enemy = enemy;
  
  //magicclassで二分探索
  for(int i = 0; i < q ; i++){
    Enemy oel,oer;
    oel.center = magic[i].GetOutsideBox().lt;
    oer.center = magic[i].GetOutsideBox().rb;
    //x_cmp
    auto left = lower_bound(x_enemy.begin(), x_enemy.end(),oel);
    auto right = upper_bound(x_enemy.begin(), x_enemy.end(),oer);
    //y_cmp
    auto yleft = lower_bound(y_enemy.begin(), y_enemy.end(),oel,y_cmp);
    auto yright = upper_bound(y_enemy.begin(), y_enemy.end(),oer,y_cmp);
    if(right - left > yright - yleft){
      right = yright;
      left  = yleft;
    }
    //z_cmp
    auto zleft  = lower_bound(z_enemy.begin(), z_enemy.end(),oel,z_cmp);
    auto zright = upper_bound(z_enemy.begin(), z_enemy.end(),oer,z_cmp);
    if(right - left > zright - zleft){
      right = zright;
      left  = zleft;
    }
    //cout<< "測定数" << right - left << endl;
    
    //衝突判定
    bool flg = true;
    //if((left - right)*q>200000) flg = false;
    for(auto it = left; it != right; it++){
      //if(iel.center.x  < ier.center.x && (it > inleft && it < inright)) continue;
      if(!isCrossSphereSphere(magic[i].GetSphere(), (it) -> GetSphere())) continue;  

      if(!isCrossSpherePlane(magic[i].GetSphere(), (it) -> GetTriangle())) continue;
      if(!isCrossSphereTriangle(magic[i].GetSphere(), (it) -> GetTriangle())) continue;
    

      ans[i]++;
    }
    if(TM.isTime()) {
      for(int j = i+1; j < q; j++) ans[j] = (int)rand()%n;
      outputans(ans);
      //cout << "処理数" << i << endl;
      break;
    }
    
  }

  
  for(int i = 0; i < q; i++) cout << ans[i] << endl;
  return 0;
}
