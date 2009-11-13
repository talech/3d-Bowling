// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2009 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#include <NiMain.h>

#include "Bowling.h"

#include <NiLicense.h>

NiEmbedGamebryoLicenseCode;

//---------------------------------------------------------------------------
NiApplication* NiApplication::Create()
{
    return NiNew Bowling;
}
//---------------------------------------------------------------------------
Bowling::Bowling() : NiApplication("T&T Bowling",
    DEFAULT_WIDTH, DEFAULT_HEIGHT, true), 
	m_PhysContactReporter()
{
    // Tell Gamebryo where to find its data files.
#if defined(WIN32) && !defined(_XBOX)
    SetMediaPath("../Data/Win32/");   
#elif defined(_PS3)
    SetMediaPath("../../../../Data/PS3/");   
#elif defined(_WII)
    SetMediaPath("/Tutorials/PhysX/Data/Wii/");
#elif defined(_XBOX)
    SetMediaPath("D:/DATA/Win32/");
#endif // defined(WIN32) && !defined(_XBOX)

	m_PhysContactReporter.m_app = this;

    m_spTrnNode = 0;
    m_spRotNode = 0;
    
    // Initialize smart pointers to zero. In the case of early termination
    // this avoid errors.
    m_spPhysScene = 0;
}
//---------------------------------------------------------------------------
Bowling::~Bowling()
{
}
//---------------------------------------------------------------------------
bool Bowling::Initialize()
{
    // Save a pointer to the PhysXSDKManager object. This is Emergent's
    // manager for all PhysX global functionality. The file
    // <efdPhysX/PhysXSDKManager.h> must be included for this object to exist.
    m_pkPhysManager = efdPhysX::PhysXSDKManager::GetManager();
    
    // Try to initialize the PhysX SDK. By default, we are setting up
    // Gamebryo's memory manager and debug output interfaces for use
    // with PhysX.
    if (!m_pkPhysManager->Initialize())
    {
        char acMsg[1024];
        NiSprintf(acMsg, 1024,
            "Unable to initialize PhysX SDK version %d.\n"
            "This may mean you don't have PhysX installed.\n"
            "Have you installed PhysX System Software and Core?\n",
            NX_SDK_VERSION_NUMBER);
        NiMessageBox(acMsg,
            "PhysX Initialization Failed");
        return false;
    }
        
    // The manager contains a public pointer to the PhysX SDK object,
    // m_pkPhysXSDK. Here we use it to set some global SDK parameters.
    // See the PhysX documentation for an explanation of these settings.
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_SKIN_WIDTH, 0.01f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_BOUNCE_THRESHOLD, -0.75f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZATION_SCALE, 2.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_BODY_AXES, 1.0f);
    m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
	m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_BODY_LIN_VELOCITY, 1.0f);


    if (!NiApplication::Initialize())
        return false;
            
    // Set up camera control
    SetTurretControls();

    // Update the scene graph before rendering begins.
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spScene->Update(0.0f);
        
    return true;
}
//---------------------------------------------------------------------------
void Bowling::Terminate()
{
    m_spTrnNode = 0;
    m_spRotNode = 0;
	spCone = 0;

    // The PhysX scenes should be deleted before the SDK is shut down.
    m_spPhysScene = 0;

    // The PhysX manager must be shut down after all PhysX content has been
    // deleted and before the application terminates.
    m_pkPhysManager->Shutdown();
    
    NiApplication::Terminate();
}
//---------------------------------------------------------------------------
bool Bowling::CreateScene()
{
    if(!NiApplication::CreateScene())
        return false;

    // Set the background color
    NiColor kColor(0.5f, 0.6f, 1.0f);
    m_spRenderer->SetBackgroundColor(kColor);
    
    // We first create a PhysX scene and the Gamebryo wrapper for it. First
    // the wrapper.
    m_spPhysScene = NiNew NiPhysXScene();
    
    // Then the PhysX scene
    NxSceneDesc kSceneDesc;
    kSceneDesc.gravity.set(0.0f, -9.8f, 0.0f);
    NxScene* pkScene = m_pkPhysManager->m_pPhysXSDK->createScene(kSceneDesc);
    NIASSERT(pkScene != 0);
	pkScene->setUserContactReport(&m_PhysContactReporter);
    
    // Attach the physics scene to the wrapper
    m_spPhysScene->SetPhysXScene(pkScene);
    
    // Load some PhysX-enabled content.
    NiStream kStream;
    if (!kStream.Load(ConvertMediaFilename("bowling.nif")))
	{
        NIASSERT(0 && "Couldn't load nif file\n");
        NiMessageBox("Could not load bowling.nif. Aborting\n",
            "Missing nif file.");
 
        return false;
    }

    // We know that this NIF file has the scene graph at location 0.
    NIASSERT(NiIsKindOf(NiNode, kStream.GetObjectAt(0)));
    m_spScene = (NiNode*)kStream.GetObjectAt(0);

    // We expect the world to have been exported with a camera, so we 
    // look for it here.
    // In order to render the scene graph, we need a camera. We're now going
    // to recurse the scene graph looking for a camera.
	m_spCamera = FindFirstCamera(m_spScene);
    if (!m_spCamera)
    {
        NiMessageBox("The NIF file has no camera!", "Camera Error");
        return false;
    }

    // Look for a camera and the PhysX content. In this case, the PhysX
    // content is the bowling scene.
    //NiPhysXPropPtr spBoxProp = 0;
    //for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    //{
    //    if (NiIsKindOf(NiCamera, kStream.GetObjectAt(ui)))
    //    {
    //        m_spCamera = (NiCamera*)kStream.GetObjectAt(ui);
    //    }
    //    else if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
    //    {
    //        // We have found the PhysX content in the NIF.
    //        spBoxProp = (NiPhysXProp*)kStream.GetObjectAt(ui);
    //    }
    //}
    //NIASSERT(spBoxProp != 0);
    
	// This scene contains only the static box, so we do not need to set
    // anything else.
    
    // Repeat the process with the ball.
    kStream.RemoveAllObjects();
    if (!kStream.Load(ConvertMediaFilename("Ball.nif")))
    {
        NIASSERT(0 && "Couldn't load nif file\n");
        NiMessageBox("Could not load Ball.nif. Aborting\n",
            "Missing nif file.");
 
        return false;
    }

    // We know that this NIF file has the ball at location 0. Attach the
    // ball to the scene graph.
    NIASSERT(NiIsKindOf(NiAVObject, kStream.GetObjectAt(0)));
    m_spScene->AttachChild((NiAVObject*)kStream.GetObjectAt(0));

    // Look for the PhysX content.
    NiPhysXPropPtr spBallProp = 0;
    for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    {
        if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
        {
            // We have found the PhysX content in the NIF.
            spBallProp = (NiPhysXProp*)kStream.GetObjectAt(ui);
        }
    }
    NIASSERT(spBallProp != 0);
	
	
    
    // Now we want the ball and the box to be in the scene.
    // We are doing this after the PhysX scnee has been created. Any props
    // added to a scene after the PhysX scene has been set are automatically
    // instanciated in that scene.
	//m_spPhysScene->AddProp(spCubeProp);
	//m_spPhysScene->AddProp(spCylinderProp);
    m_spPhysScene->AddProp(spBallProp);
    //m_spPhysScene->AddProp(spBoxProp);

    // The ball is a destination object - the Gamebryo scene graph ball
    // receives pose information from the PhysX actor. We need to enable
    // the updating of destinations to start the transfer of information.
    // Each object needs to be enabled and the whole scene. By default the
    // object update is enabled, but the scene update is not.
    m_spPhysScene->SetUpdateDest(true);

	// Show Debug data
	m_spPhysScene->SetDebugRender(true, m_spScene);


	// Setup collision checks
	//NxActor* cylinder = ((NiPhysXRigidBodyDest*)spCylinderProp->GetDestinationAt(0))->GetActor();
	//m_spPhysScene->GetPhysXScene()->setActorPairFlags(*box, *cylinder, NX_NOTIFY_ON_TOUCH | NX_NOTIFY_FORCES);

    return true;
}

