#ifndef GAMEMENUHMDMANAGER_H
#define GAMEMENUHMDMANAGER_H

#include "../HmdRenderer/IHmdRenderer.h"
#include <string>
#include <unordered_set>

class GameMenuHmdManager
{
public:
    GameMenuHmdManager();
    ~GameMenuHmdManager();

    void SetHmdRenderer(IHmdRenderer* pHmdRenderer);

    void OnMenuOpen(std::string menuName);
    void OnMenuClose(std::string menuName);

    void OnCloseAllMenus();
    void SetCameraControlled(bool active);

    void Update();

private:
    void SetHmdUiMode();

    IHmdRenderer* mpHmdRenderer;
    bool mIsFullscreenMenuOpen;
    bool mIsCameraControlled;
    bool mShowCutScenesInVr;

    std::unordered_set<std::string> mInGameHudNames;
    std::unordered_set<std::string> mCurrentOpenMenu;

};

#endif