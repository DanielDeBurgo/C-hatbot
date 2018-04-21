#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/unistd.h>
#include "Backchat.h"

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

/*
int main(int argc, char const *argv[]) {
  char Squig[512] = "";
  for (int i = 0; i < 512; i++){
    toReturn2[i].lineText = Squig;
  }

  FILE *testfile1 = fopen("test.txt", "r");
  FILE *testfile2 = fopen("test2.txt", "r+");

  printf("%s\n", respond("Hi", testfile1, testfile2));

  fclose(testfile1);
  fclose(testfile2);
  return 0;
}
*/
