#pragma once
#include <cmath>
#include <memory>
#include <vector>
#include "sisl.h"

namespace iges
{
	using namespace std;
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
}