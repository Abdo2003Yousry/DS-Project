#include "Freezer.h"
#include "../Castle/Tower.h"
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
        double iceAmount = GetPower() / GetDistance(); //decided by trial and error // Çáßãíå åÊÚÊãÌ Úáì ãÓÇÝÊå ãä ÇáÈÑÌ æßãÇä åæ ÞæÊå ßÇã 

        // Send ice amount and freeze duration to tower
        tower->ReceiveIce(iceAmount); 

        UpdateNextActionTime(currentTime);
    }
}