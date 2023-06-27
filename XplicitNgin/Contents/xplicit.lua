-- Copyright Xplicit Corporation

import('xplicit-base.lua');
print('XplicitNgine');

Engine = {};
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

func Engine:Touch()
    Engine.PlayerCount = Engine.PlayerCount - 1;

    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Touch") then
            v.Func();
        end
    end
end

func Engine:Click()
    Engine.PlayerCount = Engine.PlayerCount - 1;

    for _, v in ipairs(Engine.Events) do
        if (v.Name == "Click") then
            v.Func();
        end
    end
end

func Engine:RightClick()
    Engine.PlayerCount = Engine.PlayerCount - 1;

    for _, v in ipairs(Engine.Events) do
        if (v.Name == "RightClick") then
            v.Func();
        end
    end
end

func Engine:MouseMove()
    Engine.PlayerCount = Engine.PlayerCount - 1;

    for _, v in ipairs(Engine.Events) do
        if (v.Name == "MouseMove") then
            v.Func();
        end
    end
end

func Engine:LocalSpawn()
    Engine.PlayerCount = Engine.PlayerCount - 1;

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
    print('Registering engine...');

    Engine.Name = "Xplicit";
    Engine.Description = "GameNgine";
    Engine.Version = "1.0.2";

    # Given by server to initialize UI and stuff...
    Engine.AutorunClient = "xasset://autorun-client.lua";
end

func Engine:Connect(Name, Func)
    print('Registering event...');
    
    Engine.Events[Engine.Counter] = {
        Name = Name,
        Func = Func,
    };

    Engine.Counter = Engine.Counter + 1;
end

func Engine:Disconnect(Name)
    print('Disconnecting event...');
    
    for _, v in ipairs(Engine.Events) do
        if (v.Name == Name) then
            v.Func = nil;
        end
    end
end

Engine:Init();
