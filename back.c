#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char words[30];
  char phenome[20][30];
  int phenome_count;
}dictionary;

// typedef struct {
//   word *data;
//   int size;
//   int cap;
// }list;

dictionary dict[135166];
dictionary temp[30];
dictionary key[30];
// void make_list (list *list){
//   list->size=0;
//   list->cap=1;
//   list->data=malloc(list->cap*sizeof(word));
// }
//
// void add_list (list *list, dictionary item)
// {
//   if(list->size==list->cap){
//     list->cap *=2;
//     word *tmp=realloc(list->data, list->cap * sizeof(word)); 
//     list->data=tmp; 
//   }
//
//   list->data[list->size]=item;
//   list->size++;
// }
//
//
// void print_list (list *list){
//
//   for( int i = 0;  i < list->size;  i++ )
//     printf("%d. %s", i+1, list->data[i].words);
// }
//
// void remove_from_list (list *list, int i ){
//
//
//   for(; i < list->size; i++){
//     list->data[i-1]=list->data[i];
//   }
//
//   list->size--;
// }

typedef struct{
  int letters;
  int words;
  int sentence;
  char text[500];
  char individual[30][30];
}content;

void count_all(content *all){
  all->letters = 0;
  all->words   = 1;
  all->sentence = 0;

  for(int i =0, len = strlen(all->text);i<len; i++){
    if(isalpha(all->text[i])){
      all->letters++;

    }

    if (all->text[i] == ' '){
      all->words++;
    }

    if (all->text[i] == '.' || all->text[i] == '?'){
      all->sentence++;
    }
  }
}



int formulate (content *all){
   float L = (float) all->letters / all->words * 100;
   float S = (float) all->sentence / all->words * 100;
   int grade = round(0.0588 * L - 0.296 * S - 15.8);
   return grade;
}
void parse_dict(dictionary dict[]){
  FILE *file = fopen("cmudict.dict", "r");
  
  for(int i = 0; i < 135166; i++ ){

    fscanf(file,"%s", dict[i].words );
    dict[i].phenome_count=0;
    char peek = 0;
    while(peek !='\n'){

      fscanf(file,"%s", dict[i].phenome[dict[i].phenome_count]);
      dict[i].phenome_count++;
      peek=fgetc(file);
      if(peek=='\n')break;
      ungetc(peek, file);

    }

  }
  fclose(file);
}


void convert(dictionary dict[],dictionary word[], content *yes ){
  for (int n = 0; yes->text[n]; n++) {
    printf("%02X ", (unsigned char)yes->text[n]);
}
printf("\n");
  char *token = strtok(yes->text, " \t\n\r,.;:!?");
  int i = 0;
  while(token!=NULL && i<yes->words){

    strcpy(yes->individual[i], token);
   
    i++;
    token= strtok(NULL, " \t\n\r,.;:!?");
  }
  
  

  for(int x=0; x<yes->words; x++){
    printf("Searching for: '%s'\n", yes->individual[x]);  
    for(int z=0;z<135166; z++){
      if(strcmp(dict[z].words, yes->individual[x])==0){
        word[x]=dict[z];
        printf("%s\n",word[x].words);
        for(int v=0; v<word[x].phenome_count; v++){
          printf("%s\n",word[x].phenome[v]);
        }

      }
    }
  }

}

void calculate(dictionary word[], dictionary key[], content *yes){

  int u;
  for(int i=0; i<yes->words;i++){
    u=0;
    for(int x=word[i].phenome_count;x>0;x--){
      if(x==word[i].phenome_count)continue;
      u++;
      for(int v =0; word[i].phenome[x][v]!='\0'; v++){

        key[i].phenome[u][v] = word[i].phenome[x][v];
        if(word[i].phenome[x][v]=='0' || word[i].phenome[x][v]=='1' || word[i].phenome[x][v]=='2'){
          key[i].phenome[u][v]='\0';
          break;
        }
      }
      key[i].phenome_count = u;
    }
  }
}

float grade(dictionary key[], content *yes){
  int matches[30] = {0};
  int total = 0;

  for(int i = 0; i < yes->words; i++){
    for(int x = 0; x < yes->words; x++){
      //if(i == x) continue;
      for(int v = 1; v <= key[i].phenome_count && v <= key[x].phenome_count; v++){
        if(strcmp(key[i].phenome[v], key[x].phenome[v]) == 0){
          matches[i]++;
          break;
        }
      }
    }
  }
  
  int best = 0;
  for(int i = 1; i < yes->words; i++){
    if(matches[i] > matches[best]){
      best = i;
      

    }
  }


  float score = (float)matches[best]*100/ yes->words;

    printf("%s has the most matches (%d)\n", key[best].words, matches[best]);
  printf("rhyme score: %.2f\n", score);
  return score;
  }
