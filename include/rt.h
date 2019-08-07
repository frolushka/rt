#ifndef RT_H
# define RT_H

# ifndef OPENCL___
#  include <OpenCL/opencl.h>
#  include "rt_camera.h"
#  include "libcl.h"
# endif

# include "rt_light.h"
# include "rt_object.h"
# include "rt_camera.h"
# include "transform.h"

//typedef struct			t_
/// END OF LIGHT

typedef struct			s_scene
{
	size_t				objects_count;
	size_t				lights_count;
	t_camera			camera;
# ifndef OPENCL___
	cl_float3			ambient;
	t_object			*objects;
	t_light				*lights;
# else
	float3				ambient;
	__global t_object	*objects;
	__global t_light	*lights;
# endif
}						t_scene;

# ifndef OPENCL___
typedef struct			s_rt_main
{
	t_cl				*cl;
	t_scene				*scenes;
}						t_rt_main;

int						rt_render(t_ui_main *ui, void *a);
int						rt_new_render(t_ui_main *ui, void *a);
# endif

#endif
