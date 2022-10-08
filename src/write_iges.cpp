#include "write_iges.h"

namespace iges
{
	using namespace std;

	void CurveLoop::push_back(std::shared_ptr<IGES_CURVE> curve)
	{
		loop.push_back(curve);
	}

	bool CurveLoop::check_loop()
	{
		//检查曲线是否构成一个环
		return true;
	}

	IGESNurbs::IGESNurbs()
	{
		//color			= make_unique<DLL_IGES_ENTITY_314>(DLL_IGES_ENTITY_314(model, true));
		surface			= make_unique<DLL_IGES_ENTITY_128>(DLL_IGES_ENTITY_128(model, true));
		//space_bound		= make_unique<DLL_IGES_ENTITY_102>(DLL_IGES_ENTITY_102(model, true));
		//param_bound		= make_unique<DLL_IGES_ENTITY_102>(DLL_IGES_ENTITY_102(model, true));
		//surface_bound	= make_unique<DLL_IGES_ENTITY_142>(DLL_IGES_ENTITY_142(model, true));
		//trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(DLL_IGES_ENTITY_144(model, true));

		//color->SetColor(0., 80., 0.);
		set_param_bound(0, 1, 0, 1);
	}

	void IGESNurbs::set_color(double r, double g, double b)
	{
		//range is 0.0 .. 100.0
		//color->SetColor(r, g, b);
	}

	void IGESNurbs::set_param_bound(double u0, double u1, double v0, double v1)
	{
		param_limit = { u0, u1, v0, v1 };
	}

	void IGESNurbs::set_surface(Nurbs& b, double u0, double u1, double v0, double v1)
	{
		surface->SetNURBSData(b.u_num, b.v_num, b.u_p + 1, b.v_p + 1, b.knots_u.data(),
			b.knots_v.data(), b.ctr_pnts.data(), false, false, false, u0, u1, v0, v1);
	}

	void IGESNurbs::set_surface(int degree_u, int degree_v, int cpt_u, int cpt_v, 
		vector<double>& _knot_u, vector<double>& _knot_v, 
		vector<double>& _ctr_pnts, double u0, double u1, double v0, double v1)
	{
		// DLL_IGES_ENTITY_128输入参数为阶数 order=degree + 1
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		surface->SetNURBSData(cpt_u, cpt_v, degree_u + 1, degree_v + 1, 
			_knot_u.data(), _knot_u.data(), _ctr_pnts.data(),
			false, false, false, u0, u1, v0, v1);
	}

	//void IGESNurbs::set_param_bound(CurveLoop& param_loop)
	//{
	//	for (auto line : param_loop.loop) {
	//		param_bound->AddSegment(move(line).get());
	//	}
	//}

	//void IGESNurbs::set_space_bound(CurveLoop& space_loop)
	//{
	//	for (auto curve : space_loop.loop) {
	//		space_bound->AddSegment(move(curve).get());
	//	}
	//}



	//CurveLoop IGESNurbs::square_param_bound(
	//	double u0, double u1, double v0, double v1)
	//{
	//	//参数域，由4条1次B样条（直线）构成
	//	// u0, u1, v0, v1 为参数范围，都在0-1范围内
	//	//(u0, v0, 0) - > (u1, v0, 0) - > (u1, v1, 0) - > (u0, v1, 0) - > (u0, v0, 0) 
	//	shared_ptr<IGES_CURVE> line 
	//		= make_shared<IGES_CURVE>(DLL_IGES_ENTITY_126(model, true));
	//	double line_knots[] = { 0.,0.,1.,1. };
	//	double line_cpts1[] = { u0, v0, 0., u1, v0, 0. };

	//	CurveLoop bound;
	//	return bound;

	//}

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
