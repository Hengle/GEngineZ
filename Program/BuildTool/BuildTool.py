# -*- coding: utf-8 -*-
import os
import re


STATIC_LIB = 0
DYNAMIC_LIB = 1
EXECUTABLE = 2
CUSTOM = 3

CUR_FILE_DIR = os.path.dirname(os.path.realpath(__file__))
ROOT_DIR = os.getcwd()

class Module(object):
    def __init__(self, name, tp):
        self.NAME = name
        self.TYPE = tp


class CMakeBuilder(object):
    def __init__(self):
        super(CMakeBuilder, self).__init__()
        self.output = [""]

        self.template_header = file(os.path.join(CUR_FILE_DIR, "Templates", "Header.cmake.txt")).read()
        self.template_libaray = file(os.path.join(CUR_FILE_DIR, "Templates", "Libaray.cmake.txt")).read()
        self.template_executable = file(os.path.join(CUR_FILE_DIR, "Templates", "Executable.cmake.txt")).read()
        self.template_custom = file(os.path.join(CUR_FILE_DIR, "Templates", "Custom.cmake.txt")).read()

        self.targets = {}

    def GenerateTargets(self, targets):
        self.targets = targets
        self.GenerateHeader()

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
        output = self.template_libaray
        output = output.replace("%LIB_NAME%", target.NAME)

        # include lib directory
        include_dirs = [os.path.dirname(name + '/') for name in target.SOURCE if os.path.isdir(name)]
        output = output.replace("%INCLUDE_LIB_DIR%", '.\n'.join(['include_directories(%s)'%s for s in include_dirs]))
        
        # groups && src
        groups, srcs = self.GetGroupAndSources(include_dirs)
       
        output = output.replace("%LIB_GROUPS%", '\n'.join(groups))
        output = output.replace("%LIB_SRCS%", ' '.join(srcs))

        self.AppendOutput([output])

    def GenerateExecutable(self, target_name):
        target = self.targets[target_name]
        output = self.template_executable
        output = output.replace("%EXE_NAME%", target.NAME)

        # include lib directory
        include_dirs = [os.path.dirname(name + '/') for name in target.SOURCE if os.path.isdir(name)]
        output = output.replace("%INCLUDE_EXE_DIR%", '.\n'.join(['include_directories(%s)'%s for s in include_dirs]))
        
        # groups && src
        groups, srcs = self.GetGroupAndSources(include_dirs)
       
        output = output.replace("%EXE_GROUPS%", '\n'.join(groups))
        output = output.replace("%EXE_SRCS%", ' '.join(srcs))
        output = output.replace("%DEP_LIBS%", ' '.join(target.DEPS))

        self.AppendOutput([output])

    def GenerateCustom(self, target_name):
        target = self.targets[target_name]
        output = self.template_custom
        output = output.replace("%CUSTOM_NAME%", target.NAME)
        # groups && src
        include_dirs = [os.path.dirname(name + '/') for name in target.SOURCE if os.path.isdir(name)]
        groups, srcs = self.GetGroupAndSources(include_dirs)
       
        output = output.replace("%CUSTOM_GROUPS%", '\n'.join(groups))
        output = output.replace("%CUSTOM_SRCS%", ' '.join(srcs))

        self.AppendOutput([output])

    def GenerateHeader(self):
        self.AppendOutput([self.template_header])

    def AppendOutput(self, output):
        self.output.extend(output)

    def Output(self):
        output = "\n".join(self.output)
        file("CMakeLists.txt", "w").write(output)

    def WalkDirs(self, walk_dirs, exclude=None):
        group_src = {}
        for walk_dir in walk_dirs:
            walk_dir = walk_dir.replace('\\', '/')
            for root, dirs, files in os.walk(walk_dir):
                if not files:
                    continue
                root =  root.replace('\\', '/')
                group_src[(root[:len(walk_dir) + 1], root[len(walk_dir) + 1:])] = files
        return group_src

    def GetGroupAndSources(self, include_dirs):
        group_srcs = self.WalkDirs(include_dirs)
        groups_strs = []
        all_srcs = []
        for group, srcs in group_srcs.iteritems():
            group_p, group_s = group
            src_list = ['%s%s/%s'%(group_p, group_s, src) for src in srcs]
            group_name = (group_p + group_s).replace('/', '_') + '_GROUP_FILES'
            if len(group_s) > 0:
                groups_strs.append("set(%s %s)"%(group_name, ' '.join(src_list)))
                groups_strs.append("source_group(%s FILES ${%s})"% (group_s.replace('/', '\\\\'), group_name))
            all_srcs.extend(src_list)
        return groups_strs, all_srcs





def Build(targets):
    targets = dict([(t.NAME, t) for t in targets])
    CMakeBuilder().GenerateTargets(targets)