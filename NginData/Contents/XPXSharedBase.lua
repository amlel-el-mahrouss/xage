-- Copyright XPX Corporation, all rights reserved.

local function __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
end

local function __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

World.Slots = {
    Login = { 
        Connect = function(self, Func)
            return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalMove = { 
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
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = function(self, Func)
            return __addEvent(selfk, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = function(self, Func)
            return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    RenderOneFrame = { 
        Connect = function(self, Func)
        return __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
}

World.Counter = 0
World.PlayerCount = 0

function World:Login(ply)
    World.PlayerCount = World.PlayerCount + 1

    for _, v in ipairs(World.Slots.Login) do
        v.Func(ply)
    end
end

function World:Logoff(ply)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in ipairs(World.Slots.Logoff) do
        v.Func(ply)
    end
end

function World:LeftClick()
    for _, v in ipairs(World.Slots.LeftClick) do
        v.Func()
    end
end

function World:RightClick()
    for _, v in ipairs(World.Slots.RightClick) do
        v.Func()
    end
end

function World:MouseMove()
    for _, v in ipairs(World.Slots.MouseMove) do
        v.Func()
    end
end

function World:LocalSpawn()
    for _, v in ipairs(World.Slots.LocalSpawn) do
        v.Func()
    end
end

function World:Move(ply)
    for _, v in ipairs(World.Slots.Move) do
        v.Func(ply)
    end
end

function World:LocalMove(x, y, z)
    for _, v in ipairs(World.Slots.LocalMove) do
        v.Func(x, y, z)
    end
end

function World:Damage(ply)
    for _, v in ipairs(World.Slots.Damage) do
        v.Func(ply)
    end
end

function World:Death(ply)
    for _, v in ipairs(World.Slots.Death) do
        v.Func(ply)
    end
end

function World:Spawn(ply)
    for _, v in ipairs(World.Slots.Spawn) do
        v.Func(ply)
    end
end

function World:RenderOneFrame()
    for _, v in ipairs(World.Slots.RenderOneFrame) do
        v.Func()
    end
end

World.HumanoidState = {}
World.HumanoidState.ALIVE = 0
World.HumanoidState.DEAD = 1
World.HumanoidState.INVALID = 3

-- Replications enums
World.DEVLIER_INVALID = 0;
World.DELIVER_SCRIPT = 1;
World.DELIVER_SCENE = 2;

World.DELIVER_CREATE = 522;
World.DELIVER_REMOVE = 523;
World.DELIVER_UPDATE = 524;

function World:DumpArray(o)
    if type(o) == 'table' then
       local s = '{ '
       for k,v in pairs(o) do
          if type(k) ~= 'number' then k = '"'..k..'"' end
          s = s .. '['..k..'] = ' .. Wolrd:Dump(v) .. ','
       end
       return s .. '} '
    else
       return tostring(o)
    end
end
