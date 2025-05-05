#define CLAY_IMPLEMENTATION
#include "clay.h"

#include <SDL3/SDL.h>

#include <iostream>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

const Clay_Color COLOR_LIGHT = {224, 215, 210, 255};
const Clay_Color COLOR_RED = {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData)
{
    // See the Clay_ErrorData struct for more information
    // printf("%s", errorData.errorText.chars);
    switch(errorData.errorType) {
        // etc
    }
}

// Layout config is just a struct that can be declared statically, or inline
Clay_ElementDeclaration sidebarItemConfig = {
    .layout = {
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
    },
    .backgroundColor = COLOR_ORANGE
};

// Re-useable components are just normal functions
void SidebarItemComponent() {
    CLAY(sidebarItemConfig) {
        // children go here...
    }
}

int main()
{
    // Setup clay
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(arena, { WIDTH, HEIGHT }, { HandleClayErrors });

    // Iniatializer SDL
    if ( ! SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL failed to initiate! %s", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("SDL 3 Window", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Event loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // CLAY LAYOUT
        Clay_BeginLayout();
        CLAY({ .id = CLAY_ID("OuterContainer"), .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = {250,250,255,255} }) {
            CLAY({
                .id = CLAY_ID("SideBar"),
                .layout = { .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .layoutDirection = CLAY_TOP_TO_BOTTOM, },
                .backgroundColor = COLOR_LIGHT
            }) {
                CLAY({ .id = CLAY_ID("ProfilePictureOuter"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } }, .backgroundColor = COLOR_RED }) {
                    // CLAY({ .id = CLAY_ID("ProfilePicture"), .layout = { .sizing = { .width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) }}, .image = { .imageData = &profilePicture, .sourceDimensions = {60, 60} } }) {}
                    CLAY_TEXT(CLAY_STRING("Clay - UI Library"), CLAY_TEXT_CONFIG({ .textColor = {255, 255, 255, 255}, .fontSize = 24 }));
                }

                // Standard C code like loops etc work inside components
                for (int i = 0; i < 5; i++) {
                    SidebarItemComponent();
                }

                CLAY({ .id = CLAY_ID("MainContent"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } }, .backgroundColor = COLOR_LIGHT }) {}
            }
        }
        Clay_RenderCommandArray renderCommands = Clay_EndLayout();
        

        // Clear the screen to a color (black)
        SDL_SetRenderDrawColor(renderer, 100, 50, 25, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; 
}
