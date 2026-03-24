import textwrap

# -- Project information -------------------------------------------------------

project = "mutiny"
copyright = "mutiny contributors"
author = "mutiny contributors"

# -- General configuration -----------------------------------------------------

extensions = [
    "breathe",
    "exhale",
    "myst_parser",
    "sphinx.ext.intersphinx",
    "sphinxcontrib.moderncmakedomain",
]

intersphinx_mapping = {
    "cmake": ("https://cmake.org/cmake/help/latest/", None),
}

# -- Breathe -------------------------------------------------------------------

# Exhale writes XML here (relative to confdir); must match exhale_args below.
breathe_projects = {"mutiny": "./doxyoutput/xml"}
breathe_default_project = "mutiny"
breathe_domain_by_extension = {"hpp": "cpp"}

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
    "exhaleDoxygenStdin": textwrap.dedent("""\
        INPUT                = ../include/mutiny
        RECURSIVE            = YES
        # Include C headers so that C-only macros (TEST_GROUP_SETUP,
        # TEST_GROUP_TEARDOWN, CHECK_EQUAL_INT, …) are available via Breathe.
        # Macros shared with the C++ headers (CHECK, FAIL, TEST, …) produce
        # Doxygen "multiple definitions" warnings; these are harmless — the C++
        # versions in Shell.hpp / test.hpp take precedence in the rendered docs.
        FILE_PATTERNS        = *.hpp *.h
        EXTRACT_ALL          = YES
        EXTRACT_PRIVATE      = NO
        EXTRACT_STATIC       = YES
        ENABLE_PREPROCESSING = YES
        MACRO_EXPANSION      = YES
        # Explicitly define capability macros so Doxygen processes the full
        # API surface without relying on compiler-detection macros
        # (e.g. __EXCEPTIONS, __GXX_RTTI) that it doesn't have.
        PREDEFINED           += MUTINY_HAVE_EXCEPTIONS=1 \\
                                MUTINY_HAVE_RTTI=1 \\
                                MUTINY_USE_STD_CPP_LIB=1 \\
                                MUTINY_USE_STD_STRING=0
        WARN_IF_UNDOCUMENTED = NO
        QUIET                = YES
        """),
}

# -- Options for HTML output ---------------------------------------------------

html_theme = "furo"
html_static_path = ["_static"]
html_title = "mutiny"
html_logo = "_static/logo.png"

# Macros shared between test.h (C API) and test.hpp/Shell.hpp (C++ API) produce
# duplicate c:macro declarations — Sphinx has no cpp:macro domain, so this is a
# known upstream limitation rather than a documentation error.
suppress_warnings = ["duplicate_declaration.c"]
