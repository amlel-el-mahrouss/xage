-- Copyright PlayXPlicit SARL

world.HumanoidState = {}
world.HumanoidState.ALIVE = 0
world.HumanoidState.DEAD = 1
world.HumanoidState.INVALID = 3

world.Cursor = {}

local function __addEvent(Tbl, Func)
    Tbl.Slots[#Tbl.Slots + 1] = Func;
    return #Tbl.Slots - 1;
end

local function __rmEvent(Tbl, Index)
    Tbl.Slots[Index] = nil;
end

world.Slots = {
    Login = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalMove = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Logoff = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(selfk, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Slots = {},
        Connect = function(self, Func)
        __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Slots = {},
        Connect = function(self, Func)
            __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Slots = {},
        Connect = function(self, Func)
        __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Slots = {},
        Connect = function(self, Func)
        __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Slots = {},
        Connect = function(self, Func)
        __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
    RenderOneFrame = { 
        Slots = {},
        Connect = function(self, Func)
        __addEvent(self, Func); 
        end,
        Disconnect = function(Index)
            __rmEvent(self, Index); 
        end 
    },
}

world.Counter = 0
world.PlayerCount = 0

function World:Login(ply)
    world.PlayerCount = world.PlayerCount + 1

    for _, v in pairs(world.Slots.Login.Slots) do
        v(ply)
    end
end

function World:Logoff(ply)
    world.PlayerCount = world.PlayerCount - 1

    for _, v in pairs(world.Slots.Logoff.Slots) do
        v(ply)
    end
end

function World:LeftClick()
    for _, v in pairs(world.Slots.LeftClick.Slots) do
        v()
    end
end

function World:RightClick()
    for _, v in pairs(world.Slots.RightClick.Slots) do
        v()
    end
end

function World:MouseMove()
    for _, v in pairs(world.Slots.MouseMove.Slots) do
        v()
    end
end

function World:LocalSpawn()
    for _, v in pairs(world.Slots.LocalSpawn.Slots) do
        v()
    end
end

function World:Move(ply)
    for _, v in pairs(world.Slots.Move.Slots) do
        v(ply)
    end
end

function World:LocalMove(x, y, z)
    for _, v in pairs(world.Slots.LocalMove.Slots) do
        v(x, y, z)
    end
end

function World:Damage(ply)
    for _, v in pairs(world.Slots.Damage.Slots) do
        v(ply)
    end
end

function World:Death(ply)
    for _, v in pairs(world.Slots.Death.Slots) do
        v(ply)
    end
end

function World:Spawn(ply)
    for _, v in pairs(world.Slots.Spawn.Slots) do
        v(ply)
    end
end

function World:RenderOneFrame()
    for _, v in pairs(world.Slots.RenderOneFrame.Slots) do
        v()
    end
end

-- Replications enums
world.REPLICATE_INVALID = 0;
world.REPLICATE_SCRIPT = 1;
world.REPLICATE_SCENE = 2;

world.REPLICATE_TYPE_CREATE = 522;
world.REPLICATE_TYPE_REMOVE = 523;
world.REPLICATE_TYPE_UPDATE = 524;

function World:DumpTable(o)
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

function World:AnyKeyDown()
    return XPXKeyDown();
end

function World:IsKeyDown(key)
    return XPXIsKeyDown(key);
end

function World:IsLeftDown()
    return XPXIsLeftDown();
end

function World:IsRightDown()
    return XPXIsRightDown();
end

function world.Cursor:GetY()
    return XPXGetY();
end

function world.Cursor:GetX()
    return XPXGetX();
end

-- Some part of the specs.
-- World = Root table of components.
-- Script = Root table of scripts.
-- world.Settings = GameVars.
-- world.Players = Players.
-- world.<Object> = C++ or lua exposed object.