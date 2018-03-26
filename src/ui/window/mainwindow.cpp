/* Copyright (c) 2018 Gerry Agbobada
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

SDL_Surface *MainWindow::g_bg_surface = NULL;

MainWindow::MainWindow(int width, int height)
    : gWindow(NULL), gScreenSurface(NULL), gRenderer(NULL), gTexture(NULL),
      bg_render_color{0xFF, 0xFF, 0x00, 0xFF}, success(true) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError()
              << "\n";
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      std::cerr << "Window could not be created! SDL Error: " << SDL_GetError()
                << "\n";
      success = false;
    } else {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
      if (gRenderer == NULL) {
        success = false;
        std::cerr << "Renderer could not be created ! SDL Error: "
                  << SDL_GetError() << "\n";
      } else {
        // Initialise Renderer color
        SDL_SetRenderDrawColor(gRenderer, bg_render_color[0],
                               bg_render_color[1], bg_render_color[2],
                               bg_render_color[3]);
        // Initialise PNG Loading
        int img_flags = IMG_INIT_PNG;
        if (!(IMG_Init(img_flags) & img_flags)) {
          std::cerr << "SDL_image could not initialize! SDL_image Error: "
                    << IMG_GetError() << "\n";
          success = false;
        } else {
          // Get window surface
          gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
      }
    }
  }
}

bool MainWindow::load_media_bg(std::string path) {
  // Loading success flag
  bool success = true;
  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    success = false;
    std::cerr << "Unable to load image " << path
              << "! SDL Error: " << IMG_GetError() << "\n";
  } else {
    // Convert surface to screen format
    g_bg_surface =
        SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
    if (g_bg_surface == NULL) {
      std::cerr << "Unable to optimize image " << path
                << "! SDL Error: " << SDL_GetError() << "\n";
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  return success;
}

bool MainWindow::load_texture(std::string path) {
  // Loading success flag
  bool success = true;
  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    success = false;
    std::cerr << "Unable to load image " << path
              << "! SDL Error: " << IMG_GetError() << "\n";
  } else {
    // Convert surface to texture
    gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (gTexture == NULL) {
      std::cerr << "Unable to create texture from " << path
                << "! SDL Error: " << SDL_GetError() << "\n";
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  return success;
}

void MainWindow::update() {
  // Reset Render color
  SDL_SetRenderDrawColor(gRenderer, bg_render_color[0], bg_render_color[1],
                         bg_render_color[2], bg_render_color[3]);
  // Clear screen
  SDL_RenderClear(gRenderer);

  // Prepare the scaling surface
  SDL_Rect stretchRect;
  stretchRect.x = 0;
  stretchRect.y = 0;
  SDL_GetWindowSize(gWindow, &stretchRect.w, &stretchRect.h);

  // Blit the different sprites
  SDL_BlitScaled(g_bg_surface, NULL, gScreenSurface, &stretchRect);

  // Render texture to screen
  SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

  // Update screen
  SDL_RenderPresent(gRenderer);

  // Put back buffer in front
  SDL_UpdateWindowSurface(gWindow);
}

MainWindow::~MainWindow() {
  SDL_DestroyTexture(gTexture);
  gTexture = NULL;

  // Destroy window
  SDL_DestroyRenderer(gRenderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  gRenderer = NULL;

  SDL_FreeSurface(g_bg_surface);
  g_bg_surface = NULL;

  // Quit SDL subsystems
  IMG_Quit();
  SDL_Quit();
}
