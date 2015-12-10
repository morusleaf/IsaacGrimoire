INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include
)

ADD_EXECUTABLE(grimoire
    main.cc
    Memory.cc
    MemoryMach.cc
    Item.cc
    CheatSheet.cc
    Inquirer.cc
    UIFltk/SheetWindow.cc
    UIFltk/InquirerPanel.cc
    UIFltk/ItemTab.cc
    UIFltk/ItemPane.cc
    UIFltk/InfoPane.cc
    UIFltk/ItemImageBox.cc
    UIFltk/StatTab.cc
    UIFltk/HUDPane.cc
    UIFltk/GameButton.cc
    exception/get_process_error.cc
    exception/get_task_error.cc
    exception/read_memory_error.cc
    exception/not_in_game.cc
)

TARGET_LINK_LIBRARIES(grimoire
    png
    ${FLTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PROJECT_SOURCE_DIR}/lib/libjsoncpp.a
    /usr/local/lib/libfltk_images.a
)

ADD_EXECUTABLE(auth
    auth.cc
)

TARGET_LINK_LIBRARIES(auth
    ${SECURITY_FRAMEWORK}
)

install(TARGETS grimoire auth
    RUNTIME DESTINATION IsaacGrimoire.app/Contents/MacOS
)
