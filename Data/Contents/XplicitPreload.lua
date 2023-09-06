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

function World:Login(tbl)
    World.PlayerCount = World.PlayerCount + 1

    for _, v in ipairs(World.Slot.Login) do
        v.Func(tbl)
    end
end

function World:Logoff(tbl)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in ipairs(World.Slot.Logoff) do
        v.Func(tbl)
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

function World:Move(tbl)
    for _, v in ipairs(World.Slot.Move) do
        v.Func(tbl)
    end
end

function World:LocalMove(x, y, z)
    for _, v in ipairs(World.Slot.LocalMove) do
        v.Func(x, y, z)
    end
end

function World:Damage()
    for _, v in ipairs(World.Slot.Damage) do
        v.Func()
    end
end

function World:Death(tbl)
    for _, v in ipairs(World.Slot.Death) do
        v.Func(tbl)
    end
end

function World:Spawn(tbl)
    for _, v in ipairs(World.Slot.Spawn) do
        v.Func(tbl)
    end
end

World.Name = "My first XPLICIT Level"
World.Info = "My first ever world in Xplicit, home sweet home."
World.Version = "1.0.0"

-- Class tbl
World.INVALID = 0;
World.SCRIPT = 1;
World.SOUND = 2;
World.PARTICLE = 3;
World.ROXML = 4;
World.TEXTURE = 5;
World.GEAR = 6;
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

function World:New(name, ...)
    if (World.RunService.IsServer) then
        if (name == "Gear") then
            return World.GearService.Create(...);
        elseif (name == "RoXML") then
            return World.RoXMLService.Load(...);
        end
    end

    return nil;
end

if (World.RunService.IsClient) then
    World.LocalHumanoid = {}

    World.MeshService.Load('Contents/Bundles/Head.obj', 'Head', 'World.LocalHumanoid');
    World.MeshService.Load('Contents/Bundles/LeftArm.obj', 'LeftArm', 'World.LocalHumanoid');
    World.MeshService.Load('Contents/Bundles/RightArm.obj', 'RightArm', 'World.LocalHumanoid');
    World.MeshService.Load('Contents/Bundles/Torso.obj', 'Torso', 'World.LocalHumanoid');
    World.MeshService.Load('Contents/Bundles/Head.obj', 'Head', 'World.LocalHumanoid');

    World.LocalHumanoid.LeftArm.Position.X = -1.5;
    World.LocalHumanoid.LeftArm.Position.Y = 3;
    World.LocalHumanoid.LeftArm.Position.Z = 0;
    
    World.LocalHumanoid.RightArm.Position.X = 1.5;
    World.LocalHumanoid.RightArm.Position.Y = 3;
    World.LocalHumanoid.RightArm.Position.Z = 0;
    
    World.LocalHumanoid.LeftLeg.Position.X = -0.5;
    World.LocalHumanoid.LeftLeg.Position.Y = 1;
    World.LocalHumanoid.LeftLeg.Position.Z = 0;
    
    World.LocalHumanoid.RightLeg.Position.X = 0.5;
    World.LocalHumanoid.RightLeg.Position.Y = 1;
    World.LocalHumanoid.RightLeg.Position.Z = 0;
    
    World.LocalHumanoid.Torso.Position.X = 0;
    World.LocalHumanoid.Torso.Position.Y = 3;
    World.LocalHumanoid.Torso.Position.Z = 0;
    
    World.LocalHumanoid.Head.Position.X = 0;
    World.LocalHumanoid.Head.Position.Y = 3;
    World.LocalHumanoid.Head.Position.Z = 0;

    World.Camera.Position.X = -0.174;
    World.Camera.Position.Y = 4.209;
    World.Camera.Position.Z = 1.85;
end