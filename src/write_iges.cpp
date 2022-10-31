#include "write_iges.h"

namespace iges
{
	using namespace std;


	SWNurbs::SWNurbs(shared_ptr<DLL_IGES> model)
	{
		_model = model;
		_surf = make_unique<DLL_IGES_ENTITY_128>(*_model, true);
		_space_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_param_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_surface_bound = make_unique<DLL_IGES_ENTITY_142>(*_model, true);
		_trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(*_model, true);
	}

	SWNurbs::SWNurbs(shared_ptr<DLL_IGES> model, BSplineSurface& b)
	{
		_model = model;
		_surf = make_unique<DLL_IGES_ENTITY_128>(*_model, true);
		_space_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_param_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_surface_bound = make_unique<DLL_IGES_ENTITY_142>(*_model, true);
		_trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(*_model, true);

		surface = b;
	}

	void SWNurbs::set_color(double r, double g, double b)
	{
		_color.set(r, g, b);
	}

	void SWNurbs::set_param_bound(double u0, double u1, double v0, double v1)
	{
		//参数域，由4条1次B样条（直线）构成 degree=1 order=2, 两个控制点
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		//(u0, v0, 0) - > (u1, v0, 0) - > (u1, v1, 0) - > (u0, v1, 0) - > (u0, v0, 0)
		surface.param_limit.set(u0, u1, v0, v1);
		double line_knots[] = { 0.,0.,1.,1. };
		double line_cpts[4][6] = { { u0, v0, 0., u1, v0, 0. },
								   { u1, v0, 0., u1, v1, 0. },
								   { u1, v1, 0., u0, v1, 0. },
								   { u0, v1, 0., u0, v0, 0. } };

		for (int i = 0; i < 4; i++) {
			DLL_IGES_ENTITY_126 line(*_model, true);
			line.SetNURBSData(2, 2, line_knots, line_cpts[i], false, 0., 1.);
			_param_bound->AddSegment(line);
		}
	}

	void SWNurbs::set_surface(BSplineSurface& b)
	{
		surface = b;

		// DLL_IGES_ENTITY_128输入参数为阶数 order=degree + 1
		// u0, u1, v0, v1 为参数范围，都在0-1范围内
		if (!surface.param_limit.ifValid()) {
			cerr << "Wrong parameter feild! Use set_param_bound first!" << endl;
		}
		// for IGES
		_surf->SetNURBSData(b.u_num, b.v_num, b.degree_u + 1, b.degree_v + 1,
			b.knots_u.data(), b.knots_v.data(), b.ctr_pnts.data(), false, false, false,
			surface.param_limit.u0, surface.param_limit.u1, 
			surface.param_limit.v0, surface.param_limit.v1);
	}

	void SWNurbs::set_surface(int degree_u, int degree_v, int u_num, int v_num,
		vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts)
	{
		surface.reset(degree_u, degree_v, u_num, v_num, knots_u, knots_v, ctr_pnts);

		if (!surface.param_limit.ifValid()) {
			cerr << "Wrong parameter feild! Use set_param_bound first!" << endl;
		}
		_surf->SetNURBSData(u_num, v_num, degree_u + 1, degree_v + 1,
			knots_u.data(), knots_v.data(), ctr_pnts.data(), false, false, false,
			surface.param_limit.u0, surface.param_limit.u1, 
			surface.param_limit.v0, surface.param_limit.v1);
	}

	void SWNurbs::upgrade_surface()
	{
		// 根据现有的surface更新model
		_surf->SetNURBSData(
			surface.u_num, surface.v_num, surface.degree_u + 1,
			surface.degree_v + 1, surface.knots_u.data(), surface.knots_v.data(),
			surface.ctr_pnts.data(), false, false, false,
			surface.param_limit.u0, surface.param_limit.u1, 
			surface.param_limit.v0, surface.param_limit.v1
		);
	}

	void SWNurbs::set_space_bound()
	{
		BSplineCurve curve;
		int param_direction[] = { 1, 2, 1, 2 };
		double param_val[] = { surface.param_limit.u0, surface.param_limit.v1,
			surface.param_limit.u1, surface.param_limit.v0 };
		double start = 0., end = 1.; //曲线起止的参数位置

		for (int i = 0; i < 4; i++) {
			pick_constant_param_curve(surface, param_direction[i], param_val[i], curve);
			DLL_IGES_ENTITY_126 boundary(*_model, true);
			if (param_direction[i] == 1) {
				boundary.SetNURBSData(curve.num, curve.degree + 1, curve.knots.data(),
					curve.ctr_pnts.data(), false, 
					surface.param_limit.v0, surface.param_limit.v1);
			}
			else {
				boundary.SetNURBSData(curve.num, curve.degree + 1, curve.knots.data(),
					curve.ctr_pnts.data(), false, 
					surface.param_limit.u0, surface.param_limit.u1);
			}
			_space_bound->AddSegment(boundary);
		}
	}

	void SWNurbs::_define_surf_boundary()
	{
		_surface_bound->SetCurveCreationFlag(CURVE_CREATE_PROJECTION);
		_surface_bound->SetSurface(*_surf);
		_surface_bound->SetParameterSpaceBound(*_param_bound);
		_surface_bound->SetModelSpaceBound(*_space_bound);
		_surface_bound->SetCurvePreference(BOUND_PREF_PARAMSPACE);
	}

	void SWNurbs::_create_TPS()
	{
		DLL_IGES_ENTITY_314 color(*_model, true);
		color.SetColor(_color.r, _color.g, _color.b);
		_trimmed_surface->SetSurface(*_surf);
		_trimmed_surface->SetBoundCurve(*_surface_bound);
		_trimmed_surface->SetColor(color);
	}

