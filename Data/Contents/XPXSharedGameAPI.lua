-- Copyright PlayXPlicit SARL

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
        Disconnect = function(Iself, Index)
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
        return __addEvent(World.Slots.Move, Func); 
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

function World:Login(tbl)
    World.PlayerCount = World.PlayerCount + 1

    for _, v in ipairs(World.Slots.Login) do
        v.Func(tbl)
    end
end

function World:Logoff(tbl)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in ipairs(World.Slots.Logoff) do
        v.Func(tbl)
    end
end

function World:Tick()
    for _, v in ipairs(World.Slots.Tick) do
        v.Func()
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

function World:Move(tbl)
    for _, v in ipairs(World.Slots.Move) do
        v.Func(tbl)
    end
end

function World:LocalMove(x, y, z)
    for _, v in ipairs(World.Slots.LocalMove) do
        v.Func(x, y, z)
    end
end

function World:Damage()
    for _, v in ipairs(World.Slots.Damage) do
        v.Func()
    end
end

function World:Death(tbl)
    for _, v in ipairs(World.Slots.Death) do
        v.Func(tbl)
    end
end

function World:Spawn(tbl)
    for _, v in ipairs(World.Slots.Spawn) do
        v.Func(tbl)
    end
end

-- Class tbl
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

World.CREATE = 512;
World.UPDATE = 523;
World.REMOVE = 522;

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

function World:New(name, ...)
    if (name == "GearComponent") then
        return XPXCreateGear(...);
    elseif (name == "Scene") then
        return XPXLoadScene(...);
    elseif (name == "PartComponent") then
        return XPXPartCreate(...);
    elseif (name == "ClassComponent") then
        return XPXCreateClass(...);
    elseif (name == "SoundComponent") then
        return XPXCreateSound(...);
    elseif (name == "Sound") then
        return XPXPlaySound(...);
    elseif (name == "RectComponent") then
        return XPXMakeRect(...);
    end
end

function World:KeyDown()
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