/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_sdf.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 05:13:21 by sbecker           #+#    #+#             */
/*   Updated: 2019/07/03 20:16:18 by sbecker          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_raycast.h"

cl_float3	f3abs(cl_float3 v)
{
	return (cl_float3){{fabs(v.x), fabs(v.y), fabs(v.z)}};
}

cl_float3	f3max(cl_float3 v1, cl_float3 v2)
{
	return (cl_float3){{fmax(v1.x, v2.x), fmax(v1.y, v2.y), fmax(v1.z, v2.z)}};
}

float		f2dot(cl_float2 v1, cl_float2 v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

float		f3dot(cl_float3 v1, cl_float3 v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float		f2len(cl_float2 v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}
