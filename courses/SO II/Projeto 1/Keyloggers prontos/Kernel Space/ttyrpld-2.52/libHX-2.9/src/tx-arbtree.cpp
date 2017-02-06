/*
 *	arbtree test program
 *	written by Jan Engelhardt
 *	this program is released in the Public Domain
 *
 *	Do not take this file as a reason to do casts on everything!
 *	only because this file has to be compilable in both C and C++ mode.
 */
#include <sys/time.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libHX/arbtree.h>
#include <libHX/misc.h>
#include <libHX/string.h>
#include "internal.h"

enum {
	S_LEFT = 0,
	S_RIGHT,
	NODE_RED = 0,
	NODE_BLACK,
};

struct t9_fruit {
	unsigned int pad;
	char name[20];
};

struct t9_fruit2 {
	unsigned int pad;
	char *name;
};

static struct HXbtree *generate_fixed_tree(unsigned int, ...);
static struct HXbtree *generate_perfect_tree(unsigned int, unsigned int);
static struct HXbtree *generate_random_tree(unsigned int);
static void destroy_random_tree(struct HXbtree *, unsigned int);
static void height_check(const struct HXbtree *);
static int sbc_strcmp(const char *, const char *);
static int strtolcmp(const void *, const void *);
static inline void timer_start(void);
static inline void timer_end(void);
static unsigned int tree_height(const struct HXbtree_node *);
static int verify_black_height(const struct HXbtree_node *);
static unsigned int verify_red_no_red_children(const struct HXbtree_node *);
static unsigned int verify_random_tree(const struct HXbtree *);
static void __walk_tree(const struct HXbtree_node *, char *, size_t);
static void walk_tree(const struct HXbtree_node *, char *, size_t);

static const char *const fruits[] = {
	/* Our garden, left side */
	"cherry",
	"strawberry",
	"gooseberry",
	/* ?salad/cabbage? */
	"plum",

	/* right side */
	"whitecurrant",
	"redcurrant",
	"blackcurrant",
	"apple",
};
static const char *const Color[] = {"RED", "BLACK"};
static struct HXbtree *btree;
static struct timeval tv_start;

//-----------------------------------------------------------------------------
static void test_1(void)
{
	struct HXbtree_node *node;
	char buf[80];
	void *trav;

	printf("Test 1A: Creating tree with 7 elements (hg 3)\n");
	btree = generate_perfect_tree(3, 2);

	printf("Test 1B: Manual traversion\n");
	walk_tree(btree->root, buf, sizeof(buf));

	printf("Test 1C: Check for correct positions and colors\n");
	sbc_strcmp(buf, "8%b(4%b(2,6),12%b(10,14))");

	/*        8
	 *      /     \
	 *    4         12
	 *   / \       /  \
	 *  2   6    10    14
	 *          /
	 *         9
	 */
	printf("Test 1D: Node insertion and test for positions/colors\n");
	HXbtree_add(btree, "9");
	walk_tree(btree->root, buf, sizeof(buf));
	sbc_strcmp(buf, "8%b(4%b(2,6),12(10%b(9),14%b))");

	printf("Test 1E: Height check\n");
	height_check(btree);

	printf("Test 1F: Standard traverse\n");
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL)
		printf("\t" "key: %s (%s)\n", node->skey, Color[node->color]);
	HXbtrav_free(trav);

	printf("Test 1G: Node deletion\n");
	HXbtree_del(btree, "8");
	walk_tree(btree->root, buf, sizeof(buf));
	sbc_strcmp(buf, "9%b(4%b(2,6),12(10%b,14%b))");

	/*        9       (8 replaced by its in-order successor 9)
	 *      /    \
	 *    4        12
	 *   / \      /  \
	 *  2   6   10    14
	 */
}

