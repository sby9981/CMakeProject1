# CMakeProject1

一个简单的B样条曲面的读写和转IGS文件接口

封装[libIGES](https://github.com/cbernardo/libIGES)和[SISL](https://github.com/SINTEF-Geometry/SISL)两个库

所有函数接口都在namespace iges 下

## 配置

先编译出两个库的静态库，根据CMakeLists中指定的路径放到对应位置

## B样条曲面

创建B样条曲面，类`iges::BSplineSurface`定义在文件sisl_tool.h中

~~~cpp
iges::BSplineSurface b_surface = iges::BSplineSurface(int degree_u, int degree_v, int u_num, int v_num, vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts)
~~~

使用txt存储B样条曲面信息，txt格式如下

~~~python
2		#第一行为曲面个数		
3 3 19 11  #第二行依次为u方向次数，v方向次数，u方向次数控制点数，v控制点数
0.000000000000000  #u方向的节点向量，一行一个值
0.000000000000000
0.000000000000000
0.000000000000000
0.116676000000000
0.199456400000000
...
0.985818000000000
1.000000000000000
1.000000000000000
1.000000000000000
1.000000000000000 #u方向的节点向量长度=u次数+控制点数+1
0.000000000000000 #v方向的节点向量
0.000000000000000
0.000000000000000
0.000000000000000
0.200000000000000
...
1.000000000000000
1.000000000000000
1.000000000000000
100.000000000000000 0.000000000000000 0.000000000000000 #控制点
96.153199999999998 0.987942000000000 0.000000000000000
...
0.257961046501000 1.122749389292000 200.000000000000000
0.089572459030000 0.671206339635000 200.000000000000000
3 3 20 11  #第二个曲面
0.000000000000000
0.000000000000000
...
100.000000000000000 0.000000000000000 200.000000000000000

~~~

控制点线性存储在一维`vector<double> ctr_pnts  `中。顺序应先遍历u再遍历v，同一点的xyz坐标相邻排列。假设控制点的三维坐标存储再二维list `P[u][v]`中，控制点顺序如下

~~~python
for v in v_knots:
	for u in u_knots:
        ctr_pnts.append(P[u][v].x)
        ctr_pnts.append(P[u][v].y)
        ctr_pnts.append(P[u][v].z)
~~~

### 读写txt接口

~~~cpp
//返回曲面个数，读入的曲面存储在第一个参数中，第二个参数为文件名
int read_surfaces(vector<BSplineSurface>&, const char*);
int read_surfaces(vector<BSplineSurface>&, const string);
//将第一个参数中的曲面写入文件
void write_surfaces(vector<BSplineSurface>&, const char*);
void write_surfaces(vector<BSplineSurface>&, const string);
~~~

### 直纹面

由于直纹面成桶状，需要切割成两个面才能进行许多操作

~~~cpp
//根据一条参数曲线将曲面切割成两部分，存储在两个子曲面new_surf中
void subdivide_along_param_line(BSplineSurface& surf, int param_direction, double param_val, BSplineSurface& new_surf1, BSplineSurface& new_surf2);
~~~

其中`param_direction`为1或2，表示沿u或v方向的参数曲线，如：

~~~cpp
//沿u=0.5将曲面surf切割，存入new_surf1和new_surf2
void subdivide_along_param_line(surf, 1, 0.5, new_surf1, new_surf2);
~~~

需要txt的格式为

~~~python
0 1 0.5   # 0表示要切割， 1 0.5 表示沿u=0.5切
3 3 35 11 # 同
~~~







## IGES

与txt的方法相对应，可将多个曲面写入iges:

~~~cpp
//将多个面写入IGES文件，不能为闭曲面，返回是否成功
bool write_surfaces_iges(vector<BSplineSurface>&, const char*);
bool write_surfaces_iges(vector<BSplineSurface>&, const string&);
~~~

写将直纹面切割，并写入IGES:

~~~cpp
bool write_ruled_surface_iges(BSplineSurface&, int, double, const string&);
~~~

SolidWorks需要确定每个曲面的参数边界和几何边界。针对这一情况，提供了类`iges::SWNurbs`，并提供按sw格式处理直纹面的函数：

~~~cpp
//将直纹面写为IGES文件，按sw格式带有边界
bool write_sw_ruled_surface_iges(BSplineSurface&, int, double, const string&);
~~~

