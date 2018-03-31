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
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int inputsize = 256, bufferchar = 0, count = 0;
char inp[256]; //Had to restrict memory usage somehow, could have done this on the heap using malloc but would have caused excess bother

void getinp(){ //gets user input
  printf(">>>"); // Print CLI thing
  while((bufferchar = getchar()) != '\n' && bufferchar != EOF){ //Clear input buffer to get input
    if(count < inputsize) //If we haven't got enough characers yet
      inp[count++] = bufferchar; //Add the input to the inp
    }
    inp[count]='\0'; //Terminate the string
    count = 0; //Reset the count for next time
}


int main(int argc, char const *argv[]) {
  //TODO Load 'database' file
  do {
    getinp();
  } while(strstr(inp, "{~}") != NULL);
  while (!(strcmp(inp, '{END CHAT}'))){ //While they want to carry on talking
    //TODO Search file for the call, if not there call method to ask user how to respond
    //TODO Check for possible responses
    //TODO Generate OTF context rating for each response
    //TODO Average each context rating with each file rating
    //TODO Output biggest Average
    printf("I would do some cool caluclations here and respond but as yet I am unable \n");
    do {
      getinp();
    } while(strstr(inp, "{~}") != NULL);
  }
}

//TODO Once this file is finished split into multiple files and make a Makefile
