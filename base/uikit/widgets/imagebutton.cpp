#include "imagebutton.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <resource_manager.h>

#include <QDebug>

namespace CherryKit {

class ImageButton::PrivateImageButton {
public:
  typedef enum {
    _kNormal,
    _kPressed,
    _kHover
  } ButtonState;

  PrivateImageButton() : m_state(_kNormal) {}
  ~PrivateImageButton() {}

  QPixmap m_button_icon;
  QString m_button_text;
  QColor mBgColor;

  ButtonState m_state;
};

void ImageButton::set_background_color(const QColor &a_color) {
  o_image_button->mBgColor = a_color;
  update();
}

StylePtr ImageButton::style() const { return ResourceManager::style(); }

ImageButton::ImageButton(Widget *a_parent_ptr)
    : Widget(a_parent_ptr), o_image_button(new PrivateImageButton) {
  setFlag(QGraphicsItem::ItemIsMovable, false);
  setFlag(QGraphicsItem::ItemIsFocusable, true);
  setAcceptHoverEvents(true);

  o_image_button->mBgColor = Qt::transparent;
}

ImageButton::~ImageButton() { delete o_image_button; }

void ImageButton::set_size(const QSize &a_size) {
  setGeometry(QRectF(0, 0, a_size.width(), a_size.height()));
}

QSizeF ImageButton::sizeHint(Qt::SizeHint which,
                             const QSizeF &a_constraint) const {
  return geometry().size();
}

void ImageButton::set_pixmap(const QPixmap &a_pixmap) {
  o_image_button->m_button_icon = a_pixmap;
  update();
}

void ImageButton::set_lable(const QString &a_text) {
  o_image_button->m_button_text = a_text;
  update();
}

QString ImageButton::label() const { return o_image_button->m_button_text; }

void ImageButton::onZoomDone() {}

void ImageButton::onZoomOutDone() {}

void ImageButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *a_event_ptr) {
  o_image_button->m_state = PrivateImageButton::_kNormal;
  update();
  Widget::mouseReleaseEvent(a_event_ptr);
}

void ImageButton::mousePressEvent(QGraphicsSceneMouseEvent *a_event_ptr) {
  Q_EMIT selected(true);
  o_image_button->m_state = PrivateImageButton::_kPressed;
  update();
  Widget::mousePressEvent(a_event_ptr);
}

void ImageButton::hoverEnterEvent(QGraphicsSceneHoverEvent *a_event_ptr) {
  a_event_ptr->accept();
  o_image_button->m_state = PrivateImageButton::_kHover;
  update();
  Widget::hoverEnterEvent(a_event_ptr);
}

void ImageButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event_ptr) {
  a_event_ptr->accept();
  o_image_button->m_state = PrivateImageButton::_kNormal;
  update();
  Widget::hoverLeaveEvent(a_event_ptr);
}

void ImageButton::paint_view(QPainter *a_painter_ptr, const QRectF &a_rect) {
  StyleFeatures feature;

  feature.text_data = o_image_button->m_button_text;
  feature.image_data = o_image_button->m_button_icon;
  feature.geometry = a_rect;
  feature.render_state = StyleFeatures::kRenderElement;

  switch (o_image_button->m_state) {
  case PrivateImageButton::_kNormal:
    feature.render_state = StyleFeatures::kRenderElement;
    break;
  case PrivateImageButton::_kPressed:
    feature.render_state = StyleFeatures::kRenderPressed;
    break;
  default:
    qDebug() << Q_FUNC_INFO << "Unknown Button State";
  }

  if (CherryKit::ResourceManager::style()) {
    CherryKit::ResourceManager::style()->draw("image_button", feature,
                                              a_painter_ptr);
  }
}
}
