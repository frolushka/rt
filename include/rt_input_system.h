/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_input.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbecker <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 05:13:21 by sbecker           #+#    #+#             */
/*   Updated: 2019/12/01 18:31:47 by sbednar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_INPUT_SYSTEM_H
# define RT_INPUT_SYSTEM_H

# include "rt.h"

typedef struct		s_input_system
{
	t_system		system;
	const Uint8		*state;
	t_rb			*active;
	t_transform		*selected;
}					t_input_system;

void				move_active(t_input_system *s);
void				rotate_active(t_input_system *s);

float				get_axis(const Uint8 *state,
		SDL_Scancode low, SDL_Scancode high);

void				change_selected(t_input_system *s, t_object *o);

int					is_func(void *isv);

#endif
