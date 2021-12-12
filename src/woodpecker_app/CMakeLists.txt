add_executable(woodpecker_app main.cpp util/qt.cpp)

set_target_properties(woodpecker_app PROPERTIES AUTOMOC ON)
target_link_libraries(woodpecker_app PRIVATE woodpecker::woodpecker Qt::Widgets)
