#include "write_iges.h"

namespace iges
{
	using namespace std;


	IGESNurbs::IGESNurbs()
	{
		//color			= make_unique<DLL_IGES_ENTITY_314>(model, true);
		surface = make_unique<DLL_IGES_ENTITY_128>(model, true);
		//space_bound		= make_unique<DLL_IGES_ENTITY_102>(model, true);
		param_bound = make_unique<DLL_IGES_ENTITY_102>(model, true);
		//surface_bound	= make_unique<DLL_IGES_ENTITY_142>(model, true);
		//trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(model, true);

		//color->SetColor(0., 80., 0.);
	}

	void IGESNurbs::set_color(double r, double g, double b)
	{
		//range is 0.0 .. 100.0
		//color->SetColor(r, g, b);
	}

	void IGESNurbs::set_param_bound(double u0, double u1, double v0, double v1)
	{
		//参数域，由4条1次B样条（直线）构成 degree=1 order=2, 两个控制点
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		//(u0, v0, 0) - > (u1, v0, 0) - > (u1, v1, 0) - > (u0, v1, 0) - > (u0, v0, 0)
		param_limit.set(u0, u1, v0, v1);
		double line_knots[] = { 0.,0.,1.,1. };
		double line_cpts[4][6] = { { u0, v0, 0., u1, v0, 0. },
								   { u1, v0, 0., u1, v1, 0. },
								   { u1, v1, 0., u0, v1, 0. },
								   { u0, v1, 0., u0, v0, 0. } };

		for (int i = 0; i < 4; i++) {
			DLL_IGES_ENTITY_126 line(model, true);
			line.SetNURBSData(2, 2, line_knots, line_cpts[i], false, 0., 1.);
			param_bound->AddSegment(line);
		}
	}

	void IGESNurbs::set_surface(BSplineSurface& b)
	{
		// DLL_IGES_ENTITY_128输入参数为阶数 order=degree + 1
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		if (!param_limit.ifValid()) {
			cerr << "Wrong parameter feild! Use set_param_bound first!" << endl;
		}
		surface->SetNURBSData(b.u_num, b.v_num, b.degree_u + 1, b.degree_v + 1, b.knots_u.data(),
			b.knots_v.data(), b.ctr_pnts.data(), false, false, false, 
			param_limit.u0, param_limit.u1, param_limit.v0, param_limit.v1);
	}

	void IGESNurbs::set_surface(int degree_u, int degree_v, int cpt_u, int cpt_v,
		vector<double>& _knot_u, vector<double>& _knot_v, vector<double>& _ctr_pnts)
	{
		if (!param_limit.ifValid()) {
			cerr << "Wrong parameter feild! Use set_param_bound first!" << endl;
		}
		surface->SetNURBSData(cpt_u, cpt_v, degree_u + 1, degree_v + 1,
			_knot_u.data(), _knot_u.data(), _ctr_pnts.data(), false, false, false,
			param_limit.u0, param_limit.u1, param_limit.v0, param_limit.v1);
	}


	//write model to iges file
	void IGESNurbs::write(const char* filename)
	{
		model.Write(filename, true);
	}

	void IGESNurbs::write(const string& filename)
	{
		model.Write(filename.c_str(), true);
	}
}
