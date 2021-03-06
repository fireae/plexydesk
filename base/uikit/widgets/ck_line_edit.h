/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@plexydesk.com>
*  Authored By  :
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/
#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <ck_style.h>
#include <ck_widget.h>

#include <plexydesk_ui_exports.h>

namespace cherry_kit {
class DECL_UI_KIT_EXPORT line_edit : public widget {
  Q_OBJECT
public:
  explicit line_edit(widget *a_parent_ptr = 0);
  virtual ~line_edit();
  virtual void set_text(const QString &a_txt);
  virtual QString text() const;

  virtual void style(style_ref a_style);

  virtual void set_size(const QSizeF &a_size);
  virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &a_constraint) const;

  virtual void on_insert(std::function<void(const QString &a_txt)> a_handler);

  virtual QString current_text_selection() const;

   virtual bool readonly();
   virtual void set_readonly(bool a_value);

   virtual bool is_password_input();
   virtual void set_password_input(bool a_secure);

   virtual bool has_input_focus() const;
   virtual void set_input_focus(bool a_focus);

Q_SIGNALS:
  void submit();
  void text(const QString &a_text);

protected:
  virtual void paint_view(QPainter *a_painter_ptr, const QRectF &a_rect);

  virtual bool eventFilter(QObject *a_object_ptr, QEvent *a_event_ptr);

  virtual void mousePressEvent(QGraphicsSceneMouseEvent *a_event_ptr);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event_ptr);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *a_event_ptr);
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *a_event_ptr);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event_ptr);
  virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *a_event_ptr);

  virtual void keyPressEvent(QKeyEvent *a_event_ptr);

  QRectF current_text_rect(QPainter *a_painter) const;

private:
  class PrivateLineEdit;
  PrivateLineEdit *const priv;
};
}
#endif // LINEEDIT_H
