
FirePower = 1000
Health = 100

CanAttack = true

AttackCooldown = 3
CurrentCooldown = 0

SprintMul = 1

SprintDecay = 40
SprintCharge = 100

StaminaCurrent = 100
StaminaMax = 100

StaminaCooldown = 0

IsSprinting = false

IsMoving = false
IsADS = false

function Tick(DeltaTime)
	if IsSprinting then
		StaminaCurrent = math.max( StaminaCurrent - (SprintDecay * DeltaTime), 0 )

		if StaminaCurrent == 0 then
			StopSprint(true)
		end
	elseif StaminaCooldown > 0 then
		StaminaCooldown = StaminaCooldown - DeltaTime
	elseif not IsSprinting then
		StaminaCurrent = math.min( StaminaCurrent + (SprintCharge * DeltaTime), StaminaMax )
	end

	if not IsMoving and IsSprinting then
		StopSprint()
	end
end

function GetSpeedMultiplier()
	if IsADS then
		return SprintMul * 0.5
	end

	return SprintMul
end

function StartSprint()
	if CanSprint() then
		SprintMul = 1.5
		IsSprinting = true
	end
end

function CanSprint()
	return StaminaCooldown <= 0 and StaminaCurrent > 10 and IsMoving
end

function StopSprint(cooldown)
	SprintMul = 1
	IsSprinting = false
	if cooldown then
		StaminaCooldown = 3
	end
end

function GetSpreadMultiplier()
	if IsADS then
		return 0.7
	end

	return 1
end

function GetADSSpeed()
	return 20
end
