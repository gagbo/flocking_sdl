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
#include <cassert>
#include <iostream>
#include <vector>

#include "FlockingConfig.h"
#include "entity/entity.h"
#include "ui/input/user_input.h"
#include "ui/window/mainwindow.h"
#include "world/world.h"

#ifndef NDEBUG
#include <iomanip>
#include <map>
#include <string>
#endif

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define ANT_COUNT 60
#define FRAMERATE 60

int main(int argc, char* argv[]) {
    std::cerr << "Flocking_SDL version " << Flocking_VERSION_MAJOR << "."
              << Flocking_VERSION_MINOR << "\n";

    std::cerr << argc << " argument(s) -> ";
    for (int i = 0; i < argc; ++i) {
        std::cerr << argv[i] << " ";
    }
    std::cerr << std::endl;

    World world;
    MainWindow main_window(WINDOW_WIDTH, WINDOW_HEIGHT, world);
#ifndef NDEBUG
    std::map<int, int> hist;
#endif  // NDEBUG

    if (!(main_window.has_correct_init() &&
          main_window.load_media_bg(DATA_DIR "bg.png") &&
          main_window.load_texture(DATA_DIR "bg.png"))) {
        std::cerr << "Problem during window initalization !\n";
        return 1;
    }

    // Initialize time variables
    float frame_in_ms = 1000.0f / FRAMERATE;
    world.set_time_step(frame_in_ms/1000.0);

    // Initialize game loop variables
    bool quit = false;
    SDL_Event e;

    if (argc <= 1) {
        for (int i = 0; i < ANT_COUNT; ++i) {
            world.add_entity(Entity::Type::ANT);
        }
    } else {
        std::fstream events_file;
        events_file.open(argv[1]);
        assert(events_file.is_open());
        world.add_events(events_file);
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
        world.update();
        main_window.update();

        // Limit Framerate
        float dura_ms = SDL_GetTicks() - start_ms;
#ifndef NDEBUG
        std::cerr << "Duration : " << dura_ms << "\n";
        ++hist[std::floor(dura_ms)];
#endif  // NDEBUG
        float delay_ms = frame_in_ms - dura_ms;
        if (delay_ms > 0) {
            SDL_Delay(delay_ms);
        }
    }
#ifndef NDEBUG
    int hist_scale(100);
    std::cerr << "Each star is " << hist_scale << " frames\n";
    for (auto p : hist) {
        std::cerr << std::fixed << std::setprecision(1) << std::setw(2)
                  << p.first << ' ' << std::string(p.second / hist_scale, '*')
                  << '\n';
    }
#endif  // NDEBUG

    return 0;
}
