#ifndef PHOTO_CELL_ADAPTOR_H
#define PHOTO_CELL_ADAPTOR_H

#include <QString>
#include <tableviewcellinterface.h>
#include <webservice.h>

using namespace cherry_kit;

class PhotoCellAdaptor : public TableModel {
  Q_OBJECT

public:
  PhotoCellAdaptor(QObject *a_parent_ptr = 0);
  virtual ~PhotoCellAdaptor();

  QList<cherry_kit::TableViewItem *> componentList();

  float margin() const;

  float padding() const;

  virtual float left_margin() const;

  virtual float right_margin() const;

  virtual bool init();

  virtual TableRenderMode render_type() const;

  void addDataItem(const QString &label, const QImage &pixmap,
                   bool selected = false,
                   const QVariantMap &metaData = QVariantMap());

  void setLabelVisibility(bool visibility);

  void setCellSize(const QSize &size);

  QUrl requestImageUrl(const QString &id, const QString &size) const;

  QList<QString> availableSizesForImage(const QString &id);

  void setSearchQuery(const QString &query, int pageNumber = 1);

Q_SIGNALS:
  void pageCount(int pagecount);
  void progressRange(int range);
  void completed(int count);

private
Q_SLOTS:
  void onServiceComplete(QuetzalSocialKit::WebService *service);

  void onSizeServiceComplete(QuetzalSocialKit::WebService *service);

  void onImageReady();

  void onImageSaveReady();

private:
  class PrivatePhotoCellAdaptor;
  PrivatePhotoCellAdaptor *const d;
};

#endif // PHOTO_CELL_ADAPTOR_H
