#ifndef _LDSDR_
#define _LDSDR_

unsigned int create_shader(const char *, unsigned int);
unsigned int create_prog(const char *, const char *);
void link_use_prog(unsigned int);
void delete_shaders(void);
void set_uniform1f(unsigned int , const char *, float);

#endif