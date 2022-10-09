#pragma once
#include <cmath>
#include <memory>
#include <vector>
//#include <cstdlib>

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

		BSplineSurface(BSplineSurface& b)
			: degree_u(b.degree_u), degree_v(b.degree_v), u_num(b.u_num), 
			v_num(b.u_num), knots_u(b.knots_u), knots_v(b.knots_v), ctr_pnts(b.ctr_pnts)
		{}

		BSplineSurface(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts)
			: degree_u(degree_u), degree_v(degree_v), u_num(u_num), v_num(v_num),
			knots_u(knots_u), knots_v(knots_v), ctr_pnts(ctr_pnts)
			// �˴�vectorΪ���
		{}

		BSplineSurface(SISLSurf*);

		void reset(SISLSurf*);   //build BSplineSurface from SISL surface

		void reset(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts);

		BSplineSurface& operator=(const BSplineSurface& other);


		SISLSurf* toSISLSurf();

	};

	// Subdivide a surface along a given internal parameter line
	void subdivide_along_param_line(BSplineSurface& surf, int param_direction,
		double param_val, BSplineSurface& new_surf1, BSplineSurface& new_surf2);

	// Make a constant parameter curve along a given parameter direction in a surface.
	void pick_constant_param_curve(BSplineSurface& surf, int param_direction,
		double param_val, BSplineCurve& curve);

}