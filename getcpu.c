#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int msleep(long msec);
long long *getproc(const char *PATH);
int main(int argc, char* argv[]) {
    const char *PATH = "/proc/stat";
    long sleep_time;
    int round = 0; 
    if (argc >= 2){
        char* str = argv[1];
        for (int i = 0; str[i] != 0; i++){
            if (str[i] > '9' || str[i] < '0'){
                printf("Usage: getcpu t\nWhere t is time in milliseconds to wait before updating cpu usage (recommended 900)\nYou can also pass r as an argument to round the returned value.\nExample: getcpu 900 r\n");
                return 1;
            }
        }
        sleep_time = atol(str);
        if (sleep_time < 200){
            printf("Minimum t is 200 ms\n");
            return 1;
        }
        if (argc >= 3 && argv[2][0] == 'r'){
            round = 1;
        }
    }
    else {
        printf("Usage: getcpu t\nWhere t is time in milliseconds to wait before updating cpu usage (recommended 900)\nYou can also pass r as an argument to round the returned value.\nExample: getcpu 900 r\n");
        return 1;
    }
    while (1){

    long long *before = getproc(PATH);
    msleep(sleep_time);
    long long *after = getproc(PATH);
    long long total = after[0] - before[0];
    long long cpu = after[1] - before[1];
    long long idle = after[2] - before[2];
    long double perc = 100 - ( 100*(idle) / ((long double)total));
    if (round){
        printf("%.0Lf\n", roundl(perc));
    } else{
        printf("%.2Lf\n", perc);
    }

    free(before);
    free(after);
    }


    return 0;
}
long long *getproc(const char *PATH){

    int fd = open(PATH, O_RDONLY);
    if (fd == -1){
        printf("ERROR: %s\n", strerror(errno));
        return NULL;
    }
    const int buffsize = 256;
    char buff[256];
    int fileread;
    char *words[11];
    fileread = read(fd, buff, buffsize);
    int word_index = 0;
    int letter_index = 0;
    char* current_ptr = (char*) malloc(10*sizeof(char));
    for (int i = 0; i < buffsize; i++){
        if (buff[i] == ' '){
            current_ptr[letter_index] = 0;
            words[word_index] = current_ptr;
            word_index++;
            letter_index = 0;
            current_ptr = (char*) malloc(10*sizeof(char));
            continue;
        }
        else if (buff[i] == '\n')
        {
            words[word_index] = current_ptr;
            word_index++;
            break;
        }
        current_ptr[letter_index] = buff[i];
        letter_index++;
    }
    close(fd);
    
    
    /*
    long long user = atoll(words[2]);
    long long nice = atoll(words[3]);
    long long system = atoll(words[4]);
    long long idle = atoll(words[5]);
    long long iowait = atoll(words[6]);
    long long irq = atoll(words[6]);
    long long softirq = atoll(words[7]);
    long long steal = atoll(words[8]);
    long long guest = atoll(words[9]);
    long long guest_nice = atoll(words[10]);
    long long cpu = user+nice+system+iowait+irq+softirq+steal+guest+guest_nice;
    Commented because now i use a for loop which is better
    */

    long long cpu = 0;
    long long idle;
    for (int i = 2; i < word_index; i++){
        if (i == 5){
            idle = atoll(words[i]);
            continue;
        }
        cpu += atoll(words[i]);
    }
    long long total = cpu+idle;
    long long *result = (long long*) malloc(3*sizeof(long long));
    result[0] = total;
    result[1] = cpu;
    result[2] = idle;

    for (int i = 0; i < word_index; i++){
        free(words[i]);
    }

    return result;
}
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}