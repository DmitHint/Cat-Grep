#include "grep.h"

typedef struct arguments {
  int b, n, s, E, T, v;
} arguments;

arguments parse_arguments(int argc, char **argv, int *idx) {
  int opt;
  arguments opts = {0};

  const char *short_options = "bnseEtTv";

  const struct option long_options[] = {
      {"input", no_argument, NULL, 'i'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        opts.b = 1;
        break;
      case 'n':
        opts.n = 1;
        break;
      case 's':
        opts.s = 1;
        break;
      case 'e':
        opts.E = 1;
        opts.v = 1;
        break;
      case 'E':
        opts.E = 1;
        break;
      case 't':
        opts.T = 1;
        opts.v = 1;
        break;
      case 'T':
        opts.T = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [OPTION] [FILE]\n", argv[0]);
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "Unexpected error\n");
        exit(EXIT_FAILURE);
    }
  }
  (*idx) = optind;
  return opts;
}

int main(int argc, char *argv[]) {
  char *line = "Hello world";

  regex_t re;
  regcomp(&re, "pattern", REG_ICASE);
  int result = regexec(&re, line, 0, NULL, 0);

  printf("%d\n", result);
  printf("%s\n", line);

  return 0;
}
