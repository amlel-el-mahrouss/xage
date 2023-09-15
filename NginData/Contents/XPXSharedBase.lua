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

function world:Login(ply)
    world.PlayerCount = world.PlayerCount + 1

    for _, v in pairs(world.Slots.Login.Slots) do
        v(ply)
    end
end

function world:Logoff(ply)
    world.PlayerCount = world.PlayerCount - 1

    for _, v in pairs(world.Slots.Logoff.Slots) do
        v(ply)
    end
end

function world:LeftClick()
    for _, v in pairs(world.Slots.LeftClick.Slots) do
        v()
    end
end

function world:RightClick()
    for _, v in pairs(world.Slots.RightClick.Slots) do
        v()
    end
end

function world:MouseMove()
    for _, v in pairs(world.Slots.MouseMove.Slots) do
        v()
    end
end

function world:LocalSpawn()
    for _, v in pairs(world.Slots.LocalSpawn.Slots) do
        v()
    end
end

function world:Move(ply)
    for _, v in pairs(world.Slots.Move.Slots) do
        v(ply)
    end
end

function world:LocalMove(x, y, z)
    for _, v in pairs(world.Slots.LocalMove.Slots) do
        v(x, y, z)
    end
end

function world:Damage(ply)
    for _, v in pairs(world.Slots.Damage.Slots) do
        v(ply)
    end
end

function world:Death(ply)
    for _, v in pairs(world.Slots.Death.Slots) do
        v(ply)
    end
end

function world:Spawn(ply)
    for _, v in pairs(world.Slots.Spawn.Slots) do
        v(ply)
    end
end

function world:RenderOneFrame()
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