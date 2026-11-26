#include "Healer.h"

#include "Healer.h"

Healer::Healer(color r_c, REGION r_region, int r_id,
    int r_arrivalTime, double r_health, double r_power,
    int r_reloadPeriod, int d)
    : Enemy(r_c, r_region, r_id, r_arrivalTime,
        r_health, r_power, r_reloadPeriod, Healer_Type, d)
{
}



#include <cstdlib>

void Healer::Act(Tower* tower, Enemy* enemies[], int count, int currentTime) //ßÇä ÚäÏí ãÔßáå İí ÇÒÇí ÇÎÊÇÑ ÇáÚÏæ Çáí ÇÒæÏ Ïãå åá ÇÏæÑ Úáì ÇŞá æÇÍÏ ŞÑíÈ ãäí İí ÇáÕÍå ¿¿¿ 
//ÈÓ ßÏÇ ãÖØÑ Çáİ Úáì ßáæÇ ãÑÊíä İÇ ŞÑÑÊ ãÈÏÇíÇ ÇÚãáåÇ ÇäæÇ Çæá áãÇ íáÇŞí ÍÏ ŞÑíÈÈ ãäå ÇŞá ãä ÇÊäíä ãÊÑ æÏãå äÇŞÕ íÒæÏå åæ æÎáÇÕ
{
    if (GetState() != Active || !CanAct(currentTime))
        return;

    for (int i = 0; i < count; i++)
    {
        if (enemies[i] == nullptr ||
            enemies[i] == this ||
            enemies[i]->IsKilled() ||
            enemies[i]->GetRegion() != GetRegion())
            continue;

        int distanceDifference = abs(enemies[i]->GetDistance() - GetDistance());

        if (distanceDifference > 2 ||
            enemies[i]->GetHealth() >= enemies[i]->GetOriginalHealth())
            continue;

        enemies[i]->Heal(GetPower() / 2.0); // decided by Trial and Error

        UpdateNextActionTime(currentTime);

        break;
    }
       
}

