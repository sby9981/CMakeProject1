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

		//设置曲面在两个参数方向的参数范围 u0 to u1, v0 to v1
		//必须先进行这一步再设置曲面
		void set_param_bound(double u0, double u1, double v0, double v1);

		//同时更新surface和model
		void set_surface(BSplineSurface&);
		void set_surface(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts);

		//在已有surface的情况下更新model
		void upgrade_surface();

		void set_space_bound(); //选取曲面的4条边界曲线

		void set_color(double, double, double);

		// the last step before out to iges
		// use the compound curve to define a surface boundary in model space
		// create the Trimmed Parametric Surface (TPS)
		void create();

		void write(const char*);
		void write(const string&);

	private:
		// libIGES中的各种类型，用于转存IGS文件
		//DLL_IGES 								_model;
		shared_ptr < DLL_IGES >					_model;
		unique_ptr < DLL_IGES_ENTITY_128 >		_surf;
		unique_ptr < DLL_IGES_ENTITY_102 >		_space_bound;
		unique_ptr < DLL_IGES_ENTITY_102 >		_param_bound;
		unique_ptr < DLL_IGES_ENTITY_142 >		_surface_bound;
		unique_ptr < DLL_IGES_ENTITY_144 >		_trimmed_surface;

		struct M_ParameterLimit {
			//曲面在两个参数方向的参数范围 u0 - u1, v0 - v1
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

	//将多个面写入IGES文件，不能为闭曲面
	void write_surfaces_iges(vector<BSplineSurface>&, const char*);
	void write_surfaces_iges(vector<BSplineSurface>&, const string&);

	//将直纹面写入IGES文件
	void write_ruled_surface_iges(BSplineSurface&, int, double, const string&);

	//将直纹面写为IGES文件，按sw格式带有边界
	void write_sw_ruled_surface_iges(BSplineSurface&, int, double, const string&);

}