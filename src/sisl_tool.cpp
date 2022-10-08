#include "sisl_tool.h"
#include "sisl.h"

namespace iges
{
	using namespace std;
	Nurbs::Nurbs(SISLSurf* s)
	{
		u_p = s->ik1 - 1; // ik1: Order of surface in first parameter direction.
		v_p = s->ik2 - 1;
		u_num = s->in1;	  // in1: Number of vertices in first parameter direction
		v_num = s->in2;
		
		knots_u.resize(s->ik1 + s->in1); 
		knots_v.resize(s->ik2 + s->in2);

		copy(s->et1, s->et1 + knots_u.size(), knots_u.begin());
		copy(s->et2, s->et2 + knots_v.size(), knots_v.begin());
	}
}