static void test_2(void)
{
	const struct HXbtree_node *node;
	char buf[80];
	void *trav;

	printf("Test 2A: Traverse with B-tree change\n");
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL) {
		walk_tree(btree->root, buf, sizeof(buf));
		printf("\t" "tree: %s\n", buf);
		printf("\t" " key: %s (%s)\n", node->skey, Color[node->color]);
		if (strcmp(node->skey, "4") == 0) {
			printf("\t" "Deleting [current] node \"4\"\n");
			HXbtree_del(btree, "4");
		} else if (strcmp(node->skey, "12") == 0) {
			printf("\t" "Deleting [next] node \"14\"\n");
			HXbtree_del(btree, "14");
		}
	}

	HXbtrav_free(trav);
	/*        9
	 *      /    \
	 *    6        12
	 *   /        /
	 *  2       10
	 */

	printf("Test 2B: Traverse with B-tree destruction\n");
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL) {
		printf("\t" "About to delete \"%s\"\n", node->skey);
		HXbtree_del(btree, node->key);
	}

	HXbtrav_free(trav);
	printf("\t" "Elements: %u (should be 0)\n", btree->items);
	printf("\t" "Root: %p (should be NULL)\n", btree->root);

	printf("Test 2C: Traversing empty tree\n");
	trav = HXbtrav_init(btree);
	while (HXbtraverse(trav) != NULL)
		printf("\t" "...failed\n");

	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("Test 2D: Pick a memory debugger, check the leaks now.\n");
}

static void test_3(void)
{
	const struct HXbtree_node *node;
	void *trav;

	printf("Test 3: Creating tree with 63 elements (hg 6), "
	       "testing traverser path pickup code\n");
	btree = generate_perfect_tree(6, 1);
	trav = HXbtrav_init(btree);

	printf("\t");
	while ((node = HXbtraverse(trav)) != NULL) {
		printf("%s", node->skey);
		if (strcmp(node->skey, "21") == 0) {
			HXbtree_del(btree, "21");
			printf("*");
		}
		printf(", ");
	}

	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("done\n");
}

static void test_4(void)
{
	unsigned long n;
	int hg, i;

	/*
	 * See how the tree expands under rather bad conditions
	 * (it is not the worst case it seems, however).
	 */
	printf("Test 4A: Tree height expansion check\n");
	btree = HXbtree_init(HXBT_ICMP);

	timer_start();
	for (n = 1; n != 0; ++n) {
		HXbtree_add(btree, (const void *)n);
		if ((n & 0xFFFFF) != 0)
			continue;
		hg = tree_height(btree->root);
		printf("\t%u objects, height %d\n", btree->items, hg);
		if (hg == 47)
			break;
	}
	timer_end();

	/* Once used to see if alignment could be optimized */
	printf("Test 4B: Lookup speed\n");
	for (i = 0; i < 5; ++i) {
		timer_start();
		for (n = btree->items; n >= 1; --n)
			HXbtree_find(btree, (const void *)n);
		timer_end();
	}

	HXbtree_free(btree);
}

static void test_5(void)
{
	/*
	 *         8
	 *       /   \
	 *     4      12
	 *    / \    /  \
	 *   2   6 10    14
	 */
	char buf[80];

	printf("Test 5: Checking simple deletion (replace) logic\n");
	btree = generate_perfect_tree(3, 2);
	walk_tree(btree->root, buf, sizeof(buf));
	printf("\t%s\n", buf);
	HXbtree_del(btree, HX_rand() % 2 ? "2" : "6");
	walk_tree(btree->root, buf, sizeof(buf));
	printf("\t%s\n", buf);
	HXbtree_del(btree, "4");
	walk_tree(btree->root, buf, sizeof(buf));
	printf("\t%s\n", buf);
	HXbtree_free(btree);
}

