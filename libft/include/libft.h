/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbednar <sbednar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 23:51:41 by sbednar           #+#    #+#             */
/*   Updated: 2019/07/10 14:41:24 by sbednar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <math.h>

# define BUFF_SIZE	32

# define HASH_P		31
# define HASH_M		1000000009

typedef struct		s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}					t_list;

typedef struct		s_vec2
{
	int				x;
	int				y;
}					t_vec2;

typedef struct		s_fvec2
{
	float			x;
	float			y;
}					t_fvec2;

typedef struct		s_gc
{
	t_list			*root;
}					t_gc;

typedef struct		s_vec
{
	int				*storage;
	size_t			capacity;
	size_t			real_cell_size;
	size_t			cell_size;
	size_t			size;
}					t_vec;

void				*ft_memset(void *b, int c, size_t len);
void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dst, void const *src, size_t n);
void				*ft_memccpy(void *dst, void const *src, int c, size_t n);
void				*ft_memmove(void *dst, void const *src, size_t len);
void				*ft_memchr(void const *s, int c, size_t n);
int					ft_memcmp(void const *s1, void const *s2, size_t n);
size_t				ft_strlen(char const *s);
char				*ft_strdup(char const *s1);
char				*ft_strcpy(char *dst, char const *src);
char				*ft_strncpy(char *dst, char const *src, size_t len);
char				*ft_strcat(char *s1, char const *s2);
char				*ft_strncat(char *s1, char const *s2, size_t n);
size_t				ft_strlcat(char *dst, char const *src, size_t dstsize);
char				*ft_strchr(char const *s, int c);
char				*ft_strrchr(char const *s, int c);
char				*ft_strstr(char const *haystack, char const *needle);
char				*ft_strnstr(char const *h, char const *n, size_t len);
int					ft_strcmp(char const *s1, char const *s2);
int					ft_strncmp(char const *s1, char const *s2, size_t n);
int					ft_atoi(char const *str);
int					ft_atoi_base(const char *str, int str_base);
int					ft_isalpha(int c);
int					ft_isdigit(int c);
int					ft_isalnum(int c);
int					ft_isascii(int c);
int					ft_isprint(int c);
int					ft_toupper(int c);
int					ft_tolower(int c);

void				*ft_memalloc(size_t size);
void				*ft_x_memalloc(size_t size);
void				ft_memdel(void **ap);
char				*ft_strnew(size_t size);
void				ft_strdel(char **as);
void				ft_strclr(char *s);
void				ft_striter(char *s, void (*f)(char *));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strmap(char const *s, char (*f)(char));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int					ft_strequ(char const *s1, char const *s2);
int					ft_strnequ(char const *s1, char const *s2, size_t n);
char				*ft_strsub(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strtrim(char const *s);
char				**ft_strsplit(char const *s, char c);
char				**ft_strsplit_on_size(char const *s, size_t size,
		size_t *num_strs);
char				*ft_itoa(int n);
void				ft_putchar(char c);
void				ft_putstr(char const *s);
void				ft_putendl(char const *s);
void				ft_putnbr(int n);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr_fd(int n, int fd);

int					ft_min(int a, int b);
float				ft_fmin(float a, float b);
int					ft_max(int a, int b);
float				ft_fmax(float a, float b);

t_list				*ft_lstnew(void const *content, size_t content_size);
void				ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void				ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void				ft_lstadd(t_list **alst, t_list *new);
void				ft_lstadd_back(t_list **alst, t_list *new);
void				ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list				*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

int					ft_islower(int c);
int					ft_isupper(int c);
int					ft_isstn(char c);
void				ft_putwchar(int wc);
void				ft_putwchar_fd(int wc, int fd);
char				*ft_strallcpy(char const *src, size_t len);
size_t				ft_nbrlen(int n);
size_t				ft_wordlen(char const *s, char c);
size_t				ft_wordscnt(const char *str, char c);
char				**ft_strsfree(char **src);
void				ft_lstdels(t_list **alst);
void				ft_lstdelones(t_list **alst);

t_fvec2				ft_fvec2_round(t_fvec2 v);

int					ft_strhash(const char *str);

int					get_next_line(int const fd, char **line);

char				*cut_str_by_num_words(const char *s, size_t num_words);
char				**get_strs(const char *s, size_t *nss,
		size_t *num_words_in_strs);

float				ft_lerp(float from, float to, float t);

t_gc				*gc_get(void);
int					gc_add(void *c);
int					gc_collect(void);
void				*ft_gcalloc(size_t size);

t_vec				*vec_init(size_t c, size_t cs);
int					vec_setup(t_vec *v, size_t c, size_t cs);
void				*vec_at(t_vec *v, size_t i);
void				*vec_last(t_vec *v);
int					vec_resize(t_vec *v, size_t c);
int					vec_clear(t_vec *v);
int					vec_insert(t_vec *v, void *d, size_t i);
int					vec_push_back(t_vec *v, void *d);

int					vec_remove_at(t_vec *v, size_t i);

#endif
