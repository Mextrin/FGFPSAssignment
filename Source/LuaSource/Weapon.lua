Spread = 6
Damage = 10

MaxAmmo = 128
ReloadAmount = 32
AmmoPickUpAmount = 32
CurrentAmmo = 0
MaxReloadTimer = 2.0
CurrentReloadTimer = 0.0

InfiniteAmmo = false
InfiniteAmmoTimer = 5.0

DecreaseSpreadTimer = 10.0
DamageBoostTimer = 8.0

function Tick(DeltaSeconds)
	CurrentReloadTimer = CurrentReloadTimer - DeltaSeconds
	InfiniteAmmoTimer = InfiniteAmmoTimer - DeltaSeconds
	DecreaseSpreadTimer = DecreaseSpreadTimer - DeltaSeconds
	DamageBoostTimer = DamageBoostTimer - DeltaSeconds
	
	if InfiniteAmmoTimer <= 0 then
		InfiniteAmmo = false
	end
	if DecreaseSpreadTimer <= 0 then
		Spread = 6
	end
	if DamageBoostTimer <= 0 then
		Damage = 10
	end
	--ue4.print(MaxAmmo)
	--ue4.print(CurrentAmmo)
end

function BeginPlay()
	CurrentAmmo = ReloadAmount
end

function Reload()
	if CurrentReloadTimer <= 0 and MaxAmmo > 0 then
		CurrentReloadTimer = MaxReloadTimer

		MaxAmmo = MaxAmmo + CurrentAmmo

		if ReloadAmount > MaxAmmo then
			ReloadAmount = MaxAmmo
		end
		CurrentAmmo = ReloadAmount

		MaxAmmo = MaxAmmo - ReloadAmount
	end
end

function AddAmmo()
		MaxAmmo = MaxAmmo + AmmoPickUpAmount
end

function SetInfiniteAmmo()
	InfiniteAmmoTimer = 5.0
	InfiniteAmmo = true
end

function DecreaseSpread()
	DecreaseSpreadTimer = 10.0
	Spread = 1
end

function DamageBoost()
	DamageBoostTimer = 8.0
	Damage = Damage * 3
end