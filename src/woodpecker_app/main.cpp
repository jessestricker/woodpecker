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
#include <QMainWindow>
#include <spdlog/spdlog.h>
#include <woodpecker/config.hpp>
#include <woodpecker/util/assert.hpp>

#include "util/qt.hpp"

namespace wdp::app {
  struct Tester {
    Tester() {
      spdlog::info("hello");
    }

    ~Tester() noexcept {
      spdlog::info("goodbye");
    }
  };

  int main(int argc, char* argv[]) {
    spdlog::info("{} v{} by {}", project_name, project_version, project_author);
    Tester tst;

    const auto app = QApplication{argc, argv};

    auto window = QMainWindow{};
    window.setWindowTitle(qstring_from_sv(project_name));
    window.setMinimumSize(640, 360);
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
