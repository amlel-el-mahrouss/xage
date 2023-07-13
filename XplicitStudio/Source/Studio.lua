Plugin.UI = {}

function Plugin.UI:Create(self, Name, Parent)
    return {
        Name = Name,
        Width = 800,
        Height = 600,
        X = 0,
        Y = 0,
        R = 0x1C,
        G = 0x1C,
        B = 0x1C,
        A = 255,
        Parent = Parent,
    };
end
