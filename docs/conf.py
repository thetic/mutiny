import os
import re
import textwrap
import xml.etree.ElementTree as ET
from pathlib import Path

# -- Project information -------------------------------------------------------

project = "mutiny"
copyright = "2026, Chad Condon"
author = "Chad Condon"
language = "en"

version = Path("../cfg/VERSION").read_text().strip()
release = version

# -- General configuration -----------------------------------------------------

exclude_patterns = [".venv"]

extensions = [
    "breathe",
    "exhale",
    "myst_parser",
    "sphinx.ext.extlinks",
    "sphinx.ext.intersphinx",
    "sphinx.ext.linkcode",
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
    # Furo does not support .. contents:: directives — suppress them.
    "contentsDirectives": False,
    # Let Exhale drive Doxygen.
    "exhaleExecutesDoxygen": True,
    # Only specify INPUT and extraction settings — Exhale manages all output
    # settings (GENERATE_XML, XML_OUTPUT, GENERATE_HTML, etc.) itself.
    "exhaleDoxygenStdin": textwrap.dedent("""\
        INPUT                = ../include/mu/tiny
        RECURSIVE            = YES
        # Include C headers so that C-only macros (TEST_GROUP_SETUP,
        # TEST_GROUP_TEARDOWN, CHECK_EQUAL_INT, …) are available via Breathe.
        # Macros shared with the C++ headers (CHECK, FAIL_TEST, TEST, …) produce
        # Doxygen "multiple definitions" warnings; these are harmless — the C++
        # versions in Shell.hpp / test.hpp take precedence in the rendered docs.
        FILE_PATTERNS        = *.hpp *.h
        EXTRACT_ALL          = YES
        EXTRACT_PRIVATE      = NO
        EXTRACT_STATIC       = YES
        ENABLE_PREPROCESSING = YES
        MACRO_EXPANSION      = YES
        INCLUDE_PATH         = ../include
        PREDEFINED           += MUTINY_EXPORT=
        WARN_IF_UNDOCUMENTED = NO
        WARN_AS_ERROR        = YES
        QUIET                = YES
        # Omit <programlisting> from XML so Exhale skips generating
        # program_listing_*.rst files; GitHub source links replace them.
        XML_PROGRAMLISTING   = NO
        """),
}

# -- GitHub source links -------------------------------------------------------

_github_root = "https://github.com/thetic/mutiny"
_github_branch = os.environ.get("DOCS_GITHUB_REF", "main")

# :source:`filename <path/to/file>` → link into the GitHub tree
extlinks = {
    "source": (f"{_github_root}/tree/{_github_branch}/%s", "%s"),
}

# -- Options for HTML output ---------------------------------------------------

html_theme = "furo"
html_static_path = ["_static"]
templates_path = ["_templates"]
html_title = "mutiny"
html_logo = "_static/logo.png"
html_theme_options = {
    "source_repository": _github_root,
    "source_branch": _github_branch,
    "source_directory": "docs/",
    "footer_icons": [
        {
            "name": "GitHub",
            "url": _github_root,
            "html": '<svg stroke="currentColor" fill="currentColor" stroke-width="0" viewBox="0 0 16 16"><path fill-rule="evenodd" d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.013 8.013 0 0 0 16 8c0-4.42-3.58-8-8-8z"></path></svg>',
            "class": "",
        },
    ],
}

html_sidebars = {
    "**": [
        "sidebar/brand.html",
        "sidebar/version-switcher.html",
        "sidebar/search.html",
        "sidebar/scroll-start.html",
        "sidebar/navigation.html",
        "sidebar/scroll-end.html",
    ],
}

pygments_style = "a11y-high-contrast-light"
pygments_dark_style = "a11y-high-contrast-dark"

# Macros shared between test.h (C API) and test.hpp/Shell.hpp (C++ API) produce
# duplicate c:macro declarations — Sphinx has no cpp:macro domain, so this is a
# known upstream limitation rather than a documentation error.
suppress_warnings = ["duplicate_declaration.c"]

# -- sphinx.ext.linkcode ------------------------------------------------------


