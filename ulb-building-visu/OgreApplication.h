/* This program is free software. It comes without any warranty, to
* the extent permitted by applicable law. You can redistribute it
* and/or modify it under the terms of the Do What The Fuck You Want
* To Public License, Version 2, as published by Sam Hocevar. See
* http://sam.zoy.org/wtfpl/COPYING for more details. */

#ifndef __OgreApplication_h__
#define __OgreApplication_h__

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>



/* Basic Ogre app : loads conf files, resources, creates RenderWindow, 
SceneManager, sets up basic mouse input handling*/
class OgreApplication   : public Ogre::FrameListener
                        , public Ogre::WindowEventListener 
                        //, public OIS::MouseListener
                        //, public OIS::KeyListener
{
public:
typedef enum{CT_MOUSE, CT_FPS_KEYBOARD, CT_FPS_JOYSTICK} ControlType;
public:
   OgreApplication(const Ogre::String&, ControlType);
   virtual ~OgreApplication();

   virtual void go();

protected:
   virtual bool initialise();
   virtual bool initOgreCore();

   virtual void createSceneManager();
   virtual void createCamera();
   virtual void createViewports();
   virtual void createResourceListener();
   virtual void createFrameListener();
   virtual void createInputSystem();

   virtual void addResourceLocations();
   virtual void initResources();

   virtual void createScene() = 0; // I am pure virtual, override me!
   virtual void destroyScene();

   // WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

   // FrameListener overrides 
   virtual bool frameStarted(const Ogre::FrameEvent& evt); 
   virtual bool frameEnded(const Ogre::FrameEvent& evt); 

   // OIS crap
   //virtual bool keyPressed( const OIS::KeyEvent &e );
   //virtual bool keyReleased( const OIS::KeyEvent &e );

   //virtual bool mouseMoved( const OIS::MouseEvent &e );
   //virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
   //virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );



    void _processKeyboardInput();
    void _processMouseInput();
    void _processJoyInput();

    template <class V, size_t V_SIZE>
    static void _normalizeAndClamp(V &_vec);


protected:
    void _createGrid(int);
    Ogre::String mTitle;
    Ogre::Root *mRoot;
    Ogre::Camera* mCamera;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mWindow;

    //Ogre::SceneNode *mCameraNode, *mCameraTargetNode, *mCameraBaseNode;
    //Ogre::Real mRotateSpeed;

    OIS::InputManager *mInputManager;
    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
    OIS::JoyStick *mJoystick;

    ControlType mControlType;
    //InputListener *mInputListener;


    bool mContinue;

    Ogre::Real     mMoveSpeed, mCurrentSpeed, mMoveSpeedLimit;
    Ogre::Real     mMoveScale;
    Ogre::Vector3  mTranslationVector;
    Ogre::Real     mAccel;

    Ogre::Radian mYawAngle, mPitchAngle;
    Ogre::Degree mRotScale;
    Ogre::Degree mRotateSpeed;

    int mPitchDirection;

};
 
#endif // __OgreApplication_h__