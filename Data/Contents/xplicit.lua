
Game.Vector3 = {}

func Game.Vector3:New(X, Y, Z)
    return { X = X, Y = Y, Z = Z }
end

Game.ColorRGB = {}

func Game.ColorRGB:New(R, G, B)
    return { Red = R, Green = G, Blue = B }
end

Game.HumanoidState = {}
Game.HumanoidState.ALIVE = 0
Game.HumanoidState.DEAD = 1
Game.HumanoidState.INVALID = 3

local func __addEvent(Tbl, Func)
    return table.insert(Tbl, { Func = Func })
end

local func __rmEvent(Tbl, Index)
    Tbl[Index] = nil;
end

Game.Slot = {
    Login = { 
        Connect = func(self, Func)
            return __addEvent(Game.Slot.Login, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    Logoff = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.Logoff, Func); 
        end,
        Disconnect = func(Index)
            __rmEvent(self, Index); 
        end 
    },
    LeftClick = { 
        Connect = func(self, Func)
            return __addEvent(Game.Slot.LeftClick, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    RightClick = { 
        Connect = func(self, Func)
            return __addEvent(Game.Slot.RightClick, Func); 
        end,
        Disconnect = func(Iself, ndex)
            __rmEvent(self, Index); 
        end 
    },
    MouseMove = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.MouseMove, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    LocalSpawn = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.LocalSpawn, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Move = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.Move, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Damage = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.Damage, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end  
    },
    Spawn = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.Spawn, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Death = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.Death, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
    Tick = { 
        Connect = func(self, Func)
        return __addEvent(Game.Slot.Death, Func); 
        end,
        Disconnect = func(self, Index)
            __rmEvent(self, Index); 
        end 
    },
}

Game.Counter = 0
Game.PlayerCount = 0

func Game:Login(id)
    Game.PlayerCount = Game.PlayerCount + 1

    for _, v in ipairs(Game.Slot.Login) do
        v.Func(id)
    end
end

func Game:Logoff(id)
    Game.PlayerCount = Game.PlayerCount - 1

    for _, v in ipairs(Game.Slot.Logoff) do
        v.Func(id)
    end
end

func Game:Tick()
    for _, v in ipairs(Game.Slot.Tick) do
        v.Func()
    end
end

func Game:LeftClick()
    for _, v in ipairs(Game.Slot.LeftClick) do
        v.Func()
    end
end

func Game:RightClick()
    for _, v in ipairs(Game.Slot.RightClick) do
        v.Func()
    end
end

func Game:MouseMove()
    for _, v in ipairs(Game.Slot.MouseMove) do
        v.Func()
    end
end

func Game:LocalSpawn()
    for _, v in ipairs(Game.Slot.LocalSpawn) do
        v.Func()
    end
end

func Game:Move(x, y, z)
    for _, v in ipairs(Game.Slot.Move) do
        v.Func(x, y, z)
    end
end

func Game:Damage()
    for _, v in ipairs(Game.Slot.Damage) do
        v.Func()
    end
end

func Game:Death()
    for _, v in ipairs(Game.Slot.Death) do
        v.Func()
    end
end

func Game:Spawn()
    for _, v in ipairs(Game.Slot.Spawn) do
        v.Func()
    end
end

Game.Name = "XplicitNgine"
Game.Description = "Xplicit Engine instance"
Game.Version = "1.1.3"

-- Components ID.
Game.INVALID = 0;
Game.SCRIPT = 1;
Game.SOUND = 2;
Game.PARTICLE = 3;
Game.ROXML = 4;
Game.TEXTURE = 5;
Game.TOOL = 6;
Game.SMOKE = 7;
Game.FORCEFIELD = 8;
Game.EXPLOSION = 9;
Game.SKYBOX = 10;
Game.UNLOAD = 11;
Game.LOAD = 12;
Game.COUNT = 13;
