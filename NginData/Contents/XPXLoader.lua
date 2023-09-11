-- Copyright PlayXPlicit SARL

World:New('Image', 'XPXLoadingUI', 'World');

local xpx_ui = World.XPXLoadingUI;

xpx_ui.Path = "xasset://Library/DR1.png";

xpx_ui.X = 0;
xpx_ui.Y = 0;

xpx_ui.W = World:GetWidth();
xpx_ui.H = World:GetHeight();

World.Slots.LocalSpawn:Connect(function()
    xpx_ui:Destroy();
end)