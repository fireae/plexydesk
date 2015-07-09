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
#ifndef PLEXY_WIDGET_CLOCK_H
#define PLEXY_WIDGET_CLOCK_H

#include <plexy.h>

#include <functional>

#include <view_controller.h>
#include <datasource.h>
#include <QTimer>
#include "clockwidget.h"

class Clock : public CherryKit::ViewController {
  Q_OBJECT

public:
  Clock(QObject *a_parent_ptr = 0);
  virtual ~Clock();

  void init();

  void set_view_rect(const QRectF &rect);
  void session_data_available(const cherry::sync_object &a_session_root);
  virtual void submit_session_data(cherry::sync_object *a_obj);

  bool remove_widget(CherryKit::Widget *widget);

  CherryKit::ActionList actions() const;
  void request_action(const QString &actionName, const QVariantMap &args);

  QString icon() const;

  QAction *createAction(int id, const QString &action_name,
                        const QString &icon_name);
public
Q_SLOTS:
  void onDataUpdated(const QVariantMap &data);

private:
  class PrivateClockController;
  PrivateClockController *const o_view_controller;
};

#endif
