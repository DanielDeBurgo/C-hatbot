#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char arrsplitby[512];
char arrGetLine[512];
int inputsize = 256, bufferchar = 0, count = 0;
char inp[256]; //Had to restrict memory usage somehow, could have done this on the heap using malloc but would have caused excess bother

typedef struct matchAndNo {
    int a;
    char *b;
} matchAndNo;

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

void replaceinp(FILE *db1, FILE *db2, FILE *db3, char *toRepCall, char *toRepResponse, char *repWith){
  //Finds toRep as a call, stores in searched
  matchAndNo searched = searchdb(db1, toRepCall, 0);
  int lineNo = searched.a;
  fprintf(db2, "%s~%i\n", repWith, lineNo);
  int omit = 0;

  //Finds all responses which respond to the toRepCall, stores in searched2
  matchAndNo searched2[512];
  matchAndNo searched2res = searchdb2(db2, lineNo, omit);
  while (searched2res != NULL) {
    searched2[omit] = searched2res;
    omit++;
    searched2res = searchdb2(db2, lineNo, omit);
  }

  //Finds all probabilities which are assigned to the relevant responses, stored in searched3
  matchAndNo searched3[512];
  for (int i = 0; i < (sizeof(searched2) / sizeof(searched2[0]); i++)){
    int lineNo2 = searched2[i].a;
    matchAndNo searched3res = searchdb2(db3, lineNo2, omit);
    while (searched3res != NULL) {
      searched3[omit] = searched3res;
      omit++;
      searched3res = searchdb2(db3, lineNo, omit);
    }
  }

  //Delete all of the probabilities
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0]); i++)){
    int lineNo3 = searched3[i].a;
    FILE *db4 = fopen("avers2.txt");
    int iterLineNo = 0;
    char currentLine[512];
    int length;
    while (fgets(currentLine, sizeof(currentLine), db3)) {
      length = strlen(currentLine);
      if (len && (line[len - 1] != '\n')){
        //Ignore
      }
      else{
        iterLineNo++;
        if (lineNo3 != iterLineNo){
          fputs(line, db4);
        }
      }
    }
    remove("avers.txt");
    rename("avers2.txt", "avers.txt");
    fclose(db4);
  }

  //Do the maths
  matchAndNo toRepSearched = searchdb(db2, toRepResponse, 0);
  matchAndNo toRepSearched2 = searchdb2(db3, toRepSearched.a, 0);
  double diff;
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0])); i++){
    if (toRepSearched2.a == searched3[i].a && toRepSearched2.b == searched3[i].b){
      char *toAppend = splitby(searched3[i].b ,"~");
      char temp = '0';
      char beginning[512];
      int count = 0;
      while (temp != '~'){
        beginning[count] = searched3[i].b[count];
        count++;
      }
      diff = atoi(beginning);
      searched3[i].b = strcat(itoa((atoi(beginning) / 2)), strcat("~", toAppend));
      diff = diff - atoi(searched3[i].b);
    }
  }
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0])); i++){
    if (toRepSearched2.a != searched3[i].a && toRepSearched2.b != searched3[i].b){
      char *toAppend = splitby(sarched3[i].b ,"~");
      char temp = '0';
      char beginning[512];
      int count = 0;
      while (temp != '~'){
        beginning[count] = searched3[i].b[count];
        count++;
      }
      searched3[i].b = strcat(itoa((atoi(beginning) * diff)), strcat("~", toAppend))
    }
  }

  //Rewrite out searched3
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0]); i++){
    fprintf("%s\n", searched3[i].b);
  }
}

int main(int argc, char const *argv[]) {
  FILE *testfile1 = fopen("test.txt", "r+");
  FILE *testfile2 = fopen("test2.txt", "r+");
  FILE *testfile3 = fopen("test3.txt", "r+");

  //for (int i = 0; i < 6; i++) {
    //printf("%s", getline(testfile, i));
    //printf("%s", searchdb2(testfile, getline(testfile, i), 0).b);
  //}

  replaceinp(testfile1, testfile2, testfile3, "Hi", "Hello", "Greetings");

  fclose(testfile1);
  return 0;
}
