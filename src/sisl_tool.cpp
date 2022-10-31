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

	void BSplineSurface::M_ParameterLimit::
		set(double u0, double u1, double v0, double v1)
	{
		this->u0 = u0;
		this->u1 = u1;
		this->v0 = v0;
		this->v1 = v1;
	}

	BSplineSurface::M_ParameterLimit& BSplineSurface::M_ParameterLimit::
		operator=(const BSplineSurface::M_ParameterLimit& other)
	{
		this->u0 = other.u0;
		this->u1 = other.u1;
		this->v0 = other.v0;
		this->v1 = other.v1;
		return *this;
	}

	bool BSplineSurface::M_ParameterLimit::ifValid()
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

	void BSplineSurface::out(ofstream& out)
	{
		out << degree_u << ' ' << degree_v << ' '
			<< u_num << ' ' << v_num << "\n";
		for (auto u : knots_u) {
			out << u << '\n';
		}

		for (auto v : knots_v) {
			out << v << '\n';
		}

		for (int i = 0; i < u_num * v_num; i++) {
			for (int j = 0; j < 3; j++) {
				out << ctr_pnts[3 * i + j] << ' ';
			}
			out << '\n';
		}
	}

	void BSplineSurface::in(ifstream& in)
	{
		int u_knots_size, v_knots_size;
		in >> degree_u >> degree_v >> u_num >> v_num;

		u_knots_size = degree_u + u_num + 1;
		v_knots_size = degree_v + v_num + 1;

		//double temp_read;
		knots_u.resize(u_knots_size);
		for (int i = 0; i < u_knots_size; i++) {
			in >> knots_u[i];
		}

		knots_v.resize(v_knots_size);
		for (int i = 0; i < v_knots_size; i++) {
			in >> knots_v[i];
		}

		ctr_pnts.resize(3 * u_num * v_num);
		for (int i = 0; i < 3 * u_num * v_num; i++) {
			in >> ctr_pnts[i];
		}
	}

	/*
	* 将BSplineSurface的信息按顺序写入txt
	* 第一行：曲面个数 1
	* 第二行：空行
	* 第三行：u,v方向的次数、控制点数、节点向量长度，空格分隔
	*		u节点向量, 一行一个值
	*		v节点向量, 一行一个值
	*		控制点位置，每行为一个点的xyz
	*/
	void write_single_surface(BSplineSurface& b_surface, const char* filename)
	{
		ofstream out;
		out.open(filename, ios::out);
		out << 1 << "\n";
		out.precision(15);
		out.setf(ios::fixed);
		b_surface.out(out);
		out.close();
	}

	void write_single_surface(BSplineSurface& b_surface, const string filename)
	{
		write_single_surface( b_surface, filename.c_str());
	}

	void read_single_surface(BSplineSurface& b_surface, const char* filename)
	{
		ifstream in;
		in.open(filename, ios::in);
		int n;
		in >> n;
		b_surface.in(in);
		in.close();

	}

	void read_single_surface(BSplineSurface& b_surface, const string filename)
	{
		read_single_surface(b_surface, filename.c_str());
	}

	void read_surfaces(vector<iges::BSplineSurface>& surfaces, const char* filename)
	{
		ifstream in;
		in.open(filename, ios::in);

		if (!in.is_open()) {
			return;
		}

		int n;
		in >> n; //曲面数量

		for (int i = 0; i < n; i++) {
			BSplineSurface new_surf;
			new_surf.in(in);
			surfaces.emplace_back(new_surf);
		}
		in.close();
	}

	void read_surfaces(vector<BSplineSurface>& surfaces, const string filename)
	{
		read_surfaces(surfaces, filename.c_str());
	}

	void write_surfaces(vector<BSplineSurface>& surfaces, const char* filename)
	{
		ofstream out;
		out.open(filename, ios::out);
		out << surfaces.size() << "\n";

		out.precision(15);
		out.setf(ios::fixed);
		for (int i = 0; i < surfaces.size(); i++) {
			surfaces[i].out(out);
		}
		out.close();
	}

	void write_surfaces(vector<BSplineSurface>& surfaces, const string filename)
	{
		write_surfaces(surfaces, filename.c_str());
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
