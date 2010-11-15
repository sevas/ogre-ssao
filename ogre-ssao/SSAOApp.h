/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef _ULBBUILDINGAPP__H_
#define _ULBBUILDINGAPP__H_

#include <vector>
#include <list>
#include <utility>

#include "OgreApplication.h"



class ULBBuildingApp : public OgreApplication
{
protected:
    typedef std::pair<Ogre::Entity*, Ogre::SceneNode*> ScenePair;
    typedef std::vector<ScenePair> ScenePairs;
public:
    ULBBuildingApp();
    void createScene();

    bool frameStarted(const Ogre::FrameEvent& evt);
    //bool keyPressed( const OIS::KeyEvent &e );
protected:
    // Scene generation
    void _populate();
    void _createLight();
    Ogre::SceneNode* _loadMesh(const Ogre::String&, const Ogre::Vector3&);

protected:
    Ogre::BillboardSet *mBBset;
    Ogre::Billboard *mLightFlare;
    Ogre::Light *mLight;
    Ogre::SceneNode *mLightNode;
    Ogre::Log *mLog;
    ScenePairs mScenePairs;
};


#endif _ULBBUILDINGAPP__H_