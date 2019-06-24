#include "ga.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *getfield(char *line, int num) {
  char *token = strtok(line, ";");
  while (token) {
    if (num == 0) {
      return token;
    }
    --num;
    token = strtok(NULL, ";\n");
  }
  return NULL;
}

int performanceCheck() {
  FILE *stream;
  char line[2048];
  char *tmp, *num_players_str, *delta_str, *cpu_time_str, *iterations_str;
  int num_players, delta, iterations;
  double cpu_time;

  stream = fopen("out.csv", "r");
  fgets(line, 2048, stream);
  while (fgets(line, 2048, stream)) {
    tmp = strdup(line);
    num_players_str = getfield(tmp, 0);

    tmp = strdup(line);
    delta_str = getfield(tmp, 1);

    tmp = strdup(line);
    cpu_time_str = getfield(tmp, 2);

    tmp = strdup(line);
    iterations_str = getfield(tmp, 3);

    num_players = atoi(num_players_str);
    delta = atoi(delta_str);
    cpu_time = atof(cpu_time_str);
    iterations = atoi(iterations_str);

    if (num_players >= 200 && cpu_time > 90.0) {
      printf("%d took %fs, max allowed: %ds.\n", num_players, cpu_time, 90);
      fclose(stream);
      return 1;
    } else if (num_players >= 100 && num_players < 200 && cpu_time > 60.0) {
      printf("%d took %fs, max allowed: %ds.\n", num_players, cpu_time, 60);
      fclose(stream);
      return 1;
    } else if (num_players < 100 && cpu_time > 30.0) {
      printf("%d took %fs, max allowed: %ds.\n", num_players, cpu_time, 30);
      fclose(stream);
      return 1;
    }

    if (iterations < 75000 && delta > 3) {
      printf("Delta %d after %d iterations insufficient.\n", delta, iterations);
      fclose(stream);
      return 1;
    }
  }
  fclose(stream);
  return 0;
}

int main(int argc, const char *argv[]) {
  FILE *fp;
  FILE *stream;
  char line[2048];
  fp = fopen("out.csv", "w");
  fprintf(fp, "num_players;delta;cpu_time;"
              "iterations;score\n");
  fclose(fp);

  printf("num_players;delta;cpu_time;"
         "iterations;score\n");

  if (access("in.csv", R_OK) == -1) {
    printf("Please provide in.csv. File cannot be read.\n");
    exit(1);
  }
  stream = fopen("in.csv", "r");
  while (fgets(line, 2048, stream)) {
    int num_plyrs, num_tms, delta, it, i;
    double variance, cpu_time;
    clock_t start, end;
    int *plyrs_arr, *result;
    char *plyrs, *num_players, *tmp, *token;

    tmp = strdup(line);
    num_players = getfield(tmp, 0);

    tmp = strdup(line);
    plyrs = getfield(tmp, 1);

    num_plyrs = atoi(num_players);
    num_tms = num_plyrs / 4;

    tmp = strdup(plyrs);
    plyrs_arr = malloc(num_plyrs * sizeof(int));
    token = strtok(tmp, ",");
    i = 0;
    while (token) {
      plyrs_arr[i++] = atoi(token);
      token = strtok(NULL, ",\n");
    }

    start = clock();
    result = run(num_plyrs, plyrs_arr, num_tms, &variance);
    end = clock();
    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    delta = result[num_plyrs];
    it = result[(num_plyrs + 1)];

    free(plyrs_arr);
    free(result);

    fp = fopen("out.csv", "a");
    fprintf(fp, "%d;%d;%f;%d;%f\n", num_plyrs, delta, cpu_time, it, variance);
    fclose(fp);
    printf("%d;%d;%f;%d;%f\n", num_plyrs, delta, cpu_time, it, variance);
  }
  fclose(stream);

  return performanceCheck();
}
