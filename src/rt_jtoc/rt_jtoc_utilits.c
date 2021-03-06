#include "rt.h"

int				rt_jtoc_sdl_log_error(const char *p, const int id)
{
	SDL_Log("%s ----> ERROR <---- %s", KRED, KNRM);
	SDL_Log("INCORRECT: %s%s%s%s%s",
			p,
			id < 0 ? "" : " IN ID = ",
			KGRN,
			id < 0 ? "" : ft_itoa(id),
			KNRM);
	return (FUNCTION_FAILURE);
}

int				rt_jtoc_get_objects_num_in_arr(unsigned int *objects_num, t_jnode *n)
{
	t_jnode	*tmp;

	if (n == NULL)
		return (FUNCTION_SUCCESS);
	if (n->type != array)
		return (rt_jtoc_sdl_log_error("TYPE IS NOT ARRAY", -1));
	tmp = n->down;
	while (tmp)
	{
		if (tmp->type != object)
			return (rt_jtoc_sdl_log_error("TYPE IS NOT OBJECT", -1));
		(*objects_num)++;
		tmp = tmp->right;
	}
	return (FUNCTION_SUCCESS);
}

int				rt_jtoc_get_float2(cl_float2 *vec, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "x")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("X ERROR", -1));
	vec->x = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "y")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("Y ERROR", -1));
	vec->y = jtoc_get_float(tmp);
	return (FUNCTION_SUCCESS);
}

int				rt_jtoc_get_float3(cl_float3 *vec, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "x")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("X ERROR", -1));
	vec->x = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "y")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("Y ERROR", -1));
	vec->y = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "z")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("Z ERROR", -1));
	vec->z = jtoc_get_float(tmp);
	return (FUNCTION_SUCCESS);
}

int				rt_jtoc_get_float4(cl_float4 *vec, t_jnode *n)
{
	t_jnode	*tmp;

	if (!(tmp = jtoc_node_get_by_path(n, "x")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("X ERROR", -1));
	vec->x = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "y")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("Y ERROR", -1));
	vec->y = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "z")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("Z ERROR", -1));
	vec->z = jtoc_get_float(tmp);
	if (!(tmp = jtoc_node_get_by_path(n, "w")) || tmp->type != fractional)
		return (rt_jtoc_sdl_log_error("W ERROR", -1));
	vec->w = jtoc_get_float(tmp);
	return (FUNCTION_SUCCESS);
}

int				rt_jtoc_ispos_float2(cl_float2 *vec)
{
	return (vec->x >= 0 && vec->y >= 0
		? FUNCTION_SUCCESS
		: FUNCTION_FAILURE);
}

int				rt_jtoc_ispos_float3(cl_float3 *vec)
{
	return (vec->x >= 0 && vec->y >= 0 && vec->z >= 0
		? FUNCTION_SUCCESS
		: FUNCTION_FAILURE);
}

int				rt_jtoc_is01_float3(cl_float3 *vec)
{
	return (vec->x >= 0 && vec->x <= 1
		&& vec->y >= 0 && vec->y <= 1
		&& vec->z >= 0 && vec->z <= 1
			? FUNCTION_SUCCESS
			: FUNCTION_FAILURE);
}

int				rt_jtoc_ispos_float4(cl_float4 *vec)
{
	return (vec->x >= 0 && vec->y >= 0 && vec->z >= 0 && vec->w >= 0
		? FUNCTION_SUCCESS
		: FUNCTION_FAILURE);
}
