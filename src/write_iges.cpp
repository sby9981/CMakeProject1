#include "write_iges.h"

namespace iges
{
	using namespace std;


	IGESNurbs::IGESNurbs(shared_ptr<DLL_IGES> model)
	{
		_model = model;
		_surf = make_unique<DLL_IGES_ENTITY_128>(*_model, true);
		_space_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_param_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_surface_bound = make_unique<DLL_IGES_ENTITY_142>(*_model, true);
		_trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(*_model, true);
	}

	IGESNurbs::IGESNurbs(shared_ptr<DLL_IGES> model, BSplineSurface& b)
	{
		_model = model;
		_surf = make_unique<DLL_IGES_ENTITY_128>(*_model, true);
		_space_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_param_bound = make_unique<DLL_IGES_ENTITY_102>(*_model, true);
		_surface_bound = make_unique<DLL_IGES_ENTITY_142>(*_model, true);
		_trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(*_model, true);

		surface = b;
	}

	void IGESNurbs::set_color(double r, double g, double b)
	{
		_color.set(r, g, b);
	}

	void IGESNurbs::set_param_bound(double u0, double u1, double v0, double v1)
	{
		//��������4��1��B������ֱ�ߣ����� degree=1 order=2, �������Ƶ�
		// u0, u1, v0, v1 Ϊ������Χ������0-1��Χ��
		//(u0, v0, 0) - > (u1, v0, 0) - > (u1, v1, 0) - > (u0, v1, 0) - > (u0, v0, 0)
		_param_limit.set(u0, u1, v0, v1);
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

	void IGESNurbs::set_surface(BSplineSurface& b)
	{
		surface = b;

		// DLL_IGES_ENTITY_128�������Ϊ���� order=degree + 1
		// u0, u1, v0, v1 Ϊ������Χ������0-1��Χ��
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

	void IGESNurbs::upgrade_surface()
	{
		// �������е�surface����model
		_surf->SetNURBSData(
			surface.u_num, surface.v_num, surface.degree_u + 1,
			surface.degree_v + 1, surface.knots_u.data(), surface.knots_v.data(),
			surface.ctr_pnts.data(), false, false, false,
			_param_limit.u0, _param_limit.u1, _param_limit.v0, _param_limit.v1
		);
	}

	void IGESNurbs::set_space_bound()
	{
		BSplineCurve curve;
		int param_direction[] = { 1, 2, 1, 2 };
		double param_val[] = { _param_limit.u0, _param_limit.v1,
			_param_limit.u1, _param_limit.v0 };
		double start = 0., end = 1.; //������ֹ�Ĳ���λ��

		for (int i = 0; i < 4; i++) {
			pick_constant_param_curve(surface, param_direction[i], param_val[i], curve);
			DLL_IGES_ENTITY_126 boundary(*_model, true);
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
		DLL_IGES_ENTITY_314 color(*_model, true);
		color.SetColor(_color.r, _color.g, _color.b);
		_trimmed_surface->SetSurface(*_surf);
		_trimmed_surface->SetBoundCurve(*_surface_bound);
		_trimmed_surface->SetColor(color);
	}

	void IGESNurbs::create()
	{
		_define_surf_boundary();
		_create_TPS();
	}

	//write model to iges file
	void IGESNurbs::write(const char* filename)
	{
		create();
		_model->Write(filename, true);
	}

	void IGESNurbs::write(const string& filename)
	{
		create();
		_model->Write(filename.c_str(), true);
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

	void write_iges(shared_ptr<DLL_IGES> model, const char* filename)
	{
		model->Write(filename, true);
	}
	void write_iges(shared_ptr<DLL_IGES> model, const string& filename)
	{
		model->Write(filename.c_str(), true);
	}

	/*
	* ��ֱ����дΪIGES�ļ�
	* IGES���ܽ���һ��Ͱ�棬��Ҫ��ֱ�����ȷ�Ϊ��������д��
	* direction : ��������
	*			=1 ��������u���򹹳ɱջ���v����Ϊĸ�߷���
	*			=2 ��������v���򹹳ɱջ���u����Ϊĸ�߷���
	*/
	void write_ruled_surface_iges(BSplineSurface& surf, 
		int direction, const string& filename)
	{
		if (direction != 1 && direction != 2) {
			cerr << "Wrong param direction!" << endl;
			return;
		}

		//�ز���0.5������ָ�
		double param_val = 0.5;

		//�������ڴ洢iges��model
		auto iges_model = make_shared<iges::IGESModel>();

		//��������������
		IGESNurbs sub_surf1(iges_model), sub_surf2(iges_model);

		if (direction == 1) {
			sub_surf1.set_param_bound(0., param_val, 0., 1.);
			sub_surf2.set_param_bound(param_val, 1., 0., 1.);
		}
		else {
			sub_surf1.set_param_bound(0., 1., 0., param_val);
			sub_surf2.set_param_bound(0., 1., param_val, 1.);
		}

		//�ز���u=0.5�������Ϊ����������������������
		subdivide_along_param_line(surf, direction,
			param_val, sub_surf1.surface, sub_surf2.surface);
		
		sub_surf1.upgrade_surface();
		sub_surf2.upgrade_surface();

		sub_surf1.set_color(0, 80, 0);
		sub_surf2.set_color(80, 0, 0);

		sub_surf1.set_space_bound();
		sub_surf2.set_space_bound();
		
		sub_surf1.create();
		sub_surf2.create();

		write_iges(iges_model, filename);
	}
}
