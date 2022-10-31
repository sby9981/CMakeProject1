#include "write_iges.h"
#include <fstream>
#include <vector>
#include <string>


//可执行程序在 "CMakeProject1/out/build/x64-release"文件夹下
#define ProjectDir	"../../../"						// 主CMakeLists所在的文件夹位置
#define BSurfaceDir "../../../data/B_surface/"		// B样条曲面信息
#define IGESDir		"../../../data/IGES/"			//暂时存放的iges文件
#define TempDir		"../../../data/temp/"


using namespace std;


int main()
{
	// in knot vector
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
	vector<double> knots_v = 
	{ 0, 0, 0, 0, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1, 1, 1 };

	vector<double> control_pts;
	ifstream cpt_file;
	cpt_file.open(string(BSurfaceDir) + string("SurfCnt.txt"), std::ios::in);
	while (!cpt_file.eof()) {
		cpt_file >> temp_read;
		control_pts.push_back(temp_read * 100.);
	}
	cpt_file.close();

	iges::BSplineSurface b_surface =
		iges::BSplineSurface(3, 3, 35, 11, knots_u, knots_v, control_pts);

	//iges::write_single_surface(b_surface, string(TempDir) + string("b_surface.txt"));
	//iges::read_single_surface(b_surface, string(TempDir) + string("b_surface.txt"));

	//vector<iges::BSplineSurface> surfaces;
	//iges::read_surfaces(surfaces, string(TempDir) + string("123.txt"));
	//iges::write_surfaces(surfaces, string(TempDir) + string("1234.txt"));
	//iges::write_surfaces_iges(surfaces, string(TempDir) + string("1234.IGS"));

	//auto iges_model = make_shared<iges::IGESModel>();
	//iges_model->Read((string(IGESDir) + string("temp.IGS")).c_str());
	//IGES* ip = iges_model->GetRawPtr();
	
	//DLL_IGES_ENTITY_128 nurbs(*iges_model, true);
	//nurbs.SetNURBSData(b_surface.u_num, b_surface.v_num, b_surface.degree_u + 1,
	//	b_surface.degree_v + 1, b_surface.knots_u.data(), b_surface.knots_v.data(),
	//	b_surface.ctr_pnts.data(), false, false, false, 0, 1, 0, 1);
	//iges_model->Write((string(IGESDir) + string("4.IGS")).c_str(), true);

	//iges::write_ruled_surface_iges(b_surface, 1, 0.5, string(IGESDir) + string("temp.IGS"));


	cout << "OK" << endl;
}