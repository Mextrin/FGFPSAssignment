
CanShoot = true

Cooldown = 0.05
CurrentCooldown = 0

RotationSpeed = 1

function Tick(DeltaTime)
	if CurrentCooldown < 0 then
		CanShoot = true
	end

	CurrentCooldown = CurrentCooldown - DeltaTime
end

function OnShoot()
	CanShoot = false
	CurrentCooldown = Cooldown
end

function CanWeaponShoot()
	return CanShoot
end
