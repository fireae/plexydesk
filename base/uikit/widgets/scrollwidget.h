/*
<one line to give the program's name and a brief idea of what it does.>
Copyright (C) 2012  <copyright holder> <email>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLEXYDESK_SCROLL_WIDGET_H
#define PLEXYDESK_SCROLL_WIDGET_H
#include <plexy.h>
#include <config.h>
#include <desktopwidget.h>

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsObject>
#include <plexydesk_ui_exports.h>

namespace UI {

class DECL_UI_KIT_EXPORT ScrollWidget : public Widget {
  Q_OBJECT
public:
  ScrollWidget(const QRectF &rect, QGraphicsObject *parent = 0);
  virtual ~ScrollWidget();

  void setViewport(QGraphicsObject *widget);
  void scrollBy(int x, int y);

  virtual void paintView(QPainter *painter, const QRectF &rect);

private:
  virtual void wheelEvent(QGraphicsSceneWheelEvent *event);
  virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
  virtual bool event(QEvent *event);
  virtual bool sceneEvent(QEvent *e);

  class Private;
  Private *const d;
};
}

#endif
