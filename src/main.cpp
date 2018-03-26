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

#include <SDL2/SDL.h>
#include <iostream>

#include "FlockingConfig.h"
#include "ui/window/mainwindow.h"
#include "ui/input/user_input.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

int main(int argc, char * argv[]) {
  std::cerr << "Flocking_SDL version "
      << Flocking_VERSION_MAJOR << "."
      << Flocking_VERSION_MINOR << "\n";

  MainWindow main_window(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (main_window.has_correct_init() &&
      main_window.load_media_bg("../data/bg.bmp")) {
    std::cout << "The window initialised without issue !\n";
  }

  bool quit = false;
  SDL_Event e;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    main_window.update();
  }

  return 0;
}
