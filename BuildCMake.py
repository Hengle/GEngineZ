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
# Config & Shader & Content
class Config(BT.Module):
    def __init__(self):
        super(Config, self).__init__("Config", BT.CUSTOM)
        self.SOURCE = ["Config"]

class Shader(BT.Module):
    def __init__(self):
        super(Shader, self).__init__("Shader", BT.CUSTOM)
        self.SOURCE = ["Shader"]

class Content(BT.Module):
    def __init__(self):
        super(Content, self).__init__("Content", BT.CUSTOM)
        self.SOURCE = ["Content"]

# Engine
class Engine(BT.Module):
    def __init__(self):
        super(Engine, self).__init__("Engine", BT.STATIC_LIB)
        self.SOURCE = ["Engine"]
        self.excludes = [r".*\.hlsl"]
        self.DEPS = ["TPLib_Stb", "imgui"]


# Program
class Game(BT.Module):
    def __init__(self):
        super(Game, self).__init__("Game", BT.EXECUTABLE)
        self.SOURCE = ["Program/Game"]
        self.DEPS = ["imgui", "Engine", "zlib", "lua"]
        self.vsfolder = "Program"

class SLConverter(BT.Module):
    def __init__(self):
        super(SLConverter, self).__init__("SLConverter", BT.EXECUTABLE)
        self.SOURCE = ["Program/SLConverter"]
        self.DEPS = ["Engine", "hlslcc"]
        self.vsfolder = "Program"

class HLSLLint(BT.Module):
    def __init__(self):
        super(HLSLLint, self).__init__("HLSLLint", BT.EXECUTABLE)
        self.SOURCE = ["Program/HLSLLint"]
        self.DEPS = ["Engine"]
        self.vsfolder = "Program"

class MeshConverter(BT.Module):
    def __init__(self):
        super(MeshConverter, self).__init__("MeshConverter", BT.EXECUTABLE)
        self.SOURCE = ["Program/MeshConverter"]
        self.DEPS = ["Engine", "assimp-${MSVC_PREFIX}-mt", "zlib"]
        self.vsfolder = "Program"

class BuildTool(BT.Module):
    def __init__(self):
        super(BuildTool, self).__init__("BuildTool", BT.CUSTOM)
        self.SOURCE = ["Program/BuildTool"]
        self.vsfolder = "Program"

# ================= Tests =================
class TestSched(BT.Module):
    def __init__(self):
        super(TestSched, self).__init__("TestSched", BT.EXECUTABLE)
        self.SOURCE = ["Test/TestSched.cc"]
        self.DEPS = ["Engine"]
        self.vsfolder = "Test"


 

# ================= ThirdParty =================



build_targets = [
    # ThirdParty
    # Main Targets
    Config(),
    Shader(),
    Content(),
    Engine(),
    # Progam
    Game(),
    SLConverter(),
    MeshConverter(),
	HLSLLint(),
    BuildTool(),
    # Tests
    TestSched(),

]


# ================= Generate =================
BT.Build(build_cfg, build_targets)

if not os.path.exists('Build'):
    os.mkdir('Build')
os.system("cd Build && cmake ..")






