PROGRAM(checkpoint-validator)

SRCS(
    main.cpp
)

PEERDIR(
    cloud/blockstore/apps/client/lib

    cloud/blockstore/tools/testing/eternal_tests/checkpoint-validator/lib
)

END()

