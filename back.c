
typedef struct {
  char words[30];
  char phenome;
}word;

typedef struct {
  word *data;
  int size;
  int cap;
}list;


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
    printf("%d. %s", i+1, list->data[i].sentance);
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


