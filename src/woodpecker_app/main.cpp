// Copyright © 2021 Jesse Stricker
//
// This file is part of Woodpecker.
//
// Woodpecker is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Woodpecker is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Woodpecker.  If not, see <https://www.gnu.org/licenses/>.

#include <QApplication>
#include <QtGlobal>
#include <spdlog/spdlog.h>
#include <woodpecker/config.hpp>

#include "main_window.hpp"

namespace wdp::app {
  void qt_msg_handler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg) {
    switch (type) {
      case QtDebugMsg:
        spdlog::debug("[QT] {}", msg.toUtf8().constData());
        break;
      case QtInfoMsg:
        spdlog::info("[QT] {}", msg.toUtf8().constData());
        break;
      case QtWarningMsg:
        spdlog::warn("[QT] [{}:{}] {}", ctx.file, ctx.line, msg.toUtf8().constData());
        break;
      case QtCriticalMsg:
        spdlog::error("[QT] [{}:{}] {}", ctx.file, ctx.line, msg.toUtf8().constData());
        break;
      case QtFatalMsg:
        try {
          spdlog::critical("[QT] [{}:{}] {}", ctx.file, ctx.line, msg.toUtf8().constData());
        } catch (...) {
          // ignore
        }
        std::abort();
    }
  }

  int main(int argc, char* argv[]) {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    qInstallMessageHandler(&qt_msg_handler);
    spdlog::info("{} v{} by {}", project_name, project_version, project_author);

    const auto app = QApplication{argc, argv};
    auto window = MainWindow{};
    window.show();
    return QApplication::exec();
  }
}

int main(int argc, char* argv[]) {
  try {
    return wdp::app::main(argc, argv);
  } catch (const std::exception& ex) {
    spdlog::critical("unhandled exception: {}", ex.what());
    return 1;
  } catch (...) {
    spdlog::critical("unhandled exception of unknown type");
    return 1;
  }
}
