#pragma once
#include <iostream>
#include <cmath>
#include "api/dll_iges.h"
#include "api/all_api_entities.h"
#include <memory>
#include <vector>

namespace iges 
{
	using namespace std;
	struct Point3
	{
		double x;
		double y;
		double z;

		Point3(double _x, double _y, double _z)
			: x(_x), y(_y), z(_z)	
		{}

		friend istream& operator>>(istream& in, Point3& p) {
			in >> p.x >> p.y >> p.z;
			return in;
		}
	};

	struct Nurbs
	{
		// 存储nurbs曲面的基本信息
		int u_p;				//次数
		int v_p;
		int u_num;				// u方向控制点数量
		int v_num;				
		vector<double> knots_u;	// u方向节点向量
		vector<double> knots_v;
		vector<double> ctr_pnts;// 控制点，对每个v，u方向控制点连续排序

		Nurbs() : u_p(0), v_p(0), knots_u(0) {}

		Nurbs(int degree_u, int degree_v, int cpt_u, int cpt_v, 
			vector<double>& _knot_u, vector<double>& _knot_v, 
			vector<double>& _ctr_pnts)
			: u_p(degree_u), v_p(degree_v), u_num(cpt_u), v_num(cpt_v), 
			knots_u(_knot_u), knots_v(_knot_v), ctr_pnts(_ctr_pnts)
		{}
	};

	struct CurveLoop
	{
		//一个由曲线组成的环
		vector< shared_ptr<IGES_CURVE> > loop;
		void push_back(shared_ptr<IGES_CURVE>);
		bool check_loop();
	};


	class IGESNurbs
	{
	public:
		//unique_ptr<Nurbs>	surf;

		DLL_IGES 								model;				// 用于存储
		unique_ptr < DLL_IGES_ENTITY_314 >		color;
		unique_ptr < DLL_IGES_ENTITY_128 >		surface;
		//CurveLoop								space_loop;
		//CurveLoop								param_loop;
		unique_ptr < DLL_IGES_ENTITY_102 >		space_bound;
		unique_ptr < DLL_IGES_ENTITY_102 >		param_bound;
		unique_ptr < DLL_IGES_ENTITY_142 >		surface_bound;
		unique_ptr < DLL_IGES_ENTITY_144 >		trimmed_surface;

		IGESNurbs();
		//~IGESNurbs() {
		//	model.Clear();
		//}
		void set_color(double, double, double);

		//设置曲面在两个参数方向的参数范围 u0 to u1, v0 to v1
		void set_param_bound(double u0, double u1, double v0, double v1); 

		void set_surface(Nurbs&, double u0, double u1, double v0, double v1);
		void set_surface(int degree_u, int degree_v, int cpt_u, int cpt_v, 
			vector<double>& _knot_u, vector<double>& _knot_v,
			vector<double>& _ctr_pnts, double u0, double u1, double v0, double v1);

		void set_param_bound(CurveLoop&);
		void set_space_bound(CurveLoop&);


		void write(const char*);
		void write(const string&);

	private:
		struct ParameterLimit {
			//曲面在两个参数方向的参数范围 u0 to u1, v0 to v1
			double u0, u1, v0, v1;
		}param_limit;

		//参数域，由4条1次B样条（直线）构成, 用于划分曲面范围
		CurveLoop square_param_bound(double u0, double u1, double v0, double v1);
	};

}