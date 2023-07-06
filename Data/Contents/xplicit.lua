
Vector3 = {}

function Vector3:New(X, Y, Z)
    return { X = X, Y = Y, Z = Z }
end

Color = {}

function Color:New(R, G, B)
    return { Red = R, Green = G, Blue = B }
end

HUMANOID = {}

HUMANOID.ALIVE = 0
HUMANOID.DEAD = 1
HUMANOID.INVALID = 3

local function __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
end

local function __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

Game.Slot = {
    Login = { 
        Connect = function(self, Func)
            return __addEvent(Game.Slot.Login, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Logoff = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.Logoff, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = function(self, Func)
            return __addEvent(Game.Slot.LeftClick, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = function(self, Func)
            return __addEvent(Game.Slot.RightClick, Func); 
        end,
        Disconnect = function(Iself, ndex)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.MouseMove, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.LocalSpawn, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.Move, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.Damage, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.Spawn, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = function(self, Func)
        return __addEvent(Game.Slot.Death, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
}

Game.Counter = 0
Game.PlayerCount = 0

function Game:Login(id)
    Game.PlayerCount = Game.PlayerCount + 1

    for _, v in ipairs(Game.Slot.Login) do
        v.Func(id)
    end
end

function Game:Logoff(id)
    Game.PlayerCount = Game.PlayerCount - 1

    for _, v in ipairs(Game.Slot.Logoff) do
        v.Func(id)
    end
end

function Game:LeftClick()
    for _, v in ipairs(Game.Slot.LeftClick) do
        v.Func()
    end
end

function Game:RightClick()
    for _, v in ipairs(Game.Slot.RightClick) do
        v.Func()
    end
end

function Game:MouseMove()
    for _, v in ipairs(Game.Slot.MouseMove) do
        v.Func()
    end
end

function Game:LocalSpawn()
    for _, v in ipairs(Game.Slot.LocalSpawn) do
        v.Func()
    end
end

function Game:Move(x, y, z)
    for _, v in ipairs(Game.Slot.Move) do
        v.Func()
    end
end

function Game:Damage()
    for _, v in ipairs(Game.Slot.Damage) do
        v.Func()
    end
end

function Game:Death()
    for _, v in ipairs(Game.Slot.Death) do
        v.Func()
    end
end

function Game:Spawn()
    for _, v in ipairs(Game.Slot.Spawn) do
        v.Func()
    end
end

Game.Name = "Xplicit"
Game.Description = "Game Engine API"
Game.Version = "1.0.2"

-- Given by server to initialize UI and stuff...
Game.AutorunClient = "xasset://autorun-client.lua"

-- Components ID.
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
