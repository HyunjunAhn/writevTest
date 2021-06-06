#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/uio.h>
#include <time.h>

typedef struct _iov_t
{
    struct iovec vec[1024];
    int cnt;
    char *buf;
    int ofs;
} iov_t;

int iprintf(iov_t *iov, char *fmt, ...)
{
    int data_len;
    char *buf = iov->buf;
    int *ofs = &iov->ofs;
    struct iovec *vec = iov->vec;
    int *cnt = &iov->cnt;

    va_list lpStart;
    va_start(lpStart, fmt);
    data_len = vsprintf(buf + *ofs, fmt, lpStart);
    va_end(lpStart);
    vec[*cnt].iov_base = buf + *ofs;
    vec[*cnt].iov_len = data_len;
    *ofs += data_len;
    (*cnt)++;
    return *cnt;
}

int main()
{
    int cnt = 0;
    int fd = 0;
    int str_len = 0;

    iov_t iov = {0};
    iov.buf = (char *)malloc(20000);

    clock_t start1, start2, end1, end2;
    float res1, res2;

    start1 = clock();
    for (int i = 0; i < 256; i++)
    {
        cnt = iprintf(&iov, "[%d] %s\n", cnt, "aaaa");
        cnt = iprintf(&iov, "[%d] %s\n", cnt, "bbbbbbbbb");
        cnt = iprintf(&iov, "[%d] %s\n", cnt, "kkkkk");
        cnt = iprintf(&iov, "[%d] %s\n", cnt, "cc");
    }

    str_len = writev(fd, iov.vec, cnt);
    end1 = clock();
    res1 = (float)(end1 - start1) / CLOCKS_PER_SEC;

    
    cnt = 0;
    start2 = clock();
    for (int i = 0; i < 256; i++)
    {
        printf("[%d] %s\n", cnt, "aaaa");
        cnt++;
        printf("[%d] %s\n", cnt, "bbbbbbbbb");
        cnt++;
        printf("[%d] %s\n", cnt, "kkkkk");
        cnt++;
        printf("[%d] %s\n", cnt, "cc");
        cnt++;
    }
    end2 = clock();
    res2 = (float)(end2 - start2) / CLOCKS_PER_SEC;
    
    printf("elapsed time for iprintf: %f\n", res1);
    printf("elapsed time for  printf: %f\n", res2);

    //printf("write bytes: %d\n", str_len);
    //puts("");

    free(iov.buf);
    return 0;
}