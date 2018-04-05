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

matchAndNo searchdb(FILE *db, char *toFind){
  char line[512];
  int lineNum = 1;
  while(fgets(line, 512, db) != NULL) {
		if((strstr(line, toFind)) != NULL) {
      matchAndNo r = {lineNum, line};
      return r;
		}
		lineNum++;
	}
}

void replaceinp(FILE *db1, FILE *db2, FILE *db3, char *toRep){
  matchAndNo searched = searchdb(db1, toRep);
  char toRep[512] = searched.b;
  int lineNo = searched.a;

  //TODO FINISH THIS


}

void notsurehowtorespond(FILE *db1, FILE *db2, FILE *db3, char *notSureAbout){
  printf("How would you respond to that? \n")
  do {
    getinp();
  } while(strstr(inp, "{-}") != NULL);
  matchAndNo searched = searchdb(db1, inp);
  int lineNo = searched.a;
  fprintf(db1, "%s\n", notSureAbout);
  fprintf(db2, "%s,%i\n", inp, lineNo);
  matchAndNo searched = searchdb(db2, inp);
  lineNo = searched.a;
  fprintf(db3, "1,%i\n", lineNo);
}

char *respond(char *call){
  //TODO
}
