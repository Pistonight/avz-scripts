#include "common.h"

void CommonKeys() {
    // Advanced pause key
    AConnect('Q', []{
        static bool isActive = false;
        isActive = !isActive;
        ASetAdvancedPause(isActive);
    });

    // Slowmode Key
    AConnect('W', []{
        static bool isActive = false;
        isActive = !isActive;
        if (isActive) {
            ASetGameSpeed(0.25);
        } else {
            ASetGameSpeed(1);
        }
    });
}

void AutoRemovePumpkin() {
    // when to remove the pumpkin
    constexpr uint16_t HAMMER_THRES = 260;
    static std::unordered_map<uint16_t, uint16_t> raise_hammer_time;

    std::vector<APlant*> pumpkins;
    for (auto&& plant: aAlivePlantFilter) {
        if (plant.Type() == APUMPKIN) {
            pumpkins.emplace_back(&plant);
        }
    }

    bool endangered_pumpkins[6][9] = {{false}};

    for (auto&& zombie: aAliveZombieFilter) {
        // skip frozen zombies since they are harmless
        if (zombie.FreezeCountdown() > 0) {
            continue;
        }
        auto idx = zombie.Index();
        if (zombie.IsDead()) {
            raise_hammer_time.erase(idx);
            continue;
        }
        auto type = zombie.Type();
        if (type == AGARGANTUAR || type == AGIGA_GARGANTUAR) {
            if (!zombie.IsHammering()) {
                raise_hammer_time.erase(idx);
                continue;
            }
            auto inc = zombie.SlowCountdown() > 0 ? 1 : 2;
            if (raise_hammer_time.find(idx) == raise_hammer_time.end()) {
                raise_hammer_time[idx] = inc;
            } else {
                raise_hammer_time[idx] += inc;
            }
            
            if (raise_hammer_time[idx] > HAMMER_THRES) {
                // check if it will hit any pumpkins
                float ax = zombie.Abscissa() - zombie.AttackAbscissa() - 181;
                int row = zombie.Row();
                for (auto&& pumpkin: pumpkins) {
                    if (pumpkin->Row() != row) {
                        continue;
                    }
                    if (pumpkin->Col() * 80 > ax) {
                        endangered_pumpkins[row][pumpkin->Col()] = true;
                    }
                }
            }
        }
    }

    // remove endangered pumpkins
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 9; ++c) {
            if (endangered_pumpkins[r][c]) {
                AShovel(r + 1, c + 1, APUMPKIN);
            }
        }
    }
}