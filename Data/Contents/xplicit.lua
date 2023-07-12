
World.Vector3 = {}

function World.Vector3:New(X, Y, Z)
    return { X = X, Y = Y, Z = Z }
end

World.ColorRGB = {}

function World.ColorRGB:New(R, G, B)
    return { Red = R, Green = G, Blue = B }
end

World.HumanoidState = {}
World.HumanoidState.ALIVE = 0
World.HumanoidState.DEAD = 1
World.HumanoidState.INVALID = 3

local function __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
end

local function __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

World.Slot = {
    Login = { 
        Connect = function(self, Func)
            return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Logoff = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = function(self, Func)
            return __addEvent(selfk, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = function(self, Func)
            return __addEvent(self, Func); 
        end,
        Disconnect = function(Iself, ndex)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = function(self, Func)
        return __addEvent(World.Slot.Move, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Tick = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(self, Index)
            __rmEvent(self, Index); 
        end 
    },
}

World.Counter = 0
World.PlayerCount = 0

function World:Login(id)
    World.PlayerCount = World.PlayerCount + 1

    for _, v in ipairs(World.Slot.Login) do
        v.Func(id)
    end
end

function World:Logoff(id)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in ipairs(World.Slot.Logoff) do
        v.Func(id)
    end
end

function World:Tick()
    for _, v in ipairs(World.Slot.Tick) do
        v.Func()
    end
end

function World:LeftClick()
    for _, v in ipairs(World.Slot.LeftClick) do
        v.Func()
    end
end

function World:RightClick()
    for _, v in ipairs(World.Slot.RightClick) do
        v.Func()
    end
end

function World:MouseMove()
    for _, v in ipairs(World.Slot.MouseMove) do
        v.Func()
    end
end

function World:LocalSpawn()
    for _, v in ipairs(World.Slot.LocalSpawn) do
        v.Func()
    end
end

function World:Move(x, y, z)
    for _, v in ipairs(World.Slot.Move) do
        v.Func(x, y, z)
    end
end

function World:Damage()
    for _, v in ipairs(World.Slot.Damage) do
        v.Func()
    end
end

function World:Death()
    for _, v in ipairs(World.Slot.Death) do
        v.Func()
    end
end

function World:Spawn()
    for _, v in ipairs(World.Slot.Spawn) do
        v.Func()
    end
end

World.Name = "WorldLand"
World.Info = "The first ever world in Xplicit, home sweet home."
World.Version = "1.0.0"

-- Class ID
World.INVALID = 0;
World.SCRIPT = 1;
World.SOUND = 2;
World.PARTICLE = 3;
World.ROXML = 4;
World.TEXTURE = 5;
World.TOOL = 6;
World.SMOKE = 7;
World.FORCEFIELD = 8;
World.EXPLOSION = 9;
World.SKYBOX = 10;
World.UNLOAD = 11;
World.LOAD = 12;
World.COUNT = 13;

World.CREATE = 0;
World.UPDATE = 1;
World.REMOVE = 2;

Util = {}

function Util.Dump(o)
    if type(o) == 'table' then
       local s = '{ '
       for k,v in pairs(o) do
          if type(k) ~= 'number' then k = '"'..k..'"' end
          s = s .. '['..k..'] = ' .. Util.Dump(v) .. ','
       end
       return s .. '} '
    else
       return tostring(o)
    end
end
