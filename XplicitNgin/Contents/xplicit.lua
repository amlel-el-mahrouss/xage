-- Copyright Xplicit Corporation

print('Xplicit Game Server');

Engine = Engine or {};

func Engine:Init()
    print('Registering Engine...');

    Engine.Name = "Xplicit";
    Engine.Description = "Game Engine";
    Engine.Version = "1.0.0";

    Engine.AutorunClient = "xasset://xplicit-client.lua";
end

Engine:Init();