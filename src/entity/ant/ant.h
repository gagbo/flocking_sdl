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

#ifndef ENTITY_ANT_ANT_H_
#define ENTITY_ANT_ANT_H_
#include <iostream>

#include "ui/window/mainwindow.h"

class Ant {
 public:
  Ant();
  Ant(MainWindow& window);
  ~Ant();

 protected:
  MainWindow *render_window;
};

#endif // ENTITY_ANT_ANT_H_
