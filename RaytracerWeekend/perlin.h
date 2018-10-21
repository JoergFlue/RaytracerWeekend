#ifndef PERLINH
#define PERLINH

#include "vec3.h"

#define drand48() ((double)rand() / (double)(RAND_MAX + 0.0000001))

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
	float uu = u * u*(3 - 2 * u);
	float vv = v * v*(3 - 2 * v);
	float ww = w * w*(3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++) {
				vec3 weight_vec(u - i, v - j, w - k);
				accum += (i*uu + (1 - i)*(1 - uu))*
						(j*vv + (1 - j)*(1 - vv))*
						(k*ww + (1 - k)*(1 - ww))*
						dot(c[i][j][k], weight_vec);
			}
	return abs(accum);
}

class perlin {
public:
	float noise(const vec3& p) const {
		float u = p.x() - int(p.x());
		float v = p.y() - int(p.y());
		float w = p.z() - int(p.z());
		int i = int(p.x());
		int j = int(p.y());
		int k = int(p.z());
		vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];

		return perlin_interp(c, u, v, w);
	}
	static vec3 *ranvec;
	static int *perm_x;
	static int *perm_y;
	static int *perm_z;
};

static vec3* perlin_generate() {
	vec3 * p = new vec3[256];
	for (int i = 0; i < 256; i++)
		p[i] = unit_vector(vec3(-1 + 2 * drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));
	return p;
}

void permute(int *p, int n) {
	for (int i = n - 1; i > 0; i--) {
		int target = int(drand48()*(i + 1));
		int tmp = p[i];
		p[i] = p[target];
		p[target] = tmp;
	}
	return;
}

static int* perlin_generate_perm() {
	int * p = new int[256];
	for (int i = 0; i < 256; i++)
		p[i] = i;
	permute(p, 256);
	return p;
}

vec3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();

#endif // PERLINH

