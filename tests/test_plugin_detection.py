#!/usr/bin/env python
#
# Copyright Contributors to the OpenTimelineIO project
#
# Licensed under the Apache License, Version 2.0 (the "Apache License")
# with the following modification; you may not use this file except in
# compliance with the Apache License and the following modification to it:
# Section 6. Trademarks. is deleted and replaced with:
#
# 6. Trademarks. This License does not grant permission to use the trade
#    names, trademarks, service marks, or product names of the Licensor
#    and its affiliates, except as required to comply with Section 4(c) of
#    the License and to reproduce the content of the NOTICE file.
#
# You may obtain a copy of the Apache License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the Apache License with the above modification is
# distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the Apache License for the specific
# language governing permissions and limitations under the Apache License.
#
import unittest
import os
import pkg_resources
import sys

try:
    # Python 3.3 forward includes the mock module
    from unittest import mock
    could_import_mock = True
except ImportError:
    # Fallback for older python (not included in standard library)
    try:
        import mock
        could_import_mock = True
    except ImportError:
        # Mock appears to not be installed
        could_import_mock = False

try:
    # Python3:  use importlib.reload
    from importlib import reload as import_reload
except ImportError:
    # Python2:
    from imp import reload as import_reload

import opentimelineio as otio
from tests import baseline_reader


@unittest.skipIf(
    not could_import_mock,
    "mock module not found. Install mock from pypi or use python >= 3.3."
)
class TestSetuptoolsPlugin(unittest.TestCase):
    def setUp(self):
        # Get the location of the mock plugin module metadata
        mock_module_path = os.path.join(
            baseline_reader.path_to_baseline_directory(),
            'plugin_module',
        )
        self.mock_module_manifest_path = os.path.join(
            mock_module_path,
            "otio_jsonplugin",
            "plugin_manifest.json"
        )

        # Create a WorkingSet as if the module were installed
        entries = [mock_module_path] + pkg_resources.working_set.entries

        self.sys_patch = mock.patch('sys.path', entries)
        self.sys_patch.start()

        working_set = pkg_resources.WorkingSet(entries)

        # linker from the entry point
        self.entry_patcher = mock.patch(
            'pkg_resources.iter_entry_points',
            working_set.iter_entry_points
        )
        self.entry_patcher.start()

    def tearDown(self):
        self.sys_patch.stop()
        self.entry_patcher.stop()
        if 'otio_mockplugin' in sys.modules:
            del(sys.modules['otio_mockplugin'])

    def test_detect_plugin(self):
        """This manifest uses the plugin_manifest function"""

        # Create a manifest and ensure it detected the mock adapter and linker
        man = otio.plugins.manifest.load_manifest()

        # Make sure the adapter is included in the adapter list
        adapter_names = [adapter.name for adapter in man.adapters]
        self.assertIn('mock_adapter', adapter_names)

        # Make sure the linker is included in the linker list
        linker_names = [linker.name for linker in man.media_linkers]
        self.assertIn('mock_linker', linker_names)

        # Make sure adapters and linkers landed in the proper place
        for adapter in man.adapters:
            self.assertIsInstance(adapter, otio.adapters.Adapter)

        for linker in man.media_linkers:
            self.assertIsInstance(linker, otio.media_linker.MediaLinker)

    def test_pkg_resources_disabled(self):
        os.environ["OTIO_DISABLE_PKG_RESOURCE_PLUGINS"] = "1"
        import_reload(otio.plugins.manifest)

        # detection of the environment variable happens on import, force a
        # reload to ensure that it is triggered
        with self.assertRaises(AssertionError):
            self.test_detect_plugin()

        # remove the environment variable and reload again for usage in the
        # other tests
        del os.environ["OTIO_DISABLE_PKG_RESOURCE_PLUGINS"]
        import_reload(otio.plugins.manifest)

    def test_detect_plugin_json_manifest(self):
        # Test detecting a plugin that rather than exposing the plugin_manifest
        # function, just simply has a plugin_manifest.json provided at the
        # package top level.
        man = otio.plugins.manifest.load_manifest()

        # Make sure the adapter is included in the adapter list
        adapter_names = [adapter.name for adapter in man.adapters]
        self.assertIn('mock_adapter_json', adapter_names)

        # Make sure the linker is included in the linker list
        linker_names = [linker.name for linker in man.media_linkers]
        self.assertIn('mock_linker_json', linker_names)

        # Make sure adapters and linkers landed in the proper place
        for adapter in man.adapters:
            self.assertIsInstance(adapter, otio.adapters.Adapter)

        for linker in man.media_linkers:
            self.assertIsInstance(linker, otio.media_linker.MediaLinker)

        self.assertTrue(
            any(
                True for p in man.source_files
                if self.mock_module_manifest_path in p
            )
        )

    def test_deduplicate_env_variable_paths(self):
        "Ensure that duplicate entries in the environment variable are ignored"

        # back up existing manifest
        bak_env = os.environ.get('OTIO_PLUGIN_MANIFEST_PATH')

        relative_path = self.mock_module_manifest_path.replace(os.getcwd(), '.')

        # set where to find the new manifest
        os.environ['OTIO_PLUGIN_MANIFEST_PATH'] = os.pathsep.join(
            (
                # absolute
                self.mock_module_manifest_path,

                # relative
                relative_path
            )
        )

        result = otio.plugins.manifest.load_manifest()
        self.assertEqual(
            len(
                [
                    p for p in result.source_files
                    if self.mock_module_manifest_path in p
                ]
            ),
            1
        )
        if relative_path != self.mock_module_manifest_path:
            self.assertNotIn(relative_path, result.source_files)

        if bak_env:
            os.environ['OTIO_PLUGIN_MANIFEST_PATH'] = bak_env
        else:
            del os.environ['OTIO_PLUGIN_MANIFEST_PATH']


if __name__ == '__main__':
    unittest.main()