static void test_6(void)
{
	char buf[80];

	printf("Test 6A: AMOV rebalancing\n");
	btree = generate_fixed_tree(7, 3, 9, 1, 0);
	/* Fixup colors for testcase (eww) */
	HXbtree_find(btree, "3")->color = NODE_RED;
	HXbtree_find(btree, "1")->color = NODE_BLACK;
	walk_tree(btree->root, buf, sizeof(buf));
	printf("\t" "Pre : %s\n", buf);

	/* Add a red node and trigger AMOV case 4 */
	HXbtree_add(btree, "5");
	walk_tree(btree->root, buf, sizeof(buf));
	printf("\t" "Post: %s\n", buf);

	printf("Test 6B: Black height is %d\n",
	       verify_black_height(btree->root));

	HXbtree_free(btree);
}

static void test_7(void)
{
	unsigned int e, order;
	printf("Test 7: DMOV\n");

	for (order = 2; order <= 12; ++order) {
		printf("\t" "Tree of order %u\n", order);
		e = (1 << order) - 1;
		btree = generate_random_tree(e);
		destroy_random_tree(btree, e);
		
	}
}

static void test_8(void)
{
	unsigned int i;

	printf("Test 8: Comparator functions\n");
	btree = HXbtree_init(HXBT_SCMP);
	HXbtree_add(btree, "Hello");
	HXbtree_add(btree, "World");
	HXbtree_free(btree);

	btree = HXbtree_init(HXBT_ICMP);
	for (i = 0; i < 10; ++i)
		HXbtree_add(btree, reinterpret_cast(void *,
		            static_cast(long, i)));
	HXbtree_free(btree);
}

static int t9_fruit_cmp(const void *pa, const void *pb, size_t len)
{
	/* need cast for C++ :-/ */
	const struct t9_fruit *a =
		reinterpret_cast(const struct t9_fruit *, pa);
	const struct t9_fruit *b =
		reinterpret_cast(const struct t9_fruit *, pb);
	return strcmp(a->name, b->name);
}

static int t9_fruit2_cmp(const void *pa, const void *pb, size_t len)
{
	const struct t9_fruit2 *a =
		reinterpret_cast(const struct t9_fruit2 *, pa);
	const struct t9_fruit2 *b =
		reinterpret_cast(const struct t9_fruit2 *, pb);
	return strcmp(a->name, b->name);
}

static void *t9_fruit2_dup(const void *pa, size_t len)
{
	const struct t9_fruit2 *src =
		reinterpret_cast(const struct t9_fruit2 *, pa);
	struct t9_fruit2 *dst;

	dst = reinterpret_cast(struct t9_fruit2 *, HX_memdup(src, sizeof(*src)));
	dst->name = HX_strdup(src->name);
	return dst;
}

static void t9_fruit2_del(void *pa)
{
	struct t9_fruit2 *a = reinterpret_cast(struct t9_fruit2 *, pa);

	if (a == NULL)
		return;
	free(a->name);
	free(a);
}

