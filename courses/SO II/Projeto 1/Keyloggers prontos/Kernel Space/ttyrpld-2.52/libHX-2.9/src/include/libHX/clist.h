#ifndef _LIBHX_CLIST_H
#define _LIBHX_CLIST_H 1

#ifdef __cplusplus
#	include <cstddef>
#else
#	include <stddef.h>
#endif

#include <libHX/list.h>

#ifdef __cplusplus
extern "C" {
#endif

struct HXclist_head {
	union {
		struct HXlist_head list;
		struct {
			struct HXlist_head *next, *prev;
		};
	};
	unsigned int items;
};

#define HXCLIST_HEAD_INIT(name) {{{&(name).list, &(name).list}}, 0}
#define HXCLIST_HEAD(name) \
	struct HXclist_head name = HXCLIST_HEAD_INIT(name)

static inline void HXclist_init(struct HXclist_head *head)
{
	head->list.next = head->list.prev = &head->list;
	head->items = 0;
}

static inline void HXclist_del(struct HXclist_head *head,
    struct HXlist_head *node)
{
	--head->items;
	HXlist_del(node);
}

static inline void HXclist_unshift(struct HXclist_head *head,
    struct HXlist_head *nu)
{
	++head->items;
	__HXlist_add(nu, &head->list, head->list.next);
}

static inline void HXclist_push(struct HXclist_head *head,
    struct HXlist_head *nu)
{
	++head->items;
	__HXlist_add(nu, head->list.prev, &head->list);
}

static inline struct HXlist_head *__HXclist_pop(struct HXclist_head *head)
{
	struct HXlist_head *p;
	if ((const void *)head == head->list.next)
		return NULL;
	p = head->list.prev;
	HXlist_del(p);
	--head->items;
	return p;
}

#define HXclist_pop(head, type, member) ({ \
	struct HXlist_head *x = __HXclist_pop(head); \
	HXlist_entry(x, type, member); \
})

static inline struct HXlist_head *__HXclist_shift(struct HXclist_head *head)
{
	struct HXlist_head *p;
	if ((const void *)head == head->list.next)
		return NULL;
	p = head->list.next;
	HXlist_del(p);
	--head->items;
	return p;
}

#define HXclist_shift(head, type, member) ({ \
	struct HXlist_head *x = __HXclist_shift(head); \
	HXlist_entry(x, type, member); \
})

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBHX_CLIST_H */
