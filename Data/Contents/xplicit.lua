
World.Vector3 = {}

func World.Vector3:New(X, Y, Z)
    return { X = X, Y = Y, Z = Z }
end

World.ColorRGB = {}

func World.ColorRGB:New(R, G, B)
    return { Red = R, Green = G, Blue = B }
end

World.HumanoidState = {}
World.HumanoidState.ALIVE = 0
World.HumanoidState.DEAD = 1
World.HumanoidState.INVALID = 3

local func __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
end

local func __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

World.Slot = {
    Login = { 
        Connect = func(self, Func)
            return __addEvent(World.Slot.Login, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Logoff = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.Logoff, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = func(self, Func)
            return __addEvent(World.Slot.LeftClick, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = func(self, Func)
            return __addEvent(World.Slot.RightClick, Func); 
        end,
        Disconnect = func(Iself, ndex)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.MouseMove, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.LocalSpawn, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.Move, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.Damage, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.Spawn, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.Death, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Tick = { 
        Connect = func(self, Func)
        return __addEvent(World.Slot.Tick, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
}

World.Counter = 0
World.PlayerCount = 0

func World:Login(id)
    World.PlayerCount = World.PlayerCount + 1

    for _, v in ipairs(World.Slot.Login) do
        v.Func(id)
    end
end

func World:Logoff(id)
    World.PlayerCount = World.PlayerCount - 1

    for _, v in ipairs(World.Slot.Logoff) do
        v.Func(id)
    end
end

func World:Tick()
    for _, v in ipairs(World.Slot.Tick) do
        v.Func()
    end
end

func World:LeftClick()
    for _, v in ipairs(World.Slot.LeftClick) do
        v.Func()
    end
end

func World:RightClick()
    for _, v in ipairs(World.Slot.RightClick) do
        v.Func()
    end
end

func World:MouseMove()
    for _, v in ipairs(World.Slot.MouseMove) do
        v.Func()
    end
end

func World:LocalSpawn()
    for _, v in ipairs(World.Slot.LocalSpawn) do
        v.Func()
    end
end

func World:Move(x, y, z)
    for _, v in ipairs(World.Slot.Move) do
        v.Func(x, y, z)
    end
end

func World:Damage()
    for _, v in ipairs(World.Slot.Damage) do
        v.Func()
    end
end

func World:Death()
    for _, v in ipairs(World.Slot.Death) do
        v.Func()
    end
end

func World:Spawn()
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
