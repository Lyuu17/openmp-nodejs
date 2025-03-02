include(FetchContent)
FetchContent_Declare(
  open.mp-sdk
  GIT_REPOSITORY https://github.com/Lyuu17/open.mp-sdk.git
  GIT_TAG        feat/object-streaming
)

FetchContent_MakeAvailable(open.mp-sdk)
