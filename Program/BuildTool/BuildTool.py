# -*- coding: utf-8 -*-
import os
import re

import CMakeTemplate as T
# TODO
# 1. [x] support os related directory, only add in specific os
# 2. [o] feature cmake define
# 3. [x] third part lib manage
# 4. [x] sub BuildCMake.py include

STATIC_LIB = 0
DYNAMIC_LIB = 1
EXECUTABLE = 2
CUSTOM = 3

CUR_FILE_DIR = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR = os.getcwd()

class CMake(object):
    def __init__(self, project_name):
        self.define = {}
        self.project_name = project_name

    def custom_text(self):
        return ''


class Module(object):
    def __init__(self, name, tp):
        self.NAME = name
        self.TYPE = tp
        self.excludes = None
        self.vsfolder = None

class CMakeBuilder(object):
    def __init__(self):
        super(CMakeBuilder, self).__init__()
        self.output = []

        self.targets = {}

    def GenerateTargets(self, config, targets):
        self.targets = targets
        self.GenerateHeader(config)

        for target_name, info in targets.iteritems():
            if info.TYPE == STATIC_LIB or info.TYPE == DYNAMIC_LIB:
                self.GenerateLibaray(target_name)
            elif info.TYPE == EXECUTABLE:
                self.GenerateExecutable(target_name)
            elif info.TYPE == CUSTOM:
                self.GenerateCustom(target_name)
        self.Output()

    def GenerateLibaray(self, target_name):
        target = self.targets[target_name]
        output = T.LIBRARY_TEMPLATE
        output = output.replace("%LIB_NAME%", target.NAME)

        # include lib directory
        include_dirs = [os.path.dirname(name + '/') for name in target.SOURCE if os.path.isdir(name)]
        output = output.replace("%INCLUDE_LIB_DIR%", '.\n'.join(["include_directories(%s)"%s for s in include_dirs]))
        
        # groups && src
        groups, srcs = self.GetGroupAndSources(include_dirs, target.excludes)
       
        output = output.replace("%LIB_GROUPS%", '\n'.join(groups))
        output = output.replace("%LIB_SRCS%", ' '.join(srcs))

        if target.vsfolder:
            output += T.TARGET_FOLDER_TEMPLATE.replace("%NAME%", target.NAME).replace("%GROUP_KEY%", target.vsfolder)

        self.AppendOutput(output)

    def GenerateExecutable(self, target_name):
        target = self.targets[target_name]
        output = T.EXECUTABLE_TEMPLATE
        output = output.replace("%EXE_NAME%", target.NAME)

        # include directory
        include_dirs = [os.path.dirname(name + '/') for name in target.SOURCE if os.path.isdir(name)]
        output = output.replace("%INCLUDE_EXE_DIR%", '.\n'.join(['include_directories(%s)'%s for s in include_dirs]))
        
        # groups && src
        groups, srcs = self.GetGroupAndSources(include_dirs, target.excludes)
        single_srcs = [name for name in target.SOURCE if not os.path.isdir(name)]
        srcs.extend(single_srcs)
       
        output = output.replace("%EXE_GROUPS%", '\n'.join(groups))
        output = output.replace("%EXE_SRCS%", ' '.join(srcs))
        output = output.replace("%DEP_LIBS%", ' '.join(target.DEPS))

        if target.vsfolder:
            output += T.TARGET_FOLDER_TEMPLATE.replace("%NAME%", target.NAME).replace("%GROUP_KEY%", target.vsfolder)

        self.AppendOutput(output)

    def GenerateCustom(self, target_name):
        target = self.targets[target_name]
        output = T.CUSTOM_TEMPLATE
        output = output.replace("%CUSTOM_NAME%", target.NAME)
        # groups && src
        include_dirs = [os.path.dirname(name + '/') for name in target.SOURCE if os.path.isdir(name)]
        groups, srcs = self.GetGroupAndSources(include_dirs)
       
        output = output.replace("%CUSTOM_GROUPS%", '\n'.join(groups))
        output = output.replace("%CUSTOM_SRCS%", ' '.join(srcs))

        
        if target.vsfolder:
            output += T.TARGET_FOLDER_TEMPLATE.replace("%NAME%", target.NAME).replace("%GROUP_KEY%", target.vsfolder)

        self.AppendOutput(output)

    def GenerateHeader(self, config):
        output = T.HEADER_TEMPLATE
        def_strs = []
        for define, value in config.define.iteritems():
            if value is False:
                continue
            if value is True:
                def_strs.append(define)
            else:
                def_strs.append("%s=%s"%(define, str(value)))
        
        output = output.replace("%DEFINES%", '\n'.join([T.DEFINE_TEMPLATE.replace("%DEF%", s) for s in def_strs]))
        output = output.replace("%PROJECT_NAME%", config.project_name)
        output = output.replace("%CUSTOM_TXT%", config.custom_text())
        self.AppendOutput(output)

    def AppendOutput(self, output):
        self.output.extend([output])

    def Output(self):
        output = "\n".join(self.output)
        file("CMakeLists.txt", "w").write(output)

    def WalkDirs(self, walk_dirs, excludes=None):
        group_src = {}
        for walk_dir in walk_dirs:
            walk_dir = walk_dir.replace('\\', '/')
            for root, dirs, files in os.walk(walk_dir):
                if not files:
                    continue
                if excludes:
                    for ex in excludes:
                        files = [f for f in files if not re.match(ex, f)]
                root =  root.replace('\\', '/')
                group_src[(root[:len(walk_dir) + 1], root[len(walk_dir) + 1:])] = files
        return group_src

    def GetGroupAndSources(self, include_dirs, excludes=None):
        group_srcs = self.WalkDirs(include_dirs, excludes)
        groups_strs = []
        all_srcs = []
        for group, srcs in group_srcs.iteritems():
            group_p, group_s = group
            src_list = ['%s%s/%s'%(group_p, group_s, src) for src in srcs]
            group_name = (group_p + group_s).replace('/', '_') + '_GROUP_FILES'

            if len(group_s) > 0:
                s = T.SRC_GROUP_TEMPLATE.replace("%GRP_SRC_KEY%", group_name).replace("%GRP_SRC%", ' '.join(src_list)). \
                    replace("%GRP_KEY%", group_s.replace('/', '\\\\'))
                groups_strs.append(s)
            all_srcs.extend(src_list)
        return groups_strs, all_srcs


def Build(config, targets):
    targets = dict([(t.NAME, t) for t in targets])
    CMakeBuilder().GenerateTargets(config, targets)