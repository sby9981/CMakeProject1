#include "write_iges.h"
#include <fstream>
#include <vector>
#include <string>


//可执行程序在 "CMakeProject1/out/build/x64-release"文件夹下
#define ProjectDir	"../../../"						// 主CMakeLists所在的文件夹位置
#define BSurfaceDir "../../../data/B_surface/"		// B样条曲面信息
#define IGESDir		"../../../data/IGES/"			//暂时存放的iges文件


using namespace std;
int main()
{
	// read knot vector
	vector<double> knots_u;
	ifstream knot_file;
	// 这里交换了一下u和v，为了和控制点顺序对应
	knot_file.open(string(BSurfaceDir) + string("SurfKnotV.txt"), std::ios::in);
	double temp_read;
	while (!knot_file.eof()) {
		knot_file >> temp_read;
		knots_u.push_back(temp_read);
	}
	knot_file.close();
	vector<double> knots_v = { 0, 0, 0, 0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1, 1, 1 };
	ofstream out_file;

	// read control points
	vector<double> control_pts;
	ifstream cpt_file;
	cpt_file.open(string(BSurfaceDir) + string("SurfCnt.txt"), std::ios::in);
	while (!cpt_file.eof()) {
		cpt_file >> temp_read;
		control_pts.push_back(temp_read);
	}
	cpt_file.close();

	iges::Nurbs b = iges::Nurbs(3, 3, 35, 11, knots_u, knots_v, control_pts);
	iges::IGESNurbs model = iges::IGESNurbs();
	model.set_param_bound(0, 1, 0, 1);
	model.set_surface(b);
	

	model.write(string(IGESDir) + string("1.IGS"));

	vector<double> a(4);
	double* aa = new double[4];
	*aa = 0;
	*(aa + 1) = 1.1;
	*(aa + 2) = 1.2;
	*(aa + 3) = 1.3;
	double aaa[] = { 1, 1.1, 1.2, 1.3 };
	copy(aa, aa + 4, a.begin());




	cout << "OK" << endl;
}