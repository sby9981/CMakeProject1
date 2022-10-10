#include "write_iges.h"

namespace iges
{
	using namespace std;


	IGESNurbs::IGESNurbs()
	{
		_surf = make_unique<DLL_IGES_ENTITY_128>(_model, true);
		_space_bound = make_unique<DLL_IGES_ENTITY_102>(_model, true);
		_param_bound = make_unique<DLL_IGES_ENTITY_102>(_model, true);
		_surface_bound = make_unique<DLL_IGES_ENTITY_142>(_model, true);
		_trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(_model, true);
	}

	IGESNurbs::IGESNurbs(BSplineSurface& b)
	{
		IGESNurbs();
		surface = b;
	}

	void IGESNurbs::set_color(double r, double g, double b)
	{
		_color.set(r, g, b);
	}

	void IGESNurbs::set_param_bound(double u0, double u1, double v0, double v1)
	{
		//参数域，由4条1次B样条（直线）构成 degree=1 order=2, 两个控制点
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		//(u0, v0, 0) - > (u1, v0, 0) - > (u1, v1, 0) - > (u0, v1, 0) - > (u0, v0, 0)
		_param_limit.set(u0, u1, v0, v1);
		double line_knots[] = { 0.,0.,1.,1. };
		double line_cpts[4][6] = { { u0, v0, 0., u1, v0, 0. },
								   { u1, v0, 0., u1, v1, 0. },
								   { u1, v1, 0., u0, v1, 0. },
								   { u0, v1, 0., u0, v0, 0. } };

		for (int i = 0; i < 4; i++) {
			DLL_IGES_ENTITY_126 line(_model, true);
			line.SetNURBSData(2, 2, line_knots, line_cpts[i], false, 0., 1.);
			_param_bound->AddSegment(line);
		}
	}

	void IGESNurbs::set_surface(BSplineSurface& b)
	{
		surface = b;

		// DLL_IGES_ENTITY_128输入参数为阶数 order=degree + 1
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		if (!_param_limit.ifValid()) {
			cerr << "Wrong parameter feild! Use set_param_bound first!" << endl;
		}
		// for IGES
		_surf->SetNURBSData(b.u_num, b.v_num, b.degree_u + 1, b.degree_v + 1,
			b.knots_u.data(), b.knots_v.data(), b.ctr_pnts.data(), false, false, false,
			_param_limit.u0, _param_limit.u1, _param_limit.v0, _param_limit.v1);
	}

	void IGESNurbs::set_surface(int degree_u, int degree_v, int u_num, int v_num,
		vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts)
	{
		surface.reset(degree_u, degree_v, u_num, v_num, knots_u, knots_v, ctr_pnts);

		if (!_param_limit.ifValid()) {
			cerr << "Wrong parameter feild! Use set_param_bound first!" << endl;
		}
		_surf->SetNURBSData(u_num, v_num, degree_u + 1, degree_v + 1,
			knots_u.data(), knots_v.data(), ctr_pnts.data(), false, false, false,
			_param_limit.u0, _param_limit.u1, _param_limit.v0, _param_limit.v1);
	}

	void IGESNurbs::set_space_bound()
	{
		BSplineCurve curve;
		int param_direction[] = { 1, 2, 1, 2 };
		double param_val[] = { _param_limit.u0, _param_limit.v1,
			_param_limit.u1, _param_limit.v0 };
		double start = 0., end = 1.; //曲线起止的参数位置

		for (int i = 0; i < 4; i++) {
			pick_constant_param_curve(surface, param_direction[i], param_val[i], curve);
			DLL_IGES_ENTITY_126 boundary(_model, true);
			if (param_direction[i] == 1) {
				boundary.SetNURBSData(curve.num, curve.degree + 1, curve.knots.data(),
					curve.ctr_pnts.data(), false, _param_limit.v0, _param_limit.v1);
			}
			else {
				boundary.SetNURBSData(curve.num, curve.degree + 1, curve.knots.data(),
					curve.ctr_pnts.data(), false, _param_limit.u0, _param_limit.u1);
			}
			_space_bound->AddSegment(boundary);
		}
	}

	void IGESNurbs::_define_surf_boundary()
	{
		_surface_bound->SetCurveCreationFlag(CURVE_CREATE_PROJECTION);
		_surface_bound->SetSurface(*_surf);
		_surface_bound->SetParameterSpaceBound(*_param_bound);
		_surface_bound->SetModelSpaceBound(*_space_bound);
		_surface_bound->SetCurvePreference(BOUND_PREF_PARAMSPACE);
	}

	void IGESNurbs::_create_TPS()
	{
		DLL_IGES_ENTITY_314 color(_model, true);
		color.SetColor(_color.r, _color.g, _color.b);
		_trimmed_surface->SetSurface(*_surf);
		_trimmed_surface->SetBoundCurve(*_surface_bound);
		_trimmed_surface->SetColor(color);
	}

	//write model to iges file
	void IGESNurbs::write(const char* filename)
	{
		_define_surf_boundary();
		_create_TPS();
		_model.Write(filename, true);
	}

	void IGESNurbs::write(const string& filename)
	{
		_define_surf_boundary();
		_create_TPS();
		_model.Write(filename.c_str(), true);
	}


	void IGESNurbs::M_ParameterLimit::set(double u0, double u1, double v0, double v1)
	{
		this->u0 = u0;
		this->u1 = u1;
		this->v0 = v0;
		this->v1 = v1;
	}

	bool IGESNurbs::M_ParameterLimit::ifValid()
	{
		if (0.0 <= u0 && u0 <= 1.0 && 0.0 <= u1 && u1 <= 1.0 &&
			0.0 <= v0 && v0 <= 1.0 && 0.0 <= v1 && v1 <= 1.0 &&
			u0 != u1 && v0 != v1) {
			return true;
		}
		else {
			return false;
		}
	}

	void IGESNurbs::M_Color::set(double r, double g, double b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
}
