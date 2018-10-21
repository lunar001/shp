#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>

#define COLOR_NONE  "\e[0m"
#define COLOR_RED   "\e[1;31m"
#define COLOR_BLUE  "\e[1;34m"

#define log_info(fmt, ...)  \
{
	fprintf(stdout, COLOR_BLUE);\
	fprintf(stdout, "log_info:%s:%s:%d:>> ", __FILE__, __func__, __LINE__);\
	fprintf(stdout, fmt, ##__VA_ARGS__);\
	fprintf(stdout, COLOR_NONE);
}

#ifdef D

#define log_debug(fmt, ...) \
{
	fprintf(stdout, COLOR_BLUE);\
	fprintf(stdout, "log_debug: %s:%s:%d: >> ", __FILE__, __func__, __LINE__);\
	fprintf(stdout, fmt, ##__VA_ARGS__);\
	fprintf(stdout, COLOR_NONE);
}
	


#else

#define log_debug(fmat, ...)

#endif

#define log_warning(fmt, ...) \
{
	fprintf(stdout, COLOR_RED); \
	fprintf(stdout, "log_warning:%s:%s:%d:>> ", __FILE__, __func__, __LINE__);\
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, COLOR_NONE);
}


#define log_error(fmt, ...) \
{
	fprintf(stderr, COLOR_RED);\
	fprintf(stderr, "log_error:%s:%s:%d:>> ", __FILE__, __func__, __LINE__);\
	fprintf(stderr, fmt, ##__VA_ARGS__);\
	fprintf(stdout, COLOR_NONE);
}

#endif /* #ifndef COMMON_H */
