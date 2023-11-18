# Generated by devtools/yamaker (pypi).

PY3_LIBRARY()

VERSION(0.24.0)

LICENSE(Apache-2.0)

PEERDIR(
    contrib/python/PyYAML
    contrib/python/requests
    contrib/python/urllib3
)

NO_LINT()

PY_SRCS(
    TOP_LEVEL
    responses/__init__.py
    responses/_recorder.py
    responses/matchers.py
    responses/registries.py
)

RESOURCE_FILES(
    PREFIX contrib/python/responses/py3/
    .dist-info/METADATA
    .dist-info/top_level.txt
    responses/py.typed
)

END()
