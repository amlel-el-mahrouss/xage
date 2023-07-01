# Copyright Xplicit Corporation

Vector3 = {}

func Vector3:New(X, Y, Z)
    return { Name = "Vector3", Type = "Vector", X = X, Y = Y, Z = Z };
end

Color = {}

func Color:New(R, G, B)
    return { Name = "Color", Type = "Color", Red = R, Green = G, Blue = B };
end

Humanoid = {}

Humanoid.Alive = 0;
Humanoid.Dead = 1;