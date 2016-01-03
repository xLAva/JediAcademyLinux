#include "GameMenuHmdManager.h"

#include "../../game/q_shared.h"
#include "../../qcommon/qcommon.h"

#include "../../client/client.h"

GameMenuHmdManager::GameMenuHmdManager()
    :mpHmdRenderer(nullptr)
    ,mIsFullscreenMenuOpen(false)
    ,mIsCameraControlled(false)
    ,mShowCutScenesInVr(true)
{
    mInGameHudNames.insert("mainhud");
    mInGameHudNames.insert("loadscreen");
}

GameMenuHmdManager::~GameMenuHmdManager()
{

}

void GameMenuHmdManager::SetHmdRenderer(IHmdRenderer* pHmdRenderer)
{
    mpHmdRenderer = pHmdRenderer;
    if (mpHmdRenderer != nullptr)
    {
        SetHmdUiMode();
    }
}

void GameMenuHmdManager::OnMenuOpen(std::string menuName)
{
    if (mInGameHudNames.find(menuName) != mInGameHudNames.end())
    {
        return;
    }

    mCurrentOpenMenu.insert(menuName);
}

void GameMenuHmdManager::OnMenuClose(std::string menuName)
{
    if (mInGameHudNames.find(menuName) != mInGameHudNames.end())
    {
        return;
    }

    auto foundMenu = mCurrentOpenMenu.find(menuName);
    if (foundMenu != mCurrentOpenMenu.end())
    {
        mCurrentOpenMenu.erase(foundMenu);
    }
}

void GameMenuHmdManager::OnCloseAllMenus()
{
    mCurrentOpenMenu.clear();
}



void GameMenuHmdManager::SetCameraControlled(bool active)
{
    if (mIsCameraControlled == active)
    {
        return;
    }

    mIsCameraControlled = active;

    if (mShowCutScenesInVr)
    {
        return;
    }

    Update();
}

void GameMenuHmdManager::Update()
{
    bool isFullscreenMenuOpen = true;
    
    // if no map is loaded we are always in fullscreen menu mode
    //if (Cvar_VariableIntegerValue("sv_running"))
    {
        isFullscreenMenuOpen = mCurrentOpenMenu.size() > 0;
    }

    if (cls.state == CA_CINEMATIC || CL_IsRunningInGameCinematic())
    {
        isFullscreenMenuOpen = true;
    }

    if (!mShowCutScenesInVr)
    {
        isFullscreenMenuOpen |= mIsCameraControlled;
    }

    if (mIsFullscreenMenuOpen == isFullscreenMenuOpen)
    {
        return;
    }

    mIsFullscreenMenuOpen = isFullscreenMenuOpen;

    SetHmdUiMode();
}


void GameMenuHmdManager::SetHmdUiMode()
{
    if (mpHmdRenderer == nullptr)
    {
        return;
    }

    IHmdRenderer::UiMode currentMode = mIsFullscreenMenuOpen ? IHmdRenderer::FULLSCREEN_MENU : IHmdRenderer::INGAME_HUD;
    mpHmdRenderer->SetCurrentUiMode(currentMode);


    bool useHmd = !mIsFullscreenMenuOpen;
    if (useHmd)
    {
        Cvar_SetValue("cg_useHmd", 1);
    }
    else
    {
        Cvar_SetValue("cg_useHmd", 0);
    }
}