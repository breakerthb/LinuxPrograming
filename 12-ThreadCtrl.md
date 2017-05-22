# 线程控制

# 1. 线程限制

通过`sysconf`函数查询

# 2. 线程属性

创建和销毁进程属性。负责申请和释放内存空间。

```cpp
#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
```

### 返回值

- 0 if OK
- error number on failure

如果不关心进程的终止状态，可以使用`pthread_detach`让操作系统回收。如果创建时就不关心，可以让线程处于分离状态创建。

```cpp
#include <pthread.h>
int pthread_attr_getdetachstate(const pthread_attr_t *restrict attr, int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
```
### return
- 0 if OK
- error number on failure

### Demo

```cpp
#include "apue.h"
#include <pthread.h>
int makethread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;
    err = pthread_attr_init(&attr);
    if (err != 0)
        return(err);
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (err == 0)
        err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);
    return(err);
}
```

管理线程属性

```cpp
#include <pthread.h>
int pthread_attr_getstack(const pthread_attr_t *restrict attr, void **restrict stackaddr, size_t *restrict stacksize);
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
// Both return: 0 if OK, error number on failure


#include <pthread.h>
int pthread_attr_getstacksize(const pthread_attr_t *restrict attr,
size_t *restrict stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
Both return: 0 if OK, error number on failure

#include <pthread.h>
int pthread_attr_getguardsize(const pthread_attr_t *restrict attr,
size_t *restrict guardsize);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
Both return: 0 if OK, error number on failure

```

# 4. 同步属性

## 4.1 互斥量属性

## 4.2 读写锁属性

## 4.3 条件变量属性

## 4.4 屏障属性

# 5. 重入

# 6. 线程特定数据

# 7. 取消选项

# 8. 线程和信号

# 9. 线程和fork

# 10. 线程和I/O



