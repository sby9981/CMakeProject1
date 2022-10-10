#include "sisl_tool.h"
#include <iostream>

namespace iges
{
	using namespace std;

	BSplineCurve::BSplineCurve(SISLCurve* s)
	{
		reset(s);
	}
	void BSplineCurve::reset(SISLCurve* s)
	{
		degree = s->ik - 1; // ik: Order of curve.
		num = s->in;

		knots.resize(s->ik + s->in);
		copy(s->et, s->et + knots.size(), knots.begin());

		ctr_pnts.resize(s->in * 3);
		copy(s->ecoef, s->ecoef + ctr_pnts.size(), ctr_pnts.begin());
	}

	SISLCurve* BSplineCurve::toSISLCurve()
	{
		return newCurve(num, degree + 1, knots.data(), ctr_pnts.data(), 1, 3, 1);
	}

	BSplineSurface::BSplineSurface(SISLSurf* s)
	{
		reset(s);
	}

	void BSplineSurface::reset(SISLSurf* s)
	{
		degree_u = s->ik1 - 1;	// ik1: Order of surface in first parameter direction.
		degree_v = s->ik2 - 1;
		u_num = s->in1;			// in1: Number of vertices in first parameter direction
		v_num = s->in2;

		knots_u.resize(s->ik1 + s->in1);
		knots_v.resize(s->ik2 + s->in2);
		copy(s->et1, s->et1 + knots_u.size(), knots_u.begin());
		copy(s->et2, s->et2 + knots_v.size(), knots_v.begin());

		ctr_pnts.resize(3 * s->in1 * s->in2);
		copy(s->ecoef, s->ecoef + ctr_pnts.size(), ctr_pnts.begin());
	}

	void BSplineSurface::reset(int degree_u, int degree_v, int u_num, int v_num,
		vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts)
	{
		this->degree_u = degree_u;
		this->degree_v = degree_v;
		this->u_num = u_num;
		this->v_num = v_num;

		this->knots_u = knots_u;
		this->knots_v = knots_v;
		this->ctr_pnts = ctr_pnts;
	}

	BSplineSurface& BSplineSurface::operator=(const BSplineSurface& other)
	{
		degree_u = other.degree_u;
		degree_v = other.degree_v;
		u_num = other.u_num;
		v_num = other.v_num;

		knots_u = other.knots_u;
		knots_v = other.knots_v;
		ctr_pnts = other.ctr_pnts;
		return *this;
	}

	SISLSurf* BSplineSurface::toSISLSurf()
	{
		return newSurf(u_num, v_num, degree_u + 1, degree_v + 1, knots_u.data(),
			knots_v.data(), ctr_pnts.data(), 1, 3, 1);
		//need freeSurf(SISLSurf*) later
	}

	/*	将曲面沿一条参数线分割
	*	surf: 待分割曲面
	*	param_direction: 分割的参数方向
	*					=1：沿u方向分割
	*					=2：沿v方向分割
	*	param_val：参数值
	*	new_surf1， new_sruf2: 得到的子曲面
	*/
	void subdivide_along_param_line(BSplineSurface& surf, int param_direction,
		double param_val, BSplineSurface& new_surf1, BSplineSurface& new_surf2)
	{
		SISLSurf* s0 = surf.toSISLSurf();
		SISLSurf* s1 = NULL, * s2 = NULL;
		int stat; // >0: warning, =0: ok, <0: error
		s1711(s0, param_direction, param_val, &s1, &s2, &stat); //function in SISL
		if (stat < 0) {
			cerr << "Faild Subdivide surface!" << endl;
			return;
		}
		new_surf1.reset(s1);
		new_surf2.reset(s2);
		freeSurf(s0);
		freeSurf(s1);
		freeSurf(s2);
	}

	/*	沿一固定参数值，从曲面上选取一条曲线
	*	param_direction: 参数方向
	*					=1：沿u方向分割
	*					=2：沿v方向分割
	*/
	void pick_constant_param_curve(BSplineSurface& surf, int param_direction,
		double param_val, BSplineCurve& curve)
	{
		SISLSurf* s = surf.toSISLSurf();
		SISLCurve* c = NULL;
		int stat; // >0: warning, =0: ok, <0: error
		s1439(s, param_val, param_direction, &c, &stat); // function in SISL
		curve.reset(c);
		freeCurve(c);
	}
}
