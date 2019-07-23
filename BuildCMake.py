# -*- coding: utf-8 -*-

BT_PATH = "Program/BuildTool"

import sys
import os
sys.path.append(BT_PATH)
import BuildTool as BT


# ================= CMake Config =================
class CMakeConfig(BT.CMake):
    def __init__(self):
        super(CMakeConfig, self).__init__("GameZ")
        self.define["CMAKE_HELLO"] = True
        
        self.define["CMAKE_HELLDDO"] = True


build_cfg = CMakeConfig()


# ================= Main Targets =================
class Config(BT.Module):
    def __init__(self):
        super(Config, self).__init__("Config", BT.CUSTOM)
        self.SOURCE = ["Config"]


class Engine(BT.Module):
    def __init__(self):
        super(Engine, self).__init__("Engine", BT.STATIC_LIB)
        self.SOURCE = ["Engine"]
        self.excludes = [r".*\.hlsl"]


class Game(BT.Module):
    def __init__(self):
        super(Game, self).__init__("Game", BT.EXECUTABLE)
        self.SOURCE = ["Program/Game"]
        self.DEPS = ["Engine"]


# ================= Tests =================
class TestSched(BT.Module):
    def __init__(self):
        super(TestSched, self).__init__("TestSched", BT.EXECUTABLE)
        self.SOURCE = ["Test/TestSched.cc"]
        self.DEPS = ["Engine"]
 

build_targets = [
    Config(),
    Engine(),
    Game(),
    # Tests
    TestSched(),
]


# ================= Generate =================
BT.Build(build_cfg, build_targets)
os.system("cd Build && cmake ..")






