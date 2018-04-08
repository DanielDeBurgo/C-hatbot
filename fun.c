struct matchAndNo {
    int a;
    char *b;
};

// Functions are below

void getinp(){ //gets user input
  printf(">>>"); // Print CLI thing
  while((bufferchar = getchar()) && bufferchar != '\n' && bufferchar != EOF){ //Clear input buffer to get input
    if(count < inputsize) //If we haven't got enough characers yet
      inp[count++] = bufferchar; //Add the input to the inp
    }
    inp[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
}

char *splitby(char *splitting, char tosplit){
  char *toreturn = "";
  for (int i = 0; i < strlen(splitting); i++){
    if (!(strcmp(splitting[i], tosplit))){
      for (int j = i; j < strlen(splitting); j++){
          toreturn = strcat(toreturn, splitting[j]);
      }
    }
  }
  return toreturn;
}

matchAndNo searchdb(FILE *db, char *toFind, int omit){
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
  return NULL;
}

matchAndNo searchdb2(FILE *db, char *toFind, int omit){
  char line[512];
  int lineNum = 1;
  while(fgets(line, 512, db) != NULL) {
		if((strstr(line, toFind)) != NULL) {
      if (omit == 0){
        char *checking = splitby(line, "~");
        if (!(strcmp(checking, line))){
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
  return NULL;
}

void replaceinp(FILE *db1, FILE *db2, FILE *db3, char *toRep, char *repWith){
  matchAndNo searched = searchdb(db1, toRep, 0);
  int lineNo = searched.a;
  fprintf(db2, "%s~%i\n", repWith, lineNo);
  int omit = 0;

  //TODO FINISH THIS
  matchAndNo searched2[512];
  matchAndNo searched2res = searchdb2(db2, lineNo, omit);
  while (searched2res != NULL) {
    searched2[omit] = searched2res;
    omit++;
    searched2res = searchdb2(db2, lineNo, omit);
  }

  matchAndNo searched3[512];
  for (int i = 0; i < (sizeof(searched2) / sizeof(searched2[0]); i++){
    lineNo2 = searched2[i].a;
    matchAndNo searched3res = searchdb2(db3, lineNo2, omit);
    while (searched3res != NULL) {
      searched3[omit] = searched3res;
      omit++;
      searched3res = searchdb2(db2, lineNo, omit);
    }
  }

  //Return all the numbers, count the amount of them,  replace them all with 1/count
}

void notsurehowtorespond(FILE *db1, FILE *db2, FILE *db3, char *notSureAbout){
  printf("How would you respond to that? \n")
  do {
    getinp();
  } while(strstr(inp, "{-}") != NULL);
  matchAndNo searched = searchdb(db1, inp, 0);
  int lineNo = searched.a;
  fprintf(db1, "%s\n", notSureAbout);
  fprintf(db2, "%s~%i\n", inp, lineNo);
  matchAndNo searched = searchdb(db2, inp, 0);
  lineNo = searched.a;
  fprintf(db3, "1~%i\n", lineNo);
}

char *respond(char *call){
  //TODO
}
