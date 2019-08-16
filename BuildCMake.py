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
        self.define["HLSLCC_DYNLIB"] = True
        self.define["COMPRESS_MESH_FILE"] = True



build_cfg = CMakeConfig()


# ================= Main Targets =================
# Config
class Config(BT.Module):
    def __init__(self):
        super(Config, self).__init__("Config", BT.CUSTOM)
        self.SOURCE = ["Config"]

# Engine
class Engine(BT.Module):
    def __init__(self):
        super(Engine, self).__init__("Engine", BT.STATIC_LIB)
        self.SOURCE = ["Engine"]
        self.excludes = [r".*\.hlsl"]
        self.DEPS = ["TPLib_Stb"]


# Program
class Game(BT.Module):
    def __init__(self):
        super(Game, self).__init__("Game", BT.EXECUTABLE)
        self.SOURCE = ["Program/Game"]
        self.DEPS = ["Engine", "zlib", "lua"]

class SLConverter(BT.Module):
    def __init__(self):
        super(SLConverter, self).__init__("SLConverter", BT.EXECUTABLE)
        self.SOURCE = ["Program/SLConverter"]
        self.DEPS = ["Engine", "hlslcc"]

class HLSLLint(BT.Module):
    def __init__(self):
        super(HLSLLint, self).__init__("HLSLLint", BT.EXECUTABLE)
        self.SOURCE = ["Program/HLSLLint"]
        self.DEPS = ["Engine"]

class MeshConverter(BT.Module):
    def __init__(self):
        super(MeshConverter, self).__init__("MeshConverter", BT.EXECUTABLE)
        self.SOURCE = ["Program/MeshConverter"]
        self.DEPS = ["Engine", "assimp-vc142-mt", "zlib"]

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
    MeshConverter(),
	HLSLLint(),
    # Tests
    TestSched(),

]


# ================= Generate =================
BT.Build(build_cfg, build_targets)

if not os.path.exists('Build'):
    os.mkdir('Build')
os.system("cd Build && cmake ..")






