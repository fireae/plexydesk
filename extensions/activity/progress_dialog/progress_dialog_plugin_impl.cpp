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
#include "progress_dialog_view.h"
#include "progress_dialog_plugin_impl.h"
#include <ck_widget.h>
#include <ck_config.h>

cherry_kit::desktop_dialog_ref progress_dialog_plugin_impl::activity() {
  cherry_kit::desktop_dialog_ref obj =
      cherry_kit::desktop_dialog_ref(new progress_dialog_view());

  return obj;
}

// Q_EXPORT_PLUGIN2(progressdialogengine, ProgressDialogInterface)
