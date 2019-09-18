//
// Created by Slaughterfish Becker on 2019-09-14.
//

#include "rt.h"
#include "rt_jtoc.h"

static void	fill_constant_screen_gpu_mem(t_rt_main *rt, cl_int2 screen_size)
{
	char			*texture[4];

	texture[0] = "./textures/earth.jpg";
	texture[1] = "./textures/texture.png";
	texture[2] = "./textures/brick.jpg";
	texture[3] = "./textures/marble.png";
	rt->gpu_mem = (t_s_gpu_mem *)ft_x_memalloc(sizeof(t_s_gpu_mem));
	rt->gpu_mem->cl_image = clCreateBuffer(*rt->cl->context,
										   CL_MEM_READ_WRITE, sizeof(int) * screen_size.x * screen_size.y,
										   NULL, NULL);
	rt->gpu_mem->cl_aux = clCreateBuffer(*rt->cl->context,
										 CL_MEM_READ_WRITE, sizeof(int) * screen_size.x * screen_size.y,
										 NULL, NULL);
	find_textures_size(rt, texture, 4);
	if (!(rt->texture.texture = (int *)ft_memalloc(sizeof(int) * rt->texture.texture_size)))
		return ;
	get_textures(rt, texture, 4);
	rt->gpu_mem->cl_texture = clCreateBuffer(*rt->cl->context,
										 CL_MEM_READ_ONLY, 4 * rt->texture.texture_size,
										 NULL, NULL);
	clEnqueueWriteBuffer(*rt->cl->queue, rt->gpu_mem->cl_texture, CL_TRUE, 0,
			4 * rt->texture.texture_size,
			rt->texture.texture, 0, NULL, NULL);
	rt->gpu_mem->cl_texture_w = clCreateBuffer(*rt->cl->context,
												  CL_MEM_READ_ONLY, sizeof(int) * 100, NULL, NULL);
	clEnqueueWriteBuffer(*rt->cl->queue, rt->gpu_mem->cl_texture_w, CL_TRUE, 0,
						 sizeof(int) * 100, &rt->texture.texture_w, 0, NULL, NULL);
	rt->gpu_mem->cl_texture_h = clCreateBuffer(*rt->cl->context,
											   CL_MEM_READ_ONLY, sizeof(int) * 100, NULL, NULL);
	clEnqueueWriteBuffer(*rt->cl->queue, rt->gpu_mem->cl_texture_h, CL_TRUE, 0,
						 sizeof(int) * 100, &rt->texture.texture_h, 0, NULL, NULL);
	rt->gpu_mem->cl_prev_texture_size = clCreateBuffer(*rt->cl->context,
			CL_MEM_READ_ONLY, sizeof(int) * 100, NULL, NULL);
	clEnqueueWriteBuffer(*rt->cl->queue, rt->gpu_mem->cl_prev_texture_size, CL_TRUE, 0,
						 sizeof(int) * 100, &rt->texture.prev_texture_size, 0, NULL, NULL);
}

t_rt_main	*setup_rt(cl_int2 screen_size)
{
	t_rt_main	*rt;

	rt = (t_rt_main *)ft_memalloc(sizeof(t_rt_main));
	rt->screen_size = screen_size;
	rt->cl = cl_setup((char *[]){
							  "src/cl/render.c",
							  "src/cl/raymarch.c",
							  "src/cl/sdf.c",
							  "src/cl/ray.c",
							  "src/cl/uv_mapping.cl",
							  "src/cl/choose_texture_for_object.cl",
							  NULL},
					  (char *[]){"render", NULL});
	rt->scenes = rt_jtoc_scenes_setup((char *[]){
			"json/scenes/test_scene.json",
			NULL});
	fill_constant_screen_gpu_mem(rt, screen_size);
	rt->params |= RT_RENDER_2;
	return (rt);
}
