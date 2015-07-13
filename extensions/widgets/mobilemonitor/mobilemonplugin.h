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
#ifndef PLEXY_WIDGET_IMAGEPILE_H
#define PLEXY_WIDGET_IMAGEPILE_H

#include <abstractplugininterface.h>
#include <controllerinterface.h>
#include <themepackloader.h>

#include "plexynowck_widget.h"
#include "notebookbase.h"

class MobileMonController : public PlexyDesk::ControllerInterface {
  Q_OBJECT

public:
  MobileMonController(QObject *object = 0);
  virtual ~MobileMonController();

  void init();

  virtual void revokeSession(const QVariantMap &args);

  virtual void handleDropEvent(PlexyDesk::AbstractDesktopWidget *widget,
                               QDropEvent *event);

  virtual void setViewRect(const QRectF &rect);

  QStringList actions() const;

  void requestAction(const QString &actionName, const QVariantMap &args);

  bool deleteWidget(PlexyDesk::AbstractDesktopWidget *widget);

public
Q_SLOTS:
  void setApprovedToken(const QString &token);

private:
  class MobileMonControllerPrivate;
  MobileMonControllerPrivate *const d;

  PlexyNowWidget *mFrameParentitem;
  QString mImageSource;
  QList<PlexyNowWidget *> mPinWidgets;
};

#endif
