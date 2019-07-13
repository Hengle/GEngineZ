# -*- coding: utf-8 -*-

BT_PATH = "Program/BuildTool"

import sys
import os
sys.path.append(BT_PATH)
import BuildTool as BT

class Config(BT.Module):
    def __init__(self):
        super(Config, self).__init__("Config", BT.CUSTOM)
        self.SOURCE = ["Config"]


class Engine(BT.Module):
    def __init__(self):
        super(Engine, self).__init__("Engine", BT.STATIC_LIB)
        self.SOURCE = ["Engine"]


class Game(BT.Module):
    def __init__(self):
        super(Game, self).__init__("Game", BT.EXECUTABLE)
        self.SOURCE = ["Program/Game"]
        self.DEPS = ["Engine"]



Targets = [
    Config(),
    Engine(),
    Game(),
]

BT.Build(Targets)
os.system("cd Build && cmake ..")






