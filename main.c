#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 320
#define HEIGHT 200

uint32_t framebuffer[WIDTH * HEIGHT];

void put_pixel (int x, int y, uint32_t color) {
  if (x < 0 || x >= WIDTH|| y < 0 || y >= HEIGHT) {
    return;
  }
  framebuffer[WIDTH * y + x] = color;
}

void clear (uint32_t color){
  for (int i = 0; i < WIDTH * HEIGHT; i++){
 framebuffer[i] = color;
  }
}


  int colors[] = {
  0xFF0000, 0x00FF00, 
  0x0000FF,0xFFFF00,
  0x00FFFF, 0x000000,
  0xFFFFFF, 0xA2A2A2
};

int main(void){
  SDL_Window *window; 
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  SDL_Event event;

  const double frame_target = 1.0 / 60;

  if (!SDL_Init(SDL_INIT_VIDEO)){
    fprintf(stderr, "SDL Init failed: %s", SDL_GetError());
    int status = EXIT_FAILURE;
    return status;
  }

  window = SDL_CreateWindow(
    "SDL Framebuffer",
    WIDTH * 4,
    HEIGHT * 4,
    0
  );

  if( window == NULL) {
    printf("SDL_CreateWindow failed, %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  renderer = SDL_CreateRenderer(
    window,
    NULL
  );

  if( renderer == NULL) {
    printf("SDL_CreateRenderer failed, %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    return EXIT_FAILURE;
  }

  texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_XRGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    WIDTH,
    HEIGHT
  );

SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  if( texture == NULL) {
    printf("SDL_CreateRenderer failed, %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);
    return EXIT_FAILURE;
  }

  uint8_t is_running = 1;

  while(is_running){

    uint64_t start = SDL_GetPerformanceCounter();

    while(SDL_PollEvent(&event)){
      if(event.type == SDL_EVENT_QUIT){
        is_running = 0;
      }
    }



    clear(0x2A2A2A);
    // draw pixel content to framebuffer array
    for ( int i = 0; i < WIDTH; i++) {
      for ( int j = 0; j < HEIGHT; j++) {
        put_pixel(i, j, colors[0]);
      }
    }

    /* Copy the contents of the framebuffer to the texture */
    SDL_UpdateTexture (
      texture,
      NULL,
      framebuffer,
      WIDTH * sizeof(uint32_t)
    );

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    uint64_t end = SDL_GetPerformanceCounter();

    double elapsed = (double) (end -  start) / (double) SDL_GetPerformanceFrequency();
    if (elapsed < frame_target ) {
      SDL_Delay((frame_target - elapsed) * 1000);
    }
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return EXIT_SUCCESS;
}


