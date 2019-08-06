
#include "config_cl.h"

void        put_pixel(int x, int y, int color, __global char* img, int width, int height)
{
    int a;

    y = height - y;
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        a = x * 4 + y * width * 4;
        img[a] = RED(color);
        img[a + 1] = GREEN(color);
        img[a + 2] = BLUE(color);
        img[a + 3] = 0;
    }
}

__kernel void render(__global char* img, int width, int height, int objects_num,
		__global t_object *objects, int lights_num, __global t_light *lights,
		float3 camera_pos, float3 camera_local_x, float3 camera_local_y, float3
		camera_local_z, float camera_min_distance, float camera_max_distance,
		float fov, int quality, float3 ambient)
{
	int			gid;
	t_scene1	scene;
	float3		color;
	float3		direction = float3(0);

	gid = get_global_id(0);
	int x = gid % width;
	int y = gid / width;
	if (x % quality != 0 || y % quality != 0)
		return ;
	scene.objects_num = objects_num;
	scene.objects = objects;
	scene.min_distance = camera_min_distance;
	scene.max_distance = camera_max_distance;
	scene.ambient = ambient;
	float mult, dist = 0;
	float3 normal = float3(0);
	t_object intersected;
	direction = get_cam_ray(gid % width, gid / width, width, height, camera_pos, camera_local_x, camera_local_y, camera_local_z, camera_min_distance, camera_max_distance, fov, &mult);
	color = ray_marching(camera_pos, direction, &scene, mult, &normal, &dist, &intersected);
	if (!(normal.x == 0 && normal.y == 0 && normal.z == 0))
	{
		float3	diffuse = color * ambient;
		for (int i = 0; i < lights_num; ++i)
		{
			float3 interpoint = camera_pos + direction * dist;
			float NoL, a, mult;
			float3 LDirectional;
			switch (lights[i].type)
			{
				case directional:
					NoL = max(dot(normal, normalize(interpoint - lights[i].transform.pos)), 0.0f);
					a = lights[i].params.directional.color.w;
					LDirectional = lights[i].params.directional.color.xyz * a * NoL;
					diffuse += color * LDirectional;
					break;
				case point:
					a = length(interpoint - lights[i].transform.pos);
					if (a < lights[i].params.point.distance)
					{
						// -(dist / light.distance)^2 + 1
						mult = -pow(a / lights[i].params.point.distance, 2) + 1;
						diffuse += color * lights[i].params.point.color.xyz * lights[i].params.point.color.w * mult;
					}
					break;
			}
		}
		color = pow(diffuse, float3(0.4545));
	}
	color = color * 255;
//	put_pixel(gid % width, gid / width + 1, COLOR(color.x, color.y, color.z), img, width, height);
	for (int i = 0; i < quality; ++i)
	{
		for (int j = 0; j < quality; ++j)
		{
			put_pixel(x + i, y + j + 1, COLOR(color.x, color.y, color.z), img, width, height);
		}
	}
}
