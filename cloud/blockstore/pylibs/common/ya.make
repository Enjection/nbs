PY3_LIBRARY()

PY_SRCS(
    __init__.py
    helpers.py
    logger.py
    module_factories.py
    profiler.py
    results_processor_fs.py
    retry.py
    ssh.py
)

PEERDIR(
    contrib/python/urllib3

    cloud/blockstore/pylibs/clusters/test_config
)

IF (NOT OPENSOURCE)
    PEERDIR(
        contrib/python/paramiko     # TODO: NBS-4453
    )
ENDIF()

END()
