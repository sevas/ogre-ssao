#include "precompiled.h"
#include "DebugOverlay.hpp"


const unsigned int DebugOverlay::sLineHeight = 20;
const unsigned int DebugOverlay::sLayoutGap = 10;
//-----------------------------------------------------------------------------
DebugOverlay::DebugOverlay(Ogre::OverlayManager *_overlayMgr, const std::string &_overlayName)
    :mOverlayMgr(_overlayMgr)
    ,mOverlayName(_overlayName)
    ,mPanel(NULL)
    ,mOverlay(NULL)
    ,mRowCount(0)
    ,mWidth(200)
    ,mHeight(1)
{
    mOverlay = mOverlayMgr->create(mOverlayName);

    mPanel = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", mOverlayName+"/Container"));
    mPanel->setDimensions(1, 1);
    mPanel->setPosition(0, 0);

    mOverlay->add2D(mPanel);

    mOverlay->show();
}
//-----------------------------------------------------------------------------
void DebugOverlay::addValueBox(const std::string &_name, const std::string &_caption)
{
    _addTextBox(_name+"_caption", _caption, 10, mRowCount*sLineHeight
        ,100, 20, Ogre::ColourValue::White);

    _addTextBox(_name, "?", 100, mRowCount*sLineHeight
        ,100, 20, Ogre::ColourValue::White);

    mRowCount++;
}
//-----------------------------------------------------------------------------
void DebugOverlay::setValue(const std::string &_name, const std::string &_value)
{
    Ogre::OverlayElement* textBox = mOverlayMgr->getOverlayElement(_name);
    textBox->setCaption(_value);
}
void DebugOverlay::setPosition(Ogre::Real _top, Ogre::Real _left)
{
	mPanel->setPosition(_left, _top);
};
//-----------------------------------------------------------------------------
void DebugOverlay::_addTextBox(const std::string &ID
                               , const std::string &text
                               , Ogre::Real x
                               , Ogre::Real y
                               , Ogre::Real width
                               , Ogre::Real height
                               , const Ogre::ColourValue &color)
{
    Ogre::OverlayElement* textBox = mOverlayMgr->createOverlayElement("TextArea", ID);
    textBox->setDimensions(width, height);
    textBox->setMetricsMode(Ogre::GMM_PIXELS);
    textBox->setPosition(x, y);
    textBox->setWidth(width);
    textBox->setHeight(height);
    textBox->setParameter("font_name", "MyFont");
    textBox->setParameter("char_height", "16");
    textBox->setColour(color);

    textBox->setCaption(text);

    mPanel->addChild(textBox);
}
//-----------------------------------------------------------------------------
