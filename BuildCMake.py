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
        self.DEPS = ["TPLib_Stb"]


class Game(BT.Module):
    def __init__(self):
        super(Game, self).__init__("Game", BT.EXECUTABLE)
        self.SOURCE = ["Program/Game"]
        self.DEPS = ["Engine"]

class SLConverter(BT.Module):
    def __init__(self):
        super(SLConverter, self).__init__("SLConverter", BT.EXECUTABLE)
        self.SOURCE = ["Program/SLConverter"]
        self.DEPS = ["Engine", "hlslcc"]

# ================= Tests =================
class TestSched(BT.Module):
    def __init__(self):
        super(TestSched, self).__init__("TestSched", BT.EXECUTABLE)
        self.SOURCE = ["Test/TestSched.cc"]
        self.DEPS = ["Engine"]
 

# ================= ThirdParty =================



build_targets = [
    # ThirdParty
    # Main Targets
    Config(),
    Engine(),
    # Progam
    Game(),
    SLConverter(),
    # Tests
    TestSched(),

]


# ================= Generate =================
BT.Build(build_cfg, build_targets)

if not os.path.exists('Build'):
    os.mkdir('Build')
os.system("cd Build && cmake ..")






