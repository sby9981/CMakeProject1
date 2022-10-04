#pragma once
#include <iostream>
#include <cmath>
#include "api/dll_iges.h"
#include "api/all_api_entities.h"
#include <memory>
#include <vector>

namespace cam 
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
		int u_p;				//阶数
		int v_p;
		int u_num;				// u方向控制点数量
		int v_num;				
		vector<double> knot_u;	// u方向节点向量
		vector<double> knot_v;
		vector<Point3> ctr_pnts;// 控制点，对每个v，u方向控制点连续排序

		Nurbs() : u_p(0), v_p(0), knot_u(0) {}

		Nurbs(int order_u, int order_v, int cpt_u, int cpt_v, 
			vector<double>& _knot_u, vector<double>& _knot_v, vector<Point3>& _ctr_pnts)
			: u_p(order_u), v_p(order_v), u_num(cpt_u), v_num(cpt_v), knot_u(_knot_u), knot_v(_knot_v), ctr_pnts(_ctr_pnts)
		{}
	};

	struct CurveLoop
	{
		std::vector< std::shared_ptr<IGES_CURVE> > loop;
		void push_back(std::shared_ptr<IGES_CURVE>);
		bool check_loop();
	};

	class IGESNurbs
	{
	public:
		unique_ptr<Nurbs>	surf;

		DLL_IGES 								model;				// 用于存储
		unique_ptr < DLL_IGES_ENTITY_314 >		color;
		unique_ptr < DLL_IGES_ENTITY_128 >		surface;
		CurveLoop								space_loop;
		CurveLoop								param_loop;
		unique_ptr < DLL_IGES_ENTITY_102 >		space_bound;
		unique_ptr < DLL_IGES_ENTITY_102 >		param_bound;
		unique_ptr < DLL_IGES_ENTITY_142 >		surface_bound;
		unique_ptr < DLL_IGES_ENTITY_144 >		trimmed_surface;

		IGESNurbs();
		void set_color(double, double, double);
		void set_param_bound(CurveLoop&);
		void set_space_bound(CurveLoop&);


		void write(const char*);

	};

}