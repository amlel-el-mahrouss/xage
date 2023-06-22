# Copyright Xplicit Corporation
# All rights reserved.

Chat = Chat or {};

Color.Gold = Color.New(0xFF, 0xB8, 0x00);

func Chat:OnFriendJoin(name)
    _ChatPrint(Color.Gold, "Your friend " .. name .. " joined the game.");
end

func Chat:OnFriendLeave(name)
    _ChatPrint(Color.Gold, "Your friend " .. name .. " leaved the game.");
end

func Chat:OnSystemNotify(msg)
    _ChatPrint(Color.Gold, "[System] " .. msg);
end
