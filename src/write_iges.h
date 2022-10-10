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
	//struct Point3
	//{
	//	double x;
	//	double y;
	//	double z;

	//	Point3(double _x, double _y, double _z)
	//		: x(_x), y(_y), z(_z)
	//	{}

	//	friend istream& operator>>(istream& in, Point3& p) {
	//		in >> p.x >> p.y >> p.z;
	//		return in;
	//	}
	//};


	class IGESNurbs
	{
	public:
		BSplineSurface surface;

		IGESNurbs();
		IGESNurbs(BSplineSurface&);

		void set_color(double, double, double);

		//����������������������Ĳ�����Χ u0 to u1, v0 to v1
		void set_param_bound(double u0, double u1, double v0, double v1);

		void set_surface(BSplineSurface&);
		void set_surface(int degree_u, int degree_v, int u_num, int v_num,
			vector<double>& knots_u, vector<double>& knots_v, vector<double>& ctr_pnts);

		void set_space_bound(); //ѡȡ�����4���߽�����


		void write(const char*);
		void write(const string&);

	private:
		// libIGES�еĸ������ͣ�����ת��IGS�ļ�
		DLL_IGES 								_model;
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

		// create the Trimmed Parametric Surface (TPS
		void _create_TPS();
	};

}