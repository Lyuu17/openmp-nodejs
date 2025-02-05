

constexpr auto bootstrapjs = ""
                             "const path = require('path');"
                             "const dns = require('dns');"
                             "const url = require('url');"
                             ""
                             "/* ***** Utils */"
                             ""
                             "/* Supresses the warning from NodeJS when importing 'super-internal' modules,"
                             "   that the embedder isn't supposed to use */"
                             "function internalRequire(id) {"
                             "    const __emitWarning = process.emitWarning;"
                             "    process.emitWarning = () => {};"
                             "    const result = require(id);"
                             "    process.emitWarning = __emitWarning;"
                             "    return result;"
                             "}"
                             ""
                             "const { getOrInitializeCascadedLoader } = internalRequire('internal/modules/esm/loader');"
                             "(async () => {"
                             "    const esmLoader = getOrInitializeCascadedLoader();"
                             ""
                             "    /* We need this handler so that NodeJS doesn't"
                             "       crash the process on oncaught exceptions*/"
                             "    process.on('uncaughtException', (err) => {"
                             "        console.error(`Uncaught exception: ${err.stack ? `${err.stack}` : `${err.message}`}`);"
                             "    });"
                             ""
                             "    /* Allows users to use 'localhost' address instead of 127.0.0.1 for tcp connections (e.g. database)"
                             "       https://github.com/nodejs/node/issues/40702#issuecomment-958157082 */"
                             "    dns.setDefaultResultOrder('ipv4first');"
                             ""
                             "    "
                             "    try {"
                             "        /* Get the path to the main file for this resource, and load it */"
                             "        const _path = path.resolve(resource.path, resource.main);"
                             "        await esmLoader.import(url.pathToFileURL(_path).toString(), '', {});"
                             "    } catch (e) {"
                             "        console.error(e);"
                             "    }"
                             "})();"
                             "__resourceLoaded();";
