include(FetchContent)
FetchContent_Declare(
  open.mp-sdk
  GIT_REPOSITORY https://github.com/openmultiplayer/open.mp-sdk.git
  GIT_TAG        7b4918eeaa3a47ab4fbcc8151e2a6088f550e970
)

FetchContent_MakeAvailable(open.mp-sdk)
