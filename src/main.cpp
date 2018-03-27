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
#include <vector>

#include "FlockingConfig.h"
#include "entity/ant/ant.h"
#include "ui/input/user_input.h"
#include "ui/window/mainwindow.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define ANT_COUNT 60
#define FRAMERATE 60

int main(int argc, char *argv[]) {
  std::cerr << "Flocking_SDL version " << Flocking_VERSION_MAJOR << "."
            << Flocking_VERSION_MINOR << "\n";

  MainWindow main_window(WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!(main_window.has_correct_init() &&
      main_window.load_media_bg("../data/bg.png") &&
      main_window.load_texture("../data/bg.png"))) {
    std::cerr << "Problem during window initalization !\n";
    return 1;
  }

  // Initialize time variables
  float frame_in_ms = 1000.0f / FRAMERATE;
  World::set_time_step(frame_in_ms);

  // Initialize game loop variables
  bool quit = false;
  SDL_Event e;

  std::vector<Ant*> ant_list;
  for (int i = 0; i < ANT_COUNT; ++i) {
    ant_list.push_back(new Ant(i, main_window));
  }

  while (!quit) {
    float start_ms = SDL_GetTicks();
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    // Update the canvas
    main_window.clear_and_draw_bg();
    for (auto ant : ant_list) {
      ant->decision();
      ant->update();
    }
    main_window.update();

    // Limit Framerate
    float end_ms = SDL_GetTicks();
    float delay_ms = frame_in_ms - (end_ms - start_ms);
    if (delay_ms > 0) {
      SDL_Delay(delay_ms);
    }
  }

  for (auto ant : ant_list) {
    delete ant;
  }
  return 0;
}
