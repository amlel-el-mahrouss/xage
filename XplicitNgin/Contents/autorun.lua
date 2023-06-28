# Copyright Xplicit Corporation
# Written by Amlal El Mahrouss.

Engine:Connect("Join", func()
	if (Hummanoid.GetHealth(0) == 100) then
		Humanoid.Hurt(0, 10);
	else
		print('no.');
	end
end)