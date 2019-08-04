#ifndef RT_H
# define RT_H

# ifndef OPENCL___
#  include <OpenCL/opencl.h>
#  include "rt_camera.h"
#  include "libcl.h"
# endif

# include "rt_camera.h"
# include "transform.h"

/// OBJECTS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
typedef struct			s_sphere
{
# ifndef OPENCL___
	cl_float			radius;
# else
	float				radius;
# endif
}						t_sphere;

typedef struct			s_box
{
# ifndef OPENCL___
	cl_float3			bounds;
# else
	float3				bounds;
# endif
}						t_box;

typedef union			__u_params
{
	t_sphere			sphere;
	t_box				box;
}						u_params;

typedef struct			s_object
{
	t_transform			transform;
	u_params			params;

# ifndef OPENCL___
	cl_int				type;
	t_rb				rb;
# else
	int					type;
	char				trash[112];
# endif
}						t_object;
/// END OF OBJECTS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

typedef struct			s_scene
{
	size_t				objects_count;
	t_camera			camera;
	t_object			*objects;
}						t_scene;

# ifndef OPENCL___
typedef struct			s_rt_main
{
	t_cl				*cl;
	t_scene				*scenes;
}						t_rt_main;

int						rt_render(t_ui_main *ui, void *a);
# endif

#endif
