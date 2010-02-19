--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
purple = gr.material({1.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
yellow = gr.material({1.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
orange = gr.material({0.3, 0.7, 0.5}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

-- Torso node
torsoNode = gr.node('torsoNode')
rootnode:add_child(torsoNode)

-- Construct torso
torso = gr.sphere('torso')
torsoNode:add_child(torso)
torso:scale(1.25, 2.25, 1)
torso:set_material(red)

-- Shoulders node
shouldersNode = gr.node('shouldersNode')
torsoNode:add_child(shouldersNode)

-- Construct shoulders
shoulders = gr.sphere('shoulders')
shouldersNode:add_child(shoulders)
shoulders:scale(2.35, 0.5, 1)
shoulders:set_material(blue)

-- Left upper Arm node
leftUpperArmNode = gr.node('leftUpperArmNode')
shouldersNode:add_child(leftUpperArmNode)

-- Construct left upper arm
leftUpperArm = gr.sphere('leftUpperArm')
leftUpperArmNode:add_child(leftUpperArm)
leftUpperArm:scale(0.4, 1.0, 0.4)
leftUpperArm:set_material(green)

-- Left forearm node
leftForearmNode = gr.node('leftForearmNode')
leftUpperArmNode:add_child(leftForearmNode)

-- Construct left forearm
leftForearm = gr.sphere('leftForearm')
leftForearmNode:add_child(leftForearm)
leftForearm:scale(0.2, 0.9, 0.2)
leftForearm:set_material(yellow)

-- Left Hand node
leftHandNode = gr.node('leftHand')
leftForearmNode:add_child(leftHandNode)

-- Construct left hand
leftHand = gr.sphere('leftHand')
leftHandNode:add_child(leftHand)
leftHand:scale(0.1, 0.1, 0.1)
leftHand:set_material(orange)


-- Right upper Arm node
rightUpperArmNode = gr.node('rightUpperArmNode')
shouldersNode:add_child(rightUpperArmNode)

-- Construct Right upper arm
rightUpperArm = gr.sphere('rightUpperArm')
rightUpperArmNode:add_child(rightUpperArm)
rightUpperArm:scale(0.4, 1.0, 0.4)
rightUpperArm:set_material(green)

-- Right forearm node
rightForearmNode = gr.node('rightForearmNode')
rightUpperArmNode:add_child(rightForearmNode)

-- Construct Right forearm
rightForearm = gr.sphere('rightForearm')
rightForearmNode:add_child(rightForearm)
rightForearm:scale(0.2, 0.9, 0.2)
rightForearm:set_material(yellow)

-- Right Hand node
rightHandNode = gr.node('rightHand')
rightForearmNode:add_child(rightHandNode)

-- Construct Right hand
rightHand = gr.sphere('rightHand')
rightHandNode:add_child(rightHand)
rightHand:scale(0.1, 0.1, 0.1)
rightHand:set_material(orange)

-- Neck Node
neckNode = gr.node('neckNode')
shouldersNode:add_child(neckNode)

-- Construct neck
neck = gr.sphere('neck')
neckNode:add_child(neck)
neck:scale(0.25, 0.4, 0.25)
neck:set_material(white)

-- Head Node
headNode = gr.node('headNode')
neckNode:add_child(headNode)

-- Construct Head
head = gr.sphere('head')
headNode:add_child(head)
head:scale(0.6, 0.6, 0.6)
head:set_material(yellow)

-- node transformations
neckNode:translate(0, 0.6, 0)
headNode:translate(0, 0.6, 0)
rightHandNode:translate(0, -1, 0)
rightForearmNode:translate(0, -1, 0)
rightUpperArmNode:translate(2.0, -1, 0.0)
leftHandNode:translate(0, -1, 0)
leftForearmNode:translate(0, -1, 0)
leftUpperArmNode:translate(-2.0, -1, 0.0)
shouldersNode:translate(0, 1.7, 0.0)
rootnode:translate(0.0, 0.0, -10.0)


--rootnode:rotate('y', -20.0)

return rootnode
