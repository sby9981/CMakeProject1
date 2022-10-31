#pragma once
#include <cmath>
#include <memory>
#include <vector>
//#include <cstdlib>

#include <string>
#include <sisl.h>
#include <fstream>

/* SISL是一个nurbs库
*  sisl_tool封装了其中部分功能
*/

namespace iges
{
	using namespace std;

	struct BSplineCurve
	{
		int degree;				// 次数
		int num;				// 控制点数量
		vector<double> knots;	// 节点向量
		vector<double> ctr_pnts;// 控制点，对每个v，u方向控制点连续排序

		BSplineCurve() : degree(0), num(0)
		{}

		BSplineCurve(int degree, int num, vector<double> knots, vector<double> ctr_pnts)
			: degree(degree), num(num), knots(knots), ctr_pnts(ctr_pnts)
		{}

		BSplineCurve(SISLCurve*);
		void reset(SISLCurve*);
		SISLCurve* toSISLCurve();
	};


	struct BSplineSurface
	{
		// 存储nurbs曲面的基本信息
		int degree_u;			// 次数 u_p
		int degree_v;			// v_p
		int u_num;				// u方向控制点数量
		int v_num;

		struct M_ParameterLimit {
			//曲面在两个参数方向的参数范围 u0 - u1, v0 - v1
			double u0, u1, v0, v1;

			explicit M_ParameterLimit() : u0(0), u1(1), v0(0), v1(1) {}
			void set(double u0, double u1, double v0, double v1);
			M_ParameterLimit& operator=(const M_ParameterLimit& other);
			bool ifValid();
		}param_limit;

		vector<double> knots_u;	// u方向节点向量，必须为递增顺序
		//knots_u.size() = degree_u + 1 + u_num
		vector<double> knots_v;
		vector<double> ctr_pnts;// 控制点，对每个v，u方向控制点连续排序


		BSplineSurface() : degree_u(0), degree_v(0), knots_u(0), u_num(0), v_num(0)
		{}

		BSplineSurface(BSplineSurface& b)
			: degree_u(b.degree_u), degree_v(b.degree_v), u_num(b.u_num), 
			param_limit(b.param_limit), v_num(b.v_num), knots_u(b.knots_u), 
			knots_v(b.knots_v), ctr_pnts(b.ctr_pnts)
		{}

		BSplineSurface(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts)
			: degree_u(degree_u), degree_v(degree_v), u_num(u_num), v_num(v_num),
			knots_u(knots_u), knots_v(knots_v), ctr_pnts(ctr_pnts)
			// 此处vector为深拷贝
		{}

		BSplineSurface(SISLSurf*);

		~BSplineSurface() {};

		void reset(SISLSurf*);   //build BSplineSurface from SISL surface

		void reset(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts);

		BSplineSurface& operator=(const BSplineSurface& other);


		SISLSurf* toSISLSurf();

		void out(ofstream&);
		void in(ifstream&);
	};

	// 读写 BSplineSurface 到 txt
	void write_single_surface(BSplineSurface&, const char*);
	void write_single_surface(BSplineSurface&, const string);
	void read_single_surface(BSplineSurface&, const char*);
	void read_single_surface(BSplineSurface&, const string);
	//读写 多个曲面到一个txt
	int read_surfaces(vector<BSplineSurface>&, const char*);
	int read_surfaces(vector<BSplineSurface>&, const string);
	void write_surfaces(vector<BSplineSurface>&, const char*);
	void write_surfaces(vector<BSplineSurface>&, const string);


	// Subdivide a surface along a given internal parameter line
	void subdivide_along_param_line(BSplineSurface& surf, int param_direction,
		double param_val, BSplineSurface& new_surf1, BSplineSurface& new_surf2);

	// Make a constant parameter curve along a given parameter direction in a surface.
	void pick_constant_param_curve(BSplineSurface& surf, int param_direction,
		double param_val, BSplineCurve& curve);

}