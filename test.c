#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char arrsplitby[512];
char arrGetLine[512];

typedef struct matchAndNo {
    int a;
    char *b;
} matchAndNo;

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

int main(int argc, char const *argv[]) {
  FILE *testfile = fopen("test.txt", "r+");

  for (int i = 0; i < 6; i++) {
    //printf("%s", getline(testfile, i));
    printf("%s", searchdb(testfile, getline(testfile, i), 0).b);
  }

  fclose(testfile);
  return 0;
}
