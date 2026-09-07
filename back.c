#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  char words[30];
  char phenome[20][10];
  int phenome_count;
}word;

typedef struct {
  word *data;
  int size;
  int cap;
}list;

word dict[133000];

void make_list (list *list){
  list->size=0;
  list->cap=1;
  list->data=malloc(list->cap*sizeof(word));
}

void add_list (list *list, word item)
{
  if(list->size==list->cap){
    list->cap *=2;
    word *tmp=realloc(list->data, list->cap * sizeof(word)); 
    list->data=tmp; 
  }

  list->data[list->size]=item;
  list->size++;
}


void print_list (list *list){

  for( int i = 0;  i < list->size;  i++ )
    printf("%d. %s", i+1, list->data[i].words);
}

void remove_from_list (list *list, int i ){


  for(; i < list->size; i++){
    list->data[i-1]=list->data[i];
  }

  list->size--;
}

typedef struct{
  int letters;
  int words;
  int sentence;
  char text[50];
}all;

void count_all(all *all){
  all->letters = 0;
  all->words   = 0;
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

int formulate (all *all){
   float L = (float) all->letters / all->words * 100;
   float S = (float) all->sentence / all->words * 100;
   int grade = round(0.0588 * L - 0.296 * S - 15.8);
   return grade;
}
void parse_dict(word dict[]){
  FILE *file = fopen("cmudict.dict", "r");

  for(int i = 0; i < 5; i++ ){

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


