Health = 100

function TakeDamage(Damage)
	if Health > 0 then
		Health = Health - Damage
		ue4.print( "Took " .. tonumber(Damage) .. " damage." .. Health .. " left" )
		if Health < 0 then
			ue4.print("dead :(")
		end
	else
		ue4.print("already dead")
	end
end