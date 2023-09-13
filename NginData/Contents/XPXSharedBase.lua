-- Copyright PlayXPlicit SARL

World.HumanoidState = {}
World.HumanoidState.ALIVE = 0
World.HumanoidState.DEAD = 1
World.HumanoidState.INVALID = 3

World.Cursor = {}
World.Cursor.URL = "xasset://Library/DefaultCursor.png";

local function __addEvent(Tbl, Func)
    Tbl.Slots[#Tbl.Slots + 1] = Func;
    return #Tbl.Slots - 1;
end

local function __rmEvent(Tbl, Index)
    Tbl.Slots[Index] = nil;
end

World.Slots = {
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

World.Counter = 0
World.PlayerCount = 0

function World:Login(ply)
    World.PlayerCount = World.PlayerCount + 1

    for _, v in pairs(World.Slots.Login.Slots) do
        v(ply)
    end
end

function World:Logoff(ply)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in pairs(World.Slots.Logoff.Slots) do
        v(ply)
    end
end

function World:LeftClick()
    for _, v in pairs(World.Slots.LeftClick.Slots) do
        v()
    end
end

function World:RightClick()
    for _, v in pairs(World.Slots.RightClick.Slots) do
        v()
    end
end

function World:MouseMove()
    for _, v in pairs(World.Slots.MouseMove.Slots) do
        v()
    end
end

function World:LocalSpawn()
    for _, v in pairs(World.Slots.LocalSpawn.Slots) do
        v()
    end
end

function World:Move(ply)
    for _, v in pairs(World.Slots.Move.Slots) do
        v(ply)
    end
end

function World:LocalMove(x, y, z)
    for _, v in pairs(World.Slots.LocalMove.Slots) do
        v(x, y, z)
    end
end

function World:Damage(ply)
    for _, v in pairs(World.Slots.Damage.Slots) do
        v(ply)
    end
end

function World:Death(ply)
    for _, v in pairs(World.Slots.Death.Slots) do
        v(ply)
    end
end

function World:Spawn(ply)
    for _, v in pairs(World.Slots.Spawn.Slots) do
        v(ply)
    end
end

function World:RenderOneFrame()
    for _, v in pairs(World.Slots.RenderOneFrame.Slots) do
        v()
    end
end

-- Replications enums
World.REPLICATE_INVALID = 0;
World.REPLICATE_SCRIPT = 1;
World.REPLICATE_SCENE = 2;

World.REPLICATE_CREATE = 522;
World.REPLICATE_REMOVE = 523;
World.REPLICATE_UPDATE = 524;

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

function World:Create(name, ...)
    if (name == "Gear") then
        return XPXCreateGear(...);
    elseif (name == "RoXML") then
        return XPXLoadScene(...);
    elseif (name == "Part") then
        return XPXPartCreate(...);
    elseif (name == "Class") then
        return XPXCreateClass(...);
    elseif (name == "Sound") then
        return XPXCreateSound(...);
    elseif (name == "Rect") then
        return XPXMakeRectangle(...);
    elseif (name == "Vector3") then
        local vec = Vector3();
        return vec;
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

function World.Cursor:GetY()
    return XPXGetY();
end

function World.Cursor:GetX()
    return XPXGetX();
end

function World:RedirectToPlace(ply, xplace)
    ply.Redirect = true;
    ply.RedirectTo = xplace;
end

function World:MakeDamageTo(ply, dmg)
    ply.Health = ply.Health - dmg;
end

-- Some part of the specs.
-- World = Root table of components.
-- Script = Root table of scripts.
-- World.Settings = GameVars.
-- World.Players = Players.
-- World.<Object> = C++ or lua exposed object.
