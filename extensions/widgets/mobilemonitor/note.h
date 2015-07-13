#ifndef M_NOTE_H
#define M_NOTE_H

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>

#include <style.h>
#include <desktopwidget.h>

class Note : public PlexyDesk::DesktopWidget {
  Q_OBJECT
public:
  explicit Note(const QRectF &rect, QGraphicsObject *parent = 0);
  virtual ~Note();

  void setTitle(const QString &title);
  void setNoteContent(const QString &status);
  void setID(const QString &id);
  QString title() const;
  QString id();
  QString noteContent() const;

  void setPixmap(const QPixmap &pixmap);

Q_SIGNALS:
  void clicked(Note *item);

public
Q_SLOTS:
  void onClicked();

protected:
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget = 0);

private:
  class PrivateNote;
  PrivateNote *const d;
};

#endif // M_NOTE_H
