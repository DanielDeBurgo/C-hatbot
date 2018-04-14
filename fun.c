typedef struct matchAndNo {
    int a;
    char *b;
} matchAndNo;
char arrGetLine[256];
char arrsplitby[512];
// Functions are below. They have been tested as working if commented DONE

//Gets input from user DONE
void getinp(){ //gets user input
  printf(">>>"); // Print CLI thing
  while((bufferchar = getchar()) && bufferchar != '\n' && bufferchar != EOF){ //Clear input buffer to get input
    if(count < inputsize) //If we haven't got enough characers yet
      inp[count++] = bufferchar; //Add the input to the inp
    }
    inp[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
}

//Gets a specific line from a file DONE
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

//Splits a string by the first instance of splitting and returns the rest of the string DONE
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

//Searches a database without ~ DONE BUT REMEMBER TO CHANGE ANY NULL CHECKS TO .b NULL CHECKS
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

//Searches a database with ~
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

//What to do when the user doesn't like what the chatbot says
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
        lineNumber++;
        if (lineNumber != todoNumber){
          fputs(line, todoFile);
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
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0]); i++){
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
      searched3[i].b = strcat(itoa((atoi(beginning) / 2)), strcat("~", toAppend))
      diff = diff - atoi(searched3[i].b);
    }
  }
  for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0]); i++){
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

//What to do when the bot doesn't know what to say
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

//How to find out what to respond with
char *respond(char *call){
  matchAndNo searched = searchdb(db1, toRepCall, 0);
  if (searched != NULL){
    int omit = 0;
    matchAndNo searched2[512];
    matchAndNo searched2res = searchdb2(db2, lineNo, omit);
    while (searched2res != NULL) {
      searched2[omit] = searched2res;
      omit++;
      searched2res = searchdb2(db2, lineNo, omit);
    }
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
    double avers[512];
    for (int i = 0; i < (sizeof(searched3) / sizeof(searched3[0]); i++)){
      char temp = '0';
      char beginning[512];
      int count = 0;
      while (temp != '~'){
        beginning[count] = searched3[i].b[count];
        count++;
      }
      avers[i] = atoi(beginning);
    }

    double cumulativeProbability = 0;
    time_t t;
    srand((unsigned) time(&t));
    double randomChosen = (double)rand() / (double)RAND_MAX ;
    for (int i = 0; i < sizeof(avers); i++)){
      cumulativeProbability = cumulativeProbability + avers[i];
      if (randomChosen <= cumulativeProbability){
        for (int j = 0; j < (sizeof(searched3) / sizeof(searched3[0]); j++){
          if (strstr(searched3[j].b, itoa(avers[i])) != NULL){
            int linedb2 = atoi(splitby(searched3[j].b, "~"));
          }
        }
        char *lineIndb2 = getline(db2, linedb2);
        char toRespond[512];
        for (int k = 0; k < strlen(lineIndb2); k++);{
          if (lineIndb2[k] == '~'){
            break;
          }
          else{
            torespond[k] = lineIndb2[k];
          }
        }
        printf("%s\n", toRespond);
      }
    }
  }
  else{
    return NULL;
  }
}
