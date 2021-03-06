require("script/cocos/overload")

overload.createSprite {
    "class",
    "string",
    "table",
    function(parent, name, pos)
        local sprite = CCSprite:create(name)
        if sprite == nil then
            print("failed to load sprite from file: " .. name)
        else
            sprite:setPosition(pos)
            if tolua.isa(parent, "Widget") then
                parent:addNode(sprite)
            else
                parent:addChild(sprite)
            end
        end
        return sprite
    end
}

overload.createSprite {
    "class",
    "string",
    "table",
    "table",
    "number",
    "number",
    function(parent, name, pos, anchor, z, tag)
        local sprite = CCSprite:create(name)
        if sprite == nil then
            print("failed to load sprite from file: " .. name)
        else
            sprite:setPosition(pos)
            sprite:setAnchorPoint(cc.p(anchor.x, anchor.y))
            if tolua.isa(parent, "Widget") then
                parent:addNode(sprite, z, tag)
            else
                parent:addChild(sprite, z, tag)
            end
        end
        return sprite
    end
}

overload.createFrameSprite {
    "string",
    function(name)
        local sprite = CCSprite:createWithSpriteFrameName(name)
        if sprite == nil then
            print("failed to load sprite from frame: " .. name)
        end
        return sprite
    end
}

overload.createFrameSprite {
    "class",
    "string",
    "table",
    function(parent, name, pos)
        local sprite = CCSprite:createWithSpriteFrameName(name)
        if sprite == nil then
            print("failed to load sprite from frame: " .. name)
        else
            sprite:setPosition(pos)
            if tolua.isa(parent, "Widget") then
                parent:addNode(sprite)
            else
                parent:addChild(sprite)
            end
        end

        return sprite
    end
}

overload.createFrameSprite {
    "class",
    "string",
    "table",
    "table",
    "number",
    "number",
    function(parent, name, pos, anchor, z, tag)
        local sprite = CCSprite:createWithSpriteFrameName(name)
        if sprite == nil then
            print("failed to load sprite from frame: " .. name)
        else
            sprite:setPosition(pos)
            sprite:setAnchorPoint(cc.p(anchor.x, anchor.y))
            if tolua.isa(parent, "Widget") then
                parent:addNode(sprite, z, tag)
            else
                parent:addChild(sprite, z, tag)
            end
        end
        return sprite
    end
}