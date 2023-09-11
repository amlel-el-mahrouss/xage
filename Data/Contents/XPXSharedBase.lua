-- Copyright PlayXPlicit SARL

World.HumanoidState = {}
World.HumanoidState.ALIVE = 0
World.HumanoidState.DEAD = 1
World.HumanoidState.INVALID = 3

World.Cursor = {}
World.Cursor.URL = "xasset://Library/DefaultCursor.png";

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

    World.RemoteEventStorage.Login.ShouldUpdate = true;
end

function World:Logoff(ply)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in ipairs(World.Slots.Logoff) do
        v.Func(ply)
    end

    World.RemoteEventStorage.Logoff.ShouldUpdate = true;
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

    World.RemoteEventStorage.Move.ShouldUpdate = true;
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

    World.RemoteEventStorage.Damage.ShouldUpdate = true;
end

function World:Death(ply)
    for _, v in ipairs(World.Slots.Death) do
        v.Func(ply)
    end

    World.RemoteEventStorage.Death.ShouldUpdate = true;
end

function World:Spawn(ply)
    for _, v in ipairs(World.Slots.Spawn) do
        v.Func(ply)
    end

    World.RemoteEventStorage.Spawn.ShouldUpdate = true;
end

function World:RenderOneFrame()
    for _, v in ipairs(World.Slots.RenderOneFrame) do
        v.Func()
    end
end

-- Replications enums
World.REPLICATE_INVALID = 0;
World.REPLICATE_SCRIPT = 1;
World.REPLICATE_SCENE = 2;

World.REPLICATE_CREATE = 512;
World.REPLICATE_REMOVE = 522;
World.REPLICATE_UPDATE = 523;

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

