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
		// �洢nurbs����Ļ�����Ϣ
		int u_p;				//����
		int v_p;
		int u_num;				// u������Ƶ�����
		int v_num;
		vector<double> knots_u;	// u����ڵ�����
		vector<double> knots_v;
		vector<double> ctr_pnts;// ���Ƶ㣬��ÿ��v��u������Ƶ���������

		Nurbs() : u_p(0), v_p(0), knots_u(0) {}

		Nurbs(int degree_u, int degree_v, int cpt_u, int cpt_v,
			vector<double>& _knot_u, vector<double>& _knot_v,
			vector<double>& _ctr_pnts)
			: u_p(degree_u), v_p(degree_v), u_num(cpt_u), v_num(cpt_v),
			knots_u(_knot_u), knots_v(_knot_v), ctr_pnts(_ctr_pnts)
		{}

	};
}