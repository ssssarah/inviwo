/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/qt/editor/processorstatusgraphicsitem.h>
#include <inviwo/core/processors/processor.h>

#include <QPen>
#include <QPainter>
#include <QBrush>

namespace inviwo {

ProcessorStatusGraphicsItem::ProcessorStatusGraphicsItem(QGraphicsRectItem* parent, Processor* processor)
    : EditorGrapicsItem(parent), processor_(processor), size_(10.0f), lineWidth_(3.0f) {


    setRect(-0.5f * size_ - lineWidth_, -0.5f * size_ - lineWidth_, size_ + 2.0*lineWidth_,
            size_ + 2.0*lineWidth_);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setPos(QPointF(64.0f, -15.0f));
}

void ProcessorStatusGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    qreal ledRadius = size_ / 2.0f;
    QColor baseColor(0, 170, 0);

    QColor ledColor;
    QColor borderColor;


    if (processor_->isReady()) {
        ledColor = baseColor;
        borderColor = QColor(124, 124, 124);
    } else {
        ledColor = baseColor.dark(400);
        borderColor = QColor(64, 64, 64);
    }

    // initialize painter
    p->save();
    p->setPen(QPen(borderColor, 3.0));
    p->setRenderHint(QPainter::Antialiasing, true);
    p->setBrush(QBrush(ledColor));
    // draw base shape
    p->drawEllipse(QPointF(0.0f, 0.0f), ledRadius, ledRadius);
    // draw light highlight
    QPointF highLightPos = QPointF(0.0f, 0.0f);
    p->setPen(Qt::NoPen);

    while (ledRadius > 0.0) {
        ledColor = ledColor.light(120);
        p->setBrush(QBrush(ledColor));
        p->drawEllipse(highLightPos, ledRadius, ledRadius);
        ledRadius -= 0.25;
        p->drawEllipse(highLightPos, ledRadius, ledRadius);
        ledRadius -= 0.25;
        p->drawEllipse(highLightPos, ledRadius, ledRadius);
        ledRadius -= 0.25;
        highLightPos.setX(highLightPos.x() - 0.25);
        highLightPos.setY(highLightPos.y() - 0.25);
    }

    // deinitialize painter
    p->restore();
}

}  // namespace
