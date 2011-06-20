

#include "settingsButton.h"
#include "renderer.h"
#include "input.h"
SettingsButton::SettingsButton(){

registerProperties();
}
SettingsButton::~SettingsButton(){}


void SettingsButton::setup(){
BasicButton::setup();

}

void SettingsButton::update(double deltaTime){}

void SettingsButton::mouseOver(){

BasicButton::mouseOver();
}

void SettingsButton::mouseDrag(){}
void SettingsButton::finishDrag(){}
void SettingsButton::clickedLeft(){}
void SettingsButton::clickedRight(){}
void SettingsButton::focusClick(){

BasicButton::focusClick();
}

void SettingsButton::deselect(int depth){

BasicButton::deselect(depth);
}

void SettingsButton::create(){renderer->addButton(this);}