static void test_9(unsigned int extra_flags)
{
	struct t9_fruit temp_fruit;
	struct t9_fruit2 temp_fruit2;
	const struct HXbtree_node *node;
	unsigned int i;
	void *trav;

	printf("Group 9: HXbtree_init2\n");
	printf("Test 9a: Integer bitmap\n\tcontents:");
	btree = HXbtree_init2(extra_flags, NULL, NULL, NULL, NULL, NULL, 0, 0);
	for (i = 0; i < 15; ++i)
		HXbtree_add(btree, reinterpret_cast(void *,
		            static_cast(long, i)));
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL)
		printf(" %u", static_cast(unsigned int,
		       reinterpret_cast(long, node->key)));
	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("\n");

	printf("Test 9b: String bitmap\n\tThe garden has:");
	btree = HXbtree_init2(HXBT_SKEY | extra_flags, NULL,
	        NULL, NULL, NULL, NULL, 0, 0);
	for (i = 0; i < ARRAY_SIZE(fruits); ++i)
		HXbtree_add(btree, fruits[i]);
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL)
		printf(" %s[%p]", node->skey, node->skey);
	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("\n");

	printf("Test 9c: String bitmap in copy mode\n\tThe garden has:");
	btree = HXbtree_init2(HXBT_SKEY | HXBT_CKEY | extra_flags, NULL,
	        NULL, NULL, NULL, NULL, 0, 0);
	for (i = 0; i < ARRAY_SIZE(fruits); ++i)
		HXbtree_add(btree, fruits[i]);
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL)
		printf(" %s[%p]", node->skey, node->skey);
	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("\n");

	printf("Test 9d: Data bitmap, copy mode\n\tThe garden has:");
	btree = HXbtree_init2(HXBT_CKEY | extra_flags, t9_fruit_cmp,
	        NULL, NULL, NULL, NULL, sizeof(struct t9_fruit), 0);
	for (i = 0; i < ARRAY_SIZE(fruits); ++i) {
		HX_strlcpy(temp_fruit.name, fruits[i], sizeof(temp_fruit.name));
		HXbtree_add(btree, &temp_fruit);
	}
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL) {
		const struct t9_fruit *f =
			reinterpret_cast(const struct t9_fruit *, node->key);
		printf(" %s[%p]", f->name, f->name);
	}
	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("\n");

	printf("Test 9e: Deep data bitmap, copy mode\n\t");
	btree = HXbtree_init2(HXBT_CKEY | extra_flags, t9_fruit2_cmp,
	        t9_fruit2_dup, t9_fruit2_del, NULL, NULL,
	        sizeof(struct t9_fruit2), 0);
	for (i = 0; i < ARRAY_SIZE(fruits); ++i) {
		temp_fruit2.name = const_cast1(char *, fruits[i]);
		HXbtree_add(btree, &temp_fruit2);
	}
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL) {
		const struct t9_fruit2 *f =
			reinterpret_cast(const struct t9_fruit2 *, node->key);
		printf(" %s[%p]", f->name, f->name);
	}
	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("\n");

	printf("Test 9f: Detached string key with data\n\t");
	btree = HXbtree_init2(HXBT_MAP | HXBT_SKEY | HXBT_CDATA | extra_flags,
	        NULL, NULL, NULL, t9_fruit2_dup, t9_fruit2_del, 0, 0);
	for (i = 0; i < ARRAY_SIZE(fruits); ++i) {
		temp_fruit2.name = const_cast1(char *, fruits[i]);
		HXbtree_add(btree, fruits[i], &temp_fruit2);
	}
	trav = HXbtrav_init(btree);
	while ((node = HXbtraverse(trav)) != NULL) {
		const struct t9_fruit2 *f =
			reinterpret_cast(const struct t9_fruit2 *, node->data);
		printf(" %s[%p]:%s[%p]", node->skey, node->skey, f->name, f->name);
	}
	HXbtrav_free(trav);
	HXbtree_free(btree);
	printf("\n");
}

static void test_10(void)
{
	printf("Test 10: Rejection of illegal parameters\n");
	btree = HXbtree_init(HXBT_SCMP | HXBT_ICMP);
	printf("\tscmp|icmp=%p (should be NULL)\n", btree);
}

static void test_11(void)
{
	void *p;

	btree = HXbtree_init(HXBT_SCMP);
	HXbtree_add(btree, "Hello");
	p = HXbtree_del(btree, static_cast(const void *, "Hello"));
	printf(">%s<\n", reinterpret_cast(const char *, p));
	HXbtree_free(btree);

	btree = HXbtree_init(HXBT_SCMP | HXBT_CKEY);
	HXbtree_add(btree, "Hello");
	p = HXbtree_del(btree, static_cast(const void *, "Hello"));
	printf(">%s<\n", reinterpret_cast(const char *, p));
	HXbtree_free(btree);
}

