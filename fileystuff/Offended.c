#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include "Offended.h"

char globTemp[512];
char globTemp2[512][512];
char toReturn[512][512];
char readLine[512];
numStruct toReturn2[512];
char arrsplitby[512];
char arrsplitby2[512];
int numsCount, linesCount;

void getTheRight(char *splitting, char tosplit){
  int count = 0;
  if(strlen(splitting) > 0)
    for (int i = 0; i < strlen(splitting); i++){
      if (splitting[i] == tosplit){
        for (int j = i + 1; j < strlen(splitting); j++){
          if (splitting[j] != '\0' && splitting[j] != '\n'){
            arrsplitby[count] = splitting[j];
            count++;
          }
        }
        arrsplitby[count] = '\0';
        break;
      }
    }
}

void getTheLeft(char *splitting, char tosplit){
  int count = 0;
  for (int i = 0; i < strlen(splitting); i++){
    if (splitting[i] == tosplit){
      for (int j = 0; j <= i-1; j++){
        if (splitting[j] != '\0' && splitting[j] != '\n'){
          arrsplitby2[count] = splitting[j];
          count++;
        }
      }
      arrsplitby2[count] = '\0';
      break;
    }
  }
}

matchAndNo searchdb(FILE *db, char *toFind, int omit){
  rewind(db);
  int lineNum = 1;
  while(fgets(readLine, 512, db) != NULL) {
    int lengthOfLine = strlen(readLine);
    if(readLine[lengthOfLine - 1] == '\n'){
      readLine[lengthOfLine - 1] = '\0';
    }
    //printf("Comparing %s and %s", line, toFind);
		if(!(strcmp(readLine, toFind))) {
      if (omit == 0){
        //printf("%s", line);
        matchAndNo r = {lineNum, readLine};
        return r;
      }
      omit--;
		}
		lineNum++;
	}
  matchAndNo r = {-1, NULL};
  return r;
}

void findnums(FILE *lookingIn, matchAndNo finding){
  numsCount = 0;
  rewind(lookingIn);
  char line[512];
  char tempArr[512];
  int lineNum = 1;
  while(fgets(line, 512, lookingIn) != NULL) {
    int lengthOfLine = strlen(line);
    if(line[lengthOfLine] == '\n'){
      line[lengthOfLine] = '\0';
    }
    if(line[lengthOfLine - 1] == '\n'){
      line[lengthOfLine - 1] = '\0';
    }
    getTheRight(line, '~');
    getTheRight(arrsplitby, '~');
    int lengthOfArr = strlen(arrsplitby);
    strncpy(tempArr, arrsplitby, lengthOfArr);
    tempArr[lengthOfArr] = '\0';
    int e = atoi(tempArr);
    printf("%s %i\n", line, e);
    //printf("%i\n", lineNum);
    if (e == finding.lineNum){
      getTheRight(line, '~');
      getTheLeft(arrsplitby, '~');
      double f;
      int lengthOfArr2 = strlen(arrsplitby2);
      strncpy(tempArr, arrsplitby2, lengthOfArr2);
      f = atof(tempArr);
      toReturn2[numsCount].lineNum = lineNum;
      //printf("%i\n", strlen(line));
      for (int i = 0; i < strlen(line); i++){
        globTemp[i] = line[i];
      }
      globTemp[strlen(line)] = '\0';
      //printf("%i\n", strlen(globTemp));
      for (int i = 0; i < strlen(globTemp); i++){
        globTemp2[numsCount][i] = globTemp[i];
      }
      //printf("%s\n", globTemp2[numsCount]);
      toReturn2[numsCount].probability = f;
      numsCount++;
    }
    lineNum++;
  }
}

void readFileToMe(FILE * db){
  linesCount = 0;
  rewind(db);
  char line[512];
  int count = 0;
  while(fgets(line, 512, db) != NULL) {
    for (int i = 0; i < strlen(line); i++){
      toReturn[count][i] = line[i];
    }
    count++;
  }
  linesCount = count;
}

void rewrite(FILE *db, FILE *out, double numToReWrite, int lineNo, numStruct num){

  //Create string before = everything before 1st ~ in num.b

  getTheLeft(num.lineText, '~');
  getTheLeft(arrsplitby2, '~');
  char *before = arrsplitby2;

  //Create string after = everything afer 2nd ~ in num.b

  getTheRight(num.lineText, '~');
  getTheRight(arrsplitby, '~');
  char *after = arrsplitby;

  //edit arrayOfStrings[lineNo] = before + ~ + string(numToReWrite) + ~ + after
  char Squiggle[512] = "~";
  char *tempString = strcat(Squiggle, after);
  char doublestr[512];
  sprintf(doublestr, "%f", numToReWrite);
  tempString = strcat(doublestr, tempString);
  char Squiggle2[512] = "~";
  tempString = strcat(Squiggle2, tempString);
  tempString = strcat(before, tempString);
  for (int i = 0; i < strlen(tempString); i++){
    toReturn[lineNo][i] = tempString[i];
  }
  toReturn[lineNo][strlen(tempString)] = '\0';

  //Write arrayOfStrings into out line by line
}

