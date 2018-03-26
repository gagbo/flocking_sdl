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
#include <iostream>
#include <string>

class MainWindow {
 public:
  MainWindow(int width, int height);
  ~MainWindow();

  bool load_media_bg(std::string);
  void update();
  inline bool has_correct_init() const {return success;}

  static SDL_Surface *g_bg_surface;


 protected:
  SDL_Window* gWindow;
  SDL_Surface* gScreenSurface;
  bool success;
};

#endif // UI_WINDOW_MAINWINDOW_H
