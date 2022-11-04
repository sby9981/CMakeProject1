#include "write_iges.h"
#include <fstream>
#include <vector>
#include <string>


//可执行程序在 "CMakeProject1/out/build/x64-release"文件夹下
#define ProjectDir	"../../../"						// 主CMakeLists所在的文件夹位置
#define BSurfaceDir "../../../data/B_surface/"		// B样条曲面信息
#define IGESDir		"../../../data/IGES/"			//暂时存放的iges文件
#define TempDir		"../../../data/temp/"
#define TXTDir		"../../../data/txt/"

using namespace std;


int main()
{
	//读入数据，构建节点向量和控制点的vector
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

	//构造B样条曲面，此面为直纹面
	iges::BSplineSurface b_surface =
		iges::BSplineSurface(3, 3, 35, 11, knots_u, knots_v, control_pts);

	//沿参数曲线u=0.5将曲面切割
	//iges::BSplineSurface sub_surf1, sub_surf2;
	//iges::subdivide_along_param_line(b_surface, 1, 0.5, sub_surf1, sub_surf2);


	//将单个曲面写入txt
	//iges::write_single_surface(b_surface, string(TempDir) + string("b_surface.txt"));
	//iges::read_single_surface(b_surface, string(TempDir) + string("b_surface.txt"));

	//从txt读取多个曲面,并写入igs
	//vector<iges::BSplineSurface> surfaces;
	//int n = iges::read_surfaces(surfaces, string(TXTDir) + string("2_b_surface.txt.txt"));
	//cout << n;
	////iges::write_surfaces(surfaces, string(TempDir) + string("CAM01.txt"));  //将多个曲面写入txt
	//iges::write_surfaces_iges(surfaces, string(TempDir) + string("2_b_surface.IGS")); //写igs

	//将直纹面切割，并写为igs
	//iges::write_ruled_surface_iges(b_surface, 1, 0.5, string(IGESDir) + string("temp.IGS"));


	cout << "OK" << endl;
}