#include "write_iges.h"
#include <fstream>
#include <vector>
#include <string>


//��ִ�г����� "CMakeProject1/out/build/x64-release"�ļ�����
#define ProjectDir	"../../../"						// ��CMakeLists���ڵ��ļ���λ��
#define BSurfaceDir "../../../data/B_surface/"		// B����������Ϣ
#define IGESDir		"../../../data/IGES/"			//��ʱ��ŵ�iges�ļ�
#define TempDir		"../../../data/temp/"
#define TXTDir		"../../../data/txt/"

using namespace std;


int main()
{
	//�������ݣ������ڵ������Ϳ��Ƶ��vector
	// in knot vector
	vector<double> knots_u;
	ifstream knot_file;
	// ���ｻ����һ��u��v��Ϊ�˺Ϳ��Ƶ�˳���Ӧ
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

	//����B�������棬����Ϊֱ����
	iges::BSplineSurface b_surface =
		iges::BSplineSurface(3, 3, 35, 11, knots_u, knots_v, control_pts);

	//�ز�������u=0.5�������и�
	//iges::BSplineSurface sub_surf1, sub_surf2;
	//iges::subdivide_along_param_line(b_surface, 1, 0.5, sub_surf1, sub_surf2);


	//����������д��txt
	//iges::write_single_surface(b_surface, string(TempDir) + string("b_surface.txt"));
	//iges::read_single_surface(b_surface, string(TempDir) + string("b_surface.txt"));

	//��txt��ȡ�������,��д��igs
	//vector<iges::BSplineSurface> surfaces;
	//int n = iges::read_surfaces(surfaces, string(TXTDir) + string("2_b_surface.txt.txt"));
	//cout << n;
	////iges::write_surfaces(surfaces, string(TempDir) + string("CAM01.txt"));  //���������д��txt
	//iges::write_surfaces_iges(surfaces, string(TempDir) + string("2_b_surface.IGS")); //дigs

	//��ֱ�����и��дΪigs
	//iges::write_ruled_surface_iges(b_surface, 1, 0.5, string(IGESDir) + string("temp.IGS"));


	cout << "OK" << endl;
}