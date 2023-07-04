-- Copyright Xplicit Corporation

PrintLn('XplicitNgine');

# Copyright Xplicit Corporation

_G.Vector3 = {}

func _G.Vector3:New(X, Y, Z)
    return { Name = "Vector3", Type = "Vector", X = X, Y = Y, Z = Z };
end

_G.Color = {}

func _G.Color:New(R, G, B)
    return { Name = "Color", Type = "Color", Red = R, Green = G, Blue = B };
end

_G.HUMANOID = {}

_G.HUMANOID.ALIVE = 0;
_G.HUMANOID.DEAD = 1;
_G.HUMANOID.INVALID = 3;

Engine.Events = {};

Engine.Counter = 0;
Engine.PlayerCount = 0;

func Engine:Join()
    Engine.PlayerCount = Engine.PlayerCount + 1;

    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Join") then
            v.Func();
        end
    end
end

func Engine:Leave()
    Engine.PlayerCount = Engine.PlayerCount - 1;

    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Leave") then
            v.Func();
        end
    end
end

func Engine:LeftClick()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "LeftClick") then
            v.Func();
        end
    end
end

func Engine:RightClick()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "RightClick") then
            v.Func();
        end
    end
end

func Engine:MouseMove()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "MouseMove") then
            v.Func();
        end
    end
end

func Engine:LocalSpawn()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "LocalSpawn") then
            v.Func();
        end
    end
end

func Engine:Move()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Move") then
            v.Func();
        end
    end
end

func Engine:Damage()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Damage") then
            v.Func();
        end
    end
end

func Engine:Death()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Death") then
            v.Func();
        end
    end
end

func Engine:Spawn()
    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Spawn") then
            v.Func();
        end
    end
end

func Engine:Init()
    PrintLn('Registering engine...');

    Engine.Name = "Xplicit";
    Engine.Description = "GameNgine";
    Engine.Version = "1.0.2";

    # Given by server to initialize UI and stuff...
    Engine.AutorunClient = "xasset://autorun-client.lua";
end

func Engine:Connect(Name, Id, Func)
    PrintLn('Registering event...');
    
    table.insert(Engine.Events, {
        Name = Name,
        Func = Func,
	    Id = id,
    });

    Engine.Counter = Engine.Counter + 1;

    PrintLn('Registered!');
end

func Engine:Disconnect(Id)
    PrintLn('Disconnecting event...');
    
    for _, v in ipairs(Engine.Events) do
        if (v.Id == Id) then
            v = {};

            Engine.Counter = Engine.Counter - 1;

            PrintLn('Event Disconnected...');
        end
    end
end

Engine:Init();

Engine.INVALID = 0;
Engine.SCRIPT = 1;
Engine.SOUND = 2;
Engine.PARTICLE = 3;
Engine.ROXML = 4;
Engine.TEXTURE = 5;
Engine.TOOL = 6;
Engine.SMOKE = 7;
Engine.FORCEFIELD = 8;
Engine.EXPLOSION = 9;
Engine.SKYBOX = 10;
Engine.UNLOAD = 11;
Engine.LOAD = 12;
Engine.COUNT;
