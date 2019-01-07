Spread = 6
Damage = 10

MaxAmmo = 128
ReloadAmount = 32
CurrentAmmo = 0
MaxReloadTimer = 2.0
CurrentReloadTimer = 0.0

function Tick(DeltaSeconds)
	CurrentReloadTimer = CurrentReloadTimer - DeltaSeconds
	ue4.print(MaxAmmo)
end

function BeginPlay()
	CurrentAmmo = ReloadAmount;
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
