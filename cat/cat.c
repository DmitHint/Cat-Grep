#include "cat.h"

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
        if (args.T && line[i] == '\t') printf("^I");
        else {
            if (args.E && line[i] == '\n') putchar('$');

            if (args.v) {
                line[i] = change_v(line[i]);
            }
            putchar(line[i]);
        }
    }
}

int show_file(arguments *args, FILE *file) {
    char *line = NULL;
    size_t memlen = 0;
    int read;

    int line_count = 1;

    int empty_count = 0;

//    TODO: remove empty_count and fix -s

    while ((read = getline(&line, &memlen, file)) != -1) {

        if (line[0] == '\n') empty_count++;
        else empty_count = 0;

        if (!(args->s && empty_count > 1)) {
            if (args->n || args->b) {
                if (args->b && line[0] != '\n') {
                    printf("%6d\t", line_count);
                } else if (args->n) {
                    printf("%6d\t", line_count);
                }
            }
            line_count++;
            print_line(*args, line, read);
        }
    }
    free(line);

    return 0;
}

//int show_file(arguments *args, FILE *file) {
//    char ch;
//
//    if (args->n == 1 && args->b == 1) {
//        args->n = 0;
//    }
//
//    int isEmptyLine = 0;
//    int isNewLine = 1;
//    int counter = 1;
//
//    while ((ch = fgetc(file)) != EOF) {
//        if (args->v) {
//            ch = change_v(ch);
//        }
//        if (args->T && ch == '\t')
//            if ((ch = 'I')) putchar('^');
//
//        if ((args->n || (args->b && ch != '\n')) && isNewLine) {
//            printNumLeft(&counter, isNewLine);
//            isNewLine = 0;
//        }
//        if (ch == '\n') {
//            if (args->E && !args->s) putchar('$');
//            if (args->n || args->b) isNewLine = 1;
//            if (args->s) {
//                if (isEmptyLine == 2) {
//                    if (args->n) isNewLine = 0;
//                    continue;
//                } else {
//                    if (args->E) putchar('$');
//                    isEmptyLine++;
//                }
//            }
//        } else if (args->s)
//            isEmptyLine = 0;
//
//        putchar(ch);
//    }
//    return 0;
//}

arguments parse_arguments(int argc, char **argv, int *idx) {
    int opt;
    arguments opts = {0};

    const char *short_options = "bnseEtTv";

    const struct option long_options[] = {
            {"input",           no_argument, NULL, 'i'},
            {"number-nonblank", no_argument, NULL, 'b'},
            {"number",          no_argument, NULL, 'n'},
            {"squeeze-blank",   no_argument, NULL, 's'},
            {NULL, 0,                        NULL, 0}};

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
            case 'v':
                opts.v = 1;
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

int output(arguments *args, int argc, char **argv) {
    FILE *file;
    for (int i = optind; i < argc; i++) {
        file = fopen(argv[i], "r");
        if (file == NULL) {
            fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
            perror("");
            exit(EXIT_FAILURE);
        }
        show_file(args, file);
        fclose(file);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int idx = 0;
    arguments args = parse_arguments(argc, argv, &idx);
    output(&args, argc, argv, idx);
    return 0;
}