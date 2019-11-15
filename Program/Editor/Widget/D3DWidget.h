#pragma once

#include <QWidget>

#include <Core/CoreHeader.h>

namespace z
{
class RHIViewport;

class D3DWidget : public QWidget {
	Q_OBJECT
public:
	D3DWidget(QWidget* parent, int id);
	virtual ~D3DWidget();

	virtual void resizeEvent(QResizeEvent* event);
	virtual void paintEvent(QPaintEvent* event);


private:
	RefCountPtr<RHIViewport> mRHIViewport;
	int mid;
};

}