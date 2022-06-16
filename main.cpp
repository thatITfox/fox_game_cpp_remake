#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <math.h>

const int WIDTH = 800, HEIGHT = 600;
int fox_x = 370, fox_y = 480;
int blueberry_x = rand() % WIDTH-64, blueberry_y = 0;
int player_speed = 0;

int GetDistance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Max fox game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;

    // load the background image
    SDL_Surface* surface = IMG_Load("images/background.png");

    // load the fox and blueberry images
    SDL_Surface* fox = IMG_Load("images/fox.png");
    SDL_Surface* blueberry = IMG_Load("images/blueberry.png");
    // check if the image loaded correctly
    if (surface == NULL)
    {
        std::cout << "Error: " << IMG_GetError() << std::endl;
        return 1;
    }

    bool running = true;
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }
            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        player_speed = -1;
                        break;
                    case SDLK_RIGHT:
                        player_speed = 1;
                        break;
                }
            }
            if(event.type == SDL_KEYUP)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        if(player_speed == -1)
                        {
                            player_speed = 0;
                        }
                        break;
                    case SDLK_RIGHT:
                        if(player_speed == 1)
                        {
                            player_speed = 0;
                        }
                        break;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // render the background image
        SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, background, NULL, NULL);

        // render the blueberry based on its x and y coordinates
        SDL_Rect blueberry_rect = {blueberry_x, blueberry_y, blueberry->w, blueberry->h};
        SDL_Texture* blueberry_texture = SDL_CreateTextureFromSurface(renderer, blueberry);
        SDL_RenderCopy(renderer, blueberry_texture, NULL, &blueberry_rect);

        // render the fox image in the middle of the screen
        // SDL_Rect foxRect = {WIDTH/2 - fox->w/2, HEIGHT/2 - fox->h/2, fox->w, fox->h};
        SDL_Rect foxRect = {fox_x, fox_y, fox->w, fox->h};
        SDL_Texture* foxTexture = SDL_CreateTextureFromSurface(renderer, fox);
        SDL_RenderCopy(renderer, foxTexture, NULL, &foxRect);

        fox_x += player_speed;
        if(fox_x < 0)
        {
            fox_x = 0;
        }
        if(fox_x > WIDTH - fox->w)
        {
            fox_x = WIDTH - fox->w;
        }
        blueberry_y += 1;
        if (blueberry_y > HEIGHT)
        {
            blueberry_x = rand() % WIDTH;
            blueberry_y = 0;
        }


        if(GetDistance(fox_x, fox_y, blueberry_x, blueberry_y) < fox->w/2 + blueberry->w/2)
        {
            blueberry_x = rand() % WIDTH;
            blueberry_y = 0;
        }
        SDL_RenderPresent(renderer);

        // clean up the texture so it doesn't leak memory
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(foxTexture);
        SDL_DestroyTexture(blueberry_texture);
    }

    // clean up all the surfaces
    SDL_FreeSurface(surface);
    SDL_FreeSurface(fox);
    SDL_FreeSurface(blueberry);

    // clean up the window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}