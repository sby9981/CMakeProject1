#include "write_iges.h"
#include <fstream>
#include <vector>
#include <string>


//��ִ�г����� "CMakeProject1/out/build/x64-release"�ļ�����
#define ProjectDir	"../../../"						// ��CMakeLists���ڵ��ļ���λ��
#define BSurfaceDir "../../../data/B_surface/"		// B����������Ϣ
#define IGESDir		"../../../data/IGES/"			//��ʱ��ŵ�iges�ļ�


using namespace std;
int main()
{
	// read knot vector
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

	iges::Nurbs surf = iges::Nurbs(3, 3, 35, 11, knots_u, knots_v, control_pts);
	iges::IGESNurbs model = iges::IGESNurbs();
	model.set_surface(surf, 0, 1, 0, 1);
	model.write(string(IGESDir) + string("1.IGS"));


	cout << "OK" << endl;
}