void Bowling::ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength)
{
	NxReal	gDeltaTime = 1.0/60.0;
	NxVec3 forceVec = forceStrength*forceDir*gDeltaTime;
	actor->addForce(forceVec);
}

//---------------------------------------------------------------------------
void Bowling::ProcessInput()
{
	NxReal gForceStrength = 10000;
    NiApplication::ProcessInput();
    NiInputKeyboard* pkKeyboard = GetInputSystem()->GetKeyboard();
    if (pkKeyboard)
    {
        if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_R))
        {
            // Reset the ball's position
            ResetBall();
        }

		if (spCubeProp != NULL) {		
			NxActor* cubeActor = ((NiPhysXRigidBodyDest*)spCubeProp->GetDestinationAt(0))->GetActor();

			// Apply forces to the cube
			// Remember this demo is using different X, Y, Z axis 
			if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_LEFT))
			{
				ApplyForceToActor(cubeActor, NxVec3(0, -1, 0), gForceStrength);
			}
			else if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_RIGHT))
			{
				ApplyForceToActor(cubeActor, NxVec3(0, 1, 0), gForceStrength);
			}
			else if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_UP))
			{
				ApplyForceToActor(cubeActor, NxVec3(-1, 0, 0), gForceStrength);
			}
			else if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_DOWN))
			{
				ApplyForceToActor(cubeActor, NxVec3(1, 0, 0), gForceStrength);
			}

			if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_1))
			{
				if (cubeActor->getMass() == 1.0f) {
					cubeActor->setMass(5.0f);
				}
				else {
					cubeActor->setMass(1.0f);
				}

			}

			if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_2))
			{				
				NxReal paramSetting = m_pkPhysManager->m_pPhysXSDK->getParameter(NX_VISUALIZE_COLLISION_SHAPES);
				if (paramSetting == 0.0f) {
					paramSetting = 1.0f;
				}
				else {
					paramSetting = 0.0f;
				}

				m_pkPhysManager->m_pPhysXSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, paramSetting);
			}

			if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_G))
			{
				spCone->SetTranslate(spCone->GetTranslate() + NiPoint3(1.0f, 0.0f, 0.0f));
			}
			else if (pkKeyboard->KeyWasPressed(NiInputKeyboard::KEY_T))
			{
				spCone->SetTranslate(spCone->GetTranslate() + NiPoint3(-1.0f, 0.0f, 0.0f));
			}
		}

    }

    NiInputGamePad* pkGamePad;
    for (unsigned int uiPort = 0; uiPort < NiInputSystem::MAX_GAMEPADS; 
        uiPort++)
    {
        pkGamePad = m_spInputSystem->GetGamePad(uiPort);
        if (pkGamePad)
        {
            if (pkGamePad->ButtonWasPressed(NiInputGamePad::NIGP_START))
            {
                // Reset the ball's position
                ResetBall();
            }
        }
    }

}
//---------------------------------------------------------------------------
void Bowling::UpdateFrame()
{
    NiApplication::UpdateFrame(); // Calls process input

	soundSystem.Update();

    // Update the camera. This uses global time.
    if (m_kTurret.Read())
        m_spTrnNode->Update(m_fAccumTime);
        
    // A call to Simulate starts the simulation.
    // We pass it the target time. 
    m_spPhysScene->Simulate(m_fAccumTime);
    
    // We want the results immediately, so we call FetchResults with
    // the same time we just asked for, and the second argument as true
    // to block on the results.
    m_spPhysScene->FetchResults(m_fAccumTime, true);

    // Now the actors have been moved, but we need to update the
    // Gamebryo objects. UpdateDestinations does that. Note it is called
    // on the NiPhysXScene that owns the objects being updated.
    m_spPhysScene->UpdateDestinations(m_fAccumTime);
    
    // FInally we update the scene graph.
    m_spScene->Update(m_fAccumTime);
}
//---------------------------------------------------------------------------
void Bowling::ResetBall()
{
    // To restore the scene to its initial conditions, we use the data
    // stored in the snapshot that was originally loaded from the NIF file.
    // By default, that is snapshot 0.
    m_spPhysScene->RestoreSnapshotState(0);
}
//---------------------------------------------------------------------------
void Bowling::SetTurretControls()
{   
    m_spCamera->Update(0.0f);
    m_spTrnNode = NiNew NiNode();
    m_spTrnNode->SetTranslate(m_spCamera->GetWorldTranslate());
    m_spCamera->SetTranslate(NiPoint3::ZERO);
    m_spRotNode = NiNew NiNode();
    m_spTrnNode->AttachChild(m_spRotNode);
    m_spRotNode->SetRotate(m_spCamera->GetWorldRotate());
    m_spCamera->SetRotate(NiMatrix3::IDENTITY);
    m_spRotNode->AttachChild(m_spCamera);
    m_spTrnNode->Update(0.0f);
    
    float fTrnSpeed = 0.05f;
    float fRotSpeed = 0.005f;

    m_kTurret.SetStandardTrn(fTrnSpeed, m_spTrnNode);
    m_kTurret.SetStandardRot(fRotSpeed, m_spTrnNode, m_spRotNode);
    NiMatrix3 kRot;
    kRot.SetCol(0, 1.0f, 0.0f, 0.0f);
    kRot.SetCol(1, 0.0f, 0.0f, 1.0f);
    kRot.SetCol(2, 0.0f, -1.0f, 0.0f);
    m_kTurret.SetAxes(kRot);
    
    if (m_kTurret.GetInputDevice() == NiTurret::TUR_KEYBOARD)
    {
        m_kTurret.SetTrnButtonsKB(0,
            NiInputKeyboard::KEY_W, NiInputKeyboard::KEY_S);
        m_kTurret.SetTrnButtonsKB(1,
            NiInputKeyboard::KEY_Q, NiInputKeyboard::KEY_E);
        m_kTurret.SetTrnButtonsKB(2,
            NiInputKeyboard::KEY_D, NiInputKeyboard::KEY_A);
            
        m_kTurret.SetRotButtonsKB(1,
            NiInputKeyboard::KEY_J, NiInputKeyboard::KEY_L);
        m_kTurret.SetRotButtonsKB(2,
            NiInputKeyboard::KEY_I, NiInputKeyboard::KEY_K);
    }
    else if (m_kTurret.GetInputDevice() == NiTurret::TUR_GAMEPAD)
    {
        m_kTurret.SetTrnButtonsStickDirGP(0, 
            NiInputGamePad::NIGP_STICK_LEFT, 
            NiInputGamePad::NIGP_STICK_AXIS_V);
        m_kTurret.SetTrnButtonsGP(1, 
            NiInputGamePad::NIGP_L1, 
            NiInputGamePad::NIGP_R1);
        m_kTurret.SetTrnButtonsStickDirGP(2, 
            NiInputGamePad::NIGP_STICK_LEFT, 
            NiInputGamePad::NIGP_STICK_AXIS_H);

        m_kTurret.SetRotButtonsGP(0, NiInputGamePad::NIGP_NONE,
            NiInputGamePad::NIGP_NONE);
        m_kTurret.SetRotButtonsStickDirGP(1, 
            NiInputGamePad::NIGP_STICK_RIGHT, 
            NiInputGamePad::NIGP_STICK_AXIS_H);
        m_kTurret.SetRotModifiers(1, NiInputGamePad::NIGP_MASK_NONE);
        m_kTurret.SetRotButtonsStickDirGP(2, 
            NiInputGamePad::NIGP_STICK_RIGHT, 
            NiInputGamePad::NIGP_STICK_AXIS_V);
        m_kTurret.SetRotModifiers(2, NiInputGamePad::NIGP_MASK_NONE);
    }
}
//---------------------------------------------------------------------------


void Bowling::processContacts(NxContactPair& pair, NxU32 events) {
	//soundSystem.PlayContactSound();
}
