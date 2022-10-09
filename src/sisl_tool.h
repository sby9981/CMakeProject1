#pragma once
#include <cmath>
#include <memory>
#include <vector>

#include <sisl.h>
/* SISL��һ��nurbs��
*  sisl_tool��װ�����в��ֹ���
*/

namespace iges
{
	using namespace std;

	struct BSplineCurve
	{
		int degree;				// ����
		int num;				// ���Ƶ�����
		vector<double> knots;	// �ڵ�����
		vector<double> ctr_pnts;// ���Ƶ㣬��ÿ��v��u������Ƶ���������

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
		// �洢nurbs����Ļ�����Ϣ
		int degree_u;			// ���� u_p
		int degree_v;			// v_p
		int u_num;				// u������Ƶ�����
		int v_num;
		vector<double> knots_u;	// u����ڵ�����������Ϊ����˳��
		vector<double> knots_v;
		vector<double> ctr_pnts;// ���Ƶ㣬��ÿ��v��u������Ƶ���������

		BSplineSurface() : degree_u(0), degree_v(0), knots_u(0), u_num(0), v_num(0)
		{}

		BSplineSurface(int degree_u, int degree_v, int cpt_u, int cpt_v,
			vector<double>& knot_u, vector<double>& knot_v,
			vector<double>& ctr_pnts)
			: degree_u(degree_u), degree_v(degree_v), u_num(cpt_u), v_num(cpt_v),
			knots_u(knot_u), knots_v(knot_v), ctr_pnts(ctr_pnts)
			// �˴�vectorΪ���
		{}

		BSplineSurface(SISLSurf*);
		void reset(SISLSurf*);
		SISLSurf* toSISLSurf();
	};

	// Subdivide a surface along a given internal parameter line
	void subdivide_along_param_line(BSplineSurface& surf, int param_direction,
		double param_val, BSplineSurface& new_surf1, BSplineSurface& new_surf2);

	// Make a constant parameter curve along a given parameter direction in a surface.
	void pick_constant_param_curve(BSplineSurface& surf, int param_direction,
		double param_val, BSplineCurve& curve);

}