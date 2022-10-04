#include "write_iges.h"

namespace cam
{
	using namespace std;

	void CurveLoop::push_back(std::shared_ptr<IGES_CURVE> curve)
	{
		loop.push_back(curve);
	}

	bool CurveLoop::check_loop()
	{
		//检查曲线是否构成一个环
		return true;
	}

	IGESNurbs::IGESNurbs()
	{
		color			= make_unique<DLL_IGES_ENTITY_314>(DLL_IGES_ENTITY_314(model, true));
		surface			= make_unique<DLL_IGES_ENTITY_128>(DLL_IGES_ENTITY_128(model, true));
		space_bound		= make_unique<DLL_IGES_ENTITY_102>(DLL_IGES_ENTITY_102(model, true));
		param_bound		= make_unique<DLL_IGES_ENTITY_102>(DLL_IGES_ENTITY_102(model, true));
		surface_bound	= make_unique<DLL_IGES_ENTITY_142>(DLL_IGES_ENTITY_142(model, true));
		trimmed_surface = make_unique<DLL_IGES_ENTITY_144>(DLL_IGES_ENTITY_144(model, true));

		color->SetColor(0., 80., 0.);
	}

	void IGESNurbs::set_color(double r, double g, double b)
	{
		//range is 0.0 .. 100.0
		color->SetColor(r, g, b);
	}

	void IGESNurbs::set_param_bound(CurveLoop& param_loop)
	{
		for (auto line : param_loop.loop) {
			param_bound->AddSegment(move(line).get());
		}
	}

	void IGESNurbs::set_space_bound(CurveLoop& space_loop)
	{
		for (auto curve : space_loop.loop) {
			space_bound->AddSegment(move(curve).get());
		}
	}
}
