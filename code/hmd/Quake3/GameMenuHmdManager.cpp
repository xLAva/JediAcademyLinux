#include "GameMenuHmdManager.h"

#include "../../game/q_shared.h"
#include "../../qcommon/qcommon.h"

GameMenuHmdManager::GameMenuHmdManager()
    :mpHmdRenderer(nullptr)
    ,mIsFullscreenMenuOpen(false)
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

void GameMenuHmdManager::Update()
{
    bool isFullscreenMenuOpen = true;
    
    // if no map is loaded we are always in fullscreen menu mode
    //if (Cvar_VariableIntegerValue("sv_running"))
    {
        isFullscreenMenuOpen = mCurrentOpenMenu.size() > 0;
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
}