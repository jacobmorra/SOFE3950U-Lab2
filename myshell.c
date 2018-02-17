/*
 * SOFE 3950U / CSCI 3020U: Operating Systems -- Lab 2 Week 1 -- MyShell Project
 *
 * <GROUP MEMBERS> Jacob Morra (100395426), Vrund Shah (100586175), Kevin Apuyan (100561117)
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"
#include <pwd.h>

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here
char path[BUFFER_LEN];
char origin[BUFFER_LEN];

// Define functions declared in myshell.h here
int main(int argc, char *argv[])
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char command[BUFFER_LEN] = { 0 };
    char arg[BUFFER_LEN] = { 0 };
    
    // Parse the commands provided using argc and argv
    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        
        //signify end of string once end of line reached
        if(buffer[strlen(buffer)-1]=='\n'){ 
            //printf("end of line");
            buffer[strlen(buffer)-1]='\0';
        }
        arg[0] = '\0';
        
        // Perform string tokenization to get the command and argument
        char *mytoken;
        mytoken = strtok(buffer, " "); //split string into tokens, tokenize by spaces
        
        int count = 0;
        while( mytoken != NULL ) {
            
            //initially, copy mytoken to command
            if(count==0){
                strcpy(command, mytoken);
            }
            //append copy of string pointed to by mytoken to end of arg
            else{
                strcat(arg, mytoken);
            }
            
            count=count+1;
            //go from a token to the next token,
            mytoken = strtok(NULL, " ");
            // allow for reading of arguments
            if( mytoken != NULL){
                strcat(arg, " ");
            }
        }

        // Check the command and execute the operations for each command
        // cd command -- change the current directory
        if (strcmp(command, "cd") == 0)
        {
            getcwd(command, sizeof(command));
            // Check for any argument 
            if(strcmp(arg,"") == 0){
                printf("Already at the specific path");
                system("pwd");
                fflush(stdout);
                printf("$");
                fflush(stdout);
            }
            else{
                //Sets the environment variable
                setenv("PWD", arg, 2);
                
                //bug testing
                char t[10]; //correcting a string error with getenv()
                strcpy(t,getenv("PWD")+1);
                
            

                //Checks if directory exists
                if(chdir(t) != 0){
                    printf("No such directory exists.");
                    system("pwd");
                    fflush(stdout);
                    printf("$");
                    fflush(stdout);
                        
                }
                else{
                    printf("Currently at directory %s\n", t);

                    system("pwd");
                    fflush(stdout);
                    printf("$");
                    fflush(stdout);
                }
            }
        }
        
        // dir command -- list directory contents
        else if (strcmp(command, "dir") == 0){
            
            DIR *directory;
            struct dirent *ent;
            
            //Get current path
            getcwd(origin, sizeof(origin));
        
            //if arg exists
            if (strcmp(arg,"") != 0){
                //Changes path to argument
                setenv("PWD", arg, 2);

                char s[10]; //correcting a string error with getenv()
                strcpy(s,getenv("PWD")+1);


                //Checks if directory exists
                if(chdir(s) != 0){
                    printf("Directory does not exist.");
                    system("pwd");
                    fflush(stdout);
                    printf("$");
                    fflush(stdout);
                }
                //Get current dir, print
                getcwd(arg, sizeof(arg));
                directory = opendir(arg);
                while ((ent = readdir(directory)) != NULL){
                    printf("%s\n", ent->d_name);
                }
                //Revert to orig dir
                setenv("PWD", origin, 3);
                chdir(getenv("PWD"));
                closedir(directory);
            }
            else{
                printf("Please enter an argument.\n");
                system("pwd");
                fflush(stdout);
                printf("$");
                fflush(stdout);
            }

        }
        
        // echo command -- print argument
        else if (strcmp(command, "echo") == 0){
            printf("%s\n",arg);
            system("pwd");
            fflush(stdout);
            printf("$");
            fflush(stdout);
        }

        // clr command -- clear window
        else if (strcmp(command, "clr") == 0){
            system("clear");
            system("pwd");
            fflush(stdout); 
            printf("$");
            fflush(stdout);
        }
        
        // pause command -- pause until key pressed
        else if (strcmp(command, "pause") == 0){
            printf("Press any key to continue.\n");
            getchar();
            system("pwd");
            fflush(stdout);
            printf("$");
            fflush(stdout);
        }
        
        // environ command -- print env variables
        else if (strcmp(command, "environ") == 0){
            system("printenv");
            system("pwd");
            fflush(stdout);
            printf("$");
            fflush(stdout);
        }
        
        // help command -- open readme
        else if (strcmp(command, "help") == 0){
            FILE *fpt;
            char string[50];
            
            //Open readme
            fpt = fopen("README.md", "r");
            fread(string, 50, 50, fpt);
            fgets(string, 50, fpt);
            //print readme
            printf("%s\n", string);
            fclose(fpt);

            system("pwd");
            fflush(stdout);
            printf("$");
            fflush(stdout);
        }

        // quit command -- exit the shell
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }

        // Unsupported command
        else
        {
            fputs("Unsupported command, use help to display the manual\n", stderr);
            system("pwd");
            fflush(stdout);
            printf("$");
            fflush(stdout);
        }
    }
    return EXIT_SUCCESS;
}

