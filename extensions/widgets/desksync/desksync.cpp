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
#include "desksync.h"
#include <ck_config.h>

#include <ck_sync_object.h>
#include <ck_disk_engine.h>
#include <ck_data_sync.h>

class desksync_controller::Privatedesksync {
public:
  Privatedesksync() {}
  ~Privatedesksync() {}
};

desksync_controller::desksync_controller(QObject *object)
    : cherry_kit::desktop_controller_interface(object),
      priv(new Privatedesksync) {}

desksync_controller::~desksync_controller() { delete priv; }

void desksync_controller::init() {}

void desksync_controller::session_data_ready(
    const cherry_kit::sync_object &a_session_root) {
}

void desksync_controller::submit_session_data(cherry_kit::sync_object *a_obj) {}

void desksync_controller::set_view_rect(const QRectF &a_rect) {
  if (a_rect.width() < 1 && a_rect.height() < 1) {
    return;
  }

  cherry_kit::data_sync *sync = new cherry_kit::data_sync("Global");
  cherry_kit::disk_engine *engine = new cherry_kit::disk_engine();

  sync->set_sync_engine(engine);

  sync->on_object_found([&](cherry_kit::sync_object &a_object,
                            const std::string &a_app_name, bool a_found) {

    if (!a_found) {
        show_auth_dialog();
    }

    delete sync;
  });

  sync->find("social", "service_name", "plexydesk");
}

bool desksync_controller::remove_widget(cherry_kit::widget *a_widget_ptr) {
    return false;
}

void desksync_controller::show_auth_dialog() {
  cherry_kit::desktop_dialog_ref ck_activity =
          viewport()->create_activity("auth_dialog");

  ck_activity->on_notify([=](const std::string &key, const std::string &value) {
  });
}
