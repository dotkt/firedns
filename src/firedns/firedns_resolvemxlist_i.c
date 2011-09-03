#include "firedns_internal.h"

struct firedns_mxlist *firedns_resolvemxlist_i(firedns_state* self, const char* name, char *(* const result)(firedns_state*, int)) {
	int fd;
	int t,i;
	struct firedns_mxlist* ret;
	fd_set s;
	struct timeval tv;
	for (t = 0; t < FIREDNS_TRIES; t++) {
		fd = firedns_getmxlist(self, name);
		if (fd == -1)
			return NULL;
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_ZERO(&s);
		FD_SET(fd,&s);
		i = select(fd + 1,&s,NULL,NULL,&tv);
		ret = (struct firedns_mxlist *) result(self, fd);
		if (ret != NULL || i != 0)
			return ret;
	}
	return NULL;
}

