World.Gears = World.Gears or {}

World.InstanceWizard[Gear] = {
    __init = function(self, name)
        World.GearService.Create(Gears.GearComponent)
        World.Gear.ID = name;

        for _, v in ipairs(World) do
            if (v.ID == name) then
                __core_tbl = v;
                return;
            end
        end
    end,
    __core_tbl = {},
    IsPointingAt = function(self, pos)
        return self.__core_tbl.LookAt.X == pos.X and self.__core_tbl.LookAt.Y == pos.Y and
                self.__core_tbl.LookAt.X == pos.Z;
    end,
    Enabled = self.__core_tbl.Enabled,
    LookAt = self.__core_tbl.LookAt,
}