#include <avz.h>
#include "common.h"

ATickRunner tick;
void AScript() {
    CommonKeys();
    tick.Start(AutoRemovePumpkin);
}