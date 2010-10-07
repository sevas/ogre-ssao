#pragma once
#ifndef __071010_DEBUGOVERLAY_HPP__
#define __071010_DEBUGOVERLAY_HPP__

#include <string>
#include <Ogre.h>
#include "TextRenderer.h"

class DebugOverlay
{
public:
    DebugOverlay(Ogre::OverlayManager *_overlayMgr, const std::string &_overlayName);
    ~DebugOverlay();
    void setPosition(Ogre::Real _top, Ogre::Real _left);

    void addValueBox(const std::string &_name, const std::string &_caption);
    void setValue(const std::string &_name, const std::string &_value);

protected:
    void _addTextBox(const std::string& ID,
                     const std::string& text,
                     Ogre::Real x, Ogre::Real y,
                     Ogre::Real width, Ogre::Real height,
                     const Ogre::ColourValue& color);

protected:
    std::string mOverlayName;
    int mWidth, mHeight;

    Ogre::OverlayManager *mOverlayMgr;
    Ogre::Overlay *mOverlay;
    Ogre::OverlayContainer *mPanel;

    int mRowCount;
};


#endif 