def _build_doxygen_index():
    """Parse Doxygen XML output into a {id_or_name: (filepath, lineno)} map.

    Keys stored per entry:
    - compounddef@id   (e.g. classmu_1_1tiny_1_1test_1_1Shell)
    - compoundname     (e.g. mu::tiny::test::Shell)
    - memberdef@id     (e.g. classmu_1_1tiny_1_1test_1_1Shell_1a70e3...)
    - qualifiedname    (e.g. mu::tiny::test::Shell::run)
    - name             (bare macro name, fallback for C defines)

    Breathe inserts both the Doxygen refid and the Sphinx-style qualified name
    into signode['ids'], so any of these lookups can match.
    """
    xml_dir = Path(__file__).parent / "doxyoutput" / "xml"
    index = {}
    skip = {"index.xml", "Doxyfile.xml", "index.xsd", "compound.xsd"}

    for xml_file in sorted(xml_dir.glob("*.xml")):
        if xml_file.name in skip:
            continue
        try:
            root = ET.parse(xml_file).getroot()
        except ET.ParseError:
            continue

        for compound in root.iter("compounddef"):
            compound_id = compound.get("id", "")
            cname = compound.findtext("compoundname") or ""
            loc = compound.find("location")
            if loc is not None:
                f, ln = loc.get("file"), loc.get("line")
                if f and ln:
                    entry = (f, int(ln))
                    if compound_id:
                        index.setdefault(compound_id, entry)
                    if cname:
                        index.setdefault(cname, entry)

            for member in compound.iter("memberdef"):
                member_id = member.get("id", "")
                name = member.findtext("name") or ""
                qname = member.findtext("qualifiedname") or name
                loc = member.find("location")
                if loc is None:
                    continue
                f, ln = loc.get("file"), loc.get("line")
                if not (f and ln):
                    continue
                entry = (f, int(ln))
                if member_id:
                    index.setdefault(member_id, entry)
                if qname:
                    index.setdefault(qname, entry)
                if name and name != qname:
                    index.setdefault(name, entry)

    return index


_doxygen_index = None


def linkcode_resolve(domain, info):
    """Return a GitHub blob URL for a C/C++ symbol, or None to suppress the link."""
    if domain not in ("cpp", "c"):
        return None
    # With add_linkcode_domain overrides in setup(), info['ids'] is a list of
    # all signode IDs: Doxygen refids, Sphinx C++ qualified names, etc.
    ids = info.get("ids") or []
    if isinstance(ids, str):
        ids = [ids]
    if not ids:
        return None

    global _doxygen_index
    if _doxygen_index is None:
        _doxygen_index = _build_doxygen_index()

    for node_id in ids:
        if not node_id:
            continue
        # Skip mangled Sphinx ABI IDs (_CPPv4...) — not present in our index
        if node_id.startswith("_"):
            continue
        entry = _doxygen_index.get(node_id)
        if entry:
            file_path, line = entry
            return f"{_github_root}/blob/{_github_branch}/{file_path}#L{line}"
        # Sphinx C domain prefixes macro names with "c." → try the bare name
        if node_id.startswith("c."):
            entry = _doxygen_index.get(node_id[2:])
            if entry:
                file_path, line = entry
                return f"{_github_root}/blob/{_github_branch}/{file_path}#L{line}"
    return None


# -- GitHub source links for generated file pages -----------------------------


# Inject a "View source on GitHub" link into each Exhale-generated file page.
# These pages previously linked to program_listing_*.rst (local code dumps);
# XML_PROGRAMLISTING = NO in exhaleDoxygenStdin suppresses those, and this
# handler replaces them with direct links into the GitHub source tree.
def _add_github_source_links(app, docname, source):
    if not docname.startswith("api/file_"):
        return
    m = re.search(r"Definition \(``(.+?)``\)", source[0])
    if not m:
        return
    path = m.group(1)
    link = f"\n:source:`View source on GitHub <{path}>`\n"
    source[0] = re.sub(
        r"(Definition \(``[^`]+``\)\n-+\n)",
        r"\1" + link,
        source[0],
    )


def setup(app):
    # Switch linkcode from signode['names'] (always empty in Breathe) to
    # signode['ids'], which contains both Doxygen refids and Sphinx qualified
    # names.  Must be called after sphinx.ext.linkcode has been loaded.
    from sphinx.ext.linkcode import add_linkcode_domain

    add_linkcode_domain("cpp", ["ids"], override=True)
    add_linkcode_domain("c", ["ids"], override=True)
    app.connect("source-read", _add_github_source_links)
