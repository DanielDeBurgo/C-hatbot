#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

char globTemp[512];
char globTemp2[512][512];
char toReturn[512][512];
char readLine[512];
numStruct toReturn2[512];
char arrsplitby[512];
char arrsplitby2[512];
int numsCount = 0, linesCount = 0;

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
  int linesOfResps = getLinesOfFile(botResponses);
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
}

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
    int e = atoi(tempArr);
    //printf("%i\n", lineNum);
    if (e == finding.lineNum){
      getTheRight(line, '~');
      getTheLeft(arrsplitby, '~');
      double f;
      int lengthOfArr2 = strlen(arrsplitby2);
      strncpy(tempArr, arrsplitby2, lengthOfArr2);
      f = atof(tempArr);
      toReturn2[numsCount].lineNum = lineNum;
      for (int i = 0; i < strlen(line); i++){
        globTemp[i] = line[i];
      }
      for (int i = 0; i < strlen(globTemp); i++){
        globTemp2[numsCount][i] = globTemp[i];
      }
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

//REMEMBER WHEN YOU CALL THIS FUNCTION OUT MUST BE AN EMPTY FILE, AFTER RUNNING THE FUNCTION WE DELETE botResponses and replace it wih out
void replaceinp(FILE *userCalls, FILE *botResponses, FILE *out, char *userCall, char *offensiveResponse, char *repWith){
  matchAndNo call = searchdb(userCalls, userCall, 0);
  fseek(botResponses, 0, SEEK_END);
  fprintf(botResponses, "%s~1.5~%i\n", repWith, call.lineNum);
  rewind(botResponses);
  findnums(botResponses, call);
  for (int i = 0; i <= numsCount; i++){
    toReturn2[i].lineText = globTemp2[i];
  }

  readFileToMe(botResponses);
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
}

char *respond(char *userCall, FILE *userCalls, FILE *botResponses){
  matchAndNo call = searchdb(userCalls, userCall, 0);
  if (call.lineText != NULL){
    rewind(botResponses);
    findnums(botResponses, call);
    for (int i = 0; i <= numsCount; i++){
      toReturn2[i].lineText = globTemp2[i];
    }

    double random_value;
    srand ( time ( NULL));
    random_value = (double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
    while (random_value == 0){
      random_value = (double)rand()/RAND_MAX*2.0-1.0;
    }
    if (random_value < 0){
      random_value = random_value * -1;
    }
    double cumulativeProbability = 0;
    for (int i = 0; i <= numsCount; i++){
      cumulativeProbability = cumulativeProbability + toReturn2[i].probability;
      if (random_value <= cumulativeProbability){
        getTheLeft(toReturn2[i].lineText, '~');
        getTheLeft(arrsplitby2, '~');
        return arrsplitby2;
      }
    }
  }
  else{
    return NULL;
  }
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
      }
      else{
        int j = 0;
        while (inp0[j] != '\0' && inp0[j] != '\n' && inp0[j] != EOF){
          inp2[j] = inp0[j];
          j++;
        }
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
      }
      else{
        int j = 0;
        while (inp1[j] != '\0' && inp1[j] != '\n' && inp1[j] != EOF){
          inp2[j] = inp1[j];
          j++;
        }
        response = respond(inp1, testfile1, testfile2);
        if (response != NULL){
          printf("%s\n", response);
        }
        else{
          notsurehowtorespond(inp1, testfile1, testfile2);
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
