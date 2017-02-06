#ifndef _LIBHX_LIST_H
#define _LIBHX_LIST_H 1

#ifdef __cplusplus
#	include <cstddef>
#else
#	include <stddef.h>
#endif
#include <libHX/defs.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HXlist_entry(ptr, type, member) containerof((ptr), type, member)

struct HXlist_head {
	struct HXlist_head *next, *prev;
};

#define HXLIST_HEAD_INIT(name) {&(name), &(name)}
#define HXLIST_HEAD(name) \
	struct HXlist_head name = HXLIST_HEAD_INIT(name)

static inline void HXlist_init(struct HXlist_head *list)
{
	list->next = list->prev = list;
}

static inline void __HXlist_add(struct HXlist_head *nu,
    struct HXlist_head *prev, struct HXlist_head *next)
{
	nu->next = next;
	nu->prev = prev;
	next->prev = nu;
	prev->next = nu;
}

static inline void HXlist_add(struct HXlist_head *head, 
    struct HXlist_head *entry)
{
	__HXlist_add(entry, head, head->next);
}

static inline void HXlist_add_tail(struct HXlist_head *head,
    struct HXlist_head *entry)
{
	__HXlist_add(entry, head->prev, head);
}

static inline void HXlist_del(struct HXlist_head *entry)
{
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
	entry->next = NULL;
	entry->prev = NULL;
}

#define HXlist_for_each(pos, head) \
	for ((pos) = (head)->next; (pos) != (void *)(head); \
	     (pos) = (pos)->next)

#define HXlist_for_each_prev(pos, head) \
	for ((pos) = (head)->prev; (pos) != (void *)(head); \
	     (pos) = (pos)->prev)

#define HXlist_for_each_safe(pos, n, head) \
	for ((pos) = (head)->next, (n) = (pos)->next; (pos) != (void *)(head); \
	     (pos) = (n), (n) = (pos)->next)

#define HXlist_for_each_prev_safe(pos, n, head) \
	for ((pos) = (head)->prev, (n) = (pos)->prev; (pos) != (void *)(head); \
	     (pos) = (n), (n) = (pos)->prev)

#define HXlist_for_each_entry(pos, head, member) \
	for ((pos) = HXlist_entry((head)->next, typeof(*(pos)), member); \
	     &(pos)->member != (void *)(head); \
	     (pos) = HXlist_entry((pos)->member.next, typeof(*(pos)), member))

#define HXlist_for_each_entry_rev(pos, head, member) \
	for ((pos) = HXlist_entry((head)->prev, typeof(*(pos)), member); \
	     &(pos)->member != (void *)(head); \
	     (pos) = HXlist_entry((pos)->member.prev, typeof(*(pos)), member))

#define HXlist_for_each_entry_safe(pos, n, head, member) \
	for ((pos) = HXlist_entry((head)->next, typeof(*(pos)), member), \
	     (n) = HXlist_entry((pos)->member.next, typeof(*(pos)), member); \
	     &(pos)->member != (void *)(head); \
	     (pos) = (n), (n) = HXlist_entry((n)->member.next, typeof(*(n)), \
	     member))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBHX_LIST_H */
