#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QGraphicsObject>
#include <widget.h>

#include <functional>

namespace UI
{

class ModelView : public Window
{
  Q_OBJECT

public:
  typedef enum {
    kGridModel,
    kListModel,
    kTableModel
  } ModelType;

  ModelView(QGraphicsObject *parent = 0);
  virtual ~ModelView();

  virtual void insert(Window *widget);
  virtual void remove(Window *widget);

  virtual void clear();

  virtual void setViewGeometry(const QRectF &rect);
  //Qt
  virtual QRectF boundingRect() const;
  virtual void setGeometry(const QRectF &rect);
  virtual QSizeF sizeHint(Qt::SizeHint which,
                          const QSizeF &constraint) const;
protected:
  bool sceneEvent(QEvent *e);
  bool event(QEvent *e);
private:
  class PrivateModelView;
  PrivateModelView *const d;
};

}
#endif // MODELVIEW_H
