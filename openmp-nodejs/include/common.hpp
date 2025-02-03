
#pragma once

#include <filesystem>
#include <unordered_map>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#define NODE_WANT_INTERNALS 1
#define BUILDING_NODE_EXTENSION 1
#define NODE_SHARED_MODE 1

#include <node.h>
#include <node_platform.h>
#include <node_internals.h>
#include <env.h>
#include <v8.h>
#include <libplatform/libplatform.h>
