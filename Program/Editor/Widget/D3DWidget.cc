#include "D3DWidget.h"
#include <QKeyEvent>
#include <Core/CoreHeader.h>
#include <RHIDX12/DX12Device.h>
#include <Render/RenderStage.h>
#include <Render/RenderTarget.h>

using namespace z;

D3DWidget::D3DWidget(QWidget* parent, int id) {
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	if (GDevice == nullptr)
		new DX12Device();

	mid = id;
}

D3DWidget::~D3DWidget() {

}

void D3DWidget::resizeEvent(QResizeEvent* event) {
	const QSize &size = event->size();
	z::Log<LINFO>() << "resized";
	if (!mRHIViewport) {
		mRHIViewport = GDevice->CreateViewport(size.width(), size.height(), PF_R8G8B8A8, (HWND)winId());
	} else {
		mRHIViewport->Resize(size.width(), size.height());
	}
}

void D3DWidget::paintEvent(QPaintEvent* event) {

	if (mRHIViewport == nullptr)
		return;

	mRHIViewport->BeginDraw();
	RefCountPtr<RenderTarget> mBackRT = new RenderTarget(mRHIViewport->GetBackBuffer());



	//RenderStage::CurStage()->SetRenderRect({ 0.f, 0.f, (float)mViewportWidth, (float)mViewportHeight, 0.f, 1.f });
	//RenderStage::CurStage()->SetScissorRect({ 0, 0, (int)mViewportWidth, (int)mViewportHeight });
	if (mid == 1)
		mBackRT->Clear({ 0.f, 0.5f, 0.f, 1.f });
	else
		mBackRT->Clear({ 0.f, 0.5f, 0.5f, 1.f });


	mRHIViewport->EndDraw();
	GDevice->EndDraw();
	update();
}