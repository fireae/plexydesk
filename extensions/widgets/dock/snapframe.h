#ifndef SNAPFRAME_H
#define SNAPFRAME_H

#include <plexy.h>
#include <widget.h>

class SnapFrame : public UI::Window
{
  Q_OBJECT
public:
  SnapFrame(QGraphicsObject *parent = 0);
  virtual ~SnapFrame();

private:
  class PrivateSnapFrame;
  PrivateSnapFrame *const d;
};

#endif // SNAPFRAME_H
