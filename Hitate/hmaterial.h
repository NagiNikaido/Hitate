#pragma once

#include "hitate.h"
#include "hutils.h"

struct HMaterial {
	HColor color, absor;
	double refl, refr, diff, spec;
	double rindex, drfl;
	HMaterial();
	HMaterial(HColor _color, HColor _absor = HColor(0, 0, 0),
		double _refl = 0, double _refr = 0, double _diff = 0, double _spec = 0, double _rindex = 0, double _drfl = 0);
	~HMaterial();
};