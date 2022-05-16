#include <Arduino.h>

#include "attack.hpp"
#include "bitboardTools.hpp"
#include "cli.hpp"
#include "com.hpp"
#include "definition.hpp"
#include "distributed.h"
#include "dynamicConfig.hpp"
#include "egt.hpp"
#include "evalConfig.hpp"
#include "hash.hpp"
#include "kpk.hpp"
#include "logging.hpp"
#include "material.hpp"
#include "nnue.hpp"
#include "option.hpp"
#include "pgnparser.hpp"
#include "searchConfig.hpp"
#include "searcher.hpp"
#include "testSuite.hpp"
#include "evaluationTuning.hpp"
#include "threading.hpp"
#include "timeMan.hpp"
#include "timers.hpp"
#include "tools.hpp"
#include "transposition.hpp"
#include "uci.hpp"
#include "xboard.hpp"

// Initialize all the things that should be ...
void init(int argc, char** argv) {
   Distributed::init();
   Logging::hellooo();
   Options::initOptions(argc, argv);
   Logging::init(); // after reading options
   Zobrist::initHash();
   TT::initTable();
   BBTools::initMask();
#ifdef WITH_MAGIC
   BBTools::MagicBB::initMagic();
#endif
   KPK::init();
   MaterialHash::MaterialHashInitializer::init();
   EvalConfig::initEval();
   ThreadPool::instance().setup();
   Distributed::lateInit();
   COM::init(COM::p_uci); // let's do this ... (usefull to reset position in case of NNUE)
}

void finalize() {
   Logging::LogIt(Logging::logInfo) << "Stopping all threads";
   ThreadPool::instance().stop();
   Logging::LogIt(Logging::logInfo) << "Waiting all threads";
   ThreadPool::instance().wait(); ///@todo is this necessary ?
   Logging::LogIt(Logging::logInfo) << "Deleting all threads";
   ThreadPool::instance().resize(0);
   Logging::LogIt(Logging::logInfo) << "Syncing process...";
   Distributed::sync(Distributed::_commInput, __PRETTY_FUNCTION__);
   Logging::LogIt(Logging::logInfo) << "Bye bye...";
   Logging::finalize();
   Logging::LogIt(Logging::logInfo) << "See you soon...";
   Distributed::finalize();
}

void setup() {
   init(0, nullptr);

#ifdef WITH_UCI // UCI is prefered option
   UCI::init();
   UCI::uci();
#endif // WITH_UCI
#ifdef WITH_XBOARD
   XBoard::init();
   XBoard::xboard();
#endif // WITH_XBOARD
}

void loop() {
}