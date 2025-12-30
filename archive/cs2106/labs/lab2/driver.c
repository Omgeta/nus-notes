#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myshell.h"

static void process_commands(FILE *file);
static bool handle_command(const size_t num_tokens, char ***tokensp);
static size_t tokenise(char *const line, char ***tokens);

pid_t __monitor_pid = 0;

int main(int argc, char *argv[]) {
    if (argc > 1) {
        __monitor_pid = strtol(argv[1], NULL, 0);
        printf("Monitoring by %d\n", __monitor_pid);
    }

    my_init();
    process_commands(stdin);
    return 0;
}

static void print_prompt(void) {
    printf("myshell> ");
    fflush(stdout);
}

static void process_commands(FILE *file) {
    bool exiting = false;
    char *line = NULL;
    size_t line_size = 0;
    print_prompt();
    while (!exiting) {
        if (getline(&line, &line_size, file) == -1) {
            if (feof(file)) {
                printf("End of commands; shutting down\n");
            } else {
                perror("Error while reading command; shutting down\n");
            }
            break;
        }
        char **tokens = NULL;
        size_t num_tokens = tokenise(line, &tokens);
        if (!tokens) {
            printf("Failed to tokenise command\n");
            exit(1);
        }

        exiting = handle_command(num_tokens, &tokens);
        free(tokens);

        if (!exiting) {
            print_prompt();
        }
    }

    if (line) {
        free(line);
    }

    if (ferror(file)) {
        perror("Failed to read line");
        exit(1);
    }
}

static bool handle_command(const size_t num_tokens, char ***tokensp) {
    const char *const cmd = (*tokensp)[0];
    if (!cmd) {
        // no-op
    } else if (strcmp(cmd, "quit") == 0) {
        my_quit();
        return true;
    } else {
        *tokensp = realloc(*tokensp, (num_tokens + 1) * sizeof(char *));
        (*tokensp)[num_tokens] = NULL;
        my_process_command(num_tokens + 1, *tokensp);
    }

    return false;
}

static size_t tokenise(char *const line, char ***tokens) {
    size_t reg_argv_buf_index = 0;
    size_t ret_argv_nmemb = 8;
    size_t ret_argv_index = 0;
    char **ret = calloc(ret_argv_nmemb, sizeof(char *));
    if (!ret) {
        goto fail;
    }

    bool last_was_tok = false;
    while (1) {
        char *const cur = line + reg_argv_buf_index;
        if (*cur == '\0') {
            // if we've hit the end of the line, break
            break;
        } else if (isspace(*cur)) {
            // this is whitespace; if the the last character was part of a
            // token, write a null byte here to terminate the last token
            if (last_was_tok) {
                *cur = '\0';
            }
            last_was_tok = false;
        } else {
            // this is not whitespace (so part of a token)
            // if the previous character was not part of a token (start of line
            // or whitespace), then add this to the result
            if (!last_was_tok) {
                // + 1 for the NULL at the end
                if (ret_argv_index + 1 >= ret_argv_nmemb) {
                    // our result array is full, resize it
                    ret_argv_nmemb += 8;
                    ret = realloc(ret, ret_argv_nmemb * sizeof(char *));
                    if (!ret) {
                        goto fail;
                    }
                }
                ret[ret_argv_index++] = cur;
            }
            last_was_tok = true;
        }
        reg_argv_buf_index++;
    }

    // NULL-terminate the result array
    ret[ret_argv_index] = NULL;
    *tokens = ret;
    return ret_argv_index;

    // N.B. goto is idiomatic for error-handling in C
fail:
    if (ret) {
        free(ret);
    }
    *tokens = NULL;
    return 0;
}
