-- Copyright XPX Corporation, all rights reserved.

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
