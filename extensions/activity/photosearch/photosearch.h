/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@plexydesk.org>
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
#ifndef PHOTOSEARCH_ACTIVITY_H
#define PHOTOSEARCH_ACTIVITY_H

#include <QtCore>
#include <plexy.h>

#include <window.h>
#include <datasource.h>
#include <QtNetwork>
#include <desktopactivity.h>
#include <abstractcellcomponent.h>

using namespace CherryKit;
class PhotoSearchActivity : public CherryKit::DesktopActivity {
  Q_OBJECT

public:
  PhotoSearchActivity(QGraphicsObject *object = 0);

  virtual ~PhotoSearchActivity();

  void create_window(const QRectF &window_geometry, const QString &aWindowTitle,
                     const QPointF &window_pos);

  virtual QRectF geometry() const;

  QVariantMap result() const;

  Window *window() const;
  void cleanup();

private
Q_SLOTS:

  void onShowAnimationFinished();
  void onClicked(TableViewItem *item);
  void onProgressValue(int value);

  // void onDirectoryReady();

  // void onPictureAvailable(const QString &key);

  // void onImageSaved();

  void locateLocalFiles() const;

private:
  class PrivatePhotoSearch;
  PrivatePhotoSearch *const d;
};

#endif
