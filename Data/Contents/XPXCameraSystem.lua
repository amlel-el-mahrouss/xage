-- Copyright PlayXPlicit SARL

local camera = World.Camera
local human = World.Players.LocalPlayer;
local cameraOffset = { X = 2, Y = 2, Z = 8 };

local function overTheShoulder()
    local cameraX = 0
    local cameraY = 0

    cameraX = World.Cursor:GetX() - cameraX
    cameraY = math.clamp((World.Cursor:GetY() * 0.4) - cameraY, -75, 75)

    World.Players.LocalPlayer.RootPart.Position = cameraOffset;
end

World.Slots.Tick:Connect(function()
    overTheShoulder();
end)