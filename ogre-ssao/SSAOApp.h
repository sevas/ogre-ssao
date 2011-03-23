/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef _SSAOAPP__H_
#define _SSAOAPP__H_

#include <vector>
#include <list>
#include <utility>

#include "OgreApplication.h"



class SSAOApp : public OgreApplication
{
protected:
    typedef std::pair<Ogre::Entity*, Ogre::SceneNode*> ScenePair;
    typedef std::vector<ScenePair> ScenePairs;
public:
    SSAOApp();
    void createScene();

    bool frameStarted(const Ogre::FrameEvent& evt);
    //bool keyPressed( const OIS::KeyEvent &e );
protected:
    // Scene generation
    void _populate();

    void _loadHebeCassini();
    void _buildRoom();
    Ogre::SceneNode* _makeWall(const std::string &_meshName, const std::string &_name);

    void _loadULBCampus();

    void _loadSponzaAtrium();
    void _loadSponzaMesh(const std::string&_meshName);

    void _loadOgreMaxScene(const std::string&_path, const std::string&_filename, Ogre::SceneNode *_node);

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


#endif _SSAOAPP__H_