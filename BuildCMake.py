# -*- coding: utf-8 -*-

BT_PATH = "Program/BuildTool"

import sys
import os
sys.path.append(BT_PATH)
import BuildTool as BT

class Engine(BT.Module):
    NAME = "Engine"
    TYPE = BT.STATIC_LIB
    SOURCE = ["Engine"]


class Game(BT.Module):
    NAME = "Game"
    TYPE = BT.EXECUTABLE
    SOURCE = ["Program/Game/"]
    DEPS = ["Engine"]



Targets = [
    Engine(),
    Game(),
]

BT.Build(Targets)
os.system("cd Build && cmake ..")






