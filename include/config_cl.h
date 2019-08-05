#ifndef CONFIG_CL_H
# define CONFIG_CL_H

# define CL_SILENCE_DEPRECATION

# include "rt.h"
# include "color.h"

typedef struct	s_scene1
{
	int						objects_num;
	float					min_distance;
	float					max_distance;
	float3					ambient;
	__global t_object*		objects;
}				t_scene1;

float3	get_cam_ray(int x, int y, int width, int height, float3 camera_pos, float3 camera_local_x, float3 camera_local_y, float3 camera_local_z, float camera_min_distance, float camera_max_distance, float fov, float *mult);
void		put_pixel(int x, int y, int color, __global char* img, int width, int height);
float3	ray_marching(float3 start_ray, float3 dir_ray, t_scene1 *scene, float
	mult, float3 *normal, float *intersect_dist);
float3 repeatSDF(float3 pos, float3 cen, float rx, float ry, float rz);
float sphereSDF(float3 posc, float radius);

float	sdf_sphere(float3 pos, float radius);
float	sdf_box(float3 pos, float3 bounds);
float	sdf_round_box(float3 pos, float4 bounds);
float	sdf_torus(float3 pos, float2 t);
float	sdf_cylinder(float3 pos, float3 c);

#endif
