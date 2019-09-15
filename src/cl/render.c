#include "rt_cl.h"

void			put_pixel(__global char *image, int2 pixel, int2 screen, float3 color)
{
    int a;

    pixel.y = screen.y - pixel.y - 1;
    if (pixel.x >= 0 && pixel.x < screen.x && pixel.y >= 0 && pixel.y < screen.y)
    {
        a = 4 * (pixel.y * screen.x + pixel.x);
        color *= 255;
        image[a] = color.x;
        image[a + 1] = color.y;
        image[a + 2] = color.z;
        image[a + 3] = 0;
    }
}

void			fill_camera_pixel(__global char *image, int2 pixel, int2 screen, float3 color, int quality)
{
	int2 cur_pixel;

	for (int i = 0; i < quality; ++i)
	{
		for (int j = 0; j < quality; ++j)
		{
			cur_pixel = (int2)(pixel.x + i, pixel.y + j);
			put_pixel(image, cur_pixel, screen, color);
		}
	}
}

float3			get_skybox_color(float3 direction)
{
	return (min(1, max(0, (float3){
		mad(direction.y, -0.7f, 0.6f),
		mad(direction.y, -0.7f, 0.36f),
		mad(direction.y, -0.7f, 0.3f)
	})));
}

float2			uv_mapping_for_plane(t_raycast_hit rh)
{
	float3 point = rh.point;
	float3 normvec;
	float3 crossvec;
	float v;
	float u;

	if (rh.normal.x != 0.0f || rh.normal.y != 0.0f)
		normvec = normalize((float3) {rh.normal.y, -rh.normal.x, 0.0f});
	else
		normvec = (float3) {0.0f, 0.0f, 1.0f};
	crossvec = cross(normvec, rh.normal);
	u = 0.5 + fmod(dot(normvec, point), 1.0f) / 2;
	v = 0.5 + fmod(dot(crossvec, point), 1.0f) / 2;
	return ((float2){u, v});
}

float2			uv_mapping_for_sphere(t_raycast_hit rh)
{
	float3	point = rh.point;
	float3	obj_pos = rh.hit->transform.pos;
	float3	vec;
	float 	v;
	float 	u;

	vec = normalize(point - obj_pos);
	u = 0.5f + (atan2(vec.z, vec.x) / TWO_PI);
	v = 0.5f - (asin(vec.y) / PI);
	return ((float2){u, v});
}

float2			uv_mapping_for_cylinder(t_raycast_hit rh)
{
	float3	point = rh.point;
	float3	obj_pos = rh.hit->transform.pos;
	float3	vec;
	float 	v;
	float 	u;

	vec = normalize(point - obj_pos);
	u = 0.5f + (atan2(vec.z, vec.x) / TWO_PI);
	v = 0.5f + (modf(point.y * 1000 / 1024, &v) / 2);
	return ((float2){u, v});
}

__kernel void	render(__global char *image, __global t_scene *scene, __global t_object *objects, __global t_light *lights, int2 screen, __global int *texture)
{
	int		gid = get_global_id(0);

	int2	pixel = (int2)(gid % screen.x, gid / screen.x);

	if (pixel.x % scene->camera.quality || pixel.y % scene->camera.quality)
		return;

	scene->objects = objects;
	scene->lights = lights;

	t_camera cached_camera = scene->camera;

	float3 k = screen_to_world(pixel, screen, cached_camera.fov);

	t_transform t = cached_camera.transform;
	float3 direction = normalize(mad(t.right, k.x, mad(t.up, k.y, t.forward * k.z)));

	float3	color;
	t_raycast_hit rh;
	rh.clip_ratio = length(k) / k.z;
	if (!raymarch(cached_camera.transform.pos, direction, 0, scene, &rh))
	{
		color = get_skybox_color(direction);
		fill_camera_pixel(image, pixel, screen, color, cached_camera.quality);
		return;
	}
	if (rh.hit->type == o_sphere)
	{
		float2 uv;
		uv = uv_mapping_for_sphere(rh);
		int coord = int(uv.x * 1024) + int(uv.y * 1024) * 1024;
		color.x = (RED(texture[coord]));
		color.y = (GREEN(texture[coord]));
		color.z = (BLUE(texture[coord]));
		color.x /= 255;
		color.y /= 255;
		color.z /= 255;
	}
	else if (rh.hit->type == o_cylinder)
	{
		float2 uv;
		uv = uv_mapping_for_cylinder(rh);
		int coord = int(uv.x * 1024) + int(uv.y * 1024) * 1024;
		color.x = (RED(texture[coord]));
		color.y = (GREEN(texture[coord]));
		color.z = (BLUE(texture[coord]));
		color.x /= 255;
		color.y /= 255;
		color.z /= 255;
	}
	else if (rh.hit->type == o_plane)
	{
		float2 uv;
		uv = uv_mapping_for_plane(rh);
		int coord = int(uv.x * 1024) + int(uv.y * 1024) * 1024;
		color.x = (RED(texture[coord]));
		color.y = (GREEN(texture[coord]));
		color.z = (BLUE(texture[coord]));
		color.x /= 255;
		color.y /= 255;
		color.z /= 255;
	}
	else
		color = rh.hit->material.color.xyz;
//		color = rh.hit->material.color.xyz;
	// TODO refactor
	// Light processing.
	float3 diffuse = color * scene->ambient;
//	diffuse = clamp(diffuse, (float3){0, 0, 0}, (float3){1, 1, 1});
	for (uint i = 0; i < scene->lights_count; ++i)
	{
		t = scene->lights[i].transform;

		float NoL, a, mult;
		float3 LDirectional, dir, ndir;
		t_raycast_hit rhl;
		rhl.clip_ratio = length(k) / k.z;

		switch (scene->lights[i].type)
		{
			case directional:
				dir = -t.forward;
				ndir = normalize(dir);
				if (raymarch(rh.point + rh.normal * F_EPS, ndir, length(dir), scene, &rhl))
					continue;

				NoL = max(dot(rh.normal, ndir), 0.0f);
				LDirectional = scene->lights[i].params.directional.color * NoL;
				diffuse += color * LDirectional;
				diffuse = clamp(diffuse, (float3){0, 0, 0}, (float3){1, 1, 1});
			case point:
				dir = t.pos - rh.point;
				ndir = normalize(dir);
				if (raymarch(rh.point + rh.normal * F_EPS, ndir, length(dir), scene, &rhl))
					continue;

				a = length(dir);
				if (a < scene->lights[i].params.point.distance)
				{
					NoL = max(dot(rh.normal, ndir), 0.0f);
					mult = -pow(min(a / scene->lights[i].params.point.distance, 1.0f), 2) + 1.0f;
					diffuse += color * scene->lights[i].params.point.color * NoL * mult;
					diffuse = clamp(diffuse, (float3){0, 0, 0}, (float3){1, 1, 1});
				}
		}
	}

	// Gamma correction.
	//TODO COLOR IS BAD
/*	if (diffuse.x > 255)
		diffuse.x = 255;
	if (diffuse.y > 255)
		diffuse.y = 255;
	if (diffuse.z > 255)
		diffuse.z = 255;
	color = diffuse;*/
//	color = pow(diffuse, 0.4545f);
	color = diffuse;
	fill_camera_pixel(image, pixel, screen, color, cached_camera.quality);
}