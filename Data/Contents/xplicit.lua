
Vector3 = {}

func Vector3:New(X, Y, Z)
    return { X = X, Y = Y, Z = Z }
end

Color = {}

func Color:New(R, G, B)
    return { Red = R, Green = G, Blue = B }
end

HUMANOID = {}

HUMANOID.ALIVE = 0
HUMANOID.DEAD = 1
HUMANOID.INVALID = 3

local func __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
    
end

local func __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

Game.Slot = {
    Login = { 
        Connect = func(Func)
            return __addEvent(Game.Slot.Login, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Logoff = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.Logoff, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = func(Func)
            return __addEvent(Game.Slot.LeftClick, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = func(Func)
            return __addEvent(Game.Slot.RightClick, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.MouseMove, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.LocalSpawn, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.Move, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.Damage, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.Spawn, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = func(Func)
        return __addEvent(Game.Slot.Death, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
}

Game.Counter = 0
Game.PlayerCount = 0

func Game:Login(id)
    Game.PlayerCount = Game.PlayerCount + 1

    for _, v in ipairs(Game.Slot.Login) do
        v.Func(id)
    end
end

func Game:Logoff(id)
    Game.PlayerCount = Game.PlayerCount - 1

    for _, v in ipairs(Game.Slot.Logoff) do
        v.Func(id)
    end
end

func Game:LeftClick()
    for _, v in ipairs(Game.Slot.LeftClick) do
        v.Func()
    end
end

func Game:RightClick()
    for _, v in ipairs(Game.Slot.RightClick) do
        v.Func()
    end
end

func Game:MouseMove(x, y)
    for _, v in ipairs(Game.Slot.MouseMove) do
        v.Func(x, y)
    end
end

func Game:LocalSpawn()
    for _, v in ipairs(Game.Slot.LocalSpawn) do
        v.Func()
    end
end

func Game:Move(x, y, z)
    for _, v in ipairs(Game.Slot.Move) do
        v.Func()
    end
end

func Game:Damage()
    for _, v in ipairs(Game.Slot.Damage) do
        v.Func()
    end
end

func Game:Death()
    for _, v in ipairs(Game.Slot.Death) do
        v.Func()
    end
end

func Game:Spawn()
    for _, v in ipairs(Game.Slot.Spawn) do
        v.Func()
    end
end

Game.Name = "Xplicit"
Game.Description = "Game Engine API"
Game.Version = "1.0.2"

# Given by server to initialize UI and stuff...
Game.AutorunClient = "xasset://autorun-client.lua"

## Components ID.
Game.INVALID = 0;
Game.SCRIPT = 1;
Game.SOUND = 2;
Game.PARTICLE = 3;
Game.ROXML = 4;
Game.TEXTURE = 5;
Game.TOOL = 6;
Game.SMOKE = 7;
Game.FORCEFIELD = 8;
Game.EXPLOSION = 9;
Game.SKYBOX = 10;
Game.UNLOAD = 11;
Game.LOAD = 12;
Game.COUNT = 13;
