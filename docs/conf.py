import textwrap

# -- Project information -------------------------------------------------------

project = "mutiny"
copyright = "mutiny contributors"
author = "mutiny contributors"

# -- General configuration -----------------------------------------------------

extensions = [
    "breathe",
    "exhale",
    "sphinxcontrib.moderncmakedomain",
]

# -- Breathe -------------------------------------------------------------------

# Exhale writes XML here (relative to confdir); must match exhale_args below.
breathe_projects = {"mutiny": "./doxyoutput/xml"}
breathe_default_project = "mutiny"

# -- Exhale --------------------------------------------------------------------

exhale_args = {
    "containmentFolder": "./api",
    "rootFileName": "library_root.rst",
    "rootFileTitle": "API Reference",
    # Strip prefix from reported file paths in the API docs.
    "doxygenStripFromPath": "..",
    "createTreeView": False,
    # Let Exhale drive Doxygen.
    "exhaleExecutesDoxygen": True,
    # Only specify INPUT and extraction settings — Exhale manages all output
    # settings (GENERATE_XML, XML_OUTPUT, GENERATE_HTML, etc.) itself.
    "exhaleDoxygenStdin": textwrap.dedent(
        """\
        INPUT                = ../include/mutiny
        RECURSIVE            = YES
        # Only C++ headers — the C interface (test.h, mock.h) is documented in
        # testing-in-c.rst.  Including them causes duplicate macro warnings
        # because CHECK/FAIL are also defined in Shell.hpp.
        FILE_PATTERNS        = *.hpp
        EXTRACT_ALL          = YES
        EXTRACT_PRIVATE      = NO
        EXTRACT_STATIC       = YES
        ENABLE_PREPROCESSING = YES
        MACRO_EXPANSION      = YES
        # Explicitly define capability macros so Doxygen processes the full
        # API surface without relying on compiler-detection macros
        # (e.g. __EXCEPTIONS, __GXX_RTTI) that it doesn't have.
        PREDEFINED           = MUTINY_HAVE_EXCEPTIONS=1 \\
                               MUTINY_HAVE_RTTI=1 \\
                               MUTINY_USE_STD_CPP_LIB=1 \\
                               MUTINY_USE_STD_STRING=0
        WARN_IF_UNDOCUMENTED = NO
        QUIET                = YES
        """
    ),
}

# -- Options for HTML output ---------------------------------------------------

html_theme = "furo"
html_static_path = ["_static"]
html_title = "mutiny"
html_logo = "_static/logo.png"
