#pragma once
#include <iostream>
#include <cmath>
#include <memory>
#include <vector>
#include "api/dll_iges.h"
#include "api/all_api_entities.h"
#include "sisl_tool.h"

namespace iges 
{
	using namespace std;
	struct Point3
	{
		double x;
		double y;
		double z;

		Point3(double _x, double _y, double _z)
			: x(_x), y(_y), z(_z)	
		{}

		friend istream& operator>>(istream& in, Point3& p) {
			in >> p.x >> p.y >> p.z;
			return in;
		}
	};


	class IGESNurbs
	{
	public:
		//unique_ptr<Nurbs>	surf;

		DLL_IGES 								model;				// 用于存储
		//unique_ptr < DLL_IGES_ENTITY_314 >		color;
		unique_ptr < DLL_IGES_ENTITY_128 >		surface;
		unique_ptr < DLL_IGES_ENTITY_102 >		space_bound;
		unique_ptr < DLL_IGES_ENTITY_102 >		param_bound;
		unique_ptr < DLL_IGES_ENTITY_142 >		surface_bound;
		unique_ptr < DLL_IGES_ENTITY_144 >		trimmed_surface;

		IGESNurbs();
		void set_color(double, double, double);

		//设置曲面在两个参数方向的参数范围 u0 to u1, v0 to v1
		void set_param_bound(double u0, double u1, double v0, double v1);

		void set_surface(Nurbs&);
		void set_surface(int degree_u, int degree_v, int cpt_u, int cpt_v, 
			vector<double>& _knot_u, vector<double>& _knot_v, vector<double>& _ctr_pnts);

		void set_space_bound();


		void write(const char*);
		void write(const string&);

	private:
		struct ParameterLimit {
			//曲面在两个参数方向的参数范围 u0 - u1, v0 - v1
			double u0, u1, v0, v1;

			explicit ParameterLimit() : u0(0), u1(0), v0(0), v1(0)
			{}

			void set(double _u0, double _u1, double _v0, double _v1) {
				u0 = _u0;
				u1 = _u1;
				v0 = _v0;
				v1 = _v1;
			}

			bool ifValid() {
				if (0.0 <= u0 && u0 <= 1.0 && 0.0 <= u1 && u1 <= 1.0 &&
					0.0 <= v0 && v0 <= 1.0 && 0.0 <= v1 && v1 <= 1.0 &&
					u0 != u1 && v0 != v1) {
					return true;
				}
				else {
					return false;
				}
			}
		}param_limit;

	};

}