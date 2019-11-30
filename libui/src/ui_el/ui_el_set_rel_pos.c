/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_el_set_rel_pos.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbednar  <sbednar @student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 13:35:28 by sbednar           #+#    #+#             */
/*   Updated: 2019/05/23 14:38:13 by sbednar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libui.h"

void	ui_el_set_rel_pos(t_ui_el *el, float x, float y)
{
	t_ui_el	*p;

	if (!el || !(p = el->parent))
		return ;
	el->rrect.x = x;
	el->rrect.y = y;
	el->rect.x = (int)((float)p->rect.w * x);
	el->rect.y = (int)((float)p->rect.h * y);
	el->crect.x = el->rect.x;
	el->crect.y = el->rect.y;
}
