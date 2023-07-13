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

function Plugin:Loaded(self)
    Plugin.UI.Toolbar = Plugin.UI:Create('Toolbar', {});
    Plugin.UI.Toolbar.File = Plugin.UI:Create('Button', Plugin.UI.Toolbar);
    
    Plugin.UI.Toolbar.Height = 41;
    Plugin.UI.Toolbar.Width = Plugin.GetWindowSize().X;
end