	void SWNurbs::create()
	{
		_define_surf_boundary();
		_create_TPS();
	}

	//out model to iges file
	void SWNurbs::write(const char* filename)
	{
		create();
		_model->Write(filename, true);
	}

	void SWNurbs::write(const string& filename)
	{
		create();
		_model->Write(filename.c_str(), true);
	}

	void SWNurbs::M_Color::set(double r, double g, double b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	
	bool write_iges(shared_ptr<DLL_IGES> model, const char* filename)
	{
		return model->Write(filename, true);
	}
	bool write_iges(shared_ptr<DLL_IGES> model, const string& filename)
	{
		return model->Write(filename.c_str(), true);
	}

	bool write_surfaces_iges(vector<BSplineSurface>& surfaces, const char* filename)
	{
		auto model = make_shared<IGESModel>();
		for (int i = 0; i < surfaces.size(); i++) {
			DLL_IGES_ENTITY_128 sub_surf(*model, true);
			sub_surf.SetNURBSData(surfaces[i].u_num, surfaces[i].v_num,
				surfaces[i].degree_u + 1, surfaces[i].degree_v + 1,
				surfaces[i].knots_u.data(), surfaces[i].knots_v.data(),
				surfaces[i].ctr_pnts.data(), false, false, false,
				surfaces[i].param_limit.u0, surfaces[i].param_limit.u1,
				surfaces[i].param_limit.v0, surfaces[i].param_limit.v1);
		}

		return write_iges(model, filename);
	}

	bool write_surfaces_iges(vector<BSplineSurface>& surfaces, const string& filename)
	{
		return write_surfaces_iges(surfaces, filename.c_str());
	}

	/*
	* 将直纹面写为IGES文件
	* IGES不能接受一个桶面，需要将直纹面先分为两个面再写入
	* 无边界
	* direction : 参数方向
	* 		 =1 ：曲面在u方向构成闭环，v方向为母线方向
	*		 =2 ：曲面在v方向构成闭环，u方向为母线方向
	* param_val:沿参数param_val将曲面分割
	*/
	bool write_ruled_surface_iges(BSplineSurface& surf,
		int direction, double param_val, const string& filename)
	{
		if (direction != 1 && direction != 2) {
			cerr << "Wrong param direction!" << endl;
			return false;
		}

		//创建用于存储iges的model
		auto iges_model = make_shared<IGESModel>();

		//创建两个子曲面
		DLL_IGES_ENTITY_128 sub_surf1(*iges_model, true);
		DLL_IGES_ENTITY_128 sub_surf2(*iges_model, true);

		BSplineSurface nurbs1, nurbs2;

		//沿参数=param_val将曲面分为两个，并存入两个子曲面
		subdivide_along_param_line(surf, direction, param_val, nurbs1, nurbs2);

		sub_surf1.SetNURBSData(nurbs1.u_num, nurbs1.v_num, nurbs1.degree_u + 1,
			nurbs1.degree_v + 1, nurbs1.knots_u.data(), nurbs1.knots_v.data(),
			nurbs1.ctr_pnts.data(), false, false, false, nurbs1.param_limit.u0,
			nurbs1.param_limit.u1, nurbs1.param_limit.v0, nurbs1.param_limit.v1);

		sub_surf2.SetNURBSData(nurbs2.u_num, nurbs2.v_num, nurbs2.degree_u + 1,
			nurbs2.degree_v + 1, nurbs2.knots_u.data(), nurbs2.knots_v.data(),
			nurbs2.ctr_pnts.data(), false, false, false, nurbs2.param_limit.u0,
			nurbs2.param_limit.u1, nurbs2.param_limit.v0, nurbs2.param_limit.v1);

		return write_iges(iges_model, filename);
	}

	/*
	* 将直纹面写为IGES文件
	* IGES不能接受一个桶面，需要将直纹面先分为两个面再写入
	* 按SolidWorks所能接受的IGS格式，带有边界
	* direction : 参数方向
	*			=1 ：曲面在u方向构成闭环，v方向为母线方向
	*			=2 ：曲面在v方向构成闭环，u方向为母线方向
	* param_val:沿参数param_val将曲面分割
	*/
	bool write_sw_ruled_surface_iges(BSplineSurface& surf,
		int direction,double param_val, const string& filename)
	{
		if (direction != 1 && direction != 2) {
			cerr << "Wrong param direction!" << endl;
			return false;
		}

		//创建用于存储iges的model
		auto iges_model = make_shared<IGESModel>();

		//创建两个子曲面
		SWNurbs sub_surf1(iges_model), sub_surf2(iges_model);

		if (direction == 1) {
			sub_surf1.set_param_bound(0., param_val, 0., 1.);
			sub_surf2.set_param_bound(param_val, 1., 0., 1.);
		}
		else {
			sub_surf1.set_param_bound(0., 1., 0., param_val);
			sub_surf2.set_param_bound(0., 1., param_val, 1.);
		}

		//沿参数=param_val将曲面分为两个，并存入两个子曲面
		subdivide_along_param_line(surf, direction,
			param_val, sub_surf1.surface, sub_surf2.surface);

		//更新两个子面的信息
		sub_surf1.upgrade_surface();
		sub_surf2.upgrade_surface();

		sub_surf1.set_color(0, 80, 0);
		sub_surf2.set_color(80, 0, 0);

		sub_surf1.set_space_bound();
		sub_surf2.set_space_bound();

		sub_surf1.create();
		sub_surf2.create();

		return write_iges(iges_model, filename);
	}
}
