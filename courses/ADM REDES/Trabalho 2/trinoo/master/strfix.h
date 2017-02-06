#ifndef _lint
#ifdef __GNUC__
#ifndef _STRFIX_H
#define _STRFIX_H
#define strcpy(dst, src) \
({ \
	char *_out = (dst); \
	if (sizeof(dst) <= sizeof(char *)) \
		_out = strcpy(_out, (src)); \
	else { \
		*_out = 0; \
		_out = strncat(_out, (src), sizeof(dst) - 1); \
	} \
	_out; \
})
#define strcat(dst, src) \
({ \
	char *_out = (dst); \
	if (sizeof(dst) <= sizeof(char *)) \
		_out = strcat(_out, (src)); \
	else { \
		size_t _size = sizeof(dst) - strlen(_out) - 1; \
		if (_size > 0) _out = strncat(_out, (src), _size); \
	} \
	_out; \
})
#endif
#ifndef _STDIOFIX_H
#define _STDIOFIX_H
#endif
#endif
#endif

