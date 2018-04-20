#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>

typedef struct matchAndNo {
    int lineNum;
    char *lineText;
} matchAndNo;

typedef struct numStruct {
    int lineNum;
    char *lineText;
    double probability;
} numStruct;

char *toReturn[512];
char arrsplitby[512];
char arrsplitby2[512];
int numsCount = 0, linesCount = 0;

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
  char line[512];
  int lineNum = 1;
  while(fgets(line, 512, db) != NULL) {
    int lengthOfLine = strlen(line);
    if(line[lengthOfLine] == '\n'){
      line[lengthOfLine] = '\0';
    }
    if(line[lengthOfLine - 1] == '\n'){
      line[lengthOfLine - 1] = '\0';
    }
    printf("Comparing %s and %s", line, toFind);
		if(!(strcmp(line, toFind))) {
      if (omit == 0){
        //printf("%s", line);
        matchAndNo r = {lineNum, line};
        return r;
      }
      omit--;
		}
		lineNum++;
	}
  matchAndNo r = {-1, NULL};
  return r;
}

numStruct *findnums(FILE *lookingIn, matchAndNo finding){
  numsCount = 0;
  rewind(lookingIn);
  numStruct toReturn[512];
  char line[512];
  char tempArr[512];
  int lineNum = 1;
  while(fgets(line, 512, lookingIn) != NULL) {
    getTheRight(line, '~');
    getTheRight(arrsplitby, '~');
    int lengthOfArr = strlen(arrsplitby);
    strncpy(tempArr, arrsplitby, lengthOfArr);
    int e = atoi(tempArr);
    if (e == finding.lineNum){
      getTheLeft(line, '~');
      getTheRight(arrsplitby2, '~');
      double f;
      int lengthOfArr2 = strlen(arrsplitby);
      strncpy(tempArr, arrsplitby, lengthOfArr2);
      f = atof(tempArr);
      toReturn[numsCount].lineNum = lineNum;
      toReturn[numsCount].lineText = line;
      toReturn[numsCount].probability = f;
      numsCount++;
    }
    lineNum++;
  }
}

char **readFileToMe(FILE * db){
  linesCount = 0;
  rewind(db);
  char line[512];
  int count = 0;
  while(fgets(line, 512, db) != NULL) {
    toReturn[count] = line;
    count++;
  }
  linesCount = count;
  return toReturn;
}

void rewrite(FILE *db, FILE *out, double numToReWrite, int lineNo, numStruct num){
  //Read db into array of strings

  char **fileAsArray = readFileToMe(db);

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
  tempString = strcat(Squiggle, tempString);
  tempString = strcat(before, tempString);
  fileAsArray[lineNo] = tempString;

  //Write arrayOfStrings into out line by line
  for (int i = 0; i < linesCount; i++){
    fprintf(out, "%s\n", fileAsArray[i]);
  }

}

//REMEMBER WHEN YOU CALL THIS FUNCTION OUT MUST BE AN EMPTY FILE, AFTER RUNNING THE FUNCTION WE DELETE botResponses and replace it wih out
void replaceinp(FILE *userCalls, FILE *botResponses, FILE *out, char *userCall, char *offensiveResponse, char *repWith){
  matchAndNo call = searchdb(userCalls, userCall, 0);
  fseek(botResponses, 0, SEEK_END);
  fprintf(botResponses, "%s~1.5~%i\n", repWith, call.lineNum);
  numStruct *nums;
  nums = findnums(botResponses, call);
  for (int i = 0; i < numsCount; i++){
    printf("%i", i);
    rewrite(botResponses, out, nums[i].probability * 0.4, call.lineNum, nums[i]);
  }
}

int main(int argc, char const *argv[]) {
  FILE *testfile1 = fopen("test.txt", "r");
  FILE *testfile2 = fopen("test2.txt", "r+");
  FILE *testfile3 = fopen("output.txt", "w+"); //trunc

  replaceinp(testfile1, testfile2, testfile3, "Hi", "Hello", "Greetings");
  unlink("test2.txt");
  rename("output.txt", "test2.txt");

  fclose(testfile1);
  fclose(testfile2);
  return 0;
}
