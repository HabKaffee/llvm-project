import os
import lit.formats

config.name = "Lab1PluginTests"
config.test_format = lit.formats.ShTest(True)
config.suffixes = [".cpp"]
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = config.test_source_root
