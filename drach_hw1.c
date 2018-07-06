#include <time.h>
#include <stdio.h>
#include <openssl/md5.h>

double timespec_diff(struct timespec *stop, struct timespec *start)
{
    double diff = difftime(stop->tv_sec, start->tv_sec);
    diff *= 1000;
    diff += (stop->tv_nsec - start->tv_nsec) / 1e6;
    return diff;
}

void makeHash(char *filename, unsigned char* hash) {
    int i;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        return;
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0) {
        MD5_Update (&mdContext, data, bytes);
    }

    MD5_Final (hash, &mdContext);

    fclose (inFile);
}

int main()
{
    struct timespec time_now, time_after;
    char filename[256];
    unsigned char hash[MD5_DIGEST_LENGTH];
    printf("Enter path to file: ");
    scanf("%s", filename);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_now);
    makeHash(filename, hash);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time_after);

    printf("Hash for %s -> ", filename);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }


    printf("\nHashing took %g ms\n",
            timespec_diff(&time_after, &time_now));

    return 0;
}