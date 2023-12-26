#include "grep.h"

#include <stdio.h>

// arguments parse_arguments(int argc, char **argv, int *idx) {
// void add_reg_from_file(arguments *args, char *filepath) {
//   FILE *f = fopen(filepath, "r");
//   if (f == NULL) {
//     if (!args->s) perror(filepath);
//     exit(EXIT_FAILURE);
//   }
//   char *line = NULL;
//   size_t memlen = 0;
//   int read = 0;
//   while ((read = getline(&line, &memlen, f)) != -1) {
//     // ?
//   }
//   free(line);
//   fclose(f);
// }

arguments parse_arguments(int argc, char **argv) {
  int opt;
  arguments opts = {0};

  const char *short_options = "eivclnhsfo";

  const struct option long_options[] = {{"f", required_argument, NULL, 'f'},
                                        {NULL, 0, NULL, 0}};

  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'e':
        opts.e = 1;
        opts.pattern = optarg;
        break;
      case 'i':
        opts.i = 1;
        break;
      case 'v':
        opts.v = 1;
        break;
      case 'c':
        opts.c = 1;
        break;
      case 'l':
        opts.l = 1;
        break;
      case 'n':
        opts.n = 1;
        break;
      case 'h':
        opts.h = 1;
        break;
      case 's':
        opts.s = 1;
        break;
      case 'f':
        opts.f = 1;
        // ?
        break;
      case 'o':
        opts.o = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [OPTION] template [FILE]\n", argv[0]);
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "Unexpected error\n");
        exit(EXIT_FAILURE);
    }
  }
  if (opts.pattern == NULL) {
    opts.pattern = argv[optind++];
  }
  // (*idx) = optind;
  return opts;
}

void output_line(char *line, int len) {
  for (int i = 0; i < len; ++i) {
    putchar(line[i]);
  }
  // ?
}

void process_file(arguments arg, char *path, regex_t *reg) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    if (!arg.s) perror(path);
    exit(EXIT_FAILURE);
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = 0;

  while ((read = getline(&line, &memlen, f)) != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if (result == 0) output_line(line, read);
  }
  free(line);
  fclose(f);
}

// void print_match(arguments args, regex_t *reg, char *line, char *path,
//                  int line_count) {
void print_match(regex_t *reg, char *line) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(reg, line + offset, 1, &match, 0);
    if (result != 0) {
      break;
    }
    // print_flags(arg, path, line_count);
    for (int i = match.rm_so; i < match.rm_eo; ++i) {
      putchar(line[offset + i]);
    }
    putchar('\n');
    offset += match.rm_eo;
  }
}

void output(int argc, char **argv) {
  arguments arg = parse_arguments(argc, argv);
  regex_t re;
  int error = regcomp(&re, arg.pattern, 0);
  if (error) perror("Error");
  for (int i = optind; i < argc; ++i) {
    process_file(arg, argv[i], &re);
  }
}

int main(int argc, char *argv[]) {
  // arguments args = parse_arguments(argc, argv);
  output(argc, argv);

  return 0;
}
