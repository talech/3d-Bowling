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

#include "GameStateManager.h"
#include "GameState.h"
#include "RunningState.h"
#include "InitializeState.h"
#include "LoadState.h"
#include "NewGameMenu.h"
#include <NiUIManager.h>

NiEmbedGamebryoLicenseCode;

//---------------------------------------------------------------------------
NiApplication* NiApplication::Create()
{
    return NiNew Bowling;
}
//---------------------------------------------------------------------------
Bowling::Bowling() : NiSample("T&T Bowling",
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

#if defined(_XENON)
    SetUISkinFilename("D:\\Data\\UISkinFull.dds");
#elif defined(_PS3)
    NIVERIFY(FindSampleDataFile("UISkinFull_ps3.dds", m_acSkinPath));
#elif defined(WIN32)
    NIVERIFY(FindSampleDataFile("UISkinFull.dds", m_acSkinPath));
#elif defined(_WII)
    SetUISkinFilename("/Data/UISkinFull_wii.dds");
#else
#error "Unsupported platform"
#endif

	m_PhysContactReporter.m_app = this;

    m_spTrnNode = 0;
    m_spRotNode = 0;
    
    // Initialize smart pointers to zero. In the case of early termination
    // this avoid errors.
    m_spPhysScene = 0;

	GameStateManager::getInstance()->addApplication(this);
	GameStateManager::getInstance()->start(RunningState::getInstance());

	m_kColor = NiColorA(0.0f, 0.0f, 0.0f, 1.0f);
	m_whiteColor = NiColorA(1.0f, 1.0f, 1.0f, 1.0f);
    m_uiFlags = NiFontString::COLORED | NiFontString::CENTERED;
    m_kUnicodeRenderClickName = "Unicode Render Click";

}
//---------------------------------------------------------------------------
Bowling::~Bowling()
{
}
//---------------------------------------------------------------------------
bool Bowling::Initialize()
{
	GameStateManager::getInstance()->pushState(InitializeState::getInstance());

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

    if (!NiApplication::Initialize())
        return false;

	/*if (!NiSample::CreateUISystem())
		return false;

	if (!CreateUIElements())
		return false;

	if (!NiSample::CompleteUISystem())
		return false;*/
            
    // Set up camera control
    SetTurretControls();

    // Update the scene graph before rendering begins.
    m_spScene->UpdateProperties();
    m_spScene->UpdateEffects();
    m_spScene->Update(0.0f);

	GameStateManager::getInstance()->physManager = m_pkPhysManager;
	GameStateManager::getInstance()->physScene = m_spPhysScene;
	GameStateManager::getInstance()->scene = m_spScene;
	GameStateManager::getInstance()->changeState(NewGameMenu::getInstance());
    
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
    NiSample::Terminate();
}
//---------------------------------------------------------------------------
bool Bowling::CreateScene()
{
	GameStateManager::getInstance()->pushState(LoadState::getInstance());
    if(!NiApplication::CreateScene())
        return false;

    // Set the background color
    NiColor kColor(0.5f, 0.5f, 1.0f);
    m_spRenderer->SetBackgroundColor(kColor);

	// Because the scene will have some elements with alpha, use an 
    // NiAlphaAccumulator so alpha gets sorted and drawn correctly.
    NiAlphaAccumulator* pkAccum = NiNew NiAlphaAccumulator;
    m_spRenderer->SetSorter(pkAccum);
    
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
    if (!kStream.Load(ConvertMediaFilename("bowlingAlley.nif")))
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
    NiPhysXPropPtr spLaneProp = 0;
    for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    {
        if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
        {
            // We have found the PhysX content in the NIF.
            spLaneProp = (NiPhysXProp*)kStream.GetObjectAt(ui);
        }
    }
    NIASSERT(spLaneProp != 0);
    
	// This scene contains only the static box, so we do not need to set
    // anything else.
    
    // Repeat the process with the ball.
    kStream.RemoveAllObjects();
    if (!kStream.Load(ConvertMediaFilename("BallText.nif")))
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
    //NiPhysXPropPtr spBallProp = 0;
    for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    {
        if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
        {
            // We have found the PhysX content in the NIF.
            spBallProp = (NiPhysXProp*)kStream.GetObjectAt(ui);
        }
    }
    NIASSERT(spBallProp != 0);

	// Repeat the process with the ball.
    kStream.RemoveAllObjects();
    if (!kStream.Load(ConvertMediaFilename("pins2.nif")))
    {
        NIASSERT(0 && "Couldn't load nif file\n");
        NiMessageBox("Could not load Ball.nif. Aborting\n",
            "Missing nif file.");
 
        return false;
    }

    // We know that this NIF file has the pins at location 0. Attach the
    // pins to the scene graph.
    NIASSERT(NiIsKindOf(NiAVObject, kStream.GetObjectAt(0)));
    m_spScene->AttachChild((NiAVObject*)kStream.GetObjectAt(0));
	
	// Look for the PhysX content.
    NiPhysXPropPtr spPinsProp = 0;
    for (unsigned int ui = 1; ui < kStream.GetObjectCount(); ui++)
    {
        if (NiIsKindOf(NiPhysXProp, kStream.GetObjectAt(ui)))
        {
            // We have found the PhysX content in the NIF.
            spPinsProp = (NiPhysXProp*)kStream.GetObjectAt(ui);
        }
    }
    NIASSERT(spPinsProp != 0);

    
    // Now we want the ball and the box to be in the scene.
    // We are doing this after the PhysX scnee has been created. Any props
    // added to a scene after the PhysX scene has been set are automatically
    // instanciated in that scene.
	//m_spPhysScene->AddProp(spCubeProp);
	m_spPhysScene->AddProp(spPinsProp);
    m_spPhysScene->AddProp(spBallProp);
    m_spPhysScene->AddProp(spLaneProp);

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

	
	if(! CreateScoreElements())
		return false;

	CreateScreenElements();


	GameStateManager::getInstance()->popState();
    return true;
}
//---------------------------------------------------------------------------
bool Bowling::CreateFrame()
{
	if (!NiApplication::CreateFrame())
    {
        return false;
    }

    // Create a render click to render the Ni2DString objects.
    Ni2DStringRenderClick* pkUnicodeRenderClick = NiNew Ni2DStringRenderClick;
    pkUnicodeRenderClick->SetName(m_kUnicodeRenderClickName);
	for(int i=0; i<NUM_THROWS; i++){
		pkUnicodeRenderClick->Append2DString(m_spStrThrows[i]);
	}

	for(int i=0; i<NUM_TOTALS; i++){
		pkUnicodeRenderClick->Append2DString(m_spStrTotals[i]);
	}
	pkUnicodeRenderClick->Append2DString(m_spStrGameOver);
	pkUnicodeRenderClick->Append2DString(m_spStrFrameInfo);

    // Insert render click at the end of the main render step.
    NIASSERT(m_spFrame);
    NiDefaultClickRenderStep* pkScreenSpaceStep = NiDynamicCast(
        NiDefaultClickRenderStep, m_spFrame->GetRenderStepByName(
        m_kScreenSpaceRenderStepName));
    NIASSERT(pkScreenSpaceStep);
    pkScreenSpaceStep->AppendRenderClick(pkUnicodeRenderClick);

    return true;
}
//---------------------------------------------------------------------------
bool Bowling::CreateScreenElements()
{

	 // Create screen textures for scoresheet.
    NiTexture::FormatPrefs kPrefs;
    kPrefs.m_eMipMapped = NiTexture::FormatPrefs::NO;

    NiTexture* pkTexture = NiSourceTexture::Create(
        NiApplication::ConvertMediaFilename("scoresheet2.TGA"), kPrefs);
    if (!pkTexture)
        return false;

    NiMeshScreenElements* pkLogo = NiMeshScreenElements::Create(
        NiRenderer::GetRenderer(),
        0.0f, 0.0f, 
        pkTexture->GetWidth(), pkTexture->GetHeight(), 
        NiRenderer::CORNER_TOP_LEFT);

    NiTexturingProperty *pkTexProp = NiNew NiTexturingProperty();
    pkTexProp->SetBaseTexture(pkTexture);
    pkTexProp->SetBaseFilterMode(NiTexturingProperty::FILTER_NEAREST);
    pkTexProp->SetApplyMode(NiTexturingProperty::APPLY_REPLACE);
    pkTexProp->SetBaseClampMode(NiTexturingProperty::CLAMP_S_CLAMP_T);

    NiAlphaProperty *pkAlphaProp = NiNew NiAlphaProperty();
    pkAlphaProp->SetAlphaBlending(true);

    NiZBufferProperty *pkZBufProp = NiNew NiZBufferProperty();
    pkZBufProp->SetZBufferTest(false);
    pkZBufProp->SetZBufferWrite(true);

    pkLogo->AttachProperty(pkTexProp);
    pkLogo->AttachProperty(pkAlphaProp);
    pkLogo->AttachProperty(pkZBufProp);
    pkLogo->UpdateProperties();


    if (!pkLogo)
        return false;



    GetScreenElements().AddTail(pkLogo);
    

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

	NiInputMouse* pkMouse = GetInputSystem()->GetMouse();

	GameStateManager::getInstance()->processMouse( pkMouse );



    NiInputKeyboard* pkKeyboard = GetInputSystem()->GetKeyboard();
	GameStateManager::getInstance()->processKeyboard(pkKeyboard);


    NiInputGamePad* pkGamePad;
    for (unsigned int uiPort = 0; uiPort < NiInputSystem::MAX_GAMEPADS; 
        uiPort++)
    {
        pkGamePad = m_spInputSystem->GetGamePad(uiPort);

		GameStateManager::getInstance()->processGamePad(pkGamePad);

    }

}
//---------------------------------------------------------------------------
void Bowling::UpdateFrame()
{
   

	NiApplication::UpdateFrame(); // Calls process input
	//NiUIManager::GetUIManager()->UpdateUI();
	GameStateManager::getInstance()->update(m_fAccumTime);    

	DrawScore();
	
        
   
}
//---------------------------------------------------------------------------
void Bowling::ResetBall()
{
    // To restore the scene to its initial conditions, we use the data
    // stored in the snapshot that was originally loaded from the NIF file.
    // By default, that is snapshot 0.
    m_spPhysScene->RestoreSnapshotState(0);
	m_ballInMotion = false;
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
    kRot.SetCol(0, -1.0f, 0.0f, 0.0f);
    kRot.SetCol(1, 0.0f, -1.0f, 0.0f);
    kRot.SetCol(2, 0.0f, 0.0f, 1.0f);
    m_kTurret.SetAxes(kRot);
    
    if (m_kTurret.GetInputDevice() == NiTurret::TUR_KEYBOARD)
    {
        m_kTurret.SetTrnButtonsKB(2,
            NiInputKeyboard::KEY_W, NiInputKeyboard::KEY_S);
        m_kTurret.SetTrnButtonsKB(1,
            NiInputKeyboard::KEY_Q, NiInputKeyboard::KEY_E);
        m_kTurret.SetTrnButtonsKB(0,
            NiInputKeyboard::KEY_D, NiInputKeyboard::KEY_A);
            
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

void Bowling::SetCamera(NiCameraPtr cam)
{
	m_spCamera = cam;
}

//---------------------------------------------------------------------------
bool Bowling::CreateScoreElements(){
	//Setup text display
		
	m_spFont = NiFont::Create( NiRenderer::GetRenderer(),
    NiApplication::ConvertMediaFilename("Font/ArialUnicodeMS_BA_36.nff"));
    if (!m_spFont)
    {
        NiOutputDebugString("ERROR:  NFF Font File Load Failed.\n");
        return false;
    }

	// frames text positions
	unsigned int uiWelcomeOffsetX = 34;
    unsigned int uiWelcomeOffsetY = 63;

		//"Hello"
	m_num = 10;
	char displ [50];
	sprintf (displ, "%d", m_num);
		
	for(int i=0; i<NUM_THROWS; i++){
		m_spStrThrows[i] = NiNew Ni2DString(m_spFont, m_uiFlags, 128,
			   displ, m_kColor, 
			   (short)uiWelcomeOffsetX,
			   (short)uiWelcomeOffsetY);

		uiWelcomeOffsetX += 33;
		m_spStrThrows[i]->SetPointSize(12);
	}

	// total scores positions
	uiWelcomeOffsetX = 50;
    uiWelcomeOffsetY = 100;

	m_num = 0;
	sprintf (displ, "%d", m_num);
		
	for(int i=0; i<NUM_TOTALS; i++){
		m_spStrTotals[i] = NiNew Ni2DString(m_spFont, m_uiFlags, 128,
			   displ, m_kColor, 
			   (short)uiWelcomeOffsetX,
			   (short)uiWelcomeOffsetY);

		uiWelcomeOffsetX += 66;
		m_spStrTotals[i]->SetPointSize(24);
	}

	uiWelcomeOffsetX = 880;
    uiWelcomeOffsetY = 50;

	m_num = 0;
	sprintf (displ, "Frame: %d \n Throw: ", m_num);
		
	m_spStrFrameInfo = NiNew Ni2DString(m_spFont, m_uiFlags, 128,
			  displ, m_kColor, 
			  (short)uiWelcomeOffsetX,
			  (short)uiWelcomeOffsetY);

	m_spStrFrameInfo->SetPointSize(24);


	uiWelcomeOffsetX = (NiRenderer::GetRenderer()->GetDefaultBackBuffer()->GetWidth())/2;
    uiWelcomeOffsetY = 2*(NiRenderer::GetRenderer()->GetDefaultBackBuffer()->GetWidth())/5;
		
	m_spStrGameOver = NiNew Ni2DString(m_spFont, m_uiFlags, 128,
			  "", m_whiteColor, 
			  (short)uiWelcomeOffsetX,
			  (short)uiWelcomeOffsetY);


	m_spStrGameOver->SetPointSize(24);
	

	return true;
}

//----------------------------------------------------------------------------
void Bowling::DrawScore(){
	// Draw Updated Score
	for(int i=0; i<NUM_THROWS; i++){
		int score = GameStateManager::getInstance()->score->getScoreThrow(i);
		if(score == -1){
			m_spStrThrows[i]->SetText(" ");
		}
		else if(score == 0){
			m_spStrThrows[i]->SetText("-");
		}
		else if((((float)i/2.0) == 0) && score == 10){
			m_spStrThrows[i]->SetText("/");
		}
		else if((((float)i/2.0) != 0) && score == 10){
			m_spStrThrows[i]->SetText("X");
		}
		else{
			m_spStrThrows[i]->sprintf("%d",score);
		}
	}

	for(int i=0; i<(NUM_TOTALS-1); i++){
		int score = GameStateManager::getInstance()->score->getFrameScore(i+1);
		if(score == -1){
			m_spStrTotals[i]->SetText(" ");
		}
		else{
			m_spStrTotals[i]->sprintf("%d",score);
		}
	}
	int score = GameStateManager::getInstance()->score->getTotalScore();
	if(score == -1){
		m_spStrTotals[NUM_TOTALS-1]->SetText(" ");
	}
	else
		m_spStrTotals[NUM_TOTALS-1]->sprintf("%d",score);
	
	int m_f = GameStateManager::getInstance()->score->getFrame();
	int m_t = GameStateManager::getInstance()->score->getThrow();
	m_spStrFrameInfo->sprintf("Frame: %d \n Throw: %d", m_f,m_t);

	if(GameStateManager::getInstance()->score->isGameOver()){
		int score = GameStateManager::getInstance()->lastScore;
		m_spStrGameOver->sprintf("Game Over!\n Your score was: %d \n Press space for a new game!", score);
	}
	else if(GameStateManager::getInstance()->waitingForNewGame)
	{
		m_spStrGameOver->sprintf("Press space for a new game! \n To position the ball, press the left or right arrows. \n To throw the ball, click, drag back, then drag forward and release!");
	}
	else
	{
		m_spStrGameOver->sprintf("");
	}

}