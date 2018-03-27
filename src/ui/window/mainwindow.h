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

#ifndef UI_WINDOW_MAINWINDOW_H
#define UI_WINDOW_MAINWINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "world/world.h"

// Class that encapsulates SDL Renderer and display window
class MainWindow {
public:
  // Default constructor has 2 parameters for the size of the window in pixels
  MainWindow(int width, int height, World& world);
  ~MainWindow();

  // Add a FillRect of given color to the renderer for the next frame
  void add_FillRect_to_renderer(int w0, int h0, int w_total, int h_total,
                                int color[4]);
  // Add a DrawRect (outline only) of given color to the renderer for the next
  // frame
  void add_DrawRect_to_renderer(int w0, int h0, int w_total, int h_total,
                                int color[4]);

  // Load a picture into the global SDL_Surface g_bg_surface
  bool load_media_bg(std::string path);
  // Load a picture into the texture gTexture
  bool load_texture(std::string path);
  // Clears the window using the Renderer and add background shapes
  void clear_and_draw_bg();
  // Add World-level foreground shapes and put back buffer in front
  void update();
  // Returns true if the initialization of SDL happened without problem
  inline bool has_correct_init() const { return success; }

  // SDL_Surface pointing to the background image
  static SDL_Surface *g_bg_surface;

protected:
  // Pointer to the world associated to the window
  World* displayed_world;
  // Window associated to the SDL app (soon deprecated ?)
  SDL_Window *gWindow;
  // Surface associated to the SDL app
  SDL_Surface *gScreenSurface;
  // Renderer associated to the SDL app
  SDL_Renderer *gRenderer;
  // Example Texture
  SDL_Texture *gTexture;
  // Background color for the Renderer
  int bg_render_color[4];
  // Marks the success of the initialization in construction
  bool success;
};

#endif // UI_WINDOW_MAINWINDOW_H
