﻿#!/usr/bin/env python
# encoding: utf-8

import os
import sys
import json
import subprocess
import shutil

project_name = 'endian'


def run_command(args, env_ext={}):
    print("Running: {}".format(args))
    sys.stdout.flush()
    subprocess.check_call(args, env=dict(os.environ.copy(), **env_ext))


def get_tool_options(properties):
    options = []
    if 'tool_options' in properties:
        # Make sure that the values are correctly comma separated
        for key, value in properties['tool_options'].items():
            if value is None:
                options += ['--{0}'.format(key)]
            else:
                options += ['--{0}={1}'.format(key, value)]

    return options


def configure(properties):
    command = [sys.executable, 'waf']

    if properties.get('build_distclean'):
        command += ['distclean']

    command += ['configure', '--git_protocol=git@']

    if 'waf_resolve_path' in properties:
        command += ['--resolve_path=' + properties['waf_resolve_path']]

    if 'dependency_project' in properties:
        command += ['--{0}_checkout={1}'.format(
            properties['dependency_project'],
            properties['dependency_checkout'])]

    if 'cxx_mkspec' in properties:
        command += ["--cxx_mkspec={}".format(properties['cxx_mkspec'])]

    if 'nodebug' in properties:
        command += ["--cxx_nodebug"]

    command += get_tool_options(properties)

    run_command(command)


def build(properties):
    command = [sys.executable, 'waf', 'build', '-v']
    run_command(command)


def run_tests(properties):
    command = [sys.executable, 'waf', '-v', '--run_tests']
    run_cmd = None

    if properties.get('valgrind_run'):
        run_cmd = 'valgrind --error-exitcode=1 %s'

    if run_cmd:
        command += ["--run_cmd={}".format(run_cmd)]

    command += get_tool_options(properties)

    run_command(command)


def install(properties):
    command = [sys.executable, 'waf', '-v', 'install']

    if 'install_path' in properties:
        command += ['--install_path={0}'.format(properties['install_path'])]
    if properties.get('install_relative'):
        command += ['--install_relative']

    run_command(command)


def cmake(properties):
    build_path = 'build'
    if os.path.exists(build_path):
        print("Path '{}' already exists - removing".format(build_path))
        shutil.rmtree(build_path)
    os.mkdir(build_path)

    old_cwd = os.getcwd()
    os.chdir(build_path)
    cmd = ['cmake']

    if properties['is_release']:
        cmd.append('-DCMAKE_BUILD_TYPE=Release')
    cmd.append('../')
    run_command(cmd, env_ext={'VERBOSE': '1'})
    run_command(['cmake', '--build', '.'], env_ext={'VERBOSE': '1'})
    os.chdir(old_cwd)


def coverage_settings(options):
    options['required_line_coverage'] = 100.0


def main():
    argv = sys.argv

    if len(argv) != 3:
        print("Usage: {} <command> <properties>".format(argv[0]))
        sys.exit(0)

    cmd = argv[1]
    properties = json.loads(argv[2])

    if cmd == 'configure':
        configure(properties)
    elif cmd == 'build':
        build(properties)
    elif cmd == 'run_tests':
        run_tests(properties)
    elif cmd == 'install':
        install(properties)
    elif cmd == 'cmake':
        cmake(properties)
    else:
        print("Unknown command: {}".format(cmd))


if __name__ == '__main__':
    main()
