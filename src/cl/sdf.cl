#include "rt_cl.h"

static float ft_mod2(float a, float b)
{
	if (b == 0)
		b = 0.001;
	return (a - ((int)(a / b)) * b);
}


static float3 ft_mod(float3 a, float3 b)
{
	return ((float3){ft_mod2(a.x, b.x), ft_mod2(a.y, b.y), ft_mod2(a.z, b.z)});
}

//TODO try to make a restriction on repeat, or del rx/ry/rz
float3	repeatSDF(float3 pos, float3 c, float rx, float ry, float rz)
{
	(void)rx;
	(void)ry;
	(void)rz;
	return (ft_mod(pos, c) - c * 0.5f);
}

float	sdf_sphere(float3 pos, float radius)
{
//	return sin(pow(((mv_length(posc) - radius)), 2)); fun
	return (fast_length(pos) - radius);
}

float	sdf_box(float3 pos, float3 bounds)
{
	float3 d = fabs(pos) - bounds;
	return fast_length(fmax(d, 0.f)) + fmin(fmax(d.x, fmax(d.y, d.z)), 0);
}

float	sdf_round_box(float3 pos, float3 bounds, float r)
{
	float3 d = fabs(pos) - bounds;
	return fast_length(fmax(d, 0.f)) - r + fmin(fmax(d.x, fmax(d.y, d.z)), 0);
}

float	sdf_torus(float3 pos, float radius, float inner_radius)
{
	float2 q = (float2)(fast_length(pos.xz) - radius, pos.y);
	return fast_length(q) - inner_radius;
}

float	sdf_capped_torus(float3 pos, float2 sc, float ra, float rb)
{
	pos.x = fabs(pos.x);
	float k = (sc.y * pos.x > sc.x * pos.y) ? dot(pos.xy, sc) : fast_length(pos.xy);
	return sqrt(dot(pos, pos) + ra * ra - 2.0f * ra * k) - rb;
}

float	sdf_link(float3 pos, float le, float r1, float r2)
{
	float3 q = (float3)(pos.x, fmax(fabs(pos.y) - le, 0.0f), pos.z);
	return fast_length((float2)(fast_length(q.xy) - r1, q.z)) - r2;
}

float	sdf_cylinder(float3 pos, float3 c)
{
	return fast_length(pos.xz - c.xy) - c.z;
}

float	sdf_cone(float3 pos, float2 c)
{
	float q = fast_length(pos.xy);
	return dot(normalize(c), (float2)(q, pos.z));
}

float	sdf_plane(float3 pos, float3 n, float d)
{
	return dot(pos, n) + d;
}

//float	sdf_mandelbumb(float3 p, float power)
//{
//	float3 w = p;
//	float m = dot(w,w);
//
//	float4 trap = (float4)(fabs(w),m);
//	float dz = power;
//	for (int i; i < 10; i++)
//	{
//		float m2 = m * m;
//		float m4 = m2 * m2;
//		dz = 8.0f * sqrt(m4 * m2 * m) * dz + 1.0f;
//
//		float x = w.x;
//		float x2 = x * x;
//		float x4 = x2 * x2;
//		float y = w.y;
//		float y2 = y * y;
//		float y4 = y2 * y2;
//		float z = w.z;
//		float z2 = z * z;
//		float z4 = z2 * z2;
//
//		float k3 = x2 + z2;
//		float k2 = rsqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
//		float k1 = x4 + y4 + z4 - 6.0f * y2 * z2 - 6.0f * x2 * y2 + 2.0f * z2 * x2;
//		float k4 = x2 - y2 + z2;
//
//		w.x = p.x + 64.0f * x * y * z * (x2 - z2) * k4 * (x4 - 6.0f * x2 * z2 + z4) * k1 * k2;
//		w.y = p.y + -16.0f * y2 * k3 * k4 * k4 + k1 * k1;
//		w.z = p.z + -8.0f * y * k4 * (x4 * x4 - 28.0f * x4 * x2 * z2 + 70.0f * x4 * z4 - 28.0f * x2 * z2 * z4 + z4 * z4) * k1 * k2;
//		dz = 8.0f * pow(sqrt(m), 7.0f) * dz + 1.0f;
////		dz = 8.0f * pow(m,3.5) * dz + 1.0f;
////		dz = 8.0f * powf(m,3.5f) * dz + 1.0f;
//
//		float r = fast_length(w);
//		float b = 8.0f * acos(w.y / r);
//		float a = 8.0f * atan2(w.x, w.z);
//		w = p + pow(r, 8.0f) * (float3)(sin(b) * sin(a), cos(b), sin(b) * cos(a));
//
//		trap = fmin(trap, (float4)(fabs(w), m));
//
//		m = dot(w, w);
//		if (m > 256.0f)
//			break;
//	}
//	return (0.25f * log(m) * sqrt(m)/dz);
//}

