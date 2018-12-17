Spread = 6
Damage = 10

CurrentTime = 0

function Tick(DeltaSeconds)

	if CurrentTime > 1 and MyWeapon then
		--MyWeapon:Fire()
		CurrentTime = 0
	end

	CurrentTime = CurrentTime + DeltaSeconds
end
