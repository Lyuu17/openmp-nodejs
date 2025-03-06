include(FetchContent)
FetchContent_Declare(
  open.mp-sdk
  GIT_REPOSITORY https://github.com/openmultiplayer/open.mp-sdk.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(open.mp-sdk)
