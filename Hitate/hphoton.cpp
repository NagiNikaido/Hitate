#include "hphoton.h"

inline HNPhotons::HNPhotons(double _max_dist, int _max_sample) {
	max_dist = _max_dist; max_sample = _max_sample;
	size = 0;
	lst = new PDH[_max_sample + 1];
	lst[0]=PDH(max_dist*max_dist, nullptr);
}

HNPhotons::~HNPhotons()
{
	delete[] lst;
}

inline double HNPhotons::bound() {
	return lst[0].first;
}

inline void HNPhotons::push(double dist2, HPhoton * p) {
	if (dist2 > lst[0].first) return;
	if (size >= max_sample) {
		std::pop_heap(lst + 1, lst + size + 1);
		size--;
	}
	lst[++size] = PDH(dist2, p);
	std::push_heap(lst + 1, lst + size + 1);
	if (size >= max_sample) lst[0] = lst[1];
}

HPhotonMap::HPhotonMap(int _stored, int _total)
{
	stored = _stored; total = _total; cnt = 0;
	php = new HPhoton[4*stored + 1];
	axis = new int[4*stored + 1];
}

HPhotonMap::~HPhotonMap()
{
	delete[] php;
	delete[] axis;
}

void HPhotonMap::insertPhoton(HPhoton p)
{
	if (cnt >= stored) return;
	php[++cnt] = p;
	if (cnt == 1) axis_min = axis_max = p.ray.op;
	else {
		axis_min.x = std::min(axis_min.x, p.ray.op.x);
		axis_min.y = std::min(axis_min.y, p.ray.op.y);
		axis_min.z = std::min(axis_min.z, p.ray.op.z);
		axis_max.x = std::max(axis_max.x, p.ray.op.x);
		axis_max.y = std::max(axis_max.y, p.ray.op.y);
		axis_max.z = std::max(axis_max.z, p.ray.op.z);
	}
}


void HPhotonMap::buildStructure()
{
	HPhoton *tph = new HPhoton[cnt + 1];
	for (int i = 1; i <= cnt; i++) tph[i] = php[i];
	memset(axis, 0xff, sizeof(axis));
	build(tph, 1, 1, cnt);
	delete[] tph;
}

HColor HPhotonMap::calcL(HIntersection inter, double max_dist, int max_sample)
{
	HColor res(0, 0, 0);
	HNPhotons nph(max_dist, max_sample);

	query(1, inter.pos, inter.norm, nph);
	if (nph.size <= _Min_Photon_Sample_Size) return res;
	for (int i = 1; i <= nph.size; i++)
		if (inter.norm.dotPro(nph.lst[i].second->ray.d) < 0)
			res += nph.lst[i].second->color;
	return res * 4 / nph.lst[0].first / total;
}

void HPhotonMap::build(HPhoton *tph, int c, int l, int r) {
	if (l == r) { php[c] = tph[l]; return; }
	int mid = 1, sp;

	while (4 * mid <= r - l + 1) mid <<= 1;
	if (3 * mid <= r - l + 1) mid = mid * 2 + l - 1;
	else mid = r + 1 - mid;

	//printf("%d %d %d\n", l, mid, r);
	double dt0 = axis_max.$(0) - axis_min.$(0),
		   dt1 = axis_max.$(1) - axis_min.$(1),
		   dt2 = axis_max.$(2) - axis_min.$(2);

	if (dt0 > dt1 && dt0 > dt2) sp = 0;
	else if (dt1 > dt2) sp = 1;
	else sp = 2;

	std::nth_element(tph + l, tph + mid, tph + r + 1,
		[&](HPhoton a, HPhoton b) -> bool {
			return a.ray.op.$(sp) < b.ray.op.$(sp);
		});
	php[c] = tph[mid]; axis[c] = sp;
	HPhoton *pp = tph + mid;
	if (l < mid) {
		double t = axis_max.$(sp);
		axis_max.$(sp) = php[c].ray.op.$(sp);
		build(tph, c * 2, l, mid - 1);
		axis_max.$(sp) = t;
	}
	if (mid < r) {
		double t = axis_min.$(sp);
		axis_min.$(sp) = php[c].ray.op.$(sp);
		build(tph, c * 2 + 1, mid + 1, r);
		axis_min.$(sp) = t;
	}
}

void HPhotonMap::query(int c, HVec3 p, HVec3 n, HNPhotons & nph)
{
	if (axis[c]==-1) return;
	int sp = axis[c];
	HPhoton *pp = php + c;
	if (c * 2 <= cnt) {
		double dist = p.$(sp) - pp->ray.op.$(sp);
		if (dist < 0) {
			query(c * 2, p, n, nph);
			if (dist * dist < nph.lst[0].first) query(c * 2 + 1, p, n, nph);
		}
		else {
			query(c * 2 + 1, p, n, nph);
			if (dist * dist < nph.lst[0].first) query(c * 2, p, n, nph);
		}
	}
	nph.push((p - pp->ray.op).len2(), pp);
}