int isvalueinarray(int val, int *arr, int size){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return 1;
    }
    return 0;
}

//REMEMBER WHEN YOU CALL THIS FUNCTION OUT MUST BE AN EMPTY FILE, AFTER RUNNING THE FUNCTION WE DELETE botResponses and replace it wih out
void replaceinp(FILE *userCalls, FILE *botResponses, FILE *out, char *userCall, char *offensiveResponse, char *repWith){
  matchAndNo call = searchdb(userCalls, userCall, 0);
  fseek(botResponses, 0, SEEK_END);
  fprintf(botResponses, "%s~1.5~%i\n", repWith, call.lineNum);
  rewind(botResponses);
  rewind(out);
  findnums(botResponses, call);
  for (int i = 0; i < numsCount; i++){
    toReturn2[i].lineText = globTemp2[i];
    //printf("%s\n", toReturn2[i].lineText);
  }
  int skip[512];
  int skipCount = 0;
  for (int i = 0; i < numsCount; i++){
    printf("%i %s\n", i, toReturn2[i].lineText);
    char doublestr[512];
    sprintf(doublestr, "%f", toReturn2[i].probability * 0.4);
    getTheLeft(toReturn2[i].lineText, '~');
    getTheLeft(arrsplitby2, '~');
    char *before = arrsplitby2;
    getTheRight(toReturn2[i].lineText, '~');
    getTheRight(arrsplitby, '~');
    char *after = arrsplitby;
    //printf("%s\n", after);
    skip[skipCount] = atoi(after);
    skipCount++;
    char toPrint[512];
    //printf("%s - %i, %s - %i\n", before, strlen(before), after, strlen(after));
    for (int j = 0; j < strlen(before) + strlen(doublestr) + strlen(after) + 2; j++){
      if (j < strlen(before)){
        toPrint[j] = before[j];
      }
      else if (j == strlen(before)){
        toPrint[j] = '~';
      }
      //FIX
      else if (j <= strlen(before) + strlen(doublestr)){
        toPrint[j] = doublestr[j - strlen(before) - 1];
      }
      //FIX
      else if (j == strlen(before) + strlen(doublestr) + 1){
        toPrint[j] = '~';
      }
      else if (j < strlen(before) + strlen(doublestr) + strlen(after) + 2){
        toPrint[j] = after[j - strlen(doublestr) - strlen(before) - 2];
      }
    }
    toPrint[strlen(before) + strlen(doublestr) + strlen(after) + 2] = '\0';
    //printf("%s\n", toPrint);
    fprintf(out, "%s\n", toPrint);
  }

  readFileToMe(botResponses);
  for (int i = 0; i <= linesCount; i++){
    int lengthOfLine = strlen(toReturn[i]);
    if(toReturn[i][lengthOfLine] == '\n'){
      toReturn[i][lengthOfLine] = '\0';
    }
    if(toReturn[i][lengthOfLine - 1] == '\n'){
      toReturn[i][lengthOfLine - 1] = '\0';
    }
    getTheRight(toReturn[i], '~');
    getTheRight(arrsplitby, '~');
    if (!(isvalueinarray(atoi(arrsplitby), skip, skipCount))){
      fprintf(out, "%s\n", toReturn[i]);
    }
  }
  /*readFileToMe(botResponses);
  for (int i = 0; i < numsCount; i++){
    rewrite(botResponses, out, toReturn2[i].probability * 0.4, toReturn2[i].lineNum, toReturn2[i]);
  }
  rewind(out);


  for (int i = 0; i <= linesCount; i++){
    int lengthOfLine = strlen(toReturn[i]);
    if(toReturn[i][lengthOfLine] == '\n'){
      toReturn[i][lengthOfLine] = '\0';
    }
    if(toReturn[i][lengthOfLine - 1] == '\n'){
      toReturn[i][lengthOfLine - 1] = '\0';
    }
    if (i != toReturn2[0].lineNum - 1){ //if i is not equal to the first line number of the ones we're replacing
      fprintf(out, "%s\n", toReturn[i]);
    }
  }
  */
}
