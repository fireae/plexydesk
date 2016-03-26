#include "photowidget.h"

class PhotoWidget::PrivatePhotoFrame {
public:
  PrivatePhotoFrame() {}
  ~PrivatePhotoFrame() {}
  QString m_photo_url;
};

PhotoWidget::PhotoWidget(widget *parent)
    : cherry_kit::widget(parent), d(new PrivatePhotoFrame) {
  set_widget_flag(cherry_kit::widget::kRenderDropShadow, true);
  mValid = 0;
}

PhotoWidget::~PhotoWidget() { delete d; }

void PhotoWidget::setContentImage(const QPixmap &pixmap) {
  mContentPixmap = pixmap;
  QRectF pixmapRect = pixmap.rect();

  /*calculate the new height for the content rect width */
  float pixmapHeight =
      (pixmapRect.height() / pixmapRect.width()) * geometry().width();
  pixmapRect.setHeight(pixmapHeight);
  pixmapRect.setWidth(geometry().width());
  set_geometry(pixmapRect);

  mValid = 1;
  update();
}

void PhotoWidget::setPhotoURL(const QString &name) { d->m_photo_url = name; }

QString PhotoWidget::photoURL() const { return d->m_photo_url; }

bool PhotoWidget::validPhotoFrame() { return mValid; }

QRectF PhotoWidget::scaledDockRect(const QPixmap &pixmap) const {
  QRectF rect;
  float scaleTo = 128.0;
  QRectF pixmapRect = pixmap.rect();
  float scaleFactor = 10.0;
  float scaledHeight = pixmapRect.height();
  float scaledWidth = pixmapRect.width();

  if (scaledWidth > 0) {
    scaleFactor = scaledWidth / scaleTo;
  }

  if (scaledHeight > 0) {
    scaledHeight = scaledHeight / scaleFactor;
  }

  rect.setHeight(scaledHeight);
  rect.setWidth(128.0);

  return rect;
}

void PhotoWidget::paint_view(QPainter *painter, const QRectF &rect) {
  painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::SmoothPixmapTransform);
  painter->setPen(QColor(255, 255, 255));

  painter->setOpacity(1.0);

  if (mContentPixmap.isNull()) {
    cherry_kit::widget::paint_view(painter, rect);
    painter->drawText(QRect(0, 0, 190, 180), Qt::AlignCenter | Qt::AlignRight,
                      QLatin1String("Drag and Drop a Photo Here!"));
    cherry_kit::widget::paint_view(painter, rect);
  } else {
    // experimental photo frame around the image
    // painter->fillRect(rect.x(), rect.y(), rect.width(), rect.height(),
    // QColor(255, 255, 255));
    // float frameWidth = 4.0;
    // painter->translate(frameWidth, frameWidth);
    // painter->drawPixmap(rect.x(), rect.y(), rect.width() - (frameWidth * 2),
    // rect.height() - (frameWidth * 2), mContentPixmap);
    painter->drawPixmap(rect.x(), rect.y(), rect.width(), rect.height(),
                        mContentPixmap);
  }
}
