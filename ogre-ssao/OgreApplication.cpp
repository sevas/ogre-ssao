/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#include "precompiled.h"

#include "OgreApplication.h"
#include <fstream>

using namespace Ogre;


#define LEFT_ANALOG_STICK_X  0
#define LEFT_ANALOG_STICK_Y  1
#define RIGHT_ANALOG_STICK_X 2
#define RIGHT_ANALOG_STICK_Y 3
#define LEFT_RIGHT_TRIGGERS  4

#define SSAO_PASS_ID 42

//-----------------------------------------------------------------------------
OgreApplication::OgreApplication(const String &_title, ControlType _controlType)
    :mRoot(NULL)
    ,mCamera(NULL)
    ,mSceneMgr(NULL)
    ,mWindow(NULL)
    ,mInputManager(NULL)
    ,mKeyboard(NULL)
    ,mMouse(NULL)
    ,mJoystick(NULL)
    ,mTitle(_title)
    ,mControlType(_controlType)
    ,mTimeSinceLastToggle(0.0f)
    ,mDebugOverlay(NULL)
    , mContinue(true)
    , mCurrentSpeed(0)
    , mMoveSpeed(50)
    , mMoveScale(0.0f)
    , mMoveSpeedLimit(0)
    , mTranslationVector(Ogre::Vector3::ZERO)
    , mAccel(5.0f)
    , mRotScale(0.0f)
    , mRotateSpeed(36)
    , mSSAOCompositor(NULL)
    , mImageCounter(0)
    , mOgreAppLog(NULL)
{
    mPitchDirection = 1;                                
}
//-----------------------------------------------------------------------------
OgreApplication::~OgreApplication()
{
    delete mRoot;
}
//-----------------------------------------------------------------------------
void OgreApplication::go()
{
    if (!initialise())
        return;

    mRoot->startRendering();
    destroyScene();
}
//-----------------------------------------------------------------------------
bool OgreApplication::initialise()
{
    mRoot = new Root();
    addResourceLocations();

    // if we cannot initialise Ogre, just abandon the whole deal
    if ( !initOgreCore() ) return false;


    mOgreAppLog = Ogre::LogManager::getSingleton().createLog("OgreApp.log");

    createSceneManager();
    createCamera();
    createViewports();

    // Create any resource listeners (for loading screens)
    createResourceListener();

    initResources();
    createInputSystem();
    createFrameListener();

    _initSSAO();


    createScene();

    return true;
}
//-----------------------------------------------------------------------------
bool OgreApplication::initOgreCore()
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->restoreConfig() || mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        mWindow = mRoot->initialise(true, mTitle);
        return true;
    }
    else
    {
        return false;
    }
}
//-----------------------------------------------------------------------------
void OgreApplication::createSceneManager()
{
    mSceneMgr = mRoot->createSceneManager(ST_GENERIC);
}
//-----------------------------------------------------------------------------
void OgreApplication::createCamera()
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");


    mCamera->setNearClipDistance(0.5);
    mCamera->setFarClipDistance(1500.0);
}
//-----------------------------------------------------------------------------
void OgreApplication::createViewports()
{
    // Create one viewport, entire window
    Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0.5,0.5,0.5));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    mCamera->setQueryFlags(0);
}
//-----------------------------------------------------------------------------
void OgreApplication::createResourceListener(){}
//-----------------------------------------------------------------------------
void OgreApplication::createFrameListener()
{
    mRoot->addFrameListener(this);
}
//-----------------------------------------------------------------------------
void OgreApplication::createInputSystem()
{
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    

    mInputManager = OIS::InputManager::createInputSystem( pl );

    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));


        try
        {
            mJoystick = static_cast<OIS::JoyStick*>(mInputManager->createInputObject(OIS::OISJoyStick, true));
        }
        catch (const OIS::Exception &e)
        {
            //silent failure
            mJoystick = NULL;
        }



    unsigned int width, height, depth;
    int top, left;
    mWindow->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//-----------------------------------------------------------------------------
void OgreApplication::addResourceLocations()
{
    // Load resource paths from config file
    ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}
