#include "perf_man.h"

static FILE* file;
static struct timeval t1, t2;

void init_timer(int num_print, int num_chunk) {
    char fn[100];
    sprintf(fn, "%s_%d_%d%s", FILE_NAME, num_chunk, num_print, FILE_EXT); 
    file = fopen(fn, "w");
}

void start_timer() {
    gettimeofday(&t1, NULL);
}

void write_data(char* data) {
    fwrite(data, sizeof(char), strlen(data), file);
    fflush(file);
}

void stop_timer() {
   stop_timer_message(""); 
}

void stop_timer_message(char* message) {
    printf("%s\n", message);
    double elapsedTime;
    gettimeofday(&t2, NULL);

    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

    char final_msg[strlen(message) + 11];
    sprintf(final_msg, "%s: %.10f ms.\n", message, elapsedTime);
    
    write_data(final_msg);
}

