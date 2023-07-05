
_G.Vector3 = {}

func _G.Vector3:New(X, Y, Z)
    return { Name = "Vector3", Type = "Vector", X = X, Y = Y, Z = Z }
end

_G.Color = {}

func _G.Color:New(R, G, B)
    return { Name = "Color", Type = "Color", Red = R, Green = G, Blue = B }
end

_G.HUMANOID = {}

_G.HUMANOID.ALIVE = 0
_G.HUMANOID.DEAD = 1
_G.HUMANOID.INVALID = 3

local func __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
    
end

local func __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

_G.Engine.Events = {
    Join = { 
        Connect = func(Func)
            return __addEvent(_G.Engine.Events.Join, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Leave = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.Leave, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = func(Func)
            return __addEvent(_G.Engine.Events.LeftClick, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = func(Func)
            return __addEvent(_G.Engine.Events.RightClick, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.MouseMove, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.LocalSpawn, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.Move, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.Damage, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.Spawn, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = func(Func)
        return __addEvent(_G.Engine.Events.Death, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
}

_G.Engine.Counter = 0
_G.Engine.PlayerCount = 0

func _G.Engine:Join()
    _G.Engine.PlayerCount = _G.Engine.PlayerCount + 1

    for _, v in ipairs(_G.Engine.Events.Join) do
        v.Func()
    end
end

func _G.Engine:Leave()
    _G.Engine.PlayerCount = _G.Engine.PlayerCount - 1

    for _, v in ipairs(_G.Engine.Events.Leave) do
        v.Func()
    end
end

func _G.Engine:LeftClick()
    for _, v in ipairs(_G.Engine.Events.LeftClick) do
        v.Func()
    end
end

func _G.Engine:RightClick()
    for _, v in ipairs(_G.Engine.Events.RightClick) do
        v.Func()
    end
end

func _G.Engine:MouseMove()
    for _, v in ipairs(_G.Engine.Events.MouseMove) do
        v.Func()
    end
end

func _G.Engine:LocalSpawn()
    for _, v in ipairs(_G.Engine.Events.LocalSpawn) do
        v.Func()
    end
end

func _G.Engine:Move()
    for _, v in ipairs(_G.Engine.Events.Move) do
        v.Func()
    end
end

func _G.Engine:Damage()
    for _, v in ipairs(_G.Engine.Events.Damage) do
        v.Func()
    end
end

func _G.Engine:Death()
    for _, v in ipairs(_G.Engine.Events.Death) do
        v.Func()
    end
end

func _G.Engine:Spawn()
    for _, v in ipairs(_G.Engine.Events.Spawn) do
        v.Func()
    end
end

_G.Engine.Name = "Xplicit"
_G.Engine.Description = "Engine Engine API"
_G.Engine.Version = "1.0.2"

# Given by server to initialize UI and stuff...
_G.Engine.AutorunClient = "xasset://autorun-client.lua"

## Components ID.
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
Engine.COUNT = 13;
