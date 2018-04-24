#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/unistd.h>
#include "Offended.h"
#include "Backchat.h"

extern char globTemp[512];
extern char globTemp2[512][512];
extern char toReturn[512][512];
extern char readLine[512];
extern numStruct toReturn2[512];
extern char arrsplitby[512];
extern char arrsplitby2[512];
extern int numsCount, linesCount;

int inputsize = 256, bufferchar = 0, count = 0, zerone = 0;
char *response = "";
char inp0[512] = ""; //Had to restrict memory usage somehow, could have done this on the heap using malloc but would have caused excess bother
char inp1[512] = "";
char inp2[512] = "";

void getinp(){ //gets user input
  printf(">>>"); // Print CLI thing
  if (zerone){
    while((bufferchar = getchar()) && bufferchar != '\n' && bufferchar != EOF){ //Clear input buffer to get input
      if(count < inputsize) //If we haven't got enough characers yet
        inp1[count++] = bufferchar; //Add the input to the inp
    }
    inp1[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
    zerone = 0;
  }
  else{
    while((bufferchar = getchar()) && bufferchar != '\n' && bufferchar != EOF){ //Clear input buffer to get input
      if(count < inputsize) //If we haven't got enough characers yet
        inp0[count++] = bufferchar; //Add the input to the inp
    }
    inp0[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
    zerone = 1;
  }
}

int getLinesOfFile(FILE *linedFile){
  rewind(linedFile);
  char line[512];
  int count = 1;
  while(fgets(line, 512, linedFile) != NULL) {
    count++;
  }
  return count;
}

void notsurehowtorespond(char *notSureAbout, FILE *userCalls, FILE *botResponses){
  fseek(userCalls, 0, SEEK_END);
  fprintf(userCalls, "%s\n", notSureAbout);
  int linesOfResps = getLinesOfFile(userCalls);
  printf("How would you respond to that?\n");
  getinp();
  if (zerone){
    fseek(botResponses, 0, SEEK_END);
    fprintf(botResponses, "%s~1~%i\n", inp0, linesOfResps -1);
  }
  else{
    fseek(botResponses, 0, SEEK_END);
    fprintf(botResponses, "%s~1~%i\n", inp1, linesOfResps -1);
  }
  printf("Okay, that's what I'll say from now on. If you don't like it you can always use {REP} to tell me to say something else\n");
  exit(0);
}

int main(int argc, char const *argv[]) {
  char Squig[512] = "";
  for (int i = 0; i < 512; i++){
    toReturn2[i].lineText = Squig;
  }

  FILE *testfile1 = fopen("test.txt", "r+");
  FILE *testfile2 = fopen("test2.txt", "r+");
  FILE *testfile3 = fopen("output.txt", "w+"); //trunc

  getinp();
  while ((strcmp(inp0, "{END CHAT}")) && (strcmp(inp1, "{END CHAT}"))){ //While they want to carry on talking
    if (zerone){
      if (!(strcmp(inp0, "{REP}"))){
        printf("What should I say instead?\n");
        getinp();
        replaceinp(testfile1, testfile2, testfile3, inp2, response, inp1);
        remove("test2.txt");
        rename("output.txt", "test2.txt");
        fclose(testfile2);
        fclose(testfile3);
        FILE *testfile2 = fopen("test2.txt", "r+");
        FILE *testfile3 = fopen("output.txt", "w+");
        printf("Okay, I'll do that more often now\n");
        exit(0);
      }
      else{
        int j = 0;
        while (inp0[j] != '\0' && inp0[j] != '\n' && inp0[j] != EOF){
          inp2[j] = inp0[j];
          j++;
        }
        inp2[j] = '\0';
        response = respond(inp0, testfile1, testfile2);
        if (response != NULL){
          printf("%s\n", response);
        }
        else{
          notsurehowtorespond(inp0, testfile1, testfile2);
        }
      }
    }
    else{
      if (!(strcmp(inp1, "{REP}"))){
        printf("What should I say instead?\n");
        int j = 0;
        getinp();
        replaceinp(testfile1, testfile2, testfile3, inp2, response, inp0);
        remove("test2.txt");
        rename("output.txt", "test2.txt");
        fclose(testfile2);
        fclose(testfile3);
        FILE *testfile2 = fopen("test2.txt", "r+");
        FILE *testfile3 = fopen("output.txt", "w+");
        printf("Okay, I'll do that more often now\n");
        exit(0);
      }
      else{
        int j = 0;
        while (inp1[j] != '\0' && inp1[j] != '\n' && inp1[j] != EOF){
          inp2[j] = inp1[j];
          j++;
        }
        inp2[j] = '\0';
        response = respond(inp1, testfile1, testfile2);
        if (response != NULL){
          printf("%s\n", response);
        }
        else{
          notsurehowtorespond(inp1, testfile1, testfile2);
          fclose(testfile1);
          fclose(testfile2);
          FILE *testfile1 = fopen("test.txt", "r+");
          FILE *testfile2 = fopen("test2.txt", "r+");
        }
      }
    }
    getinp();
  }

  fclose(testfile1);
  fclose(testfile2);
  fclose(testfile3);
  return 0;
}
