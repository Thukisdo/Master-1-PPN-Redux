
message(STATUS "Fetching external dependencies")
include(FetchContent)

# Fetch spdlog
message(STATUS "Fetching spdlog")
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.10.0
)
FetchContent_MakeAvailable(spdlog)

# Fetch zlib
message(STATUS "Fetching zlib")
FetchContent_Declare(
    zlib
    GIT_REPOSITORY "https://github.com/madler/zlib.git"
    GIT_TAG "v1.2.11"
)
FetchContent_GetProperties(zlib)

# Fetch lbspng
message(STATUS "Fetching libspng")
FetchContent_Declare(
        libspng
        GIT_REPOSITORY https://github.com/randy408/libspng.git
        GIT_TAG 75c39ce0948d3b9c623a674ebfeb63a703a7c116
)
FetchContent_MakeAvailable(libspng)

# Fetch lbconfig
message(STATUS "Fetching libconfig")
FetchContent_Declare(
        libconfig
        GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
        GIT_TAG 525922a6b49ca19235cdf307376e7c9c539d1b93
)
FetchContent_MakeAvailable(libconfig)

add_subdirectory(stb)