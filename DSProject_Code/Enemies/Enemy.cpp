#include "Enemy.h"


Enemy::Enemy(color r_c, REGION r_region, int r_id, int r_arrivalTime,
	double r_health, double r_power, int r_reloadPeriod,
	enemyType r_type, int d)
{
	ID = r_id;
	Clr = r_c;
	Region = r_region;
	Distance = d;
	Health = r_health;
	OriginalHealth = r_health;
	Power = r_power;
	State = Inactive;
	Type = r_type;
	ArrivalTime = r_arrivalTime;
	ReloadPeriod = r_reloadPeriod;
	LastActionTime = 0;
	FreezeEndTime = 0;
	FirstShotTime = -1;
	KilledTime = -1;
}

Enemy::~Enemy()
{
}

color Enemy::GetColor() const
{
	return Clr;
}

REGION Enemy::GetRegion() const
{
	return Region;
}




// getter functions
int Enemy::GetDistance() const {
	return Distance;
}

int Enemy::GetID() const {
	return ID;
}
enemyType Enemy::GetType() const
{
	return Type;
}
double Enemy::GetHealth() const {
	return Health;
}

double Enemy::GetOriginalHealth() const {
	return OriginalHealth;
}

double Enemy::GetPower() const {
	return Power;
}

int Enemy::GetArrivalTime() const {
	return ArrivalTime;
}

int Enemy::GetReloadPeriod() const {
	return ReloadPeriod;
}

enemyState Enemy::GetState() const {
	return State;
}
int Enemy::GetNextActionTime() const {
	return (LastActionTime + ReloadPeriod);
}
//setter functions 
void Enemy::SetDistance(int d)
{
	if (d >= MinDistance && d <= MaxDistance)
		Distance = d;
	else
		Distance = MaxDistance;
}
void Enemy::SetID(int id) {
	ID = id;
}

void Enemy::SetHealth(double health) {
	Health = health;
}

void Enemy::SetOriginalHealth(double health) {
	OriginalHealth = health;
}

void Enemy::SetPower(double power) {
	Power = power;
}

void Enemy::SetArrivalTime(int time) {
	ArrivalTime = time;
}

void Enemy::SetReloadPeriod(int period) {
	ReloadPeriod = period;
}

void Enemy::SetState(enemyState state) {
	State = state;
}
void Enemy::SetFirstShotTime(int time) {
	FirstShotTime = time;
}

void Enemy::SetKilledTime(int time) {
	KilledTime = time;
}

// statistics functions 
int Enemy::GetFD() const {
	return (FirstShotTime - ArrivalTime);
}
int Enemy::GetKD() const {
	return (KilledTime - FirstShotTime);
}
int Enemy::GetLT() const {
	return (GetFD() + GetKD());
}
void Enemy::DecrementDist() //
{
	if (Distance > MinDistance)
		Distance--;
}
// Movement
void Enemy::Move(int currentTime) {
	if (CanMove(currentTime)) {
		DecrementDist();
	}
};
bool Enemy::CanMove(int currentTime) const {
	if (IsFrozen(currentTime)) {
		return false;
	}
	else {
		return true;
	}
};


// Health
void Enemy::TakeDamage(double damage, int currentTime) {
	//called bt attack tower
	if (!IsKilled()) {

		// Record first hit time only once
		if (FirstShotTime == -1) {
			SetFirstShotTime(currentTime);
		}

		SetHealth(GetHealth() - damage);

		if (GetHealth() <= 0) {
			SetHealth(0);
			SetKilledTime(currentTime);
		}
	}
}
bool Enemy::IsKilled() const {
	if (GetHealth() <= 0) {
		return true;
	}
	else {
		return false;
	}
}
void Enemy::Heal(double amount) {
	if (!IsKilled()) {
		SetHealth(GetHealth() + amount);

		if (GetHealth() > GetOriginalHealth()) {
			SetHealth(GetOriginalHealth());
		}
	}
}

// Actions
bool Enemy::CanAct(int currentTime) const {
	if (currentTime >= GetNextActionTime()) {
		return true;
	}
	else {
		return false;
	}
}
void Enemy::UpdateNextActionTime(int currentTime) {
	LastActionTime = currentTime;
}
// Freeze
void Enemy::Freeze(int duration, int currentTime) {
	FreezeEndTime = currentTime + duration;
}
bool Enemy::IsFrozen(int currentTime) const {
	if (currentTime < FreezeEndTime) {
		return true;
	}
	else {
		return false;
	}
}