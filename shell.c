#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void cd(char *path);
void pwd(void);
void help(void);
void mkdir_cmd(char *directory);
void rmdir_cmd(char *directory);
void ls(char *directory);
void cp(char *source, char *destination);
void mv(char *source, char *destination);
void rm(char *file);

int main(void)
{
    char input[1024];

    while (1)
    {
        printf("myshell> ");
        fgets(input, 1024, stdin);

        input[strcspn(input, "\n")] = 0;

        char *command = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");

        if (command == NULL)
        {
            continue; // Empty input, go back to the prompt
        }

        if (strcmp(command, "cd") == 0)
        {
            cd(arg1);
        }
        else if (strcmp(command, "pwd") == 0)
        {
            pwd();
        }
        else if (strcmp(command, "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(command, "help") == 0)
        {
            help();
        }
        else if (strcmp(command, "mkdir") == 0)
        {
            mkdir_cmd(arg1);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            rmdir_cmd(arg1);
        }
        else if (strcmp(command, "ls") == 0)
        {
            ls(arg1);
        }
        else if (strcmp(command, "cp") == 0)
        {
            cp(arg1, arg2);
        }
        else if (strcmp(command, "mv") == 0)
        {
            mv(arg1, arg2);
        }
        else if (strcmp(command, "rm") == 0)
        {
            rm(arg1);
        }
        else
        {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}

// change directory function
void cd(char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "Usage: cd <directory>\n");
    }
    else
    {
        if (chdir(path) != 0)
        {
            perror("cd");
        }
    }
}

// print working directory
void pwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("pwd");
    }
}

// help function
void help(void)
{
    printf("\n cd <directory> - Change directory\n");
    printf(" pwd - Print working directory\n");
    printf(" exit - Exit the shell\n");
    printf(" help - Display help\n");
    printf(" mkdir <directory> - Create a directory\n");
    printf(" rmdir <directory> - Remove a directory\n");
    printf(" ls <directory> - List files in a directory\n");
    printf(" cp <source> <destination> - Copy a file\n");
    printf(" mv <source> <destination> - Move a file\n");
    printf(" rm <file> - Remove a file\n");
}

// make new directory function
void mkdir_cmd(char *directory)
{
    if (directory == NULL)
    {
        fprintf(stderr, "Usage: mkdir <directory>\n");
    }
    else
    {
        if (mkdir(directory, 0777) != 0)
        {
            perror("mkdir");
        }
    }
}

// remove directory function
void rmdir_cmd(char *directory)
{
    if (directory == NULL)
    {
        fprintf(stderr, "Usage: rmdir <directory>\n");
    }
    else
    {
        if (rmdir(directory) != 0)
        {
            perror("rmdir");
        }
    }
}

// list files function
void ls(char *directory)
{
    if (directory == NULL)
    {
        directory = "."; // Default to the current directory if not specified
    }

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            printf("%s ", entry->d_name);
        }
        printf("\n");
        closedir(dir);
    }
    else
    {
        perror("ls");
    }
}

// copy files function
void cp(char *source, char *destination)
{
    if (source == NULL || destination == NULL)
    {
        fprintf(stderr, "Usage: cp <source> <destination>\n");
        return;
    }

    FILE *source_file = fopen(source, "rb");
    if (source_file == NULL)
    {
        perror("cp");
        return;
    }

    FILE *destination_file = fopen(destination, "wb");
    if (destination_file == NULL)
    {
        perror("cp");
        fclose(source_file);
        return;
    }

    char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source_file)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destination_file);
    }

    fclose(source_file);
    fclose(destination_file);
}

// move files function
void mv(char *source, char *destination)
{
    if (source == NULL || destination == NULL)
    {
        fprintf(stderr, "Usage: mv <source> <destination>\n");
        return;
    }

    if (rename(source, destination) != 0)
    {
        perror("mv");
    }
}

// remove files function
void rm(char *file)
{
    if (file == NULL)
    {
        fprintf(stderr, "Usage: rm <file>\n");
        return;
    }

    if (remove(file) != 0)
    {
        perror("rm");
    }
}
