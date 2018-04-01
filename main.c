/*
Project outline for my chatbot, made and conceptualised by Dan De Burgo
  1.) This project will take in a 'database' file containing calls and their responses with a rating as to how likely
      we are to use them in chat. I will also include a context rating but not in the file. This will be generated on
      the fly by attempting to determine the similarity of a snapshot of the conversation with a path from the start of
      this snapshot to the current call. I will take an average of the OTF context rating and the file rating and the
      highest of this average will be the response we use.
  2.) If this chatbot does not know how to respond to a call it will ask the user how they would respond and store that
      in the 'database' file.
  3.) All things the user say will be calls, these calls will be treated as responses when stored in the file if the
      bot has asked how they would respond.
  4.) Multiple calls will be concatenated to form one call. The user will type '{~}' to tell the bot when to respond
  5.) If the user wishes to end their chat they should type '{END CHAT}'

  database layout:

  ------BEGIN FILE------
  call / response / rating
  call / response / rating
  call / response / rating
  ------ END FILE ------
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int inputsize = 256, bufferchar = 0, count = 0;
char inp[256]; //Had to restrict memory usage somehow, could have done this on the heap using malloc but would have caused excess bother
char line[512];
int resps[512];
int respsize = 0;
char * pointchar;
int boolfound = 0;

void getinp(){ //gets user input
  printf(">>>"); // Print CLI thing
  while((bufferchar = getchar()) && bufferchar != '\n' && bufferchar != EOF){ //Clear input buffer to get input
    if(count < inputsize) //If we haven't got enough characers yet
      inp[count++] = bufferchar; //Add the input to the inp
    }
    inp[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
}


int main(int argc, char const *argv[]) {
  //Load 'database' file
  database = fopen("database.txt","r+");


  do {
    getinp();
  } while(!(strstr(inp, "{-}") != NULL));
  boolfound = 0;


  while (strcmp(inp, "{END CHAT}")){ //While they want to carry on talking
    //TODO check the user hasn't typed "{REP}" to replace the response in the database
    if (strcmp(inp, "{REP}")){
      //TEMP Bruteforce search while I work out how to seek
      while ((c = getc(database)) != EOF){
        if (c == '\n'){
          //We have a line do stuff to it
          pointchar = strtok(line,"/");
          while (pointchar != NULL)
          {
            //printf("%s\n", pointchar);
            //TODO Check for possible responses
            if (strcmp(pointchar, inp)){
                boolfound = 1;
                pointchar = strtok(NULL, "/");
                resps[respsize] = atoi(pointchar);
                respsize++;
            }
            pointchar = strtok(NULL, "/"); //TEMP unsure about this NULL
          }
          line = "";
        }
        else{
          line = line + c;
        }
      }
    }
    else{
      //NEED TO FETCH PREVIOUS INP SOMEHOW
    }

    if (boolfound){
      //TODO Generate OTF context rating for each response
      //TODO Average each context rating with each file rating
      //TODO Output biggest Average
    }
    else{
      printf("How would you respond to that? \n")
      fprintf(database, "%s / ", inp);
      do {
        getinp();
      } while(strstr(inp, "{-}") != NULL);
      fprintf(database, "%s / 1\n", inp);
      //Add to the file with the response and file rating
    }
    do {
      getinp();
    } while(strstr(inp, "{-}") != NULL);
    boolfound = 0;
  }
  fclose(database);
}

//TODO Once this file is finished split into multiple files and make a Makefile
