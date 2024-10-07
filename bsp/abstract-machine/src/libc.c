#include <klib.h>
#include <rtthread.h>
char *strchr(const char *s, int c) {
	for(int i=0;i<strlen(s);i++){
		if(s[i]==c) return (char *)s+i; 
	}
	return NULL; 
}

char *strrchr(const char *s, int c) {
	int s_len=strlen(s);
	for(int i=s_len-1;i>0;i--){
		if(s[i]==c) return (char *)s+i;
	}
  return NULL;
}

char *strstr(const char *haystack, const char *needle) {
  return rt_strstr(haystack, needle);
}

long strtol(const char *restrict nptr, char **restrict endptr, int base) {
  assert(0);
}

// char *strncat(char *restrict dst, const char *restrict src, size_t sz) {
//   assert(0);
// }