//-----------------------------------------------------------------------------
void OgreApplication::initResources()
{
    // Initialise, parse scripts etc
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
}
//-----------------------------------------------------------------------------
void OgreApplication::destroyScene()
{
    mInputManager->destroyInputObject(mMouse);
    mInputManager->destroyInputObject(mKeyboard);
    if(mJoystick)
        mInputManager->destroyInputObject(mJoystick);
    OIS::InputManager::destroyInputSystem(mInputManager);

    mSceneMgr->destroyAllEntities();
}
//-----------------------------------------------------------------------------
//Adjust mouse clipping area
void OgreApplication::windowResized(RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//-----------------------------------------------------------------------------
//Unattach OIS before window shutdown (very important under Linux)
void OgreApplication::windowClosed(RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//-----------------------------------------------------------------------------
bool OgreApplication::frameStarted(const FrameEvent& evt)
{
    _updateDebugOverlay();

    mKeyboard->capture();
    mMouse->capture();
    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
        return false;

    if (mKeyboard->isKeyDown(OIS::KC_P))
        _saveBuffers();



    if(mTimeSinceLastToggle >= 0.0f)
        mTimeSinceLastToggle -= evt.timeSinceLastFrame;    
    else
    {
        if (mKeyboard->isKeyDown(OIS::KC_O))
            mSSAOCompositor->setEnabled(!mSSAOCompositor->getEnabled());

        mTimeSinceLastToggle = 0.1f;
    }



    mMoveSpeedLimit = mMoveScale * evt.timeSinceLastFrame;

    Ogre::Vector3 lastMotion = mTranslationVector;

    mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
    mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
    mYawAngle = 0;
    mPitchAngle = 0;
    mTranslationVector = Ogre::Vector3::ZERO;

    if(mJoystick)
    {
        mJoystick->capture();
        _processJoyInput();
    }
    else
    {
        _processKeyboardInput();
        _processMouseInput();
    }

    // ramp up / ramp down speed
    if (mTranslationVector == Ogre::Vector3::ZERO)
    {
        // decay (one third speed)
        mCurrentSpeed -= evt.timeSinceLastFrame * 0.3;
        mTranslationVector = lastMotion;
    }
    else
    {
        // ramp up
        mCurrentSpeed += evt.timeSinceLastFrame;

    }
    // Limit motion speed
    if (mCurrentSpeed > 1.0)
        mCurrentSpeed = 1.0;
    if (mCurrentSpeed < 0.0)
        mCurrentSpeed = 0.0;

    mTranslationVector *= mCurrentSpeed;


    mCamera->yaw(mYawAngle);
    mCamera->pitch(mPitchAngle);
    mCamera->moveRelative(mTranslationVector);

    return mContinue;
}
//-----------------------------------------------------------------------------
void OgreApplication::_processKeyboardInput()
{
    if(mKeyboard->isKeyDown(OIS::KC_A))
        mTranslationVector.x = -mMoveScale;	// Move camera left

    if(mKeyboard->isKeyDown(OIS::KC_D))
        mTranslationVector.x = mMoveScale;	// Move camera RIGHT

    if(mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W) )
        mTranslationVector.z = -mMoveScale;	// Move camera forward

    if(mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S) )
        mTranslationVector.z = mMoveScale;	// Move camera backward

    if(mKeyboard->isKeyDown(OIS::KC_PGUP))
        mTranslationVector.y = mMoveScale;	// Move camera up

    if(mKeyboard->isKeyDown(OIS::KC_PGDOWN))
        mTranslationVector.y = -mMoveScale;	// Move camera down
}
//------------------------------------------------------------------------------
void OgreApplication::_processMouseInput()
{
    const OIS::MouseState &ms = mMouse->getMouseState();
    if(ms.buttonDown(OIS::MB_Right))
    {		
		mTranslationVector.x = ms.X.rel * 0.13 * mMoveScale;
		mTranslationVector.y = -ms.Y.rel * 0.13 * mMoveScale;
    } 
	else
	{
		mYawAngle = Ogre::Degree(-ms.X.rel * 0.13);
		mPitchAngle = Ogre::Degree(-ms.Y.rel * 0.13);
	}
}
//------------------------------------------------------------------------------
void OgreApplication::_processJoyInput()
{
    const OIS::JoyStickState &state = mJoystick->getJoyStickState();

    // get translation
    OIS::Axis x = state.mAxes[LEFT_ANALOG_STICK_X];
    OIS::Axis y = state.mAxes[LEFT_ANALOG_STICK_Y];
    OIS::Axis z = state.mAxes[LEFT_RIGHT_TRIGGERS];


    Ogre::Vector3 ratioVector(y.abs, z.abs, x.abs);
    _normalizeAndClamp<Ogre::Vector3, 3>(ratioVector);

    mTranslationVector = ratioVector * mMoveScale;

    //get rotation
    OIS::Axis rotX = state.mAxes[RIGHT_ANALOG_STICK_X];
    OIS::Axis rotY = state.mAxes[RIGHT_ANALOG_STICK_Y];
    mXAxis = rotX;
    mYAxis = rotY;


    Ogre::Vector2 rotRatioVector(rotX.abs, rotY.abs);
    _normalizeAndClamp<Ogre::Vector2, 2>(rotRatioVector);

    mYawAngle = Ogre::Degree(-rotRatioVector[1] * mRotScale);
    mPitchAngle = mPitchDirection * Ogre::Degree(rotRatioVector[0] * mRotScale);
}
//------------------------------------------------------------------------------
template <class V, size_t V_SIZE>
void OgreApplication::_normalizeAndClamp(V &_vec)
{
    // map to [-1.0 ; +1.0] range
    _vec *= (2.0f/ 65536.0f);
    // clamp near-0 values to 0
    for (int i = 0 ; i<V_SIZE ; i++)
    {
        if(Ogre::Math::Abs(_vec[i]) < 0.3f)
            _vec[i] = 0.0f;
    }
}
//-----------------------------------------------------------------------------
bool OgreApplication::frameEnded(const FrameEvent& evt)
{
    return true;
}
////-----------------------------------------------------------------------------
//bool OgreApplication::keyPressed( const OIS::KeyEvent &e )
//{
//    return true;
//}
////-----------------------------------------------------------------------------
//bool OgreApplication::keyReleased( const OIS::KeyEvent &e )
//{
//    return true;
//}
////-----------------------------------------------------------------------------
//bool OgreApplication::mouseMoved( const OIS::MouseEvent &e )
//{
//    if(mControlType == CT_MOUSE)
//    {
//        if (e.state.buttonDown(OIS::MB_Right))
//        {
//            int dx = e.state.X.rel * mRotateSpeed;
//            int dy = e.state.Y.rel * mRotateSpeed;
//
//            mCameraTargetNode->yaw(Degree(dx), SceneNode::TS_PARENT);
//            mCameraTargetNode->pitch(Degree(dy));
//        }
//        else if (e.state.buttonDown(OIS::MB_Middle))
//        {
//            int dy = e.state.Y.rel;
//
//            mCameraNode->translate(Vector3(0.0, 0.0, dy)); 
//        }
//    }
//    else if(mControlType == CT_FPS_KEYBOARD)
//    {
//        //TODO
//    }
//
//      
//
//   return true;
//}
////-----------------------------------------------------------------------------
//bool OgreApplication::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
//{
//    return true;
//}
////-----------------------------------------------------------------------------
//bool OgreApplication::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
//{
//    return true;
//}
//-----------------------------------------------------------------------------
void OgreApplication::_createGrid(int _units)
{
    SceneNode *gridNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("WorldGrid Node");
    ManualObject *axes = mSceneMgr->createManualObject("AXES");


    axes->begin("WorldGrid/Axes", RenderOperation::OT_LINE_LIST);
    // X axis
    axes->position(-_units, 0.0, 0.0);     
    axes->colour(0.1, 0.0, 0.0);
    
    axes->position( _units, 0.0, 0.0);     
    axes->colour(1.0, 0.0, 0.0);
    
    // Y Axis
    axes->position(0.0, -_units, 0.0);     
    axes->colour(0.0, 0.1, 0.0);
    
    axes->position(0.0,  _units, 0.0);     
    axes->colour(0.0, 1.0, 0.0);
  
    // Z axis
    axes->position( 0.0, 0.0, -_units);     
    axes->colour(0.0, 0.0, 0.1);
    
    axes->position( 0.0, 0.0,  _units);  
    axes->colour(0.0, 0.0, 1.0);
    
    axes->end();
    gridNode->attachObject(axes);
    axes->setQueryFlags(0x00);

    ManualObject *grid = mSceneMgr->createManualObject("Grid Lines");

    grid->begin("WorldGrid/Lines", RenderOperation::OT_LINE_LIST);
    float c;
    for (int i = 10; i<=_units ; i+=10)
    {
        c = (i%100) ? 0.6 : 0.7;

        grid->position(-_units, 0, i);
        grid->colour(c, c, c);
        grid->position( _units, 0, i);
        grid->colour(c, c, c);

        grid->position(-_units, 0, -i);
        grid->colour(c, c, c);
        grid->position( _units, 0, -i);
        grid->colour(c, c, c);


        grid->position(i, 0, -_units);
        grid->colour(c, c, c);
        grid->position(i, 0,  _units);
        grid->colour(c, c, c);

        grid->position(-i, 0, -_units);
        grid->colour(c, c, c);
        grid->position(-i, 0,  _units);
        grid->colour(c, c, c);
    }


    grid->end();
    grid->setQueryFlags(0x00);
    gridNode->attachObject(grid);
}
//-----------------------------------------------------------------------------
void OgreApplication::_createDebugOverlay()
{
    mDebugOverlay = new DebugOverlay(Ogre::OverlayManager::getSingletonPtr(), "statistics");

    mDebugOverlay->addValueBox("Batches", "#batches :");
    mDebugOverlay->addValueBox("FPS", "#fps : ");
    mDebugOverlay->addValueBox("Triangles", "#tris : ");
    mDebugOverlay->addValueBox("JoyXAxis", "Joy X Axis : ");
    mDebugOverlay->addValueBox("JoyYAxis", "Joy Y Axis : ");   
    mDebugOverlay->addValueBox("SSAO", "SSAO On : ");

	Ogre::Real verticalOffset = float(mDebugOverlay->getHeight()) / mWindow->getHeight();
	mDebugOverlay->setPosition(1.0f - verticalOffset, 0.0);

}
//-----------------------------------------------------------------------------
void OgreApplication::_updateDebugOverlay()
{
    mDebugOverlay->setValue("Batches", Ogre::StringConverter::toString(mWindow->getBatchCount()));
    mDebugOverlay->setValue("FPS", Ogre::StringConverter::toString(mWindow->getLastFPS()));
    mDebugOverlay->setValue("Triangles", Ogre::StringConverter::toString(mWindow->getTriangleCount()));
    mDebugOverlay->setValue("JoyXAxis", Ogre::StringConverter::toString(mXAxis.abs));
    mDebugOverlay->setValue("JoyYAxis", Ogre::StringConverter::toString(mYAxis.abs));

    mDebugOverlay->setValue("SSAO", Ogre::StringConverter::toString(mSSAOCompositor->getEnabled()));
}
//-----------------------------------------------------------------------------
void OgreApplication::_initSSAO()
{
    Ogre::Viewport *viewport = mWindow->getViewport(0);
    assert(viewport);
    mSSAOCompositor = Ogre::CompositorManager::getSingletonPtr()->addCompositor(viewport, "SSAO/DebugCompositor");
 
    if(!mSSAOCompositor)
        OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, "Failed to create ssao compositor", "OgreApplication::_initSSAO");

    mSSAOCompositor->setEnabled(false);
    mSSAOCompositor->addListener(this);
}
//-----------------------------------------------------------------------------
void OgreApplication::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
{
    if (pass_id != SSAO_PASS_ID) // not SSAO, return
        return;

    // this is the camera you're using
    Ogre::Camera *cam =  mCamera;

    // calculate the far-top-right corner in view-space
    Ogre::Vector3 farCorner = cam->getViewMatrix(true) * cam->getWorldSpaceCorners()[4];


   // get the pass
    Ogre::Pass *pass = mat->getBestTechnique()->getPass(0);

    // get the vertex shader parameters
    Ogre::GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();
    // set the camera's far-top-right corner
    //if (params->_findNamedConstantDefinition("farCorner"))
    //    params->setNamedConstant("farCorner", farCorner);
    //else
    //    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS
    //    , "Could not find parameter <farCorner> for vertex shader <"+pass->getVertexProgramName()+">in material <" + mat->getName()+">"
    //    , "Ogre::Application::notifyMaterialRenderer()");

       

    // get the fragment shader parameters
    params = pass->getFragmentProgramParameters();
    //// set the projection matrix we need
    static const Ogre::Matrix4 CLIP_SPACE_TO_IMAGE_SPACE(
        0.5,    0,    0,  0.5,
        0,   -0.5,    0,  0.5,
        0,      0,    1,    0,
        0,      0,    0,    1);

  
 /*   if (params->_findNamedConstantDefinition("clip_to_image_matrix"))
        params->setNamedConstant("clip_to_image_matrix", CLIP_SPACE_TO_IMAGE_SPACE * cam->getProjectionMatrixWithRSDepth());
 */   //else
    //    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS
    //               , "Could not find parameter 'clip_to_image_matrix' in material " + mat->getName()
    //               , "Ogre::Application::notifyMaterialRenderer()");
                            

    float farDistance = cam->getFarClipDistance();
    //if (params->_findNamedConstantDefinition("farDistance"))    
    //    params->setNamedConstant("farDistance", farDistance);
   //else
   //     OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS
   //                , "Could not find parameter <farDistance> for fragment shader <"+pass->getFragmentProgramName()+"> in material <" + mat->getName()+">"
   //                , "Ogre::Application::notifyMaterialRenderer()");



  /*  mOgreAppLog->logMessage(Ogre::StringConverter::toString(mCamera->getProjectionMatrix()));
    mOgreAppLog->logMessage(Ogre::StringConverter::toString(mCamera->getViewMatrix()));
    mOgreAppLog->logMessage("-----------------");*/

}

