/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@plexydesk.com>
*  Authored By  : *
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
#include "timepicker.h"
#include <widget.h>
#include <plexyconfig.h>
#include <QTimer>
#include <view_controller.h>

class TimePickerActivity::PrivateTimePicker {
public:
  PrivateTimePicker() {}
  ~PrivateTimePicker() {}
};

TimePickerActivity::TimePickerActivity(QGraphicsObject *object)
    : CherryKit::DesktopActivity(object), o_desktop_activity(new PrivateTimePicker) {}

TimePickerActivity::~TimePickerActivity() { delete o_desktop_activity; }

void TimePickerActivity::create_window(const QRectF &window_geometry,
                                       const QString &window_title,
                                       const QPointF &window_pos) {
  // todo: invoke UI
}

QRectF TimePickerActivity::geometry() const { return QRectF(); }

QVariantMap TimePickerActivity::result() const { return QVariantMap(); }

CherryKit::Window *TimePickerActivity::window() const { return 0; }

void TimePickerActivity::cleanup() {}

void TimePickerActivity::onWidgetClosed(CherryKit::Widget *widget) {}

void TimePickerActivity::onHideAnimationFinished() {}
