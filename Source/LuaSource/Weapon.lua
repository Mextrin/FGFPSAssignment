Spread = 6
Damage = 10

MaxAmmo = 32
CurrentAmmo = 0
MaxReloadTimer = 2.0
CurrentReloadTimer = 0.0

function Tick(DeltaSeconds)
	CurrentReloadTimer = CurrentReloadTimer - DeltaSeconds
end

function BeginPlay()
	CurrentAmmo = MaxAmmo;
end

function Reload()
	if CurrentReloadTimer <= 0 then
		CurrentReloadTimer = MaxReloadTimer
		CurrentAmmo = MaxAmmo
	end
end
