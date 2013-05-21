#include <inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec2PropertyWidgetQt::FloatVec2PropertyWidgetQt(FloatVec2Property *property) : property_(property),
valueVec2Min_(property->getMinValue()),
valueVec2Max_(property->getMaxValue()),
valueIncrement_(property->getIncrement()) {
	generateWidget();
    generatesSettingsWidget();
	updateFromProperty();
	}


void FloatVec2PropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

	QWidget* sliderWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();
	sliderWidget->setLayout(vLayout);

    //sliderX_ = new FloatSliderWidgetQt(valueVec2Min_.x,valueVec2Max_.x,valueIncrement_.x);
    //sliderY_ = new FloatSliderWidgetQt(valueVec2Min_.y,valueVec2Max_.y,valueIncrement_.y);
    
    sliderX_ = new FloatSliderWidgetQt();
    sliderY_ = new FloatSliderWidgetQt();

	vLayout->addWidget(sliderX_);
	vLayout->addWidget(sliderY_);
	hLayout->addWidget(sliderWidget);
	setLayout(hLayout);


    connect(sliderX_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
    connect(sliderY_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));

}

void FloatVec2PropertyWidgetQt::updateFromProperty() {
    valueVec2Max_ = property_->getMaxValue();
    valueVec2Min_ = property_->getMinValue();
    valueIncrement_ = property_->getIncrement();
    valueVec2_ = property_->get();
    sliderX_->initValue(valueVec2_.x);
    sliderY_->initValue(valueVec2_.y);

    sliderX_->setRange(valueVec2Min_.x,valueVec2Max_.x);
    sliderY_->setRange(valueVec2Min_.y,valueVec2Max_.y);
    
    sliderX_->setValue(valueVec2_.x);
    sliderY_->setValue(valueVec2_.y);

    sliderX_->setIncrement(valueIncrement_.x);
    sliderY_->setIncrement(valueIncrement_.y);
}


void FloatVec2PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = new QMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));
}

void FloatVec2PropertyWidgetQt::showContextMenuX( const QPoint& pos ) {
    QPoint globalPos = sliderX_->mapToGlobal(pos);
    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVec2Min_ = property_->getMinValue();
        valueVec2Min_.x = sliderX_->getValue();
        property_->setMinValue(valueVec2Min_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVec2Max_ = property_->getMaxValue();
        valueVec2Max_.x = sliderX_->getValue();
        property_->setMaxValue(valueVec2Max_);
        updateFromProperty();
    }
}

void FloatVec2PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    QPoint globalPos = sliderY_->mapToGlobal(pos);

    QAction* selecteditem = settingsMenu_->exec(globalPos);
    if (selecteditem == settingsMenu_->actions().at(0)) {
        settingsWidget_->reload();
        settingsWidget_->show();
    }
    else if (selecteditem == settingsMenu_->actions().at(1)) {
        //Set current value of the slider to min value of the property
        valueVec2Min_ = property_->getMinValue();
        valueVec2Min_.y = sliderY_->getValue();
        property_->setMinValue(valueVec2Min_);
        updateFromProperty();
    }
    else if (selecteditem == settingsMenu_->actions().at(2)){
        //Set current value of the slider to max value of the property
        valueVec2Max_ = property_->getMaxValue();
        valueVec2Max_.y = sliderY_->getValue();
        property_->setMaxValue(valueVec2Max_);
        updateFromProperty();
    }
}

void FloatVec2PropertyWidgetQt::setPropertyValue() {
    valueVec2_ = property_->get();
    valueVec2_.x = sliderX_->getValue();
    valueVec2_.y = sliderY_->getValue();
    property_->set(valueVec2_);
    emit modified();
}

} //namespace