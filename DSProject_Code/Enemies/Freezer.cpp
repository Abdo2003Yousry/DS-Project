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
    if (GetState() == Active && CanAct(currentTime))
    {
        double iceAmount = GetPower() / GetDistance(); //decided by trial and error // Çáßãíå åÊÚÊãÌ Úáì ãÓÇİÊå ãä ÇáÈÑÌ æßãÇä åæ ŞæÊå ßÇã 

        int freezeDuration = GetPower() / 5; //decided by trial and error //ÇÎÊÑÊ ÇáÑŞã ÚÔæÇÆí æåÌÑÈ áãÇ äÚãá ÓãíáæíÔä æÇÔæİ ÇáãÏå ßæíÓå æáÇ áÇ

        if (freezeDuration < 1) // áÇÒã ÑŞã ÕÍíÍ ÚáÔÇä ÇäÇ ÈÊÚÇãá ãÚ ÇáÊíã ÓÊíÈ ËÇäíå ËÇäíå
            freezeDuration = 1;

        // Send ice amount and freeze duration to tower
        tower->ReceiveIce(iceAmount, freezeDuration);

        UpdateNextActionTime(currentTime);
    }
}