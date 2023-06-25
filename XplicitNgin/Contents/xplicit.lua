-- Copyright Xplicit Corporation

print('Xplicit Game Server');

Engine = {};
Engine.Events = {};

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
    print('Registering Engine...');

    Engine.Name = "Xplicit";
    Engine.Description = "Game Engine";
    Engine.Version = "1.0.0";

    Engine.AutorunClient = "xasset://xplicit-client.lua";
end

Engine:Init();