//-----------------------------------------------------------------------------
void OgreApplication::_saveBuffers()
{
    Ogre::RenderTarget *gbuffer = mSSAOCompositor->getRenderTarget("GBuffer");

    unsigned int w = gbuffer->getWidth();
    unsigned int h = gbuffer->getHeight();
    float *data = new float[w * h * 4];


   // mSSAOCompositor->getRenderTarget("ssao")->writeContentsToTimestampedFile("gbuffer_", ".png");
     //mWindow->writeContentsToTimestampedFile("final scene", ".png");

    Ogre::PixelBox gbufferContent(w, h, 1, gbuffer->suggestPixelFormat(), data);
    gbuffer->copyContentsToMemory(gbufferContent);

    std::string prefix = Ogre::StringConverter::toString(mImageCounter);
    mImageCounter++;

    prefix += "_" + Ogre::StringConverter::toString(w) + "_" + Ogre::StringConverter::toString(h);


    std::ofstream depthFile(std::string(prefix+"_zbuf.dat").c_str(), std::ios_base::binary | std::ios_base::out);
    std::ofstream normalsFile(std::string(prefix+"_normals.dat").c_str(), std::ios_base::binary | std::ios_base::out);



    for(unsigned int i=0 ; i<h ; i++)
    {
        for(unsigned int j=0 ; j<w ; j++)
        {
            float r = data[4*(i*w + j)]; 
            float g = data[4*(i*w + j)+1]; 
            float b = data[4*(i*w + j)+2]; 
            float a = data[4*(i*w + j)+3]; 
            float one = 1.0f;

            
            depthFile.write((char*)&r, sizeof(float));

            normalsFile.write((char*)&g, sizeof(float));
            normalsFile.write((char*)&b, sizeof(float));
            normalsFile.write((char*)&a, sizeof(float));
            normalsFile.write((char*)&one, sizeof(float));

        }
    }

    depthFile.close();
    normalsFile.close();
    delete [] data;

}