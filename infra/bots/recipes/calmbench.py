# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.


# Recipe module for Skia Swarming calmbench.

DEPS = [
  'core',
  'flavor',
  'recipe_engine/context',
  'recipe_engine/file',
  'recipe_engine/path',
  'recipe_engine/properties',
  'recipe_engine/python',
  'recipe_engine/raw_io',
  'recipe_engine/step',
  'recipe_engine/time',
  'run',
  'vars',
]

# TODO (liyuqian): Currently, this recipe combines both compile and nanobench
# functions. In the future, we may want to break it into two recipes, which
# would be useful for Android/iOS tests. To do that, I also have to add compile-
# only option to tools/calmbench/calmbench.py.
def RunSteps(api):
  api.core.setup()
  api.flavor.install(skps=True, svgs=True)
  api.flavor.compile("most")
  with api.context(cwd=api.vars.skia_dir):
    extra_arg = '--svgs %s --skps %s' % (api.flavor.device_dirs.svg_dir,
                                         api.flavor.device_dirs.skp_dir)

    # measuring multi-picture-draw in our multi-threaded CPU test is inaccurate
    if api.vars.builder_cfg.get('cpu_or_gpu') == 'CPU':
      extra_arg += ' --mpd false'
      config = "8888"
    else:
      config = "gl"

    command = [
        'python', 'tools/calmbench/calmbench.py', 'modified',
        '--config', config,
        '--ninjadir', api.vars.skia_out.join("Release"),
        '--extraarg', extra_arg,
        '--writedir', api.vars.swarming_out_dir,
        '--concise',
        '--githash', api.vars.got_revision,
    ]

    keys_blacklist = ['configuration', 'role', 'test_filter']
    command.append('--key')
    for k in sorted(api.vars.builder_cfg.keys()):
      if not k in keys_blacklist:
        command.extend([k, api.vars.builder_cfg[k]])

    api.run(api.step, 'Run calmbench', cmd=command)
  api.run.check_failure()

def GenTests(api):
  builders = [
    "Calmbench-Debian9-Clang-GCE-CPU-AVX2-x86_64-Release-All",
    "Calmbench-Ubuntu17-Clang-Golo-GPU-QuadroP400-x86_64-Release-All",
  ]

  for builder in builders:
    test = (
      api.test(builder) +
      api.properties(buildername=builder,
                     repository='https://skia.googlesource.com/skia.git',
                     revision='abc123',
                     path_config='kitchen',
                     swarm_out_dir='[SWARM_OUT_DIR]') +
      api.path.exists(
          api.path['start_dir'].join('skia'),
          api.path['start_dir'].join('skia', 'infra', 'bots', 'assets',
                                     'svg', 'VERSION'),
          api.path['start_dir'].join('skia', 'infra', 'bots', 'assets',
                                     'skp', 'VERSION'),
      )
    )

    yield test