static float4		sphere_fold(float3 z, float dz, float fixedradius, float minradius)
{
	float r2 = dot(z,z);

	if (r2 < minradius)
	{
		float temp = (fixedradius / minradius);
		z *= temp;
		dz *= temp;
	}
	else if (r2 < fixedradius)
	{
		float temp =(fixedradius/r2);
		z *= temp;
		dz *= temp;
	}
	return ((float4){z.x, z.y, z.z, dz});
}

static float3		box_fold(float3 z, float3 cube_size)
{
	z.x = clamp(z.x, -cube_size.x, cube_size.x) * 2.0 - z.x;
	z.y = clamp(z.y, -cube_size.y, cube_size.y) * 2.0 - z.y;
	z.z = clamp(z.z, -cube_size.z, cube_size.z) * 2.0 - z.z;
	return (z);
}

float	sdf_mandelbox(float3 pos, float scale, float fixedradius,
		float minradius, float3 cube_size, int iter)
{
	float3 z = pos;
	float3 offset = z;
	float dr = 2.0f;
	for (int n = 0; n < iter; n++)
	{
		float4 tmp;
		z = box_fold(z, cube_size);
		tmp = sphere_fold(z, dr, fixedradius, minradius);
		z.x = tmp.x;
		z.y = tmp.y;
		z.z = tmp.z;
		dr = tmp.w;
		z= scale * z + offset;
		dr = dr * fabs(scale) + 1.0f;
	}
	float r = fast_length(z);
	return (r / fabs(dr));
}

float	sdf_mandelbulb(float3 pos, float power, int iter, int breakout)
{
	float3 z = pos;
	float dr = 1;
	float r;

	for (int i = 0; i < iter; i++)
	{
		r = fast_length(z);
		if (r > breakout)
			break ;

		float theta = acos(z.z / r) * power;
		float phi = atan2(z.y, z.x) * power;
		float zr = pow(r, power);
		dr = pow(r, power - 1) * power * dr + 1;

		z = zr * (float3)(half_sin(theta) * half_cos(phi), half_sin(phi) * half_sin(theta), half_cos(theta));
		z += pos;
	}
	return ((0.5 * log(r) * r / dr));
}

float	sdf_cross_box(float3 pos, float3 b)
{
	float da = sdf_box(pos.xyz,(float3)(10.0, 1.0, 1.0));
	float db = sdf_box(pos.yzx,(float3)(1.0, 10.0, 1.0));
	float dc = sdf_box(pos.zxy,(float3)(1.0, 1.0, 10.0));
	return min(da,min(db,dc));
}

//float	sdf_menger(float3 pos, float3 b)
//{
//	float3 a;
//	float3 r;
//
//	float d = sdf_box(pos, (float3)(1.0));
//	float c = sdf_cross_box(pos * 3.f, b) / 3.f;
//	d = max( d, -c );
//	a = fmod(pos * 1.f, 2.f ) - 1.f;
//	r = 1.f - 1.f * fabs(a);
//	c = sdf_cross_box(r, b) / 9.f;
//	d = max( d, -c );
//	return (d);
//}

float	sdf_menger_sponge(float3 pos, float3 offset, float scale, int iteration)
{
   float4 z = (float4)(pos, 2.f);
   for (int n = 0; n < iteration; n++)
   {
       z = fabs(z);
       if (z.x < z.y)
           z.xy = z.yx;
       if (z.x < z.z)
           z.xz = z.zx;
       if (z.y < z.z)
           z.yz = z.zy;
       z = z * scale;
       z.xyz -= offset * (scale - 1.f);
       if (z.z < -0.5f * offset.z * (scale - 1.f))
           z.z += offset.z * (scale - 1.f);
   }
   return (length(max(fabs(z.xyz) - (float3)(1.f),0.f)) - 0.05f) / z.w;
}

float 	sdf_octahedron(float3 pos, float s)
{
	pos = fabs(pos);
	return ((pos.x + pos.y + pos.z - s) * 0.57735027f);
}

static float dot2(float3 v)
{
	return (dot(v, v));
}

float opsubtraction(float d1, float d2)
{
	return (max(-d1, d2));
}

float opunion( float d1, float d2)
{
	return (min(d1, d2));
}