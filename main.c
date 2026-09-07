#define SDL_MAIN_USE_CALLBACKS 1

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "back.c"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>


#define WIDTH 600
#define HEIGHT 400
all yes;
typedef struct{
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
TTF_TextEngine *engine;
TTF_Text *text; 
TTF_Font *font;
}App;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
 
  App *app=SDL_malloc(sizeof(App));
  *appstate=app;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

 
  SDL_CreateWindowAndRenderer("SDL Framebuffer", WIDTH, HEIGHT,SDL_WINDOW_RESIZABLE,&app->window, &app->renderer );

  app->texture=SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  app->engine = TTF_CreateRendererTextEngine(app->renderer);

  SDL_StartTextInput(app->window);

  app->font=TTF_OpenFont("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Medium.ttf", 24);

  app->text=TTF_CreateText(app->engine, app->font, yes.text ,0);

  TTF_SetTextWrapWidth(app->text, 0);

  char dict[1024] = parse_dict();

  return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
  App *app = appstate;
  int textW, textH; 
  TTF_GetTextSize(app->text, &textW, &textH);
  
  int winW, winH;
  SDL_GetWindowSize(app->window, &winW, &winH);

  int x = (winW - textW)/2;
  int y = (winH - textH)/2;



  SDL_RenderClear(app->renderer); 

  TTF_SetTextColor(app->text, 255, 255, 255, 255);

  TTF_DrawRendererText(app->text, x, y);

  SDL_RenderPresent(app->renderer);

  return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event *event){
  App *app=appstate;
  switch(event->type){
    case SDL_EVENT_TEXT_INPUT:
      strcat(yes.text, event->text.text);
      TTF_SetTextString(app->text, yes.text, 0);
      break;
    case SDL_EVENT_KEY_DOWN:
      if(event->key.scancode==SDL_SCANCODE_BACKSPACE){
        yes.text[SDL_strlen(yes.text)-1]='\0';
        TTF_SetTextString(app->text, yes.text, 0);
      }

      if(event->key.scancode==SDL_SCANCODE_RETURN){
        yes.text[SDL_strlen(yes.text)]='\n';
      }

      if(event->key.scancode == SDL_SCANCODE_ESCAPE ){
        count_all(&yes);
        int grade = formulate(&yes);
        snprintf(yes.text, sizeof(yes.text), "%d", grade);


        TTF_SetTextString(app->text, yes.text, 0);


      }

      break;

    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
      break;


  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
  App *app = appstate;
  SDL_DestroyTexture(app->texture);
  SDL_DestroyRenderer(app->renderer);
  SDL_DestroyWindow(app->window);
  TTF_CloseFont(app->font);
  TTF_Quit();
  SDL_Quit();

}
