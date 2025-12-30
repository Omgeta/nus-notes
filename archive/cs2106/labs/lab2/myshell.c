/**
 * CS2106 AY25/26 Semester 1 - Lab 2
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */


#include "myshell.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static struct PCBTable ptable[MAX_PROCESSES];
static size_t pcount = 0;

// Private
static int find_pid(pid_t pid) {
  for (size_t i = 0; i < pcount; i++) {
    if (ptable[i].pid == pid) return i;
  }
  return -1;
}

static void mark_process_exited(pid_t pid, int st) {
  int i = find_pid(pid);
  if (i < 0) return;

  if (ptable[i].status != 1) {
    ptable[i].status = 1; // mark exited
    if (WIFEXITED(st))
      ptable[i].exitCode = WEXITSTATUS(st);
    else if (WIFSIGNALED(st))
      ptable[i].exitCode = WTERMSIG(st);
    else
      ptable[i].exitCode = 0;
  }
}

static void sigchld_handler(int signo) {
  if (signo != SIGCHLD) return;

  int status;
  pid_t pid;

  // non-blocking wait on all finished children on SIGCHLD
  // then mark then as exited
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    mark_process_exited(pid, status);
  }
}

static void print_all_process() {
  for (size_t i = 0; i < pcount; i++) {
    printf("[%d] ", ptable[i].pid);
    switch (ptable[i].status) {
      case 1:
        printf("Exited %d\n", ptable[i].exitCode); break;
      case 2:
        printf("Running\n"); break;
      case 3:
        printf("Terminating\n"); break;
    }
  }
}

static int count_status(int status) {
  int c = 0;
  for (size_t i = 0; i < pcount; i++)
    if (ptable[i].status == status) c++;
  return c;
}

static int handle_info(char *option) {
  if (option != NULL) {
    switch (*option) {
      case '0': print_all_process(); return 0;
      case '1': printf("Total exited process: %d\n", count_status(1)); return 0;
      case '2': printf("Total running process: %d\n", count_status(2)); return 0;
      case '3': printf("Total terminating process: %d\n", count_status(3)); return 0;
    } 
  }
  return 1;
}

static void handle_wait(pid_t pid) {
  int i = find_pid(pid);
  if (i < 0 || ptable[i].status == 1) return; // not a real PID, or already exited

  int st;
  waitpid(pid, &st, 0);
  mark_process_exited(pid, st);
}


static void handle_terminate(pid_t pid) {
  int i = find_pid(pid);
  if (i < 0) return;

  // only terminate running
  if (ptable[i].status == 2) {
    kill(pid, SIGTERM);
    ptable[i].status = 3;
  }
}

static void handle_program(char **tokens) {
  if (pcount >= MAX_PROCESSES) {
    fprintf(stderr, "Max processes reached");
    return;
  }

  if (access(tokens[0], F_OK) || access(tokens[0], X_OK)) {
    fprintf(stderr, "%s not found\n", tokens[0]);
    return;
  }

  // handle & in background
  int argc = 0;
  while (tokens[argc] != NULL) argc++;
  int bg = 0;
  if (argc > 1 && strcmp(tokens[argc - 1], "&") == 0) {
    bg = 1;
    tokens[--argc] = NULL; // NULL-terminator for execv args
  }

  // compress tokens, removing redirections
  char *in_file = NULL;
  char *out_file = NULL;
  char *err_file = NULL;
  int j = 0;
  for (int i = 0; i < argc; i++) {
    if (strcmp(tokens[i], "<") == 0 && i + 1 < argc) {
      in_file = tokens[++i];
    } else if (strcmp(tokens[i], ">") == 0 && i + 1 < argc) {
      out_file = tokens[++i];
    } else if (strcmp(tokens[i], "2>") == 0 && i + 1 < argc) {
      err_file = tokens[++i];
    } else {
      tokens[j++] = tokens[i];  
    }
  }
  tokens[j] = NULL;
  if (j == 0) return; // if no tokens left after stripping of redirections
  
  pid_t pid = fork();
  if (pid == 0) {
    if (in_file) {
      int fd = open(in_file, O_RDONLY);
      if (fd < 0) {
        fprintf(stderr, "%s does not exist\n", in_file);
        _exit(1);
      }
      dup2(fd, STDIN_FILENO);
      close(fd);
    }

    if (out_file) {
      int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd >= 0) { 
        dup2(fd, STDOUT_FILENO); 
        close(fd);
      }
    }

    if (err_file) {
      int fd = open(err_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd >= 0) { 
        dup2(fd, STDERR_FILENO); 
        close(fd);
      }
    }

    execv(tokens[0], tokens);
    _exit(1);
  }
  else {
    ptable[pcount].pid = pid;
    ptable[pcount].status = 2;
    ptable[pcount].exitCode = -1;
    pcount++;

    if (bg) printf("Child [%d] in background\n", pid);
    else handle_wait(pid);
  }
}

static void process_segment(char **tokens) {
  if (strcmp(tokens[0], "info") == 0) {
    int err = handle_info(tokens[1]);
    if (err) fprintf(stderr, "Wrong command\n");

  } else if (strcmp(tokens[0], "wait") == 0 && tokens[1]) {
    pid_t pid = (pid_t) atoi(tokens[1]);
    handle_wait(pid);

  } else if (strcmp(tokens[0], "terminate") == 0 && tokens[1]) {
    pid_t pid = (pid_t) atoi(tokens[1]);
    handle_terminate(pid);

  } else {
    handle_program(tokens);
  }
}

// Public
void my_init(void) {
  // Initialize what you need here
  memset(ptable, 0, sizeof(ptable));
  pcount = 0;
  if (signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
    fprintf(stderr, "Failed to register SIGCHLD handler\n");
    exit(1); // failed to register
  }
}

void my_process_command(size_t num_tokens, char **tokens) {
  // Your code here, refer to the lab document for a description of the
  // arguments
  if (num_tokens == 0 || tokens[0] == NULL) return;

  size_t i = 0;
  size_t start = 0;
  // split ; by NULL terminators and process segments separately
  while (i < num_tokens - 1) {
    if (strcmp(tokens[i], ";") == 0) {
      tokens[i] = NULL;
      if (tokens[start] != NULL) {
        process_segment(tokens + start);
      }
      start = i + 1; // move start of segment pointer after the ;
    }
    i++;
  }
  // process last segment
  if (start < num_tokens - 1 && tokens[start] != NULL)
    process_segment(tokens + start);
}


void my_quit(void) {
  // Clean up function, called after "quit" is entered as a user command
  for (size_t i = 0; i < pcount; i++) {
    if (ptable[i].status == 2) {
      printf("Killing [%d]\n", ptable[i].pid);
      kill(ptable[i].pid, SIGTERM);
      ptable[i].status = 3;
    }
  }
  printf("\nGoodbye\n");
}
