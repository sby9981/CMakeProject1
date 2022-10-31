#pragma once
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>

#include <api/dll_iges.h>
#include <api/all_api_entities.h>
#include "sisl_tool.h"

namespace iges
{
	using namespace std;
	using IGESModel = DLL_IGES;


	class SWNurbs
	{
	public:
		BSplineSurface surface;

		SWNurbs() = delete;
		SWNurbs(shared_ptr<DLL_IGES>);
		SWNurbs(shared_ptr<DLL_IGES>, BSplineSurface&);

		//����������������������Ĳ�����Χ u0 to u1, v0 to v1
		//�����Ƚ�����һ������������
		void set_param_bound(double u0, double u1, double v0, double v1);

		//ͬʱ����surface��model
		void set_surface(BSplineSurface&);
		void set_surface(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts);

		//������surface������¸���model
		void upgrade_surface();

		void set_space_bound(); //ѡȡ�����4���߽�����

		void set_color(double, double, double);

		// the last step before out to iges
		// use the compound curve to define a surface boundary in model space
		// create the Trimmed Parametric Surface (TPS)
		void create();

		void write(const char*);
		void write(const string&);

	private:
		// libIGES�еĸ������ͣ�����ת��IGS�ļ�
		//DLL_IGES 								_model;
		shared_ptr < DLL_IGES >					_model;
		unique_ptr < DLL_IGES_ENTITY_128 >		_surf;
		unique_ptr < DLL_IGES_ENTITY_102 >		_space_bound;
		unique_ptr < DLL_IGES_ENTITY_102 >		_param_bound;
		unique_ptr < DLL_IGES_ENTITY_142 >		_surface_bound;
		unique_ptr < DLL_IGES_ENTITY_144 >		_trimmed_surface;

		struct M_ParameterLimit {
			//������������������Ĳ�����Χ u0 - u1, v0 - v1
			double u0, u1, v0, v1;

			explicit M_ParameterLimit() : u0(0), u1(0), v0(0), v1(0) {}
			void set(double u0, double u1, double v0, double v1);
			bool ifValid();
		}_param_limit;

		struct M_Color {
			double r, g, b;

			explicit M_Color() :r(0), g(0), b(0) {}
			void set(double r, double g, double b);
		}_color;

		// use the compound curve to define a surface boundary in model space 
		void _define_surf_boundary();

		// create the Trimmed Parametric Surface (TPS)
		void _create_TPS();
	};

	void write_iges(shared_ptr<IGESModel>, const char*);
	void write_iges(shared_ptr<IGESModel>, const string&);

	//�������д��IGES�ļ�������Ϊ������
	void write_surfaces_iges(vector<BSplineSurface>&, const char*);
	void write_surfaces_iges(vector<BSplineSurface>&, const string&);

	//��ֱ����д��IGES�ļ�
	void write_ruled_surface_iges(BSplineSurface&, int, double, const string&);

	//��ֱ����дΪIGES�ļ�����sw��ʽ���б߽�
	void write_sw_ruled_surface_iges(BSplineSurface&, int, double, const string&);

}