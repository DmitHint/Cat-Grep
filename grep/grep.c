#include "grep.h"

int main(int argc, char *argv[]) {
  arguments arg = parse_arguments(argc, argv);

  regex_t *re_list = malloc(arg.patterns_len * sizeof(regex_t));

  for (int i = 0; i < arg.patterns_len; i++) {
    regcomp(&re_list[i], arg.patterns[i], arg.i);
  }

  for (int i = optind; i < argc; ++i) {
    process_file(arg, argv[i], re_list);
  }

  for (int i = 0; i < arg.patterns_len; i++) {
    free(arg.patterns[i]);
  }
  free(arg.patterns);

  for (int i = 0; i < arg.patterns_len; i++) {
    regfree(&re_list[i]);
  }

  free(re_list);

  return 0;
}

void pattern_add(arguments *opts, char *pattern) {
  opts->patterns =
      realloc(opts->patterns, sizeof(char *) * (++opts->patterns_len));
  opts->patterns[opts->patterns_len - 1] =
      malloc(sizeof(char) * (strlen(pattern) + 1));
  strcpy(opts->patterns[opts->patterns_len - 1], pattern);
}

void regs_from_file(arguments *opts, char *reg_path) {
  FILE *f = fopen(reg_path, "r");

  if (f == NULL) {
    if (!opts->s) {
      fprintf(stderr, "grep: %s: ", reg_path);
      perror("");
    }
    exit(EXIT_FAILURE);
  }

  char *line = NULL;
  size_t memlen = 0;
  while ((getline(&line, &memlen, f)) != -1) {
    line[strcspn(line, "\n")] = '\0';
    pattern_add(opts, line);
  }

  free(line);
  fclose(f);
}

arguments parse_arguments(int argc, char **argv) {
  int opt;
  arguments args = {0};

  const char *short_options = "e:ivclnhsf:o";

  args.patterns = malloc(0 * sizeof(char *));

  while ((opt = getopt(argc, argv, short_options)) != -1) {
    switch (opt) {
      case 'e':
        args.e = 1;
        pattern_add(&args, optarg);
        break;
      case 'i':
        args.i = REG_ICASE;
        break;
      case 'v':
        args.v = 1;
        break;
      case 'c':
        args.c = 1;
        break;
      case 'l':
        args.l = 1;
        break;
      case 'n':
        args.n = 1;
        break;
      case 'h':
        args.h = 1;
        break;
      case 's':
        args.s = 1;
        break;
      case 'f':
        args.f = 1;
        regs_from_file(&args, optarg);
        break;
      case 'o':
        args.o = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...");
        exit(EXIT_FAILURE);
      default:
        fprintf(stderr, "Unexpected error\n");
        exit(EXIT_FAILURE);
    }
  }

  if (args.patterns[0] == NULL)
    if (argv[optind]) pattern_add(&args, argv[optind++]);

  if (argc - optind == 1) {
    args.h = 1;
  }

  return args;
}

void output_line(char *line, int len) {
  for (int i = 0; i < len; ++i) {
    putchar(line[i]);
  }
  if (line[len - 1] != '\n') putchar('\n');
}

int check_line(regex_t *re_list, arguments arg, char *line) {
  if (arg.v) {
    for (int i = 0; i < arg.patterns_len; i++) {
      regex_t *re = &re_list[i];
      int result = regexec(re, line, 0, NULL, 0);
      if (result == 0) return -1;
    }
    return 0;
  } else {
    for (int i = 0; i < arg.patterns_len; i++) {
      regex_t *re = &re_list[i];
      int result = regexec(re, line, 0, NULL, 0);
      if (result == 0) return i;
    }
    return -1;
  }
}

void process_file(arguments arg, char *path, regex_t *re_list) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    if (!arg.s) {
      fprintf(stderr, "grep: %s: ", path);
      perror("");
    }
    return;
  }

  char *line = NULL;
  size_t memlen = 0;
  int read;
  int line_count = 0;
  int reg_count = 0;

  while ((read = getline(&line, &memlen, f)) != -1) {
    line_count++;

    int re_idx = check_line(re_list, arg, line);

    if (re_idx != -1) {
      if (!arg.c && !arg.l) {
        if (!arg.h && !arg.o) printf("%s:", path);
        if (arg.n && !arg.o) printf("%d:", line_count);
        if (arg.o) {
          print_match(&re_list[re_idx], line, line_count, arg, path);
        } else {
          output_line(line, read);
        }
      }
      reg_count++;
    }
  }

  if (arg.l && reg_count > 0) printf("%s\n", path);

  if (arg.c && !arg.l) {
    if (!arg.h) printf("%s:", path);
    printf("%d\n", reg_count);
  }

  free(line);
  fclose(f);
}

void print_match(regex_t *re, char *line, int line_count, arguments args,
                 char *path) {
  regmatch_t matches[1];
  size_t offset = 0;
  while (regexec(re, line + offset, 1, matches, 0) == 0) {
    if (!args.h) printf("%s:", path);
    if (args.n) printf("%d:", line_count);

    printf("%.*s\n", (int)(matches[0].rm_eo - matches[0].rm_so),
           line + offset + matches[0].rm_so);
    offset += matches[0].rm_eo;
  }
}