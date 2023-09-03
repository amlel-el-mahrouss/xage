World.Instance = {}
World.InstanceWizard = World.InstanceWizard or {}

function Instance.new(name, ...)
    return World.InstanceWizard[name].__init(unpack(...));
end