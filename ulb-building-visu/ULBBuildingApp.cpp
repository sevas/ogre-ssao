/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"

#include "ULBBuildingApp.h"

#include <sstream>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

//-----------------------------------------------------------------------------
ULBBuildingApp::ULBBuildingApp()
    :OgreApplication("ULB_Building_viz")
    ,mBBset(NULL)
    ,mLightFlare(NULL)
    ,mLight(NULL)
    ,mLightNode(NULL)
    ,mDebugText(NULL)
    ,mEdgesVisible(true)
{
}
//-----------------------------------------------------------------------------
void ULBBuildingApp::createScene()
{
    mLog  = Ogre::LogManager::getSingleton().createLog("NPR.log");

    const Ogre::RenderSystemCapabilities* caps = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities();
    if (!caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) || !(caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM)))
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "Your card does not support vertex and fragment programs, so cannot "
            "run this application. Sorry!", 
            "createScene");
    }


    mSceneMgr->setNormaliseNormalsOnScale(true);
    _createGrid(500);
    _createLight();
    _populate();

    _createDebugOverlay();
}
//-----------------------------------------------------------------------------
bool ULBBuildingApp::frameStarted(const Ogre::FrameEvent& evt)
{
    _updateDebugOverlay();
    return OgreApplication::frameStarted(evt);
}
//-----------------------------------------------------------------------------
bool ULBBuildingApp::keyPressed (const OIS::KeyEvent &e )
{
    if(mKeyboard->isKeyDown(OIS::KC_E))
    {
        mStaticEdges->setVisible(!mStaticEdges->isVisible());
    }
    return OgreApplication::keyPressed(e);
}
//-----------------------------------------------------------------------------
void ULBBuildingApp::_createDebugOverlay()
{
    new TextRenderer();

    mDebugText = TextRenderer::getSingletonPtr();

    int x_offset=100, y_offset=18, w=100, h=18;

    mDebugText->addTextBox("Batches_", "#Batches : "
                            , 10, 10, w, h
                            , Ogre::ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("Batches", "0"
                            , x_offset, 10, w, h
                            , Ogre::ColourValue(1.0,1.0,1.0));
    mDebugText->addTextBox("FPS_", "#FPS : "
                            , 10, 10+y_offset, w, h
                            , Ogre::ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("FPS", "0"
                            , x_offset, 10+y_offset, w, h
                            , Ogre::ColourValue(1.0,1.0,1.0));

    mDebugText->addTextBox("Triangles_", "#tris : "
                            , 10, 10+y_offset*2, w, h
                            , Ogre::ColourValue(0.7,0.7,0.7));
    mDebugText->addTextBox("Triangles", "0"
                            , x_offset, 10+y_offset*2, w, h
                            , Ogre::ColourValue(1.0,1.0,1.0));



}
//-----------------------------------------------------------------------------
void ULBBuildingApp::_updateDebugOverlay()
{
    mDebugText->setText("Batches", Ogre::StringConverter::toString(mWindow->getBatchCount()));
    mDebugText->setText("FPS", Ogre::StringConverter::toString(mWindow->getLastFPS()));
    mDebugText->setText("Triangles", Ogre::StringConverter::toString(mWindow->getTriangleCount()));
}

//-----------------------------------------------------------------------------
void ULBBuildingApp::_populate()
{   using namespace Ogre;

    //_loadMesh("TorusKnot01", Vector3(-50, 0, 0));
    //_loadMesh("Teapot01",    Vector3(50, 0, 0));
    //_loadMesh("Gengon01",   Vector3(-50, 0, 50));
    //_loadMesh("Cone01",     Vector3(50, 0, 50));
    //_loadMesh("Box01",      Vector3(-50, 0, -50));

	//_loadMesh("sphere2", Vector3(500, 0, 0));
	//
    //_loadMesh("Rectangle01", Vector3(0, 0, 0));

    // stanford models

 //   SceneNode *bunny = _loadMesh("bunny", Vector3::ZERO);
    
	//bunny->scale(100, 100, 100);
	//bunny->pitch(Degree(-90));
	//bunny->translate(0, 20, 0);

	//SceneNode *dragon = _loadMesh("dragon", Vector3(50, 0, 100));
    
 //   SceneNode *dragon = _loadMesh("dragon", Vector3::ZERO);

	//dragon->scale(50, 50, 50);
	//dragon->pitch(Degree(180));
	//dragon->translate(0, 15, 0);

    //_loadMesh("math_back wall", Vector3(0, 0, 0));
    //_loadMesh("math_front wall", Vector3(0, 0, 0));
    //_loadMesh("math_left wall", Vector3(0, 0, 0));
    //_loadMesh("math_right wall", Vector3(0, 0, 0));
    //_loadMesh("math_roof", Vector3(0, 0, 0));
    //_loadMesh("math_ground", Vector3(0, 0, 0));
    //_loadMesh("math_chairs", Vector3(0, 0, 0));

    // loading a .scene scene
    //SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("titanic_root");
    //_loadScene("titanic", node);




    _loadMesh("ulb_building_BATIMENTS_EST", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_bat_C", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_ASCENCEUR", Vector3(-1300, 0, 0));

 
    _loadMesh("ulb_building_BAT_NEXT", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_BAT_PRINCIPAL", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_bat_projet_broullion", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_bat_S_W", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_batiment_plus", Vector3(-1300, 0, 0));


    _loadMesh("ulb_building_BATIMENTS_PROJET", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_BATIMENTS_TOUT", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Calque1", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Calque2", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_COURBES", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_galleries", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Layer0", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_Le_Batiment", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_route_relief", Vector3(-1300, 0, 0));

    _loadMesh("ulb_building_tour", Vector3(-1300, 0, 0));
    _loadMesh("ulb_building_Z_LAYOUT", Vector3(-1300, 0, 0));

    _buildStaticEdges();
}
//-----------------------------------------------------------------------------
Ogre::SceneNode* ULBBuildingApp::_loadMesh(const Ogre::String &_name, const Ogre::Vector3 &_pos)
{
    Ogre::Entity *ent = mSceneMgr->createEntity(_name, _name+".mesh");
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(_name+"Node", _pos);

	ent->setMaterialName("Shading/PerPixel/Gooch");
    node->attachObject(ent);

    mScenePairs.push_back(ULBBuildingApp::ScenePair(ent, node));

    return node;
}
//-----------------------------------------------------------------------------
void ULBBuildingApp::_buildStaticEdges()
{
    mStaticEdges = new EdgeGeometryBuilder("ULB static edges", mLog, mSceneMgr);
    mStaticEdges->begin();

    BOOST_FOREACH(ULBBuildingApp::ScenePair pair, mScenePairs)
    {
        mStaticEdges->addEdgesForEntity(pair.first);

    }
    mStaticEdges->end();

    mStaticEdges->setVisible(true);
    Ogre::SceneNode *edgesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("edges node", Ogre::Vector3(-1300, 0, 0));
    mStaticEdges->attachToSceneNode(edgesNode);
}
//-----------------------------------------------------------------------------
void ULBBuildingApp::_createLight()
{
    mBBset = mSceneMgr->createBillboardSet("Light BB");
    mBBset->setMaterialName("Objects/Flare");
    mLightFlare = mBBset->createBillboard(Ogre::Vector3::ZERO);

    mLight = mSceneMgr->createLight("main light");
    mLight->setType(Ogre::Light::LT_POINT);
    mLight->setDiffuseColour(Ogre::ColourValue::White);
    mLight->setSpecularColour(Ogre::ColourValue::White);

    mLightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("light node");
    mLightNode->attachObject(mLight);
    mLightNode->attachObject(mBBset);

    mLightNode->setPosition(Ogre::Vector3(0, 500, 500));

}
//-----------------------------------------------------------------------------