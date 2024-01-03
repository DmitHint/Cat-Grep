#include "cat.h"

int main(int argc, char *argv[]) {
  arguments args = parse_arguments(argc, argv);
  output(&args, argc, argv);
  return 0;
}

char change_v(char ch) {
  if (ch == '\n' || ch == '\t') return ch;
  if (ch < 0) {
    printf("M-");
    ch = ch & 0x7F;
  }
  if (ch <= 31) {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  }
  return ch;
}

void print_line(arguments args, char *line, int len) {
  for (int i = 0; i < len; i++) {
    if (args.T && line[i] == '\t')
      printf("^I");
    else {
      if (args.E && line[i] == '\n') putchar('$');

      if (args.v) line[i] = change_v(line[i]);

      putchar(line[i]);
    }
  }
}

int show_file(arguments *args, FILE *file, int *line_count) {
  char *line = NULL;
  size_t memlen = 0;
  int read;

  int empty_count = 0;

  while ((read = getline(&line, &memlen, file)) != -1) {
    if (line[0] == '\n')
      empty_count++;
    else
      empty_count = 0;

    if (!(args->s && empty_count > 1)) {
      if (args->n || (args->b && line[0] != '\n')) {
        printf("%6d\t", *line_count);
        if (!(args->b && line[0] == '\n') || args->n) (*line_count)++;
      }
      print_line(*args, line, read);
    }
  }
  free(line);

  return 0;
}

arguments parse_arguments(int argc, char **argv) {
  int arg;
  arguments args = {0};

  const char *short_options = "bnseEtTv";

  const struct option long_options[] = {
      {"input", no_argument, NULL, 'i'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  while ((arg = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (arg) {
      case 'b':
        args.b = 1;
        break;
      case 'n':
        args.n = 1;
        break;
      case 's':
        args.s = 1;
        break;
      case 'e':
        args.E = 1;
        args.v = 1;
        break;
      case 'E':
        args.E = 1;
        break;
      case 'v':
        args.v = 1;
        break;
      case 't':
        args.T = 1;
        args.v = 1;
        break;
      case 'T':
        args.T = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [OPTION] [FILE]\n", argv[0]);
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "Unexpected error\n");
        exit(EXIT_FAILURE);
    }
  }

  if (args.b == 1) args.n = 0;

  return args;
}

int output(arguments *args, int argc, char **argv) {
  FILE *file;
  int line_count = 1;
  for (int i = optind; i < argc; i++) {
    file = fopen(argv[i], "r");
    if (file == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      perror("");
      continue;
    }
    show_file(args, file, &line_count);
    fclose(file);
  }
  return 0;
}