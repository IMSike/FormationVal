#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "malloc.h"

static pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

void	lock()
{
  if (pthread_mutex_lock(&g_mutex) != 0)
    raise(errno);
}

void	unlock()
{
  if (pthread_mutex_unlock(&g_mutex) != 0)
    raise(errno);
}