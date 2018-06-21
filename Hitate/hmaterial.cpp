#include "hmaterial.h"

HMaterial::HMaterial()
{
	color = absor = HColor(0, 0, 0);
	refl = refr = diff = spec = 0;
	rindex = drfl = 0;
}

HMaterial::HMaterial(HColor _color, HColor _absor, double _refl, double _refr, double _diff, double _spec, double _rindex, double _drfl)
{
	color = _color, absor = _absor;
	refl = _refl, refr = _refr, diff = _diff, spec = _spec;
	rindex = _rindex, drfl = _drfl;
}

HMaterial::~HMaterial()
{
}

