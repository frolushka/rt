/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_el_load_surface_from.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbednar <sbednar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 04:32:52 by sbednar           #+#    #+#             */
/*   Updated: 2019/07/08 23:15:16 by sbednar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

int	ui_el_load_surface_from(t_ui_el *el, const char *path)
{
	SDL_Surface	*tmp;

	if (!(tmp = IMG_Load(path)))
		return (FUNCTION_FAILURE);
	SDL_FreeSurface(el->sdl_surface);
	el->sdl_surface = tmp;
	return (FUNCTION_SUCCESS);
}
