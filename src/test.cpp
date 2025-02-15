#include <avz.h>
#include "common.h"

ATickRunner tick;
void AScript() {
    ASetReloadMode(AReloadMode::MAIN_UI_OR_FIGHT_UI);
    CommonKeys();
    tick.Start(AutoRemovePumpkin);
}