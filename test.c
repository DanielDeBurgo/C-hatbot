#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
char arrsplitby[512];
char arrGetLine[512];
int inputsize = 256, bufferchar = 0, count = 0;
char inp[256]; //Had to restrict memory usage somehow, could have done this on the heap using malloc but would have caused excess bother

typedef struct matchAndNo {
    int a;
    char *b;
} matchAndNo;

int getlinesoffile(FILE *where){
  rewind(where);
  char line[512];
  int counter = 0;

  while(fgets(line, sizeof(line), where) != NULL){
    counter++;
  }

  return counter;
}

void getinp(){ //gets user input
  printf(">>>"); // Print CLI thing
  while((bufferchar = getchar()) && bufferchar != '\n' && bufferchar != EOF){ //Clear input buffer to get input
    if(count < inputsize) //If we haven't got enough characers yet
      inp[count++] = bufferchar; //Add the input to the inp
    }
    inp[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
}


char *getline(FILE *from, int lineNo){
  rewind(from);
  int count = 0;
  if (from != NULL ){
    while (fgets(arrGetLine, sizeof(arrGetLine), from) != NULL){
      //printf("IN SCOPE %s\n", arrGetLine);
      if (count == lineNo){
        return arrGetLine;
      }
      else{
        count++;
      }
    }
  }
}

char *splitby(char *splitting, char tosplit){
  int count = 0;
  for (int k = 0; k < 512; k++) {
    arrsplitby[k] = NULL;
  }
  for (int i = 0; i < strlen(splitting) - 1; i++){
    if (splitting[i] == tosplit){
      for (int j = i + 1; j < strlen(splitting); j++){
          arrsplitby[count] = splitting[j];
          count++;
      }
    }
  }
  return arrsplitby;
}

matchAndNo searchdb(FILE *db, char *toFind, int omit){
  rewind(db);
  char line[512];
  int lineNum = 1;
  while(fgets(line, 512, db) != NULL) {
		if((strstr(line, toFind)) != NULL) {
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

matchAndNo searchdb2(FILE *db, char *toFind, int omit){
  rewind(db);
  char line[512];
  int lineNum = 1;
  while(fgets(line, 512, db) != NULL) {
    //printf("%s", line);
		if((strstr(line, toFind)) != NULL) {
      if (omit == 0){
        char *checking = splitby(line, '~');
        if ((strstr(line, checking)) != NULL){
          matchAndNo r = {lineNum, line};
          return r;
        }
        else{
          omit++;
        }
      }
      omit--;
		}
		lineNum++;
	}
  matchAndNo r = {-1, NULL};
  return r;
}

void notsurehowtorespond(FILE *db1, FILE *db2, FILE *db3, char *notSureAbout){
  printf("How would you respond to that? \n");
  do {
    getinp();
  } while(strstr(inp, "{-}") != NULL);
  fseek(db1, 0, SEEK_END);
  fprintf(db1, "%s\n", notSureAbout);
  matchAndNo searched = searchdb(db1, notSureAbout, 0);
  int lineNo = searched.a;
  fseek(db2, 0, SEEK_END);
  fprintf(db2, "%s~%i\n", inp, lineNo);
  matchAndNo searched2 = searchdb(db2, inp, 0);
  lineNo = searched2.a;
  fseek(db3, 0, SEEK_END);
  fprintf(db3, "1~%i\n", lineNo);
}

void replaceinp(FILE *db1, FILE*db2. )


/* void replaceinp(FILE *db1, FILE *db2, FILE *db3, char *toRepCall, char *toRepResponse, char *repWith){
  //Finds toRep as a call, stores in searched
  matchAndNo searched = searchdb(db1, toRepCall, 0);
  //printf("%s %d", searched.b, searched.a);
  int lineNo = searched.a;
  fseek(db2, 0, SEEK_END);
  fprintf(db2, "%s~%i\n", repWith, lineNo);
  int omit = 0;

  //Finds all responses which respond to the toRepCall, stores in searched2

  //OKAY VERY WEIRD ONE, IT'S ONLY ADDING TO MY ARRAY WHEN I PRINT OUT THE VALUES

  matchAndNo searched2[512];
  char tempNum[512];
  sprintf(tempNum, "%d", lineNo);
  matchAndNo searched2res = searchdb2(db2, tempNum, omit);
  //printf("%s",searched2res.b);
  while (searched2res.b != NULL) {
    //printf("%s", searched2res.b);
    searched2[omit] = searched2res;
    //printf("%i\n", omit);
    omit++;
    char tempNum[512];
    sprintf(tempNum, "%d", lineNo);
    searched2res = searchdb2(db2, tempNum, omit);
  }
  //printf("%s", searched2[0].b);

  //Finds all probabilities which are assigned to the relevant responses, stored in searched3 NEEDS WORK
  matchAndNo searched3[512];
  omit = 0;
  for (int i = 0; i < (sizeof(searched2) / sizeof(searched2[0])); i++){
    int lineNo2 = searched2[i].a;
    //printf("%d\n", lineNo2);
    char tempNum[512];
    sprintf(tempNum, "%d", lineNo2);
    //printf("%s %i\n", tempNum, omit);
    matchAndNo searched3res = searchdb2(db3, tempNum, omit);
    printf("%s\n", searched3res.b);
    while (searched3res.b != NULL) {
      //printf("%s", searched3res.b);
      //printf("Hit the while loop \n");
      printf("%d\n", omit);
      searched3[omit] = searched3res;
      omit++;
      char tempNum[512];
      sprintf(tempNum, "%d", lineNo);
      searched3res = searchdb2(db3, tempNum, omit);
    }
  }
  printf("%s", searched3[1].b);
  //Delete all of the probabilities

  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0])); i++){
    int lineNo3 = searched3[i].a;
    //printf("Is %i > 0 and < %i \n", lineNo3, getlinesoffile(db3));
    if (lineNo3 > 0 && lineNo3 <= getlinesoffile(db3)){
      printf("%i\n", lineNo3);
      FILE *db4 = fopen("avers2.txt", "w+");
      int iterLineNo = 0;
      char *currentLine;
      int length;

      while (fgets(currentLine, sizeof(currentLine), db3) != NULL) {
        length = strlen(currentLine);
        if (length && (currentLine[length - 1] != '\n')){
          //Ignore
        }
        else{
          iterLineNo++;
          if (lineNo3 != iterLineNo){
            fputs(currentLine, db4);
          }
        }
      }
      fclose(db4);
      remove("avers.txt");
      rename("avers2.txt", "avers.txt");
    }
  }
  /*
  //Do the maths
  matchAndNo toRepSearched = searchdb2(db2, toRepResponse, 0);
  char tempNum2[512];
  sprintf(tempNum2, "%d", toRepSearched.a);
  matchAndNo toRepSearched2 = searchdb2(db3, tempNum2, 0);
  double diff;

  //ERROR
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0])); i++){
    if (toRepSearched2.a == searched3[i].a && (!(strcmp(toRepSearched2.b,searched3[i].b)))){
      char *toAppend = splitby(searched3[i].b ,'~');
      char temp = '0';
      char beginning[512];
      int count = 0;
      while (temp != '~'){
        beginning[count] = searched3[i].b[count];
        count++;
      }
      diff = atoi(beginning);
      char tempNum[512];
      sprintf(tempNum, "%f", (diff/2));
      searched3[i].b = strcat(tempNum , strcat("~", toAppend)); //concatenate int of beginning / 2 with ~ and to append
      diff = diff - atoi(searched3[i].b);
    }
  }

  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0])); i++){
    if (toRepSearched2.a != searched3[i].a && (!(strcmp(toRepSearched2.b,searched3[i].b)))){
      char *toAppend = splitby(searched3[i].b ,'~');
      char temp = '0';
      char beginning[512];
      int count = 0;
      while (temp != '~'){
        beginning[count] = searched3[i].b[count];
        count++;
      }
      char tempNum[512];
      sprintf(tempNum, "%f", diff);
      searched3[i].b = strcat(tempNum , strcat("~", toAppend));
    }
  }

  //Rewrite out searched3
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0])); i++){
    fprintf(db3, "%s\n", searched3[i].b);
  }
  */
}

int main(int argc, char const *argv[]) {
  FILE *testfile1 = fopen("test.txt", "r+");
  FILE *testfile2 = fopen("test2.txt", "r+");
  FILE *testfile3 = fopen("avers.txt", "r+");

  //for (int i = 0; i < 6; i++) {
    //printf("%s", getline(testfile, i));
    //printf("%s", searchdb2(testfile, getline(testfile, i), 0).b);
  //}
  replaceinp(testfile1, testfile2, testfile3, "Hi", "Hello", "Greetings");

  fclose(testfile1);
  fclose(testfile2);
  fclose(testfile3);
  return 0;
}
