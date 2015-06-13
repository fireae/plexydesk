/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@plexydesk.org>
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
#include "template.h"
#include <widget.h>
#include <plexyconfig.h>
#include <QTimer>
#include <view_controller.h>

class TemplateActivity::PrivateTemplate {
public:
  PrivateTemplate() {}
  ~PrivateTemplate() {}

  UIKit::Window *m_main_window;
};

TemplateActivity::TemplateActivity(QGraphicsObject *object)
    : UIKit::DesktopActivity(object), d(new PrivateTemplate) {}

TemplateActivity::~TemplateActivity() { delete d; }

void TemplateActivity::create_window(const QRectF &window_geometry,
                                     const QString &window_title,
                                     const QPointF &window_pos) {
  d->m_main_window = new UIKit::Window();

  d->m_main_window->set_widget_flag(UIKit::Widget::kRenderBackground);
  d->m_main_window->set_widget_flag(UIKit::Widget::kConvertToWindowType);
  d->m_main_window->set_widget_flag(UIKit::Widget::kRenderDropShadow);

  set_geometry(window_geometry);
  update_content_geometry(d->m_main_window);

  exec(window_pos);
}

QVariantMap TemplateActivity::result() const { return QVariantMap(); }

void TemplateActivity::update_attribute(const QString &name,
                                        const QVariant &data) {}

UIKit::Window *TemplateActivity::window() const { return d->m_main_window; }

void TemplateActivity::cleanup() {
  if (d->m_main_window) {
    delete d->m_main_window;
  }
  d->m_main_window = 0;
}

void TemplateActivity::onWidgetClosed(UIKit::Widget *widget) {
  discard_activity();
}

void TemplateActivity::onHideAnimationFinished() {}
