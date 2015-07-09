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
#include "rangewidget.h"

#include <QDebug>

namespace CherryKit {
class RangeWidget::PrivateRangeWidget {
public:
  PrivateRangeWidget() {}
  ~PrivateRangeWidget() {}

  QString convertAngleToTimeString(float angle);
  void calculateValue();

  float mAngle;
  int mMaxAngle;
  int mMinAngle;
  int mStartAngle;
  int mMaxValue;
  QPointF mInitPos;

  int mProgressValue;
};

RangeWidget::RangeWidget(Widget *parent)
    : CherryKit::Widget(parent), o_range_widget(new PrivateRangeWidget) {
  this->set_widget_flag(CherryKit::Widget::kRenderBackground);
  this->set_widget_flag(CherryKit::Widget::kConvertToWindowType, false);
  this->set_widget_flag(CherryKit::Widget::kRenderDropShadow, false);
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
  this->setFlag(QGraphicsItem::ItemIsSelectable, true);

  o_range_widget->mAngle = 270;
  o_range_widget->mMaxAngle = 360;
  o_range_widget->mProgressValue = 0;
  o_range_widget->mStartAngle = 270;
  o_range_widget->mMaxValue = 24;
}

RangeWidget::~RangeWidget() {}

void RangeWidget::setMaxValue(float maxValue) { o_range_widget->mMaxValue = maxValue; }

float RangeWidget::maxValue() const { return o_range_widget->mMaxValue; }

float RangeWidget::currentValue() const { return o_range_widget->mProgressValue; }

void RangeWidget::dragMoveEvent(QGraphicsSceneDragDropEvent *event) {
  qDebug() << Q_FUNC_INFO << event->pos();
}

void RangeWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  o_range_widget->mInitPos = mapToScene(event->pos());
  event->accept();
  QGraphicsItem::mousePressEvent(event);
}

void RangeWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  event->accept();
  // QGraphicsItem::mouseReleaseEvent(event);
}

void RangeWidget::PrivateRangeWidget::calculateValue() {
  //
  float decimalValue = (mAngle - mStartAngle) / (mMaxAngle / mMaxValue);
  if (decimalValue < 0) {
    decimalValue += mMaxValue;
  }

  int hours = (int)decimalValue;

  if (hours == mMaxValue) {
    hours = 0;
  }

  mProgressValue = hours;
}

QString RangeWidget::PrivateRangeWidget::convertAngleToTimeString(float angle) {
  QString time = "";
  float decimalValue = (angle - mStartAngle) / 15.0;
  if (decimalValue < 0) {
    decimalValue += mMaxValue;
  }

  int hours = (int)decimalValue;

  if (hours == mMaxValue) {
    hours = 0;
  }

  float minutes = ((int)angle % (int)15) / 0.25;

  return QString("%1:%2").arg(hours).arg(minutes); // time;
}

void RangeWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  /*
  QPointF pos = mapToScene(event->pos());

  QLineF line (boundingRect().center(), event->pos());

  d->mAngle = line.angle(QLineF(0.0, 0.0, 1.0, 0.0));

  if (line.dy() < 0)
      d->mAngle = d->mMaxAngle - d->mAngle;

  d->mInitPos = pos;

  d->calculateValue();

  update();

  Q_EMIT value(d->mProgressValue);
  */
}

void RangeWidget::paint_view(QPainter *painter, const QRectF &rect) {
  QPen pen;
  pen.setColor(QColor("#F28585"));
  pen.setWidth(10);
  painter->setPen(pen);

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::HighQualityAntialiasing);

  QRectF handle(this->boundingRect().center().x() - 5,
                this->boundingRect().center().y() - 5, 10, 10);
  QRectF borderRect(boundingRect().x() + 24, boundingRect().y() + 24,
                    boundingRect().width() - 48, boundingRect().height() - 48);

  painter->save();
  QPen pen_border;
  pen_border.setColor(QColor("#F0F0F0"));
  pen_border.setWidth(4);
  painter->setPen(pen_border);

  // painter->drawEllipse(borderRect);

  painter->drawLine(
      QPointF(0.0, this->boundingRect().height() / 2),
      QPointF(this->boundingRect().width(), this->boundingRect().height() / 2));
  painter->restore();

  /*
  QPainterPath clockInisde;
  clockInisde.addEllipse(QRectF(borderRect.x() + 2, borderRect.y() + 2,
  borderRect.width() - 4 , borderRect.height() - 4));

  //painter->fillPath(clockInisde, QColor("#F28585"));
  painter->fillPath(clockInisde, QColor(Qt::white));

  QFont font = painter->font();
  font.setBold(true);
  font.setPixelSize(32);
  painter->save();
  painter->setFont(font);
  // painter->drawText(borderRect, Qt::AlignCenter,
  d->convertAngleToTimeString(d->mAngle));
  painter->restore();

  painter->save();
  QTransform xform;
  QPointF transPos = this->boundingRect().center();
  xform.translate(transPos.x(), transPos.y());
  xform.rotate(d->mAngle);
  xform.translate(-transPos.x(), -transPos.y());

  painter->setTransform(xform);

  QLineF line(handle.center(), QPointF(boundingRect().width() - 48,
  (boundingRect().height() / 2) - 48));
  //
  QRectF ctrRect(line.x2(), line.y2(), 10, 10);
  QRectF ctrFrameRect(line.x2(), line.y2(), 32, 32);
  QPainterPath path;
  path.addEllipse(ctrFrameRect);
  painter->fillPath(path, QColor(Qt::white));
  painter->fillPath(path, QColor("#F28585"));

  QPen whitePen;
  whitePen.setColor(Qt::white);
  painter->setPen(whitePen);
  painter->drawText(ctrFrameRect, Qt::AlignCenter,
  QString("%1").arg((int)d->mProgressValue));

  painter->restore();
  */
}
}
