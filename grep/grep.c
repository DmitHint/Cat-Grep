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
    free(re_list);

    return 0;
}

void pattern_add(arguments *opts, char *pattern) {
    opts->patterns = realloc(opts->patterns, sizeof(char *) * (++opts->patterns_len));
    opts->patterns[opts->patterns_len - 1] = malloc(sizeof(char) * (strlen(pattern) + 1));
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
        pattern_add(opts, line);
    }

}

arguments parse_arguments(int argc, char **argv) {
    int opt;
    arguments opts = {0};

    const char *short_options = "e:ivclnhsf:o";

    opts.patterns = malloc(0 * sizeof(char *));

    while ((opt = getopt(argc, argv, short_options)) != -1) {
        switch (opt) {
            case 'e':
                opts.e = 1;
                pattern_add(&opts, optarg);
                break;
            case 'i':
                opts.i = REG_ICASE;
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
                regs_from_file(&opts, optarg);
                break;
            case 'o':
                opts.o = 1;
                break;
            case '?':
                fprintf(stderr, "Usage: grep [OPTION]... PATTERNS [FILE]...");
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Unexpected error\n");
                exit(EXIT_FAILURE);
        }
    }

    if (opts.patterns[0] == NULL)
        pattern_add(&opts, argv[optind++]);

    if (argc - optind == 1) {
        opts.h = 1;
    }

    return opts;
}

void output_line(char *line, int len) {
    for (int i = 0; i < len; ++i) {
        putchar(line[i]);
    }
    if (line[len - 1] != '\n') putchar('\n');
}

// void process_file(arguments arg, char *path, regex_t *reg) {
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
    int read = 0;
    int line_count = 0;
    int reg_count = 0;

    while ((read = getline(&line, &memlen, f)) != -1) {
        line_count++;
        bool isResult = false;
        for (int i = 0; !isResult && i < arg.patterns_len; i++) {
            regex_t *re = &re_list[i];

            int result = regexec(re, line, 0, NULL, 0);

            if ((result == 0 && !arg.v) || (result != 0 && arg.v)) {
                if (!arg.c && !arg.l) {
                    if (!arg.h) printf("%s:", path);
                    if (arg.n) printf("%d:", line_count);
                    if (arg.o) {
                        print_match(re, line);

                    } else {
                        output_line(line, read);
                    }
                }
                reg_count++;
                isResult = true;
            }
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

void print_match(regex_t *re, char *line) {
    regmatch_t match;
    int offset = 0;
    while (true) {
        int result = regexec(re, line + offset, 1, &match, 0);

        if (result != 0)
            break;


        for (int i = match.rm_so; i < match.rm_eo; ++i) {
            putchar(line[i]);
        }
        putchar('\n');
//        offset += match.rm_eo;
        offset += match.rm_eo + 1;
    }

}

