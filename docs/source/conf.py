# Configuration file for the Sphinx documentation builder.

# TODO: UPDATE THIS FILE TO CUSTOMISE THE DOCUMENTATION.
# -- Project information
# Title can be changed in the index.rst file
# project = 'Name of the Tutorial'
copyright = '2026, National Computational Infrastructure'
author = 'NCI Training'

release = '0.1'
version = '0.1.0'

# -- General configuration

extensions = [
    # Full configuration list for myst_nb: https://myst-nb.readthedocs.io/en/latest/configuration.html
    "myst_nb", # for markdown and ipynb files 
    'sphinx.ext.duration',
    'sphinx.ext.doctest',
    'sphinx.ext.autodoc',
    # "sphinx.ext.viewcode",
    'sphinx.ext.autosummary',
    'sphinx.ext.intersphinx',
    "sphinx_copybutton",
    "sphinx_design",
    "sphinx_inline_tabs",

]
nb_execution_mode = "off"

# The imported OpenMP-C.ipynb is third-party content: it links to sibling
# source files, starts its headings below H1, and uses IPython magics
# (%%time, !cmd) that the C/ipython lexers cannot highlight. Suppress those
# (non-actionable) warning categories so they do not fail a strict build.
suppress_warnings = [
    "myst.xref_missing",
    "myst.header",
    "misc.highlighting_failure",
]

# UPDATE THIS IF YOU NEED TO RENDER MARKDOWN FILE TO DIFFERENT FLAVOR.
# render markdown format as ['commonmark', 'gfm', 'myst']
nb_render_markdown_format="myst"

intersphinx_mapping = {
    'python': ('https://docs.python.org/3/', None),
    'sphinx': ('https://www.sphinx-doc.org/en/master/', None),
    "pst": ("https://pydata-sphinx-theme.readthedocs.io/en/latest/", None)
}
intersphinx_disabled_domains = ['std']

templates_path = ['_templates']

# -- Options for HTML output

html_theme = 'sphinx_book_theme'
html_static_path = ["_static"]
html_css_files = ["custom.css"]
html_theme_options = {
    "path_to_docs": "docs/source",
    # TODO: UPDATE THIS TO THE WORKSHOP REPOSITORY URL OF THE DOCUMENTATION.
    "repository_url": "https://github.com/zora-wuw/test_template",
    "use_repository_button": True,
    "home_page_in_toc": True,
    "back_to_top_button": True,
    "logo": {
      "image_light": "_static/logo-light.png",
      "image_dark": "_static/logo-dark.png",
   },
#    Icon links are displayed in the sidebar
    "icon_links": [
        {
            "name": "NCI Documentation",
            "url": "https://opus.nci.org.au/spaces/Help/pages/12583138/NCI+Help",
            "icon": "fa-brands fa-confluence",
            "type": "fontawesome",
        },
        
        {
            "name": "Courses",
            "url": "https://nci900-training-organisation.github.io/learning-resources/courses.html",
            "icon": "fa-solid fa-graduation-cap",
            "type": "fontawesome",
        }
    ]
}

# -- Options for EPUB output
epub_show_urls = 'footnote'