int main(void)
{
	setvbuf(stdout, NULL, _IOLBF, 0);
	setvbuf(stderr, NULL, _IOLBF, 0);

	test_1(); /* allocates */
	test_2(); /* deallocates */
	test_3();
	test_5();
	test_6();
	//test_4();
	test_7();
	test_8();
	test_9(0);
	printf("With CID\n");
	test_9(HXBT_CID);
	test_10();
	test_11();
	return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
static struct HXbtree *generate_fixed_tree(unsigned int f, ...)
{
	struct HXbtree *b;
	char buf[HXSIZEOF_Z32];
	va_list argp;

	b = HXbtree_init(HXBT_CKEY | HXBT_CID | HXBT_CMPFN, strtolcmp);
	if (b == NULL)
		abort();

	va_start(argp, f);
	while (f != 0) {
		snprintf(buf, sizeof(buf), "%u", f);
		HXbtree_add(b, buf);
		f = va_arg(argp, unsigned int);
	}
	va_end(argp);
	return b;
}

static struct HXbtree *generate_perfect_tree(unsigned int height,
    unsigned int mult)
{
	unsigned int right = 1 << height;
	unsigned int incr  = right;
	unsigned int left  = incr / 2;
	unsigned int y, x;
	struct HXbtree *b;
	char buf[HXSIZEOF_Z32];

	b = HXbtree_init(HXBT_CKEY | HXBT_CID | HXBT_CMPFN, strtolcmp);
	if (b == NULL)
		abort();

	for (y = 0; y < height; ++y) {
		for (x = left; x < right; x += incr) {
			snprintf(buf, sizeof(buf), "%u", x * mult);
			HXbtree_add(b, buf);
		}
		incr /= 2;
		left /= 2;
	}

	return b;
}

static struct HXbtree *generate_random_tree(unsigned int elements)
{
	unsigned int range = elements * 4;
	struct HXbtree *ret;

	ret = HXbtree_init(HXBT_ICMP);
	if (ret == NULL)
		abort();

	while (elements--) {
		unsigned long number = HX_irand(1, range);

		if (HXbtree_find(ret, (const void *)number)) {
			++elements;
			continue;
		}
		HXbtree_add(ret, (const void *)number);
	}

	return ret;
}

static void destroy_random_tree(struct HXbtree *tree, unsigned int elements)
{
	unsigned int range = elements * 4;

	while (tree->items > 0) {
		unsigned long number = HX_irand(1, range);

		if (HXbtree_find(tree, (const void *)number) == NULL)
			continue;
		HXbtree_del(tree, (const void *)number);
		if (tree->items > 0)
			verify_random_tree(tree);
	}

	HXbtree_free(tree);
}

static void height_check(const struct HXbtree *tree)
{
	double min, max, avg;
	min = log(tree->items + 1) / log(2);
	max = 2 * log(tree->items + 1) / log(2);
	avg = log((pow(2, min) + pow(2, max)) / 2) / log(2);
	printf("\t" "Item count: %u\n", tree->items);
	printf("\t" "Minimum height: %f\n", min);
	printf("\t" "Average height: %f\n", avg);
	printf("\t" "Maximum height: %f\n", max);
	printf("\t" "Current height: %u\n", tree_height(tree->root));
}

static int sbc_strcmp(const char *result, const char *expected)
{
	int ret = strcmp(result, expected);
	printf("\t" "Expected: %s\n", expected);
	printf("\t" "  Result: %s\n", result);
	if (ret != 0)
		printf("\t\t...failed\n");
	return ret;
}

static int strtolcmp(const void *a, const void *b)
{
	return strtol(static_cast(const char *, a), NULL, 0) -
	       strtol(static_cast(const char *, b), NULL, 0);
}

static inline void timer_start(void)
{
	gettimeofday(&tv_start, NULL);
	printf("Timer started at %ld.%06ld\n",
	       static_cast(long, tv_start.tv_sec),
	       static_cast(long, tv_start.tv_usec));
}

static inline void timer_end(void)
{
	struct timeval tv_end, delta;
	unsigned long sec;
	long acc;

	gettimeofday(&tv_end, NULL);
	printf("Timer ended at: %ld.%06ld\n",
	       static_cast(long, tv_end.tv_sec),
	       static_cast(long, tv_start.tv_usec));

	sec = tv_end.tv_sec  - tv_start.tv_sec;
	acc = tv_end.tv_usec - tv_start.tv_usec;
	if (acc < 0) {
		delta.tv_sec  = sec - 1;
		delta.tv_usec = 1000000 + acc;
	} else {
		delta.tv_sec  = sec;
		delta.tv_usec = acc;
	}

	printf("Timer difference: %ld.%06ld\n",
	       static_cast(long, delta.tv_sec),
	       static_cast(long, delta.tv_usec));
}

static unsigned int tree_height(const struct HXbtree_node *node)
{
	unsigned int a = 1, b = 1;
	if (node->sub[0] != NULL)
		a += tree_height(node->sub[0]);
	if (node->sub[1] != NULL)
		b += tree_height(node->sub[1]);
	return (a > b) ? a : b;
}

static int verify_black_height(const struct HXbtree_node *node)
{
	unsigned int lh = 0, rh = 0;

	if (node->sub[S_LEFT] != NULL)
		if ((lh = verify_black_height(node->sub[S_LEFT])) ==
		    static_cast(unsigned int, -1))
			return -1;
	if (node->sub[S_RIGHT] != NULL)
		if ((rh = verify_black_height(node->sub[S_RIGHT])) ==
		    static_cast(unsigned int, -1))
			return -1;
	if (node->sub[S_LEFT] != NULL && node->sub[S_RIGHT] != NULL)
		if (lh != rh)
			return -1;
	if (node->sub[S_LEFT] != NULL)
		return lh + (node->color == NODE_BLACK);
	else
		return rh + (node->color == NODE_BLACK);
}

static unsigned int verify_red_no_red_children(const struct HXbtree_node *node)
{
	unsigned int ret = 1;

	if (node->sub[S_LEFT] != NULL) {
		if (node->color == NODE_RED &&
		    node->sub[S_LEFT]->color == NODE_RED)
			return 0;
		ret &= verify_red_no_red_children(node->sub[S_LEFT]);
	}
	if (node->sub[S_RIGHT] != NULL) {
		if (node->color == NODE_RED &&
		    node->sub[S_RIGHT]->color == NODE_RED)
			return 0;
		ret &= verify_red_no_red_children(node->sub[S_RIGHT]);
	}

	return ret;
}

static unsigned int verify_random_tree(const struct HXbtree *tree)
{
	int h;

	/* Root is black */
	if (tree->root->color != NODE_BLACK) {
		printf("\t" "Root is not black\n");
		return 0;
	}

	/* A red node may not have any red children */
	if (!verify_red_no_red_children(tree->root)) {
		printf("\t" "Red node may not have red children violated\n");
		return 0;
	}

	if ((h = verify_black_height(tree->root)) == -1) {
		printf("\t" "Black height violated\n");
		return 0;
	}

	return 1;
}

static void __walk_tree(const struct HXbtree_node *node, char *buf, size_t s)
{
	int has_children = node->sub[0] != NULL || node->sub[1] != NULL;
	HX_strlcat(buf, node->skey, s);

	if (node->color == NODE_BLACK)
		HX_strlcat(buf, "%b", s);
	if (has_children)
		HX_strlcat(buf, "(" /* ) */, s);
	if (node->sub[0] != NULL)
		__walk_tree(node->sub[0], buf, s);
	if (node->sub[1] != NULL) {
		HX_strlcat(buf, ",", s);
		__walk_tree(node->sub[1], buf, s);
	}
	if (has_children)
		HX_strlcat(buf, /* ( */ ")", s);
}

static void walk_tree(const struct HXbtree_node *node, char *buf, size_t s)
{
	*buf = '\0';
	__walk_tree(node, buf, s);
}
