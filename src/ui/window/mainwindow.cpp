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
    : gWindow(NULL), gScreenSurface(NULL) {
  success = true;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    } else {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }
}

bool MainWindow::load_media_bg(std::string path) {
  // Loading success flag
  bool success = true;
  // Load image at specified path
  SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
  if (loadedSurface == NULL) {
    success = false;
    std::cerr << "Unable to load image " << path
              << "! SDL Error: " << SDL_GetError() << "\n";
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

void MainWindow::update() {
  // Prepare the scaling surface
  SDL_Rect stretchRect;
  stretchRect.x = 0;
  stretchRect.y = 0;
  SDL_GetWindowSize(gWindow, &stretchRect.w, &stretchRect.h);

  // Blit the different sprites
  SDL_BlitScaled(g_bg_surface, NULL, gScreenSurface, &stretchRect);

  // Put back buffer in front
  SDL_UpdateWindowSurface(gWindow);
}

MainWindow::~MainWindow() {
  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}
