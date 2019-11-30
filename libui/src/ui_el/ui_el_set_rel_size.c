/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_el_set_rel_size.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbednar  <sbednar @student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 13:35:05 by sbednar           #+#    #+#             */
/*   Updated: 2019/05/23 14:37:42 by sbednar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_el_set_rel_size(t_ui_el *el, float w, float h)
{
	t_ui_el	*p;

	if (!el || !(p = el->parent) || w == 0.0 || h == 0.0)
		return ;
	el->rrect.w = w;
	el->rrect.h = h;
	el->rect.w = (int)((float)p->rect.w * w);
	el->rect.h = (int)((float)p->rect.h * h);
	el->crect.w = el->rect.w;
	el->crect.h = el->rect.h;
}
