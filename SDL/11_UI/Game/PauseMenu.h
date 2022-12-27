#ifndef __PAUSEMENU_H__
#define __PAUSEMENU_H__

#include "UIScreen.h"

class PauseMenu : public UIScreen {
  public:
    PauseMenu(class Game *game);
    ~PauseMenu();

    void HandleKeyPress(int key) override;
};

#endif // __PAUSEMENU_H__