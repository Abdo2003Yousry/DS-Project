#include "Freezer.h"

Freezer::Freezer(color r_c, REGION r_region, int r_id,
    int r_arrivalTime, double r_health, double r_power,
    int r_reloadPeriod, int d)
    : Enemy(r_c, r_region, r_id, r_arrivalTime,
        r_health, r_power, r_reloadPeriod, Freezer_Type, d)
{
}

void Freezer::Act(Tower* tower, Enemy* enemies[], int count, int currentTime)
{
    if (GetState() == Active && !IsFrozen(currentTime) && CanAct(currentTime))
    {
        double iceAmount = GetPower() / GetDistance();  // Çáßãíå åÊÚÊãÌ Úáì ãÓÇİÊå ãä ÇáÈÑÌ æßãÇä åæ ŞæÊå ßÇã 

        int freezeDuration = GetPower() / 5; //ÇÎÊÑÊ ÇáÑŞã ÚÔæÇÆí æåÌÑÈ áãÇ äÚãá ÓãíáæíÔä æÇÔæİ ÇáãÏå ßæíÓå æáÇ áÇ

        if (freezeDuration < 1)
            freezeDuration = 1;

        // Send ice amount and freeze duration to tower
       // tower->TakeIce(iceAmount, freezeDuration); wait to coorect name from somaya

        UpdateNextActionTime(currentTime);
    }
}