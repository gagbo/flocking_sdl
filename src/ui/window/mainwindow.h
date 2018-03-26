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

class MainWindow {
public:
  MainWindow(int width, int height);
  ~MainWindow();

  void add_FillRect_to_renderer(int w0, int h0, int w_total, int h_total,
                                int color[4]);
  void add_DrawRect_to_renderer(int w0, int h0, int w_total, int h_total,
                                int color[4]);

  bool load_media_bg(std::string path);
  bool load_texture(std::string path);
  void clear_and_draw_bg();
  void update();
  inline bool has_correct_init() const { return success; }

  static SDL_Surface *g_bg_surface;

protected:
  SDL_Window *gWindow;
  SDL_Surface *gScreenSurface;
  SDL_Renderer *gRenderer;
  SDL_Texture *gTexture;
  int bg_render_color[4];
  bool success;
};

#endif // UI_WINDOW_MAINWINDOW_H
