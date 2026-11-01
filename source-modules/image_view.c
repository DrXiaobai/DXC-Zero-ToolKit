#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char *argv[])
{
  if(argc < 2){
    printf("Usage: %s <image_path>\n", argv[0]);
    return 1;
  }

  if(SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("SDL Initialization failed: %s\n", SDL_GetError());
    return 1;
  }

  int w, h, channels;
  unsigned char *p_img_data = stbi_load(argv[1], &w, &h, &channels, 4);
  if(!p_img_data){
    printf("Failed to load image: %s\n", argv[1]);
    SDL_Quit();
    return 1;
  }

  SDL_Window *p_window = SDL_CreateWindow("Image Viewer --DXC Zero", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
  SDL_Renderer *p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *p_texture = SDL_CreateTexture(p_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, w, h);
  SDL_UpdateTexture(p_texture, NULL, p_img_data, w*4);

  SDL_Event e;
  int running = 1;
  while(running){
    while(SDL_PollEvent(&e)){
      if(e.type == SDL_QUIT){running = 0;}
    }

    SDL_RenderClear(p_renderer);
    SDL_RenderCopy(p_renderer, p_texture, NULL, NULL);
    SDL_RenderPresent(p_renderer);

    SDL_Delay(16);
  }

  SDL_DestroyTexture(p_texture);
  SDL_DestroyRenderer(p_renderer);
  SDL_DestroyWindow(p_window);
  SDL_Quit();
  stbi_image_free(p_img_data);

  return 0;
}

