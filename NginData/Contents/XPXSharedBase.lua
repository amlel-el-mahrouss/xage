local function __addEvent(Tbl, Func)
    Tbl.Slots[#Tbl.Slots + 1] = Func;
    return #Tbl.Slots - 1;
end

local function __rmEvent(Tbl, Index)
    Tbl.Slots[Index] = nil;
end

world.Events = {
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

function world:Login(ply)
    for _, v in pairs(world.Slots.Login.Slots) do
        v(ply)
    end
end

function world:Logoff(ply)
    for _, v in pairs(world.Events.Logoff.Slots) do
        v(ply)
    end
end

function world:LeftClick()
    for _, v in pairs(world.Events.LeftClick.Slots) do
        v()
    end
end

function world:RightClick()
    for _, v in pairs(world.Events.RightClick.Slots) do
        v()
    end
end

function world:MouseMove()
    for _, v in pairs(world.Events.MouseMove.Slots) do
        v()
    end
end

function world:LocalSpawn()
    for _, v in pairs(world.Events.LocalSpawn.Slots) do
        v()
    end
end

function world:Move(ply)
    for _, v in pairs(world.Events.Move.Slots) do
        v(ply)
    end
end

function world:LocalMove(x, y, z)
    for _, v in pairs(world.Events.LocalMove.Slots) do
        v(x, y, z)
    end
end

function world:Damage(ply)
    for _, v in pairs(world.Events.Damage.Slots) do
        v(ply)
    end
end

function world:Death(ply)
    for _, v in pairs(world.Events.Death.Slots) do
        v(ply)
    end
end

function world:Spawn(ply)
    for _, v in pairs(world.Events.Spawn.Slots) do
        v(ply)
    end
end

function world:RenderOneFrame()
    for _, v in pairs(world.Events.RenderOneFrame.Slots) do
        v()
    end
end

-- Replications enums
world.DOWNLOAD_NOTHING = -1;
world.DOWNLOAD_SCRIPT = 1;
world.DOWNLOAD_XMLSCENE = 2;

world.DOWNLOAD_TYPE_CREATE = 522;
world.DOWNLOAD_TYPE_REMOVE = 523;
world.DOWNLOAD_TYPE_UPDATE = 524;