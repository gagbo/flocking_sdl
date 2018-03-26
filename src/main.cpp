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
#include "window/mainwindow.h"

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

int main(int argc, char * argv[]) {
  std::cerr << "Flocking_SDL version "
      << Flocking_VERSION_MAJOR << "."
      << Flocking_VERSION_MINOR << "\n";

  MainWindow main_window(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (main_window.has_correct_init()) {
    std::cout << "The window initialised without issue !\n";
  }

  return 0;
}
